#pragma once
#include "TriangulationStrategy.hpp"


namespace initial_triangulation {
/*
 * класс который никак не меняет отверстие - для дебага или иных вещей
 */
class WithoutTriangulation : public TriangulationStrategy {
 public:
  std::vector<std::shared_ptr<Vertex>> ProcessHole(const Hole &h, const MeshController &m) const override;
  ~WithoutTriangulation() override = default;
  std::string get_all_params()  const override;
};
}