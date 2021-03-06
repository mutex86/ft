// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#include <gtest/gtest.h>

#include "cep/order_book/order_book.h"

using namespace ft::orderbook;

TEST(OrderBook, Case_0) {
  OrderBook order_book(1);

  LimitOrder order_0(0, true, 10, 1.00);
  order_book.add_order(&order_0);
  ASSERT_EQ(nullptr, order_book.best_ask());
  ASSERT_EQ(10, order_book.best_bid()->total_volume());

  LimitOrder order_1(0, false, 13, 1.01);
  order_book.add_order(&order_1);
  ASSERT_EQ(13, order_book.best_ask()->total_volume());
}
