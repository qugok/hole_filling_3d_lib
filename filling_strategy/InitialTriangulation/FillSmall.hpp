//
// Created by alex on 01.05.2021.
//

#pragma once

#include <InitialTriangulation/TriangulationStrategy.hpp>

namespace initial_triangulation {
/*
 * алгоритм триангуляции ровно как описан в статье
 * https://www.mdpi.com/2076-3417/10/3/969?type=check_update&version=2
 * для отверстий малого размера
 * суть : просто соеднияем все вершины отверстия с их центром масс
 */
class FillSmall : public TriangulationStrategy {
 public:
  std::vector<std::shared_ptr<Vertex>> ProcessHole(
      const TriangulationStrategy::Hole &h,
      const MeshController &m) const override;
  std::string get_all_params() const override;
  ~FillSmall() override = default;
};
}