//
// Created by rick on 19-6-13.
//

#pragma once

#include <QString>
#include <QtGlobal>
#include "NumberInteger.h"


namespace detail {
    class NumberBase;
}

class Number {
private:
    friend bool operator==(const Number& lhs,const Number& rhs);
    friend bool operator!=(const Number& lhs,const Number& rhs);
    friend bool operator>=(const Number& lhs,const Number& rhs);
    friend bool operator<=(const Number& lhs,const Number& rhs);
    friend bool operator>(const Number& lhs,const Number& rhs);
    friend bool operator<(const Number& lhs,const Number& rhs);

public:
    enum Type {
        TYPE_ERROR,
        TYPE_INTEGER,
        TYPE_FLOAT,
        TYPE_FRACTION
    };

public:
    // useful constants
    static const Number Zero;
    static const Number One;
    static const Number NegOne;
    static const Number PosInfinity;
    static const Number NegInfinity;
    static const Number NaN;

public:
    static Number Pi();
    static Number Euler();

public:
    Number();
    explicit Number(const QString &s);

    explicit Number(qint32 value);
    explicit Number(qint64 value);
    explicit Number(quint32 value);
    explicit Number(quint64 value);

    explicit Number(long double value);

    Number(qint64 num,quint64 den);
    Number(quint64 num,quint64 den);

    Number(const Number& other);

    ~Number();

public:
    Type type() const;

public:
    // assignment
    Number& operator=(const Number& rhs);

public:
    // basic math operators
    Number& operator+=(const Number& rhs);
    Number& operator-=(const Number& rhs);
    Number& operator*=(const Number& rhs);
    Number& operator/=(const Number& rhs);
    Number& operator%=(const Number& rhs);

public:
    // bitwise operators
    Number& operator&=(const Number& rhs);
    Number& operator|=(const Number& rhs);
    Number& operator^=(const Number& rhs);
    Number& operator<<=(const Number& rhs);
    Number& operator>>=(const Number& rhs);

public:
    Number operator-() const;
    Number operator~() const;

public:
    Number integerPart() const;
public:
    QString toQString(int width = -1,int precision = -1) const;
    quint64 toUint64() const;
    qint64 toInt64() const;

public:
    Number abs() const;
    Number cbrt() const;
    Number sqrt() const;
    Number pow(const Number& x) const;

    Number sin() const;
    Number cos() const;
    Number tan() const;
    Number asin() const;
    Number acos() const;
    Number atan() const;
    Number sinh() const;
    Number cosh() const;
    Number tanh() const;
    Number asinh() const;
    Number acosh() const;
    Number atanh() const;
    Number tgamma() const;
    Number factorial() const;

    Number log2() const;
    Number log10() const;
    Number ln() const;
    Number floor() const;
    Number ceil() const;
    Number exp2() const;
    Number exp10() const;
    Number exp() const;
    Number bin(const Number& x) const;

public:
    static void setDefaultFloatPrecision(int precision);
    static void setSplitoffIntegerForFractionOutput(bool x);
    static void setDefaultFractionalInput(bool x);
    static void setDefaultFloatOutput(bool x);
    static void setGroupSeparator(const QString& ch);
    static void setDecimalSeparator(const QString& ch);

    static QString groupSeparator();
    static QString decimalSeparator();

public:
    void swap(Number& other);

private:
    void simplify();

private:
    detail::NumberBase *value;

private:
    static QString GroupSeparator;
    static QString DecimalSeparator;
};
