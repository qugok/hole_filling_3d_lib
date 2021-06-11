#pragma once
#include <valarray>
#include "RefinementStrategy.hpp"
#include "unordered_set"

namespace refinement {
/*
 * исправления заделанного отверстия с помощью приведения его к виду delone-like триангуляции
 *
 * предложен Liepa в
     * Filling Holes in Meshes - Peter Liepa
     * Eurographics Symposium on Geometry Processing(2003)
     * L. Kobbelt, P. Schröder, H. Hoppe (Editors)
 * чтобы после триангуляции минимальной площади сделать сетку похожей по размерам треугольников на изначальную сетку
 *
 * рекомендуется использовать после MinAreaTriangulation
 *
 * не рекомендуется после всех остальных - там треугольки итак почти все правильные
 * и изначально алгоритм вообще не предназначен для таких вещей
 */
class DeloneLike : public RefinementStrategy{
  /*
   * коефициент "density control factor" -
   * определяет до какого момента будут происходить split triangles
   * чтобы треугольники не получались слишком малого размера
   * эмпирически найдено Liepra в его статье значение sqrt(2) - менять не рекомендуется
   */
  static constexpr double alpha2 = 2;  // alpha^2 ; alpha = sqrt(2)
 public:

  /*
   * limit - максимальное колличество итераций алгоритма
   * число итераций split triangles и swap edges
   */
  explicit DeloneLike(size_t limit = 1000);

  void Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
               const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
               const MeshController &m) const override;
  std::string get_all_params()  const override;

 private:
  static void compute_scale_factor(std::unordered_map<std::shared_ptr<Vertex>, double>& scale_factor,
                            const std::vector<std::shared_ptr<Vertex>> &inside_hole,
                            const std::vector<std::shared_ptr<Vertex>> &hole_boundary) ;

  static bool split_triangles(std::unordered_map<std::shared_ptr<Vertex>, double>& scale_factor,
                       std::vector<std::shared_ptr<Vertex>> &inside_hole,
                       std::vector<std::weak_ptr<Triangle>> &triangles_inside,
                              const BaseFillingStrategy::MeshController &m);

  static bool swap_edge(
      const MeshController &m,
      std::vector<std::weak_ptr<Triangle>> &triangles_inside,
      const std::shared_ptr<HalfEdge> &e);

  static bool swap_edges(
      const MeshController &m,
      std::vector<std::weak_ptr<Triangle>> &triangles_inside,
      const std::unordered_set<std::shared_ptr<HalfEdge>>& boundary);

 private:
  size_t limit;

};
}