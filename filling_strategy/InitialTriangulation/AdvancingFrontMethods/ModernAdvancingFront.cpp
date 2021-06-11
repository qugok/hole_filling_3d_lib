// /home/alex/mipt/1c/diploma/The_advancing_front_mesh_generation_meth.pdf
// https://www.academia.edu/40278413/The_advancing_front_mesh_generation_method_revisited

#include "ModernAdvancingFront.hpp"
#include "FillSmall.hpp"
#include "tools/MeshController.hpp"
#include <tools/Hole.hpp>
#include <cmath>
#include "../../../static/defines.hpp"

initial_triangulation::advancing_front::ModernAdvancingFront::ModernAdvancingFront(
    size_t limit, double coef)
    : base::BaseAdvancingFront(limit, coef){}

std::vector<std::shared_ptr<Vertex>> initial_triangulation::advancing_front::ModernAdvancingFront::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const TriangulationStrategy::MeshController &m) const {

  Front f(h, m, new_edge_coef);
  return process_hole_front(h, m, f);
}


std::string initial_triangulation::advancing_front::ModernAdvancingFront::get_all_params()  const {

  std::ostringstream out;
  out << base::BaseAdvancingFront::get_all_params() << "\n";
  out << get_static_params();
  return out.str();
}
std::string initial_triangulation::advancing_front::ModernAdvancingFront::get_static_params() {
  std::ostringstream out;
  out << "\t\tModernAdvancingFront:\n\t\t\t";
  out << "vertexes_normal_count:\t" << Front::vertexes_normal_count << "\n\t\t\t";
  out << "alpha:\t" << Front::alpha << "\n\t\t\t";
  out << "beta:\t" << Front::beta << "\n\t\t\t";
  out << "w1:\t" << Front::w1 << "\n\t\t\t";
  out << "w2:\t" << Front::w2 << "\n\t\t\t";
  out << "A_max:\t" << Front::A_max << "\n\t\t\t";
  out << "A_min:\t" << Front::A_min;
  return out.str();
}

initial_triangulation::advancing_front::ModernAdvancingFront::Front::Front(
    const TriangulationStrategy::Hole &h,
    const TriangulationStrategy::MeshController &m, double new_edge_coef)
    : BaseFront(h, m, new_edge_coef) {}

Vector initial_triangulation::advancing_front::ModernAdvancingFront::Front::calc_modern_vertex_normal(
    const std::shared_ptr<Element> &e) const {
  size_t n = std::max(elems.size() / vertexes_normal_count, static_cast<size_t>(1));
  Vector ne = e->calc_hole_normal(n);
  Vector ni = e->v->_end.lock()->get_normal();
//  for debug
//  return ne;
  return (ni * alpha + ne * beta).norm();
}
double initial_triangulation::advancing_front::ModernAdvancingFront::Front::compute_A(
    const std::shared_ptr<Element> &e,
    const Vector &n,
    const Vector &d) {
  auto compute_tabuin_curvinature = [&n, &e]() {
    auto left = Vector(*e->next->v);
    auto right = -Vector(*e->prev->v);
    double first = inner_product(n, left) / left.abs2();
    double second = inner_product(n, right) / right.abs2();

    return first + second;
  };
  double k = compute_tabuin_curvinature();
  double first = w1 * d.abs() * k;
  double second = w2 * inner_product(n, d) / d.abs2();
//  double second = w2 * inner_product(n, d) / d.abs();
  if (first + second > A_max) {
    return A_max;
  } else if (first + second < A_min) {
    return A_min;
  }
  return first + second;
}
Point initial_triangulation::advancing_front::ModernAdvancingFront::Front::compute_new_vertex(
    const std::shared_ptr<Element> &e,
    const Vector &d) {
  Vector normal = calc_modern_vertex_normal(e); // n_c
  double A = compute_A(e, normal, d);
  Vector n_normal = cross_product(normal, cross_product(d, normal)).norm();
  double angle = acos(A);
//  return e->v->_end.lock()->_coordinates + n_normal;
  return e->v->_end.lock()->_coordinates +
      (normal * cos(angle) + n_normal * sin(angle)).norm() * d.abs();
}
