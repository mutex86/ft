// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_INCLUDE_CEP_ORDER_BOOK_ORDER_BOOK_H_
#define FT_INCLUDE_CEP_ORDER_BOOK_ORDER_BOOK_H_

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>

#include "cep/data/tick_data.h"
#include "cep/order_book/limit_order.h"
#include "cep/order_book/price_level.h"

namespace ft::orderbook {

class OrderBook {
 public:
  explicit OrderBook(uint32_t tid) : tid_(tid) {}

  const PriceLevel* best_bid() const {
    if (!buy_levels_.empty()) return &buy_levels_.begin()->second;
    return nullptr;
  }
  const PriceLevel* best_ask() const {
    if (!sell_levels_.empty()) return &sell_levels_.begin()->second;
    return nullptr;
  }
  void to_tick(TickData* tick);

  void add_order(LimitOrder* order);
  void modify_order(LimitOrder* order);
  void remove_order(LimitOrder* order);

 private:
  PriceLevel* find_level(uint64_t decimal_price, bool is_buy) {
    if (is_buy) {
      auto it = buy_levels_.find(decimal_price);
      if (it == buy_levels_.end()) return nullptr;
      return &it->second;
    } else {
      auto it = sell_levels_.find(decimal_price);
      if (it == sell_levels_.end()) return nullptr;
      return &it->second;
    }
  }

  PriceLevel* find_or_create_level(double price, uint64_t decimal_price,
                                   bool is_buy) {
    if (is_buy)
      return __find_or_create_level(&buy_levels_, price, decimal_price);
    else
      return __find_or_create_level(&sell_levels_, price, decimal_price);
  }

  template <class LevelMap>
  static PriceLevel* __find_or_create_level(LevelMap* level_map, double price,
                                            uint64_t decimal_price) {
    PriceLevel* level;
    auto it = level_map->find(decimal_price);
    if (it == level_map->end()) {
      auto res = level_map->emplace(decimal_price, price);
      assert(res.second);
      level = &res.first->second;
    } else {
      level = &it->second;
    }
    return level;
  }

 private:
  uint32_t tid_;
  std::unordered_map<uint64_t, LimitOrder> order_map_;
  std::map<uint64_t, PriceLevel, std::greater<uint64_t>> buy_levels_;
  std::map<uint64_t, PriceLevel, std::less<uint64_t>> sell_levels_;
};

}  // namespace ft::orderbook

#endif  // FT_INCLUDE_CEP_ORDER_BOOK_ORDER_BOOK_H_
