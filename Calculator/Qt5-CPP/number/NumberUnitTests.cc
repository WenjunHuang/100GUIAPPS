//
// Created by rick on 19-6-23.
//

#include "NumberUnitTests.h"
#include <gtest/gtest.h>
#include "NumberInteger.h"


using namespace detail;
TEST(NumberIntegerTest,ConstructIntegerFromString) {
  auto first = new detail::NumberInteger("1");
  EXPECT_EQ(1,first->toInt64());
}

TEST(NumberIntegerTest,MultiplyErrorShouldReturnError) {
  auto first = new NumberInteger("1");
  auto result = first->mul(new NumberError(NumberError::ERROR_POS_INFINITY));

  EXPECT_TRUE(dynamic_cast<NumberError *>(result));
}

TEST(NUmberIntegerTest,Cmp) {
  auto testee = new NumberInteger("1");
  auto result = dynamic_cast<NumberInteger*>(testee->cmp());

  EXPECT_TRUE(result);
  EXPECT_EQ(0xfffffffffffffffe,result->toUint64());
}