//
// Created by rick on 19-6-13.
//

#include "NumberInteger.h"
#include "NumberError.h"
#include "NumberFloat.h"
#include "NumberFraction.h"
#include <memory>

namespace detail {
NumberInteger::NumberInteger(const QString& s) {
  mpz_init(_mpz);
  mpz_set_str(_mpz, s.toLatin1(), 10);
}

NumberInteger::NumberInteger(qint32 value) {
  mpz_init_set_si(_mpz, static_cast<signed long int>(value));
}

NumberInteger::NumberInteger(qint64 value) {
  mpz_init(_mpz);
  mpz_set_si(_mpz, static_cast<signed long int>(value));
}

NumberInteger::NumberInteger(quint32 value) {
  mpz_init_set_ui(_mpz, static_cast<unsigned long int>(value));
}

NumberInteger::NumberInteger(quint64 value) {
  mpz_init_set_ui(_mpz, static_cast<unsigned long int>(value));
}

NumberInteger::NumberInteger(mpz_t mpz) { mpz_init_set(_mpz, mpz); }

NumberInteger::NumberInteger(const NumberInteger* value) {
  mpz_init_set(_mpz, value->_mpz);
}
NumberInteger::NumberInteger(const NumberFraction* value) {
  mpz_init(_mpz);
  mpz_set_q(_mpz, value->_mpq);
}

NumberInteger::NumberInteger(const NumberFloat* value) {
  mpz_init(_mpz);
  mpz_set_f(_mpz, value->_mpf);
}

NumberInteger::NumberInteger(const NumberError* value) {}
NumberInteger::~NumberInteger() { mpz_clear(_mpz); }

QString NumberInteger::toString(int precision) const {
  Q_UNUSED(precision);

  auto size                 = gmp_snprintf(nullptr, 0, "%Zd", _mpz) + 1;
  std::unique_ptr<char> buf = std::make_unique<char>(size);
  gmp_snprintf(buf.get(), size, "%Zd", _mpz);
  return QLatin1String(buf.get());
}

quint64 NumberInteger::toUint64() const {
  const QString tmpstring = toString(-1);

  bool ok;
  quint64 value;

  if (sign() < 0) {
    const qint64 signedvalue = tmpstring.toLongLong(&ok, 10);
    value                    = static_cast<quint64>(signedvalue);
  } else {
    value = tmpstring.toULongLong(&ok, 10);
  }

  if (!ok) {
    value = 0;
  }

  return value;
}

qint64 NumberInteger::toInt64() const {
  const QString tmpstring = toString(-1);

  bool ok;
  qint64 value = tmpstring.toLongLong(&ok, 10);

  if (!ok) {
    value = 0;
  }
  return value;
}

bool NumberInteger::isInteger() const { return true; }
bool NumberInteger::isZero() const { return mpz_sgn(_mpz) == 0; }
int NumberInteger::sign() const { return mpz_sgn(_mpz); }
NumberBase* NumberInteger::add(NumberBase* rhs) {
  if (auto* const p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_add(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto* const f = new NumberFloat(this);
    delete this;
    return f->add(p);
  } else if (auto* const p = dynamic_cast<NumberFraction*>(rhs)) {
    const auto q = new NumberFraction(this);
    delete this;
    return q->add(p);
  } else if (const auto p = dynamic_cast<NumberError*>(rhs)) {
    delete this;
    return p->clone();
  }
  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberInteger::sub(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_sub(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->sub(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    delete this;
    return q->sub(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    auto e = p->clone();
    delete this;
    return e->neg();
  }
}

NumberBase* NumberInteger::mul(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_mul(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->mul(rhs);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    delete this;
    return q->mul(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (isZero()) {
      delete this;
      auto e = new NumberError(NumberError::ERROR_UNDEFINED);
      return e->neg();
    }

    if (sign() < 0) {
      delete this;
      auto e = p->clone();
      return e->neg();
    } else {
      delete this;
      return p->clone();
    }
  }
}

NumberBase* NumberInteger::div(NumberBase* rhs) {
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
    auto q = new NumberFraction(this);
    delete this;
    return q->div(p);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->div(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    delete this;
    return q->div(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->sign() > 0) {
      delete this;
      return new NumberInteger(0);
    } else if (p->sign() < 0) {
      delete this;
      return new NumberInteger(0);
    }

    delete this;
    return p->clone();
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberInteger::mod(NumberBase* rhs) {
  if (rhs->isZero()) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_mod(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->mod(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    delete this;
    return q->mod(rhs);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    delete this;
    return p->clone();
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberInteger::bitwiseAnd(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_and(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->bitwiseAnd(rhs);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    return q->bitwiseAnd(rhs);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    delete this;
    return p->clone();
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberInteger::bitwiseXor(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_xor(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->bitwiseXor(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    delete this;
    return q->bitwiseXor(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    delete this;
    return p->clone();
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberInteger::bitwiseOr(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_ior(_mpz, _mpz, p->_mpz);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    return f->bitwiseXor(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto q = new NumberFraction(this);
    return q->bitwiseXor(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    delete this;
    return p->clone();
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberInteger::bitwiseShift(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    auto bitCount = mpz_get_si(p->_mpz);
    if (bitCount > 0) {
      mpz_mul_2exp(_mpz, _mpz, bitCount);

    } else if (bitCount < 0) {
      if (mpz_sgn(_mpz) < 0) {
        mpz_fdiv_q_2exp(_mpz, _mpz, -bitCount);
      } else {
        mpz_tdiv_q_2exp(_mpz, _mpz, -bitCount);
      }
    }
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto e = new NumberError(NumberError::ERROR_UNDEFINED);
    delete this;
    return e;
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    auto e = new NumberError(NumberError::ERROR_UNDEFINED);
    delete this;
    return e;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    auto e = new NumberError(NumberError::ERROR_UNDEFINED);
    delete this;
    return e;
  }
  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberInteger::pow(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    if (isZero() && p->isEven() && p->sign() < 0) {
      delete this;
      return new NumberError(NumberError::ERROR_POS_INFINITY);
    }

    mpz_pow_ui(_mpz, _mpz, mpz_get_ui(p->_mpz));
    if (p->sign() < 0) {
      return reciprocal();
    } else {
      return this;
    }
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->pow(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->sign() > 0) {
      auto e = new NumberError(NumberError::ERROR_POS_INFINITY);
      delete this;
      return e;
    } else if (p->sign() < 0) {
      mpz_init_set_si(_mpz, 0);
      return this;
    } else {
      auto e = new NumberError(NumberError::ERROR_UNDEFINED);
      delete this;
      return e;
    }
  }

  Q_ASSERT(0);
  return 0;
}

NumberBase* NumberInteger::neg() {
  mpz_neg(_mpz, _mpz);
  return this;
}

NumberBase* NumberInteger::cmp() {
  mpz_swap(_mpz, NumberInteger(~toUint64())._mpz);
  return this;
}

NumberBase* NumberInteger::abs() {
  mpz_abs(_mpz, _mpz);
  return this;
}
NumberBase* NumberInteger::sqrt() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  if (mpz_perfect_square_p(_mpz)) {
    mpz_sqrt(_mpz, _mpz);
    return this;
  } else {
    auto f = new NumberFloat(this);
    delete this;
    return f->sqrt();
  }
}

NumberBase* NumberInteger::cbrt() {
  mpz_t x;
  mpz_init_set(x, _mpz);
  if (mpz_root(x, x, 3)) {
    mpz_swap(_mpz, x);
    mpz_clear(x);
  }

  mpz_clear(x);
  auto f = new NumberFloat(this);
  delete this;
  return f->cbrt();
}
NumberBase* NumberInteger::factorial() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  mpz_fac_ui(_mpz, mpz_get_ui(_mpz));
  return this;
}

NumberBase* NumberInteger::reciprocal() {
  auto q = new NumberFraction(this);
  delete this;
  return q->reciprocal();
}

NumberBase* NumberInteger::log2() {
  auto f = new NumberFloat(this);
  delete this;
  return f->log2();
}
NumberBase* NumberInteger::log10() {
  auto f = new NumberFloat(this);
  delete this;
  return f->log10();
}
NumberBase* NumberInteger::ln() {
  auto f = new NumberFloat(this);
  delete this;
  return f->ln();
}

NumberBase* NumberInteger::exp2() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp2();
}

NumberBase* NumberInteger::exp10() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp10();
}
NumberBase* NumberInteger::floor() { return this; }
NumberBase* NumberInteger::ceil() { return this; }
NumberBase* NumberInteger::exp() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp10();
}

NumberBase* NumberInteger::bin(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    mpz_bin_ui(_mpz, _mpz, mpz_get_ui(p->_mpz));
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    Q_UNUSED(p);
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    Q_UNUSED(p);
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    Q_UNUSED(p);
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberInteger::sin() {
  auto f = new NumberFloat(this);
  delete this;
  return f->sin();
}
NumberBase* NumberInteger::cos() {
  auto f = new NumberFloat(this);
  delete this;
  return f->cos();
}
NumberBase* NumberInteger::tan() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tan();
}

NumberBase* NumberInteger::asin() {
  auto f = new NumberFloat(this);
  delete this;
  return f->asin();
}
NumberBase* NumberInteger::acos() {
  auto f = new NumberFloat(this);
  delete this;
  return f->acos();
}

NumberBase* NumberInteger::atan() {
  auto f = new NumberFloat(this);
  delete this;
  return f->atan();
}

NumberBase* NumberInteger::sinh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->sinh();
}
NumberBase* NumberInteger::cosh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->cosh();
}
NumberBase* NumberInteger::tanh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tanh();
}
NumberBase* NumberInteger::asinh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->asinh();
}
NumberBase* NumberInteger::acosh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->acosh();
}

NumberBase* NumberInteger::atanh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->atanh();
}
NumberBase* NumberInteger::tgamma() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tgamma();
}

int NumberInteger::compare(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    return mpz_cmp(_mpz, p->_mpz);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    return NumberFloat(this).compare(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    return NumberFraction(this).cmp(p);
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    return -1;
  }
}

NumberBase* NumberInteger::clone() { return new NumberInteger(this); }
bool NumberInteger::isOdd() const { return mpz_odd_p(_mpz); }
bool NumberInteger::isEven() const { return mpz_even_p(_mpz); }
} // namespace detail
