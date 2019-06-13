//
// Created by rick on 19-6-14.
//

#pragma once
#include "NumberBase.h"

class Number;
namespace detail {
class NumberFloat : public NumberBase {
    friend  class ::Number;
    friend class NumberError;
    friend class NumberInteger;
    friend class NumberFaction;

public:
    explicit NumberFloat(const QString& s);
    explicit NumberFloat(double value);
};
}



