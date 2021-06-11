#pragma once

#include "TriangulationStrategy.hpp"
#include "MeshController.hpp"
#include "cmath"
#include "set"

namespace initial_triangulation::advancing_front {
namespace base {
/*
 * базовый класс для всех реализаций метода наступательного фронта - классическиго и модернизированного
 */
class BaseAdvancingFront : public TriangulationStrategy {
 protected:
  class BaseFront;

 public:
  explicit BaseAdvancingFront(size_t limit = -1, double coef = sqrt(3));
  [[nodiscard]] std::vector<std::shared_ptr<Vertex>> ProcessHole(const Hole &h, const MeshController &m) const override;
  ~BaseAdvancingFront() override = default;
  std::string get_all_params()  const override;
 protected:
  virtual std::vector<std::shared_ptr<Vertex>> process_hole_front(const Hole &h, const MeshController &m, BaseFront& f) const;

 protected:
  size_t limit;
  double new_edge_coef;
};


/*
 * базовый класс для наступательного фронта
 */
class BaseAdvancingFront::BaseFront {
  friend BaseAdvancingFront;
 protected:
  // число вершин при которых продвижение с помощью данного метода может остановиться
  static const int vertexes_count_end = 100;
  static const bool min_edge_len = true;
  /*
   * идейно элемент это вершина и ребро границы в неё приходящее
   * обход по часовой
   */
  struct Element {
    bool operator<(const Element &other) const;
    [[nodiscard]] double calc_angle() const;
    [[nodiscard]] Vector calc_hole_normal(size_t n) const;

    std::shared_ptr<HalfEdge> v;
    std::shared_ptr<Element> next, prev;
  };
 public:
  BaseFront(const Hole &h, const MeshController &m, double new_edge_coef);
  BaseFront& operator= (BaseFront& b) = default;
  explicit operator Hole();
  void fill(size_t limited = -1);
  bool step();
  std::vector<std::shared_ptr<Vertex>> get_vertecies_inside() const;

 protected:
  bool check_is_full();

  virtual Point compute_new_vertex(const std::shared_ptr<Element> &e, const Vector &d);
  virtual double get_edge_len(double left, double right);

  virtual void zero_vertex_case(const std::shared_ptr<Element> &prev,
                        const std::shared_ptr<Element> &cur,
                        const std::shared_ptr<Element> &next);
  virtual void one_vertex_case(const std::shared_ptr<Element> &prev,
                       const std::shared_ptr<Element> &cur,
                       const std::shared_ptr<Element> &next);
  virtual void two_vertex_case(const std::shared_ptr<Element> &prev,
                       const std::shared_ptr<Element> &cur,
                       const std::shared_ptr<Element> &next);

  struct cmp {
    bool operator()(const std::shared_ptr<Element> &a, const std::shared_ptr<Element> &b) const;
  };

  double new_edge_coef;
  double avg_area;
  double edge_min_len = 0;
  std::set<std::shared_ptr<Element>, cmp> elems;
  const MeshController &m;
  std::vector<std::shared_ptr<Vertex>> vertices_inside_hole;
};
}
}