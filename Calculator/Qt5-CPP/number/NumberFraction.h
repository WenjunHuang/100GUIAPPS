//
// Created by rick on 19-6-15.
//

#pragma once

#include "NumberBase.h"
#include "GmpType.h"

class Number;
namespace detail {
class NumberFloat;
class NumberError;
class NumberInteger;

class NumberFraction : public NumberBase {
  friend class ::Number;
  friend class NumberError;
  friend class NumberInteger;
  friend class NumberFloat;

public:
  static bool DefaultFractionalInput;
  static bool DefaultFractionalOutput;
  static bool SplitoffIntegerForFractionOutput;

public:
  static void setDefaultFractionalInput(bool value);
  static void setDefaultFractionalOutput(bool value);
  static void setSplitoffIntegerForFractionOutput(bool value);

public:
  explicit NumberFraction(const QString& s);
  NumberFraction(qint64 num, quint64 den);
  NumberFraction(quint64 num, quint64 den);
  explicit NumberFraction(mpq_t mpq);
  virtual ~NumberFraction();

public:
  NumberBase* clone() override;

public:
  QString toString(int precision) const override;
  quint64 toUint64() const override;
  qint64 toInt64() const override;

public:
  bool isInteger() const override;
  bool isZero() const override;
  int sign() const override;

public:
  // basic math
  NumberBase* add(NumberBase* rhs) override;
  NumberBase* sub(NumberBase* rhs) override;
  NumberBase* mul(NumberBase* rhs) override;
  NumberBase* div(NumberBase* rhs) override;
  NumberBase* mod(NumberBase* rhs) override;

public:
  // logical operators
  NumberBase* bitwiseAnd(NumberBase* rhs) override;
  NumberBase* bitwiseXor(NumberBase* rhs) override;
  NumberBase* bitwiseOr(NumberBase* rhs) override;
  NumberBase* bitwiseShift(NumberBase* rhs) override;

public:
  // algebraic functions
  NumberBase* pow(NumberBase* rhs) override;
  NumberBase* neg() override;
  NumberBase* cmp() override;
  NumberBase* abs() override;
  NumberBase* sqrt() override;
  NumberBase* cbrt() override;
  NumberBase* factorial() override;
  NumberBase* reciprocal() override;

public:
  // special functions
  NumberBase* log2() override;
  NumberBase* log10() override;
  NumberBase* ln() override;
  NumberBase* exp2() override;
  NumberBase* exp10() override;
  NumberBase* floor() override;
  NumberBase* ceil() override;
  NumberBase* exp() override;
  NumberBase* bin(NumberBase* rhs) override;

public:
  // trig functions
  NumberBase* sin() override;
  NumberBase* cos() override;
  NumberBase* tan() override;
  NumberBase* asin() override;
  NumberBase* acos() override;
  NumberBase* atan() override;
  NumberBase* sinh() override;
  NumberBase* cosh() override;
  NumberBase* tanh() override;
  NumberBase* asinh() override;
  NumberBase* acosh() override;
  NumberBase* atanh() override;
  NumberBase* tgamma() override;

public:
  int compare(NumberBase* rhs) override;

private:
  NumberInteger* numerator() const;
  NumberInteger* denominator() const;

private:
  // conversion constructors
  explicit NumberFraction(const NumberInteger* value);
  explicit NumberFraction(const NumberFraction* value);
  explicit NumberFraction(const NumberError* value);

private:
  GmpType<mpq_t> _mpq;
};
} // namespace detail
