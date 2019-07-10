//
// Created by rick on 19-7-1.
//

#pragma once
class Number;

bool operator==(const Number& lhs, const Number& rhs);
bool operator!=(const Number& lhs, const Number& rhs);
bool operator>=(const Number& lhs, const Number& rhs);
bool operator<=(const Number& lhs, const Number& rhs);
bool operator>(const Number& lhs, const Number& rhs);
bool operator<(const Number& lhs, const Number& rhs);

Number operator+(const Number& lhs, const Number& rhs);
Number operator-(const Number& lhs, const Number& rhs);
Number operator*(const Number& lhs, const Number& rhs);
Number operator/(const Number& lhs, const Number& rhs);
Number operator%(const Number& lhs, const Number& rhs);

Number operator&(const Number& lhs, const Number& rhs);
Number operator|(const Number& lhs, const Number& rhs);
Number operator^(const Number& lhs,const Number& rhs);
Number operator>>(const Number& lhs,const Number& rhs);
Number operator<<(const Number& lhs,const Number& rhs);

Number abs(const Number& x);
Number cbrt(const Number& x);
Number sqrt(const Number& x);
Number pow(const Number& x,const Number& y);

Number sin(const Number& x);
Number cos(const Number& x);
Number tan(const Number& x);
Number asin(const Number& x);
Number tgamma(const Number& x);
Number acos(const Number& x);
Number atan(const Number& x);

Number factorial(const Number& x);

Number log2(const Number& x);
Number log10(const Number& x);
Number ceil(const Number& x);
Number floor(const Number& x);
Number ln(const Number& x);
Number exp2(const Number& x);
Number exp10(const Number& x);
Number exp(const Number& x);