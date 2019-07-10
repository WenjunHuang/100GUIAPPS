//
// Created by rick on 19-6-24.
//

#pragma once
#include <gmp.h>
#include <type_traits>

template <typename T> class GmpType {};

template <> class GmpType<mpq_t> {
public:
  using Type = std::decay_t<mpq_t>;
  Type value;

  GmpType() { mpq_init(value); }
  ~GmpType() { mpq_clear(value); }

  operator Type() { return value; }
  operator mpq_srcptr() { return value; }

  operator mpq_srcptr() const { return value; }
  operator Type() const { return value; }

  Type operator->() { return value; }
};

template <> class GmpType<mpz_t> {
public:
  using Type = std::decay_t<mpz_t>;
  Type value;
  GmpType() { mpz_init(value); }

  ~GmpType() { mpz_clear(value); }

  operator Type() { return value; }
  operator mpz_srcptr() { return value; }

  Type operator->() { return value; }
};

template <> class GmpType<mpf_t> {
public:
  using Type = std::decay_t<mpf_t>;
  Type value;

  GmpType() { mpf_init(value); }
  ~GmpType() { mpf_clear(value); }

  operator Type() { return value; }
  operator mpf_srcptr() { return value; }

  Type operator->() { return value; }
};

using Mpf = GmpType<mpf_t>;
using Mpz = GmpType<mpz_t>;
using Mpq = GmpType<mpq_t>;
