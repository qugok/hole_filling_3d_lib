#pragma once

#include "FairingStrategy.hpp"

namespace fairing {
/*
 * класс который никак не сглаживает отверстие - для дебага или иных вещей
 */
class WithoutFairing : public FairingStrategy {
 public:
  void Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
               const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
               const MeshController &m) const override;
  std::string get_all_params()  const override;
};
}