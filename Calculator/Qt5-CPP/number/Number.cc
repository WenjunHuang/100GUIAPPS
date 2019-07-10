//
// Created by rick on 19-6-13.
//

#include "Number.h"
#include "NumberBase.h"
#include "NumberError.h"
#include "NumberFloat.h"
#include "NumberFraction.h"
#include "NumberInteger.h"
#include <QDebug>
#include <QRegExp>
#include <QStringList>

QString Number::GroupSeparator   = QLatin1String(",");
QString Number::DecimalSeparator = QLatin1String(".");

const Number Number::Zero(QLatin1String("0"));
const Number Number::One(QLatin1String("1"));
const Number Number::NegOne(QLatin1String("-1"));
const Number Number::PosInfinity(QLatin1String("inf"));
const Number Number::NegInfinity(QLatin1String("-inf"));
const Number Number::NaN(QLatin1String("nan"));

void increment(QString& str, int position) {
  for (auto i = position; i >= 0; i--) {
    const char lastChar = str[i].toLatin1();
    switch (lastChar) {
    case '0': str[i] = QLatin1Char('1'); break;
    case '1': str[i] = QLatin1Char('2'); break;
    case '2': str[i] = QLatin1Char('3'); break;
    case '3': str[i] = QLatin1Char('4'); break;
    case '4': str[i] = QLatin1Char('5'); break;
    case '5': str[i] = QLatin1Char('6'); break;
    case '6': str[i] = QLatin1Char('7'); break;
    case '7': str[i] = QLatin1Char('8'); break;
    case '8': str[i] = QLatin1Char('9'); break;
    case '9':
      str[i] = QLatin1Char('0');
      if (i == 0) {
        str.prepend(QLatin1Char('1'));
      }
      continue;
    case '.': continue;
    }
    break;
  }
}

void round(QString& str, int precision) {
  int decimalSymbolPos = str.indexOf(QLatin1Char('.'));

  if (decimalSymbolPos == -1) {
    if (precision == 0) {
      return;
    } else if (precision > 0) {
      str.append(QLatin1Char('.'));
      decimalSymbolPos = str.length() - 1;
    }
  }

  str.append(QString().fill(QLatin1Char('0'), precision));

  auto lastChar = str[decimalSymbolPos + precision + 1].toLatin1();
  switch (lastChar) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
    // nothing to do, rounding down
    break;
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    // rounding up
    increment(str, decimalSymbolPos + precision);
    break;
  default: break;
  }

  decimalSymbolPos = str.indexOf(QLatin1Char('.'));
  str.truncate(decimalSymbolPos + precision + 1);

  if (precision == 0) {
    str = str.section(QLatin1Char('.'), 0, 0);
  }
}

QString round(const QString& s, int precision) {
  QString tmp = s;
  if (precision < 0 ||
      !QRegExp(QLatin1String(R"(^[+-]?\d+(\.\d+)*(e[+-]?\d+)?$)"))
           .exactMatch(tmp)) {
    return s;
  }

  // Skip the sign (for now)
  const bool neg = (tmp[0] == QLatin1Char('-'));
  if (neg || tmp[0] == QLatin1Char('+')) {
    tmp.remove(0, 1);
  }

  // Split off exponential part (include 'e'-symbol)
  QString mantString =
      tmp.section(QLatin1Char('e'), 0, 0, QString::SectionCaseInsensitiveSeps);
  QString expString = tmp.section(QLatin1Char('e'), 1, 1,
                                  QString::SectionCaseInsensitiveSeps |
                                      QString::SectionIncludeLeadingSep);

  if (expString.length() == 1) {
    expString.clear();
  }

  round(mantString, precision);

  if (neg) {
    mantString.prepend(QLatin1Char('-'));
  }

  return mantString + expString;
}

Number Number::Pi() {
  QString s(QLatin1String("3."
                          "1415926535897932384626433832795028841971693993751058"
                          "20974944592307816406286208998628034825342117068"));
  s.replace('.', DecimalSeparator);
  return Number(s);
}

Number Number::Euler() {
  QString s(QLatin1String("2."
                          "7182818284590452353602874713526624977572470936999595"
                          "749669676277240766303535475945713821785251664274"));
  s.replace('.', DecimalSeparator);
  return Number(s);
}

Number::Number()
    : _value(dynamic_cast<detail::NumberBase*>(new detail::NumberInteger(0))) {}

Number::Number(const QString& s) : _value{nullptr} {
  const QRegExp specialRegex(QLatin1String("^(inf|-inf|nan)$"));
  const QRegExp integerRegex(QLatin1String("^[+-]?]\\d+$"));
  const QRegExp fractionRegex(QLatin1String("^[+-]?\\d+/\\d+$"));
  const QRegExp floatRegex(
      QString(QLatin1String(R"(^([+-]?\d*)(%1\d*)?(e([+-]?\d+))?$)"))
          .arg(QRegExp::escape(DecimalSeparator)));

  if (specialRegex.exactMatch(s)) {
    _value = static_cast<detail::NumberBase*>(new detail::NumberError(s));
  } else if (integerRegex.exactMatch(s)) {
    _value = static_cast<detail::NumberBase*>(new detail::NumberInteger(s));
  } else if (fractionRegex.exactMatch(s)) {
    _value = static_cast<detail::NumberBase*>(new detail::NumberFraction(s));
  } else if (floatRegex.exactMatch(s)) {
    if (detail::NumberFraction::DefaultFractionalInput) {
      auto const list = floatRegex.capturedTexts();
      if (list.size() == 5) {
        const QString ipart = list[1];
        const QString fpart = list[2];
        const QString epart = list[3];
        const int eVal      = list[4].toInt();

        QString num = ipart + fpart.mid(1);
        QString den =
            QLatin1String("1") + QString(fpart.size() - 1, QLatin1Char('0'));

        if (eVal < 0) {
          den = den + QString(::abs(eVal), QLatin1Char('0'));
        } else if (eVal > 0) {
          num = num + QString(::abs(eVal), QLatin1Char('0'));
        }

        _value = dynamic_cast<detail::NumberBase*>(new detail::NumberFraction(
            QString(QLatin1String("%1/%2")).arg(num, den)));
        simplify();
        return;
      }
    }

    QString newS = s;
    newS.replace(DecimalSeparator, QLatin1String("."));
    _value = dynamic_cast<detail::NumberBase*>(new detail::NumberFloat(newS));
    simplify();
  } else {
    _value = dynamic_cast<detail::NumberBase*>(
        new detail::NumberError(detail::NumberError::ERROR_UNDEFINED));
  }
}
Number::Number(qint32 value)
    : _value(
          dynamic_cast<detail::NumberBase*>(new detail::NumberInteger(value))) {
}
Number::Number(qint64 value)
    : _value(
          dynamic_cast<detail::NumberBase*>(new detail::NumberInteger(value))) {
}
Number::Number(quint32 value)
    : _value(
          dynamic_cast<detail::NumberBase*>(new detail::NumberInteger(value))) {
}
Number::Number(quint64 value)
    : _value(
          dynamic_cast<detail::NumberBase*>(new detail::NumberInteger(value))) {
}
Number::Number(long double value)
    : _value(
          dynamic_cast<detail::NumberBase*>(new detail::NumberFloat(value))) {
  simplify();
}
Number::Number(double value)
    : _value(dynamic_cast<detail::NumberBase*>(new detail::NumberFloat(value))) {
  simplify();
}

Number::Number(qint64 num, quint64 den)
    : _value(dynamic_cast<detail::NumberBase*>(
          new detail::NumberFraction(num, den))) {}

Number::Number(quint64 num, quint64 den)
    : _value(dynamic_cast<detail::NumberBase*>(
          new detail::NumberFraction(num, den))) {}

Number::Number(const Number& other) : _value(nullptr) {
  if (&other != this) {
    _value = other._value->clone();
  }
}
Number::~Number() {}
Number::Type Number::type() const {
  if (dynamic_cast<detail::NumberInteger*>(_value.get())) {
    return TYPE_INTEGER;
  } else if (dynamic_cast<detail::NumberFloat*>(_value.get())) {
    return TYPE_FLOAT;
  }
}
Number& Number::operator=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator+=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator-=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator*=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator/=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator%=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator&=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator|=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator^=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator<<=(const Number& rhs) { return <#initializer #>; }
Number& Number::operator>>=(const Number& rhs) { return <#initializer #>; }
Number Number::operator-() const { return Number(); }
Number Number::operator~() const { return Number(); }
Number Number::integerPart() const { return Number(); }
QString Number::toQString(int width, int precision) const { return QString(); }
quint64 Number::toUint64() const { return 0; }
qint64 Number::toInt64() const { return 0; }
Number Number::abs() const { return Number(); }
Number Number::cbrt() const { return Number(); }
Number Number::sqrt() const { return Number(); }
Number Number::pow(const Number& x) const { return Number(); }
Number Number::sin() const { return Number(); }
Number Number::cos() const { return Number(); }
Number Number::tan() const { return Number(); }
Number Number::asin() const { return Number(); }
Number Number::acos() const { return Number(); }
Number Number::atan() const { return Number(); }
Number Number::sinh() const { return Number(); }
Number Number::cosh() const { return Number(); }
Number Number::tanh() const { return Number(); }
Number Number::asinh() const { return Number(); }
Number Number::acosh() const { return Number(); }
Number Number::atanh() const { return Number(); }
Number Number::tgamma() const { return Number(); }
Number Number::factorial() const { return Number(); }
Number Number::log2() const { return Number(); }
Number Number::log10() const { return Number(); }
Number Number::ln() const { return Number(); }
Number Number::floor() const { return Number(); }
Number Number::ceil() const { return Number(); }
Number Number::exp2() const { return Number(); }
Number Number::exp10() const { return Number(); }
Number Number::exp() const { return Number(); }
Number Number::bin(const Number& x) const { return Number(); }
void Number::setDefaultFloatPrecision(int precision) {}
void Number::setSplitoffIntegerForFractionOutput(bool x) {
  detail::NumberFraction::setSplitoffIntegerForFractionOutput(x);
}
void Number::setDefaultFractionalInput(bool x) {
  detail::NumberFraction::setDefaultFractionalInput(x);
}
void Number::setDefaultFloatOutput(bool x) {
  detail::NumberFraction::setDefaultFractionalOutput(!x);
}
void Number::setGroupSeparator(const QString& ch) { GroupSeparator = ch; }
void Number::setDecimalSeparator(const QString& ch) { DecimalSeparator = ch; }
QString Number::groupSeparator() { return GroupSeparator; }
QString Number::decimalSeparator() { return DecimalSeparator; }
void Number::swap(Number& other) {}
void Number::simplify() {}
