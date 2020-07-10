// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_INCLUDE_CORE_ACCOUNT_H_
#define FT_INCLUDE_CORE_ACCOUNT_H_

#include <cstdint>

namespace ft {

/*
 * total_asset = cash + margin + fronzen
 * total_asset: 总资产
 * cash: 可用资金，可用于购买证券资产的资金
 * margin: 保证金，对于股票来说就是持有的股票资产（不考虑融资融券，还不支持）
 * fronzen: 冻结资金，未成交的订单也需要预先占用资金
 */
struct Account {
  uint64_t account_id;  // 资金账户号
  double total_asset;   // 总资产
  double cash;          // 可用资金
  double margin;        // 保证金
  double frozen;        // 冻结金额
};

class FundAccount {
 public:
  bool reserve(double amount);
  void release_reserved(double amount);

  bool freeze(double amount);
  void release_frozen(double amount);

 private:
  uint64_t account_id_;  // 账户ID，用于标识一个Account
  double total_asset_;  // 总资产，包括现金资产+证券资产+冻结资产
  double cash_;         // 现金，可用于购买证券资产的金额
  double reserved_;     // 预留资金
  double float_pnl_;     // 当前浮动盈亏
  double realized_pnl_;  // 当日已实现盈亏
};

}  // namespace ft

#endif  // FT_INCLUDE_CORE_ACCOUNT_H_
