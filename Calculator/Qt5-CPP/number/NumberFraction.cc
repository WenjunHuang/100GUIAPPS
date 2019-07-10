//
// Created by rick on 19-6-15.
//

#include "NumberFraction.h"
#include "NumberFloat.h"
#include "NumberInteger.h"
#include <memory>

namespace detail {
bool NumberFraction::DefaultFractionalInput;
bool NumberFraction::DefaultFractionalOutput;
bool NumberFraction::SplitoffIntegerForFractionOutput;

void NumberFraction::setDefaultFractionalInput(bool value) {
  DefaultFractionalInput = value;
}
void NumberFraction::setDefaultFractionalOutput(bool value) {
  DefaultFractionalOutput = value;
}
void NumberFraction::setSplitoffIntegerForFractionOutput(bool value) {
  SplitoffIntegerForFractionOutput = value;
}
NumberFraction::NumberFraction(const QString& s) {
  mpq_set_str(_mpq, s.toLatin1(), 10);
  mpq_canonicalize(_mpq);
}
NumberFraction::NumberFraction(qint64 num, quint64 den) {
  mpq_set_si(_mpq, num, den);
  mpq_canonicalize(_mpq);
}
NumberFraction::NumberFraction(quint64 num, quint64 den) {
  mpq_set_ui(_mpq, num, den);
  mpq_canonicalize(_mpq);
}
NumberFraction::NumberFraction(mpq_t mpq) { mpq_set(_mpq, mpq); }
NumberFraction::~NumberFraction() {}

NumberBase* NumberFraction::clone() { return new NumberFraction(this); }
QString NumberFraction::toString(int precision) const {
  if (NumberFraction::DefaultFractionalOutput) {
    NumberInteger integerPart(this);
    if (SplitoffIntegerForFractionOutput && !integerPart.isZero()) {
      GmpType<mpz_t> num;
      mpq_get_num(num, _mpq);

      NumberInteger integerPart1(this);

      mpz_mul(integerPart._mpz, integerPart._mpz, mpq_denref(_mpq));
      mpz_sub(num, num, integerPart._mpz);

      if (mpz_sgn(num) < 0) {
        mpz_neg(num, num);
      }

      auto size = gmp_snprintf(nullptr, 0, "%Zd %Zd%Zd", integerPart1._mpz, num,
                               mpq_denref(_mpq)) +
                  1;
      auto buf = std::make_unique<char[]>(size);
      gmp_snprintf(buf.get(), size, "%Zd %Zd/%Zd", integerPart1._mpz, num,
                   mpq_denref(_mpq));

      return QLatin1String(buf.get());
    } else {
      GmpType<mpz_t> num;
      mpq_get_num(num, _mpq);

      auto size =
          gmp_snprintf(nullptr, 0, "%Zd/%Zd", num, mpq_denref(_mpq)) + 1;
      auto buf = std::make_unique<char[]>(size);
      gmp_snprintf(buf.get(), size, "%Zd/%Zd", num, mpq_denref(_mpq));

      return QLatin1String(buf.get());
    }
  } else {
    return NumberFloat(this).toString(precision);
  }
}
quint64 NumberFraction::toUint64() const {
  return NumberInteger(this).toUint64();
}
qint64 NumberFraction::toInt64() const { return NumberInteger(this).toInt64(); }
bool NumberFraction::isInteger() const {
  return mpz_cmp_ui(mpq_denref(_mpq.value), 1) == 0;
}
bool NumberFraction::isZero() const { return mpq_sgn(_mpq) == 0; }
int NumberFraction::sign() const { return mpq_sgn(_mpq); }

NumberBase* NumberFraction::add(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFraction q(p);
    mpq_add(_mpq, _mpq, q._mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->add(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    mpq_add(_mpq, _mpq, p->_mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    auto e = new NumberError(p);
    delete this;
    return e;
  }

  Q_ASSERT(0);
  return nullptr;
}

NumberBase* NumberFraction::sub(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFraction q(p);
    mpq_sub(_mpq, _mpq, q._mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->sub(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    mpq_sub(_mpq, _mpq, p->_mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    auto e = new NumberError(p);
    delete this;
    return e->neg();
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberFraction::mul(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFraction q(p);
    mpq_mul(_mpq, _mpq, q._mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto q = new NumberFloat(this);
    delete this;
    return q->mul(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    mpq_mul(_mpq, _mpq, p->_mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (isZero()) {
      delete this;
      auto e = new NumberError(NumberError::ERROR_UNDEFINED);
      return e;
    }

    if (sign() < 0) {
      delete this;
      auto e = new NumberError(p);
      return e->neg();
    } else {
      delete this;
      auto e = new NumberError(p);
      return e;
    }
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberFraction::div(NumberBase* rhs) {
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
    NumberFraction f(p);
    return div(&f);
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    auto f = new NumberFloat(this);
    delete this;
    return f->div(p);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    mpq_div(_mpq, _mpq, p->_mpq);
    return this;
  } else if (auto p = dynamic_cast<NumberError*>(rhs)) {
    if (p->sign() > 0) {
      delete this;
      return new NumberInteger(0);
    } else if (p->sign() < 0) {
      delete this;
      return new NumberInteger(0);
    }

    auto e = new NumberError(p);
    delete this;
    return e;
  }

  Q_ASSERT(0);
  return nullptr;
}
NumberBase* NumberFraction::mod(NumberBase* rhs) {
  if (rhs->isZero()) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  // NOTE: Don't support modulus operations with non-integer operands
  mpq_set_d(_mpq, 0);
  return this;
}

NumberBase* NumberFraction::bitwiseAnd(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;

  // NOTE: Don't support bitwise operations with non-integer operands
  return new NumberInteger(0);
}

NumberBase* NumberFraction::bitwiseXor(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberInteger(0);
}

NumberBase* NumberFraction::bitwiseOr(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberInteger(0);
}

NumberBase* NumberFraction::bitwiseShift(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}

NumberBase* NumberFraction::pow(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    Mpz num;
    Mpz den;

    mpq_get_num(num, _mpq);
    mpq_get_den(den, _mpq);

    mpz_pow_ui(num, num, mpz_get_ui(p->_mpz));
    mpz_pow_ui(den, den, mpz_get_ui(p->_mpz));
    mpq_set_num(_mpq, num);
    mpq_set_den(_mpq, den);
    mpq_canonicalize(_mpq);

    if (p->sign() < 0) {
      return reciprocal();
    } else {
      return this;
    }
  } else if (auto p = dynamic_cast<NumberFloat*>(rhs)) {
    Q_UNUSED(p);
    auto f = new NumberFloat(this);
    delete this;
    return f->pow(rhs);
  } else if (auto p = dynamic_cast<NumberFraction*>(rhs)) {
    if (mpz_cmpabs_ui(mpq_numref(_mpq), 1000000) > 0 ||
        mpz_cmpabs_ui(mpq_denref(_mpq), 1000000) > 0 ||
        mpz_cmpabs_ui(mpq_numref(p->_mpq), 1000000) > 0 ||
        mpz_cmpabs_ui(mpq_denref(p->_mpq), 1000000) > 0) {
      auto f = new NumberFloat(this);
      delete this;
      return f->pow(rhs);
    }

    Mpz lhsNum;
    Mpz lhsDen;
    Mpz rhsNum;
    Mpz rhsDen;

    mpq_get_num(lhsNum, _mpq);
    mpq_get_den(lhsDen, _mpq);
    mpq_get_num(rhsNum, p->_mpq);
    mpq_get_den(rhsDen, p->_mpq);

    mpz_pow_ui(lhsNum, lhsNum, mpz_get_ui(rhsNum));
    mpz_pow_ui(lhsDen, lhsDen, mpz_get_ui(rhsNum));

    if (mpz_sgn(lhsNum.value) < 0 && mpz_even_p(rhsDen.value)) {
      delete this;
      return new NumberError(NumberError::ERROR_UNDEFINED);
    }

    if (mpz_sgn(lhsDen) < 0 && mpz_even_p(rhsDen)) {
      delete this;
      return new NumberError(NumberError::ERROR_UNDEFINED);
    }

    const int n1 = mpz_root(lhsNum, lhsNum, mpz_get_ui(rhsDen));
    const int n2 = mpz_root(lhsDen, lhsDen, mpz_get_ui(rhsDen));

    if (n1 && n2) {
      mpq_set_num(_mpq, lhsNum);
      mpq_set_den(_mpq, lhsDen);
      mpq_canonicalize(_mpq);

      if (p->sign() < 0) {
        return reciprocal();
      } else {
        return this;
      }
    } else {
      auto f = new NumberFloat(this);
      delete this;
      return f->pow(rhs);
    }
  }
}
NumberBase* NumberFraction::neg() {
  mpq_neg(_mpq, _mpq);
  return this;
}
NumberBase* NumberFraction::cmp() {
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}
NumberBase* NumberFraction::abs() {
  mpq_abs(_mpq, _mpq);
  return this;
}

NumberBase* NumberFraction::sqrt() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  if (mpz_perfect_square_p(mpq_numref(_mpq)) &&
      mpz_perfect_square_p(mpq_denref(_mpq))) {
    Mpz num;
    Mpz den;

    mpq_get_num(num, _mpq);
    mpq_get_den(den, _mpq);
    mpz_sqrt(num, num);
    mpz_sqrt(den, den);
    mpq_set_num(_mpq, num);
    mpq_set_den(_mpq, den);
    mpq_canonicalize(_mpq);

    return this;
  } else {
    auto f = new NumberFloat(this);
    delete this;
    return f->sqrt();
  }
}

NumberBase* NumberFraction::cbrt() {
  Mpz num;
  Mpz den;

  mpq_get_num(num, _mpq);
  mpq_get_den(den, _mpq);

  if (mpz_root(num, num, 3) && mpz_root(den, den, 3)) {
    mpq_set_num(_mpq, num);
    mpq_set_den(_mpq, den);
    mpq_canonicalize(_mpq);

    return this;
  } else {
    auto f = new NumberFloat(this);
    delete this;
    return f->cbrt();
  }
}
NumberBase* NumberFraction::factorial() {
  if (sign() < 0) {
    delete this;
    return new NumberError(NumberError::ERROR_UNDEFINED);
  }

  auto i = new NumberInteger(this);
  delete this;
  return i->factorial();
}
NumberBase* NumberFraction::reciprocal() {
  mpq_inv(_mpq, _mpq);
  return this;
}
NumberBase* NumberFraction::log2() {
  auto f = new NumberFloat(this);
  delete this;
  return f->log2();
}
NumberBase* NumberFraction::log10() {
  auto f = new NumberFloat(this);
  delete this;
  return f->log10();
}
NumberBase* NumberFraction::ln() {
  auto f = new NumberFloat(this);
  delete this;
  return f->ln();
}
NumberBase* NumberFraction::exp2() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp2();
}
NumberBase* NumberFraction::exp10() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp10();
}
NumberBase* NumberFraction::floor() {
  auto f = new NumberFloat(this);
  delete this;
  return f->floor();
}

NumberBase* NumberFraction::ceil() {
  auto f = new NumberFloat(this);
  delete this;
  return f->ceil();
}
NumberBase* NumberFraction::exp() {
  auto f = new NumberFloat(this);
  delete this;
  return f->exp();
}
NumberBase* NumberFraction::bin(NumberBase* rhs) {
  Q_UNUSED(rhs);
  delete this;
  return new NumberError(NumberError::ERROR_UNDEFINED);
}

NumberBase* NumberFraction::sin() {
  auto f = new NumberFloat(this);
  delete this;
  return f->sin();
}
NumberBase* NumberFraction::cos() {
  auto f = new NumberFloat(this);
  delete this;
  return f->cos();
}

NumberBase* NumberFraction::tan() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tan();
}
NumberBase* NumberFraction::asin() {
  auto f = new NumberFloat(this);
  delete this;
  return f->asin();
}
NumberBase* NumberFraction::acos() {
  auto f = new NumberFloat(this);
  delete this;
  return f->acos();
}

NumberBase* NumberFraction::atan() {
  auto f = new NumberFloat(this);
  delete this;
  return f->atan();
}
NumberBase* NumberFraction::sinh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->sinh();
}
NumberBase* NumberFraction::cosh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->cosh();
}
NumberBase* NumberFraction::tanh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tanh();
}
NumberBase* NumberFraction::asinh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->asinh();
}
NumberBase* NumberFraction::acosh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->acosh();
}
NumberBase* NumberFraction::atanh() {
  auto f = new NumberFloat(this);
  delete this;
  return f->atanh();
}
NumberBase* NumberFraction::tgamma() {
  auto f = new NumberFloat(this);
  delete this;
  return f->tgamma();
}
int NumberFraction::compare(NumberBase* rhs) {
  if (auto p = dynamic_cast<NumberInteger*>(rhs)) {
    NumberFraction f(p);
    return mpq_cmp(_mpq, f._mpq);
  }
}

NumberInteger* NumberFraction::numerator() const {
  GmpType<mpz_t> num;
  mpq_get_num(num, _mpq);
  auto n = new NumberInteger(num);
  return n;
}

NumberInteger* NumberFraction::denominator() const {
  GmpType<mpz_t> den;
  mpq_get_den(den, _mpq);
  auto n = new NumberInteger(den);
  return n;
}

NumberFraction::NumberFraction(const NumberInteger* value) {
  mpq_set_z(_mpq, value->_mpz);
}
NumberFraction::NumberFraction(const NumberFraction* value) {
  mpq_set(_mpq, value->_mpq);
}
NumberFraction::NumberFraction(const NumberError* value) {}
}