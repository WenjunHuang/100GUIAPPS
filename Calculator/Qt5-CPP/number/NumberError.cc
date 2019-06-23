//
// Created by rick on 19-6-14.
//

#include "NumberError.h"
#include "NumberFloat.h"
#include "NumberFraction.h"
#include "NumberInteger.h"
#include <math.h>

namespace detail {
NumberError::NumberError(const QString& s) {
  if (s == QStringLiteral("nan"))
    _error = ERROR_UNDEFINED;
  else if (s == QStringLiteral("inf"))
    _error = ERROR_POS_INFINITY;
  else if (s == QStringLiteral("-inf"))
    _error = ERROR_NEG_INFINITY;
  else
    _error = ERROR_UNDEFINED;
}

NumberError::NumberError(detail::NumberError::Error e) : _error{e} {}
NumberError::NumberError() : _error{ERROR_UNDEFINED} {}
NumberError::~NumberError() {}
NumberBase* NumberError::clone() { return new NumberError(this); }
QString NumberError::toString(int precision) const {
  Q_UNUSED(precision);

  switch (_error) {
  case ERROR_POS_INFINITY: return QLatin1String("inf");
  case ERROR_NEG_INFINITY: return QLatin1String("-inf");
  case ERROR_UNDEFINED:
  default: return QLatin1String("nan");
  }
}

quint64 NumberError::toUint64() const { return 0; }
qint64 NumberError::toInt64() const { return 0; }
bool NumberError::isInteger() const { return false; }
bool NumberError::isZero() const { return false; }
int NumberError::sign() const {
  switch (_error) {
  case ERROR_POS_INFINITY: return +1;
  case ERROR_NEG_INFINITY: return -1;
  case ERROR_UNDEFINED:
  default: return 0;
  }
}
NumberBase* NumberError::add(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::sub(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::mul(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    if (p->isZero()) {
      _error = ERROR_UNDEFINED;
    }
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    if (p->isZero())
      _error = ERROR_UNDEFINED;
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;

    return this;
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberError::div(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::mod(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::bitwiseAnd(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::bitwiseXor(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::bitwiseOr(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}
NumberBase* NumberError::bitwiseShift(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->_error == ERROR_UNDEFINED)
      _error = ERROR_UNDEFINED;
    else if (_error != p->_error)
      _error = ERROR_UNDEFINED;
  }
  return this;
}

NumberBase* NumberError::pow(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberError*>(rhs)) {
    switch (_error) {
    case ERROR_POS_INFINITY:
      if (p->sign() > 0) {
        return this;
      } else if (p->sign() < 0) {
        auto n = new NumberInteger(0);
        delete this;
        return n;
      } else {
        _error = ERROR_UNDEFINED;
        return this;
      }
      break;
    case ERROR_NEG_INFINITY:
      if (p->sign() > 0) {
        auto n = new NumberInteger(0);
        delete this;
        return n;
      } else {
        _error = ERROR_UNDEFINED;
        return this;
      }
      break;
    case ERROR_UNDEFINED: return this;
    }
  }
  return this;
}
NumberBase* NumberError::neg() {
  switch (_error) {
  case ERROR_POS_INFINITY: _error = ERROR_NEG_INFINITY; break;
  case ERROR_NEG_INFINITY: _error = ERROR_POS_INFINITY; break;
  case ERROR_UNDEFINED:
  default: break;
  }

  return this;
}

NumberBase* NumberError::cmp() {
  _error = ERROR_UNDEFINED;
  return this;
}

NumberBase* NumberError::abs() {
  switch (_error) {
  case ERROR_NEG_INFINITY: _error = ERROR_POS_INFINITY; break;
  case ERROR_POS_INFINITY:
  case ERROR_UNDEFINED:
  default: break;
  }
  return this;
}

NumberBase* NumberError::sqrt() {
  switch (_error) {
  case ERROR_NEG_INFINITY: _error = ERROR_UNDEFINED; break;
  case ERROR_POS_INFINITY:
  case ERROR_UNDEFINED:
  default: break;
  }
  return this;
}

NumberBase* NumberError::cbrt() { return this; }
NumberBase* NumberError::factorial() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::reciprocal() { return nullptr; }
NumberBase* NumberError::log2() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::log10() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::ln() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::exp2() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::exp10() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::floor() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::ceil() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::exp() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::bin(NumberBase* rhs) {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::sin() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::cos() {
  _error = ERROR_UNDEFINED;
  return this;
}
NumberBase* NumberError::tan() {
  _error = ERROR_UNDEFINED;
  return this;
NumberBase* NumberError::asin() { return nullptr; }
NumberBase* NumberError::acos() { return nullptr; }
NumberBase* NumberError::atan() {
  switch (_error) {
  case ERROR_POS_INFINITY: delete this; return new NumberFloat(M_PI / 2.0);
  case ERROR_NEG_INFINITY: delete this; return new NumberFloat(-M_PI / 2.0);
  case ERROR_UNDEFINED:
  default: return this;
  }
}
NumberBase* NumberError::sinh() { return this; }
NumberBase* NumberError::cosh() {
  _error = ERROR_UNDEFINED;
  return this;
}

NumberBase* NumberError::tanh() {
  if (sign() > 0) {
    delete this;
    return new NumberInteger(1);
  } else if (sign() < 0) {
    delete this;
    return new NumberInteger(-1);
  } else {
    return this;
  }
}

NumberBase* NumberError::asinh() { return this; }
NumberBase* NumberError::acosh() {
  if (sign() < 0) {
    _error = ERROR_UNDEFINED;
  }

  return this;
}

NumberBase* NumberError::atanh() {
  _error = ERROR_UNDEFINED;
  return this;
}

NumberBase* NumberError::tgamma() { return nullptr; }
int NumberError::compare(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    if (sign() > 0) {
      return 1;
    } else {
      return -1;
    }
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    if (sign() > 0) {
      return 1;
    } else {
      return -1;
    }
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    if (sign() > 0) {
      return 1;
    } else {
      return -1;
    }
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    return sign() == p->sign();
  }
}
NumberError::NumberError(const NumberInteger* value)
    : _error{ERROR_UNDEFINED} {}
NumberError::NumberError(const NumberFraction* value)
    : _error{ERROR_UNDEFINED} {}
NumberError::NumberError(const NumberFloat* value) : _error{ERROR_UNDEFINED} {}
NumberError::NumberError(const NumberError* value) : _error{value->_error} {}
} // namespace detail
