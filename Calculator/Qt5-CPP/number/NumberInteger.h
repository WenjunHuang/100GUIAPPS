//
// Created by rick on 19-6-13.
//

#pragma once
#include "NumberBase.h"
#include "NumberError.h"
#include "NumberFraction.h"

class Number;

namespace detail {
class NumberInteger : public NumberBase {
  friend class ::Number;
  friend class NumberError;
  friend class NumberFraction;
  friend class NumberFloat;

public:
  explicit NumberInteger(const QString& s);
  explicit NumberInteger(qint32 value);
  explicit NumberInteger(qint64 value);
  explicit NumberInteger(quint32 value);
  explicit NumberInteger(quint64 value);
  explicit NumberInteger(mpz_t mpz);
  ~NumberInteger() override;

public:
  NumberBase* clone() override;
  QString toString(int precision) const override;
  quint64 toUint64() const override;
  qint64 toInt64() const override;


  virtual bool isEven() const;
  virtual bool isOdd() const;
  bool isInteger() const override;
  bool isZero() const override;
  int sign() const override;
  NumberBase* add(NumberBase* rhs) override;
  NumberBase* sub(NumberBase* rhs) override;
  NumberBase* mul(NumberBase* rhs) override;
  NumberBase* div(NumberBase* rhs) override;
  NumberBase* mod(NumberBase* rhs) override;
  NumberBase* bitwiseAnd(NumberBase* rhs) override;
  NumberBase* bitwiseXor(NumberBase* rhs) override;
  NumberBase* bitwiseOr(NumberBase* rhs) override;
  NumberBase* bitwiseShift(NumberBase* rhs) override;
  NumberBase* pow(NumberBase* rhs) override;
  NumberBase* neg() override;
  NumberBase* cmp() override;
  NumberBase* abs() override;
  NumberBase* sqrt() override;
  NumberBase* cbrt() override;
  NumberBase* factorial() override;
  NumberBase* reciprocal() override;
  NumberBase* log2() override;
  NumberBase* log10() override;
  NumberBase* ln() override;
  NumberBase* exp2() override;
  NumberBase* exp10() override;
  NumberBase* floor() override;
  NumberBase* ceil() override;
  NumberBase* exp() override;
  NumberBase* bin(NumberBase* rhs) override;
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
  int compare(NumberBase* rhs) override;

private:
  // conversion constructors
  explicit NumberInteger(const NumberInteger* value);
  explicit NumberInteger(const NumberFraction* value);
  explicit NumberInteger(const NumberFloat* value);
  explicit NumberInteger(const NumberError* value);

private:
  mpz_t _mpz;
};
} // namespace detail
