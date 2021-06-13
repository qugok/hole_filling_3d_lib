#pragma once

#include <set>
#include <valarray>
#include "TriangulationStrategy.hpp"
#include "BaseAdvancingFront.hpp"

namespace initial_triangulation::advancing_front{
class AdvancingFront : public base::BaseAdvancingFront {
  class Front;
 public:
  explicit AdvancingFront(size_t limit = -1, double coef = sqrt(4));
  [[nodiscard]] std::vector<std::shared_ptr<Vertex>> ProcessHole(const TriangulationStrategy::Hole &h,
                                                   const MeshController &m) const override;
  ~AdvancingFront() override = default;
};

 class AdvancingFront::Front : public base::BaseAdvancingFront::BaseFront{

 public:
  Front(const Hole &h, const MeshController &m, double new_edge_coef);
};
}