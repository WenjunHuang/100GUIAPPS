//
// Created by rick on 19-6-15.
//

#include "NumberFraction.h"

namespace detail {

void NumberFraction::setDefaultFractionalInput(bool value) {}
void NumberFraction::setDefaultFractionalOutput(bool value) {}
void NumberFraction::setSplitoffIntegerForFractionOutput(bool value) {}
NumberFraction::NumberFraction(const QString& s) {}
NumberFraction::NumberFraction(qint64 num, quint64 den) {}
NumberFraction::NumberFraction(quint64 num, quint64 den) {}
NumberFraction::NumberFraction(__mpq_struct* mpq) {}
NumberFraction::~NumberFraction() {}
NumberBase* NumberFraction::clone() { return nullptr; }
QString NumberFraction::toString(int precision) const { return QString(); }
quint64 NumberFraction::toUint64() const { return 0; }
qint64 NumberFraction::toInt64() const { return 0; }
bool NumberFraction::isInteger() const { return false; }
bool NumberFraction::isZero() const { return false; }
int NumberFraction::sign() const { return 0; }
NumberBase* NumberFraction::add(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::sub(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::mul(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::div(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::mod(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::bitwiseAnd(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::bitwiseXor(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::bitwiseOr(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::bitwiseShift(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::pow(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::neg() { return nullptr; }
NumberBase* NumberFraction::cmp() { return nullptr; }
NumberBase* NumberFraction::abs() { return nullptr; }
NumberBase* NumberFraction::sqrt() { return nullptr; }
NumberBase* NumberFraction::cbrt() { return nullptr; }
NumberBase* NumberFraction::factorial() { return nullptr; }
NumberBase* NumberFraction::reciprocal() { return nullptr; }
NumberBase* NumberFraction::log2() { return nullptr; }
NumberBase* NumberFraction::log10() { return nullptr; }
NumberBase* NumberFraction::ln() { return nullptr; }
NumberBase* NumberFraction::exp2() { return nullptr; }
NumberBase* NumberFraction::exp10() { return nullptr; }
NumberBase* NumberFraction::floor() { return nullptr; }
NumberBase* NumberFraction::ceil() { return nullptr; }
NumberBase* NumberFraction::exp() { return nullptr; }
NumberBase* NumberFraction::bin(NumberBase* rhs) { return nullptr; }
NumberBase* NumberFraction::sin() { return nullptr; }
NumberBase* NumberFraction::cos() { return nullptr; }
NumberBase* NumberFraction::tan() { return nullptr; }
NumberBase* NumberFraction::asin() { return nullptr; }
NumberBase* NumberFraction::acos() { return nullptr; }
NumberBase* NumberFraction::atan() { return nullptr; }
NumberBase* NumberFraction::sinh() { return nullptr; }
NumberBase* NumberFraction::cosh() { return nullptr; }
NumberBase* NumberFraction::tanh() { return nullptr; }
NumberBase* NumberFraction::asinh() { return nullptr; }
NumberBase* NumberFraction::acosh() { return nullptr; }
NumberBase* NumberFraction::atanh() { return nullptr; }
NumberBase* NumberFraction::tgamma() { return nullptr; }
int NumberFraction::compare(NumberBase* rhs) { return 0; }
NumberInteger* NumberFraction::numerator() const { return nullptr; }
NumberInteger* NumberFraction::denominator() const { return nullptr; }
NumberFraction::NumberFraction(const NumberInteger* value) {}
NumberFraction::NumberFraction(const NumberFraction* value) {}
NumberFraction::NumberFraction(const NumberError* value) {}
}