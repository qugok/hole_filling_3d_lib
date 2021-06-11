#include <cmath>
#include "Tools.hpp"

Vector fairing::tools::weighted_umbrella_operator(
    const std::function<double(const std::shared_ptr<Vertex> &, const std::shared_ptr<Vertex> &)> &weight,
    const std::shared_ptr<Vertex> &v) {
  Point ans;
  // weights sum
  double norm_coef = 0;
  for (const auto &ver : v->neighbor_vertices()) {
    double weigh = weight(v, ver);
    ans += ver->_coordinates * weigh;
    norm_coef += weigh;
  }
  if (norm_coef != 0) {
    ans /= norm_coef;
  }
  ans -= v->_coordinates;
  return ans;
}

Point fairing::tools::second_order_weighted_umbrella_operator(
    const std::function<double(const std::shared_ptr<Vertex> &, const std::shared_ptr<Vertex> &)> &weight,
    const std::shared_ptr<Vertex> &v) {
  Point ans;
  // weights sum
  double norm_coef = 0;
  for (const auto &ver : v->neighbor_vertices()) {
    double weigh = weight(v, ver);
    ans += weighted_umbrella_operator(weight, ver) * weigh;
    norm_coef += weigh;
  }
  ans /= norm_coef;
  ans -= weighted_umbrella_operator(weight, v);
  return ans;
}
void fairing::tools::add(const std::vector<std::shared_ptr<Vertex>> &vertices, const std::vector<Vector> &delta) {
  if (vertices.size() != delta.size()){
    throw "error";
  }
  for (int i = 0; i < vertices.size(); ++i){
    vertices[i]->_coordinates += delta[i];
  }
}

Point fairing::uniform_umbrella_operator(const std::shared_ptr<Vertex>& v){
  return tools::weighted_umbrella_operator(weight_funcs::uniform_weight_func, v);
}

Point fairing::scale_dependent_umbrella_operator(const std::shared_ptr<Vertex>& v){
  return tools::weighted_umbrella_operator(weight_funcs::scale_dependent_weight_func, v);
}

Point fairing::harmonic_umbrella_operator(const std::shared_ptr<Vertex>& v){
  return tools::weighted_umbrella_operator(weight_funcs::harmonic_weight_func, v);
}
double fairing::weight_funcs::uniform_weight_func(const std::shared_ptr<Vertex> &a, const std::shared_ptr<Vertex> &b) {
  return 1;
}
double fairing::weight_funcs::scale_dependent_weight_func(const std::shared_ptr<Vertex> &a,
                                                          const std::shared_ptr<Vertex> &b) {
  return (a->_coordinates - b->_coordinates).abs();
}

// порядок обхода точек против часовой, если смотреть снаружи фигуры a d b c
double fairing::weight_funcs::harmonic_weight_func(const std::shared_ptr<Vertex> &a, const std::shared_ptr<Vertex> &b) {
  auto le = a->find_edge_to(b);
  auto re = le->_opposite.lock();
  auto lt = le->_owner.lock();
  auto rt = re->_owner.lock();
  auto c = lt->get_opposite(le);
  auto d = rt->get_opposite(re);
  auto lctg = get_cos(b->_coordinates-c->_coordinates, a->_coordinates-c->_coordinates) / get_sin(b->_coordinates-c->_coordinates, a->_coordinates-c->_coordinates);
  auto rctg = get_cos(b->_coordinates-d->_coordinates, a->_coordinates-d->_coordinates) / get_sin(b->_coordinates-d->_coordinates, a->_coordinates-d->_coordinates);
  return  (std::isnan(lctg) ? 0 : lctg) + (std::isnan(rctg) ? 0 : rctg);
}
