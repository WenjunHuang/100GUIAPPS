//
// Created by rick on 19-6-13.
//

#include "NumberInteger.h"

namespace detail {
NumberInteger::NumberInteger(const QString& s) {
  _mpz.set_str(s.toLatin1(),10);
}

NumberInteger::NumberInteger(qint32 value) {
  _mpz = static_cast<signed long int>(value);
}

NumberInteger::NumberInteger(qint64 value) {
  _mpz = static_cast<signed long int>(value);
}

NumberInteger::NumberInteger(quint32 value) {
  _mpz = static_cast<unsigned long int>(value);
}

NumberInteger::NumberInteger(quint64 value) {
  _mpz = static_cast<unsigned long int>(value);
}

NumberInteger::NumberInteger(mpz_class mpz) {
  _mpz = mpz;
}

NumberInteger::~NumberInteger() {}

QString NumberInteger::toString(int precision) const { return QString(); }

quint64 NumberInteger::toUint64() const { return 0; }

qint64 NumberInteger::toInt64() const { return 0; }

bool NumberInteger::isInteger() const { return false; }
bool NumberInteger::isZero() const { return false; }
int NumberInteger::sign() const { return 0; }
NumberBase* NumberInteger::add(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::sub(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::mul(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::div(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::mod(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::bitwiseAnd(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::bitwiseXor(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::bitwiseOr(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::bitwiseShift(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::pow(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::neg() { return nullptr; }
NumberBase* NumberInteger::cmp() { return nullptr; }
NumberBase* NumberInteger::abs() { return nullptr; }
NumberBase* NumberInteger::sqrt() { return nullptr; }
NumberBase* NumberInteger::cbrt() { return nullptr; }
NumberBase* NumberInteger::factorial() { return nullptr; }
NumberBase* NumberInteger::reciprocal() { return nullptr; }
NumberBase* NumberInteger::log2() { return nullptr; }
NumberBase* NumberInteger::log10() { return nullptr; }
NumberBase* NumberInteger::ln() { return nullptr; }
NumberBase* NumberInteger::exp2() { return nullptr; }
NumberBase* NumberInteger::exp10() { return nullptr; }
NumberBase* NumberInteger::floor() { return nullptr; }
NumberBase* NumberInteger::ceil() { return nullptr; }
NumberBase* NumberInteger::exp() { return nullptr; }
NumberBase* NumberInteger::bin(NumberBase* rhs) { return nullptr; }
NumberBase* NumberInteger::sin() { return nullptr; }
NumberBase* NumberInteger::cos() { return nullptr; }
NumberBase* NumberInteger::tan() { return nullptr; }
NumberBase* NumberInteger::asin() { return nullptr; }
NumberBase* NumberInteger::acos() { return nullptr; }
NumberBase* NumberInteger::atan() { return nullptr; }
NumberBase* NumberInteger::sinh() { return nullptr; }
NumberBase* NumberInteger::cosh() { return nullptr; }
NumberBase* NumberInteger::tanh() { return nullptr; }
NumberBase* NumberInteger::asinh() { return nullptr; }
NumberBase* NumberInteger::acosh() { return nullptr; }
NumberBase* NumberInteger::atanh() { return nullptr; }
NumberBase* NumberInteger::tgamma() { return nullptr; }
int NumberInteger::compare(NumberBase* rhs) { return 0; }
NumberInteger::NumberInteger(const NumberInteger* value) {}
} // namespace detail
