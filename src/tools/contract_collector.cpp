// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#include <spdlog/spdlog.h>

#include <getopt.hpp>

#include "cep/data/contract_table.h"
#include "cep/interface/gateway.h"
#include "cep/oms/config_loader.h"

class ContractCollector : public ft::OMSInterface {
 public:
  bool login(const ft::Config& config) {
    gateway_.reset(ft::create_gateway(config.api));
    if (!gateway_) return false;

    return gateway_->login(this, config);
  }

  bool dump(const std::string& file = "./contracts.csv") {
    if (!gateway_->query_contracts(&contracts_)) return false;
    ft::store_contracts(file, contracts_);
    return true;
  }

 private:
  std::unique_ptr<ft::Gateway> gateway_;
  std::vector<ft::Contract> contracts_;
};

int main() {
  std::string login_yml = getarg("../config/login.yml", "--config");
  std::string output = getarg("./contracts.csv", "--output");
  std::string loglevel = getarg("info", "--loglevel");

  spdlog::set_level(spdlog::level::from_str(loglevel));

  ContractCollector collector;
  ft::Config config;
  ft::load_config(login_yml, &config);
  if (!collector.login(config)) {
    printf("failed to login\n");
    exit(-1);
  }

  if (!collector.dump(output)) {
    printf("failed to dump\n");
  }

  printf("successfully dump to %s\n", output.c_str());
  exit(0);
}
