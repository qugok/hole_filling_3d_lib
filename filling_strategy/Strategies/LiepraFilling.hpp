#pragma once
#include "ClassicFillingStrategy.hpp"
#include "StupidUmbrella.hpp"
namespace strategies {

/*
 * стратегия заделывания из статьи Liepra
 * сначала триангуляция минимальной площади
 * потом исправление её до delone-like
 * потом сглаживание с помощью fairing_strategy
 * по умолчанию UniformUmbrella
 */
class LiepraFilling : public ClassicFillingStrategy{
 public:
  explicit LiepraFilling(fairing::FairingStrategy* fairing_strategy = new fairing::StupidUniformUmbrella());
  ~LiepraFilling() override = default;
  std::string get_all_params()  const override;

};
}