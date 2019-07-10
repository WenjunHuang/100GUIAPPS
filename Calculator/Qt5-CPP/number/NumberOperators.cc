//
// Created by rick on 19-7-1.
//

#include "NumberOperators.h"
#include "Number.h"
#include "NumberBase.h"
#include <QDebug>

Number operator+(const Number&lhs,const Number& rhs) {
  Number x(lhs);
}

Number operator-(const Number& lhs,const Number& rhs){
  Number x(lhs);
  x -= rhs;
  return x;
}

Number operator*(const Number& lhs,const Number& rhs) {
  Number x(lhs);
  x *= rhs;
  return x;
}

Number operator/(const Number& lhs,const Number& rhs) {
  Number x(lhs);
  x /= rhs;
  return x;
}

Number operator%(const Number& lhs,const Number& rhs) {
  Number x(lhs);
  x %= rhs;
  return x;
}

Number operator&(const Number& lhs,const Number& rhs) {
  Number x(lhs);
  x &= rhs;
  return x;
}

Number operator|(const Number& lhs,const Number& rhs) {
  Number x(lhs);
  x |= rhs;
  return x;
}

Number operator^(const Number&lhs,const Number& rhs) {
  Number x(lhs);
  x ^= rhs;
  return x;
}

Number operator>>(const Number&lhs,const Number& rhs) {
  Number x(lhs);
  x >>= rhs;
  return x;
}