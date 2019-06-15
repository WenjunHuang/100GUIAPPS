//
// Created by rick on 19-6-14.
//

#include "NumberFloat.h"
#include "NumberError.h"
#include "NumberFraction.h"
#include "NumberInteger.h"
#include <QtCore/QScopedArrayPointer>
#include <QtGlobal>
#include <math.h>
#include <memory>

namespace detail {
template <double F(double)> NumberBase* NumberFloat::executeLibCFunc(double x) {
  const double r = F(x);
  if (qIsNaN(r)) {
    auto e = new NumberError(NumberError::ERROR_UNDEFINED);
    delete this;
    return e;
  } else if (qIsInf(r)) {
    auto e = new NumberError(NumberError::ERROR_POS_INFINITY);
    delete this;
    return e;
  } else {
    _mpf = r;
    return this;
  }
}

template <double F(double, double)>
NumberBase* NumberFloat::executeLibCFunc(double x, double y) {
  const double r = F(x, y);
  if (qIsNaN(r)) {
    auto e = new NumberError(NumberError::ERROR_UNDEFINED);
    delete this;
    return e;
  } else if (qIsInf(r)) {
    auto e = new NumberError(NumberError::ERROR_POS_INFINITY);
    delete this;
    return e;
  } else {
    _mpf = r;
    return this;
  }
}
NumberFloat::NumberFloat(const QString& s) { _mpf.set_str(s.toLatin1(), 10); }

NumberFloat::NumberFloat(double value) {
  Q_ASSERT(!qIsInf(value));
  Q_ASSERT(!qIsNaN(value));

  _mpf = value;
}

NumberFloat::NumberFloat(const NumberFloat* value) { _mpf = value->_mpf; }

NumberFloat::NumberFloat(const NumberInteger* value) { _mpf = value->_mpz; }

NumberFloat::NumberFloat(const NumberFraction* value) { _mpf = value->_mpq; }

NumberFloat::~NumberFloat() {}

NumberBase* NumberFloat::clone() { return new NumberFloat(this); }

NumberBase* NumberFloat::add(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFloat f(p);
    return add(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    _mpf = _mpf + p->_mpf;
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    NumberFloat f(p);
    return add(&f);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    NumberError* e = new NumberError(p);
    delete this;
    return e;
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberFloat::sub(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFloat f(p);
    return sub(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    _mpf -= p->_mpf;
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    auto e = new NumberError(p);
    delete this;
    return e->neg();
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberFloat::mul(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFloat f(p);
    return mul(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    _mpf *= p->_mpf;
    return this;
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    NumberFloat f(p);
    return mul(&f);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (isZero()) {
      delete this;
      return new NumberError(NumberError::ERROR_UNDEFINED);
    }

    if (sign() < 0) {
      delete this;
      auto e = new NumberError(p);
      return e->neg();
    } else {
      delete this;
      return new NumberError(p);
    }
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberFloat::div(NumberBase* rhs) {
  if (rhs->isZero()) {
    if (sign() < 0) {
      delete this;
      return new NumberError(NumberError::ERROR_NEG_INFINITY);
    } else {
      delete this;
      return new NumberError(NumberError::ERROR_POS_INFINITY);
    }
  }

  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFloat f(p);
    return div(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    _mpf /= p->_mpf;
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->sign() > 0 || p->sign() < 0) {
      delete this;
      return new NumberInteger(0);
    }

    delete this;
    return new NumberError(p);
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberFloat::mod(NumberBase* rhs) {
  Q_UNUSED(rhs);

  if (rhs->isZero()) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  delete this;
  return new NumberInteger(0);
}

NumberBase* NumberFloat::bitwiseAnd(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberInteger(0);
}

NumberBase* NumberFloat::bitwiseOr(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberInteger(0);
}
NumberBase* NumberFloat::bitwiseXor(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberInteger(0);
}

NumberBase* NumberFloat::bitwiseShift(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}

NumberBase* NumberFloat::neg() {
  _mpf = -_mpf;
}

NumberBase* NumberFloat::cmp() {
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}

NumberBase* NumberFloat::abs() {
  _mpf = ::abs(_mpf);
}

NumberBase* NumberFloat::sqrt() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  _mpf = ::sqrt(_mpf);
  return this;
}

NumberBase* NumberFloat::cbrt() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else {
    return executeLibCFunc<::cbrt>(x);
  }
}

NumberBase* NumberFloat::factorial() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  auto i = new NumberInteger(this);
  delete this;
  return i->factorial();
}

NumberBase* NumberFloat::sin() {
  const double x = _mpf.get_d();
  if(isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else{
    return executeLibCFunc<::sin>(x);
  }
}

NumberBase* NumberFloat::floor() {
  const double x = _mpf.get_d();

  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else{
    return executeLibCFunc<::floor>(x);
  }
}

NumberBase* NumberFloat::ceil() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else {
    return executeLibCFunc<::ceil>(x);
  }
}

NumberBase* NumberFloat::cos() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::cos>(x);
  }
}

NumberBase* NumberFloat::tan() {
  const double x = _mpf.get_d();
  if(isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else {
    return executeLibCFunc<::tan>(x);
  }
}

NumberBase* NumberFloat::asin() {
  if (_mpf > 1.0 || _mpf < -1.0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  const double x= _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::asin>(x);
  }
}

NumberBase* NumberFloat::acos() {
  if (_mpf > 1.0 || _mpf < -1.0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::acos>(x);
  }
}

NumberBase* NumberFloat::atan() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::atan>(x);
  }
}

NumberBase* NumberFloat::sinh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::sinh>(x);
}

NumberBase* NumberFloat::cosh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::cosh>(x);
}

NumberBase* NumberFloat::tanh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::tanh>(x);
}

NumberBase* NumberFloat::tgamma() {
  const double x = _mpf.get_d();
  if (isinf(x)){
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::tgamma>(x);
  }
}

NumberBase* NumberFloat::asinh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::asinh>(x);
}

NumberBase* NumberFloat::acosh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::acosh>(x);
}

NumberBase* NumberFloat::atanh() {
  const double x = _mpf.get_d();
  return executeLibCFunc<::atanh>(x);
}

NumberBase* NumberFloat::pow(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    p->_mpz.get_ui();
    auto t = _mpf.get_mpf_t();
    mpf_pow_ui(t,t,p->_mpz.get_ui());

    if (p->sign() < 0){
      return reciprocal();
    }else{
      return this;
    }
  }else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    return executeLibCFunc<::pow>(_mpf.get_d(),p->_mpf.get_d());
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    NumberFloat f(p);
    return executeLibCFunc<::pow>(_mpf.get_d(),f._mpf.get_d());
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->sign() > 0) {
      auto e = new NumberError(NumberError::ERROR_POS_INFINITY);
      delete this;
      return e;
    } else if (p->sign() < 0) {
      auto n = new NumberInteger(0);
      delete this;
      return n;
    } else {
      auto e = new NumberError(NumberError::ERROR_UNDEFINED);
      delete this;
      return e;
    }
  }

  Q_ASSERT(0);
  return nullptr;
}

int NumberFloat::compare(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFloat f(p);
    return compare(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    return ::cmp(_mpf,p->_mpf);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    NumberFloat f(p);
    return compare(&f);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    // NOTE: any number compared to NaN/Inf/-Inf always compares less
    return -1;
  }

  Q_ASSERT(0);
  return 0;
}

QString NumberFloat::toString(int precision) const {
  size_t size;
  if (precision > 0) {
    size = gmp_snprintf(nullptr,0,"%.*Fg",precision,_mpf.get_mpf_t()) + 1;
  } else {
    size = gmp_snprintf(nullptr,0,"%.Fg",_mpf.get_mpf_t()) + 1;
  }

  auto buf = std::make_unique<char[]>(size);

  if (precision > 0) {
    gmp_snprintf(buf.get(),size,"%.*Fg",precision,_mpf.get_mpf_t());
  }else {
    gmp_snprintf(buf.get(),size,"%.Fg",_mpf.get_mpf_t());
  }

  return QLatin1String(buf.get());
}

bool NumberFloat::isInteger() const {
  return mpf_integer_p(_mpf.get_mpf_t()) != 0;
}

bool NumberFloat::isZero() const {
  return ::sgn(_mpf) == 0;
}

int NumberFloat::sign() const {
  return ::sgn(_mpf);
}

NumberBase* NumberFloat::reciprocal() {
  _mpf = 1.0 / _mpf;
  return this;
}

NumberBase* NumberFloat::log2() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  }else {
    return executeLibCFunc<::log2>(x);
  }
}

NumberBase* NumberFloat::log10() {
  const double x = _mpf.get_d();

  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::log10>(x);
  }
}

NumberBase* NumberFloat::ln() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::log>(x);
  }
}

NumberBase* NumberFloat::exp2() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::exp2>(x);
  }
}

NumberBase* NumberFloat::exp10() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::pow>(10,x);
  }
}

NumberBase* NumberFloat::exp() {
  const double x = _mpf.get_d();
  if (isinf(x)) {
    delete this;
    return new NumberError(NumberError::ERROR_POS_INFINITY);
  } else {
    return executeLibCFunc<::exp>(x);
  }
}

quint64 NumberFloat::toUint64() const {
  return NumberInteger(this).toUint64();
}

qint64 NumberFloat::toInt64() const {
  return NumberInteger(this).toInt64();
}

NumberBase* NumberFloat::bin(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}
}