#pragma once
#include <InitialTriangulation/TriangulationStrategy.hpp>



namespace initial_triangulation {
/*
 * алгоритм построения триангуляции минимальной площади описанный в статье
 * Filling Holes in Meshes - Peter Liepa
 * Eurographics Symposium on Geometry Processing(2003)
 * L. Kobbelt, P. Schröder, H. Hoppe (Editors)
 *
 * с помощью идей динамического программирования находит триангуляцию минимальной площади для задонного отверстия
 */
class MinAreaTriangulation : public TriangulationStrategy {
 public:
  std::vector<std::shared_ptr<Vertex>> ProcessHole(
      const TriangulationStrategy::Hole &h,
      const MeshController &m) const override;
  std::string get_all_params()  const override;

  ~MinAreaTriangulation() override = default;
};

}