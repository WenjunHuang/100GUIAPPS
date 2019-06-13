//
// Created by rick on 19-6-13.
//
#pragma once
#include <QString>
#include <QtGlobal>
#include <gmpxx.h>

namespace detail {
class NumberError;
class NumberInteger;
class NumberFraction;
class NumberFloat;

class NumberBase {
public:
  virtual ~NumberBase();

public:
  virtual NumberBase* clone() = 0;

public:
  virtual QString toString(int precision) const = 0;
  virtual quint64 toUint64() const              = 0;
  virtual qint64 toInt64() const                = 0;

public:
  virtual bool isInteger() const = 0;
  virtual bool isZero() const    = 0;
  virtual int sign() const       = 0;

public:
  // basic math
  virtual NumberBase* add(NumberBase* rhs) = 0;
  virtual NumberBase* sub(NumberBase* rhs) = 0;
  virtual NumberBase* mul(NumberBase* rhs) = 0;
  virtual NumberBase* div(NumberBase* rhs) = 0;
  virtual NumberBase* mod(NumberBase* rhs) = 0;

public:
  // logical operators
  virtual NumberBase* bitwiseAnd(NumberBase* rhs)   = 0;
  virtual NumberBase* bitwiseXor(NumberBase* rhs)   = 0;
  virtual NumberBase* bitwiseOr(NumberBase* rhs)    = 0;
  virtual NumberBase* bitwiseShift(NumberBase* rhs) = 0;

public:
  // algebraic functions
  virtual NumberBase* pow(NumberBase* rhs) = 0;
  virtual NumberBase* neg()                = 0;
  virtual NumberBase* cmp()                = 0;
  virtual NumberBase* abs()                = 0;
  virtual NumberBase* sqrt()               = 0;
  virtual NumberBase* cbrt()               = 0;
  virtual NumberBase* factorial()          = 0;
  virtual NumberBase* reciprocal()         = 0;

public:
  // special functions
  virtual NumberBase* log2()               = 0;
  virtual NumberBase* log10()              = 0;
  virtual NumberBase* ln()                 = 0;
  virtual NumberBase* exp2()               = 0;
  virtual NumberBase* exp10()              = 0;
  virtual NumberBase* floor()              = 0;
  virtual NumberBase* ceil()               = 0;
  virtual NumberBase* exp()                = 0;
  virtual NumberBase* bin(NumberBase* rhs) = 0;

public:
  // trig functions
  virtual NumberBase* sin()    = 0;
  virtual NumberBase* cos()    = 0;
  virtual NumberBase* tan()    = 0;
  virtual NumberBase* asin()   = 0;
  virtual NumberBase* acos()   = 0;
  virtual NumberBase* atan()   = 0;
  virtual NumberBase* sinh()   = 0;
  virtual NumberBase* cosh()   = 0;
  virtual NumberBase* tanh()   = 0;
  virtual NumberBase* asinh()  = 0;
  virtual NumberBase* acosh()  = 0;
  virtual NumberBase* atanh()  = 0;
  virtual NumberBase* tgamma() = 0;

public:
  virtual int compare(NumberBase* rhs) = 0;
};
} // namespace detail
