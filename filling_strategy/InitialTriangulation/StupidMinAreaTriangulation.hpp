#pragma once
#include <InitialTriangulation/TriangulationStrategy.hpp>


namespace initial_triangulation {

/*
 * алгоритм триангуляции ровно как описан в статье
 * https://www.mdpi.com/2076-3417/10/3/969?type=check_update&version=2
 * для отверстий среднего размера
 * основан на идеях лиепры, но написан полностью как указано в статье
 * нет гарантий корректной работы
 */
class StupidMinAreaTriangulation : public TriangulationStrategy {
 public:
  std::vector<std::shared_ptr<Vertex>> ProcessHole(
      const TriangulationStrategy::Hole &h,
      const MeshController &m) const override;
  std::string get_all_params()  const override;
  ~StupidMinAreaTriangulation() override = default;

  static double get_square_of_triangulation(const TriangulationStrategy::Hole &h,
                                            const std::vector<Point> &ps,
                                            const Point &point);
  static std::shared_ptr<Vertex> split_triangle(const MeshController &m,
                                                std::vector<std::shared_ptr<HalfEdge>> &edges_inside_hole,
                                                const std::shared_ptr<Triangle> &t);
  static bool split_condition(std::unordered_map<std::shared_ptr<Vertex>, double> &scale_factor,
                              const std::shared_ptr<Triangle> &t);
  static std::vector<std::shared_ptr<HalfEdge>> swap_edge(const MeshController &m, const std::shared_ptr<HalfEdge> &e);
};
}
