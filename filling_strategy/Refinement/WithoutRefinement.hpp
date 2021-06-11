#pragma once

#include "RefinementStrategy.hpp"

namespace refinement {
/*
 * класс который никак не исправляет отверстие - для дебага или иных вещей
 */
class WithoutRefinement : public RefinementStrategy {
 public:
  void Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
               const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
               const MeshController &m) const override;
  std::string get_all_params()  const override;
  ~WithoutRefinement() override = default;
};
}