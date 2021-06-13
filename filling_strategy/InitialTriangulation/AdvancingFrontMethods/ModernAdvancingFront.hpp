#pragma once

#include <set>
#include <sstream>
#include "TriangulationStrategy.hpp"
#include "BaseAdvancingFront.hpp"

namespace initial_triangulation::advancing_front {
/*
 * Класс реализации модернизированного наступательного фронта
 */
class ModernAdvancingFront : public base::BaseAdvancingFront {
  class Front;
 public:
  explicit ModernAdvancingFront(size_t limit = -1, double coef = sqrt(3));
  std::vector<std::shared_ptr<Vertex>> ProcessHole(const TriangulationStrategy::Hole &h,
                                                   const MeshController &m) const override;
  std::string get_all_params() const override;
  static std::string get_static_params();

};

/*
 * класс самого наступательного фронта
 * по сути изменена только функция получения новой вершины
 */
class ModernAdvancingFront::Front : public base::BaseAdvancingFront::BaseFront {
 public:
  //вынести константы в конструктор

  // доля вершин, которую берут для вычисления нормали отверстия для вершины
  static const int vertexes_normal_count = 10;
  // коефициенты для расчета умной нормали вершины сумма должна быть равна 1
  // при изначальной нормали
  static constexpr double alpha = 0.99;
//  static constexpr double alpha = 0.5;
  // при нормали отверсия
  static constexpr double beta = 0.01;
//  static constexpr double beta = 0.5;

  // веса для константы A

  static constexpr double w1 = 0.15;
  static constexpr double w2 = 0.85;
  static constexpr double A_max = 0.15;
  static constexpr double A_min = -0.15;
 public:
  Front(const Hole &h, const MeshController &m, double new_edge_coef);

 private:

  double compute_A(const std::shared_ptr<Element> &e, const Vector &n, const Vector &d);
  Point compute_new_vertex(const std::shared_ptr<Element> &e, const Vector &d) override;
  Vector calc_modern_vertex_normal(const std::shared_ptr<Element> &e) const;
};
}