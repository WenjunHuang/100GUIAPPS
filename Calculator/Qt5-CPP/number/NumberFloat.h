//
// Created by rick on 19-6-14.
//

#pragma once
#include "NumberBase.h"

class Number;
namespace detail {
class NumberFloat : public NumberBase {
  friend class ::Number;
  friend class NumberError;
  friend class NumberInteger;
  friend class NumberFaction;

public:
  explicit NumberFloat(const QString& s);
  explicit NumberFloat(double value);
  explicit NumberFloat(mpf_class mpf);
  virtual ~NumberFloat();

private:
  explicit NumberFloat(const NumberInteger* value);
  explicit NumberFloat(const NumberFraction* value);
  explicit NumberFloat(const NumberFloat* value);
  explicit NumberFloat(const NumberError* value);

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
  template<double F(double)> NumberBase* executeLibCFunc(double x);
  template<double F(double,double)> NumberBase* executeLibCFunc(double x,double y);

private:
  mpf_t _mpf;
};
} // namespace detail
