#include "base.hpp"

Vertex::Vertex(const Point &coordinates, const std::vector<std::weak_ptr<HalfEdge>> &outgoing_edges)
    : _coordinates(coordinates), _outgoing_edges(outgoing_edges) {}
void Vertex::add_edge(const std::shared_ptr<HalfEdge> &e) {
  _outgoing_edges.push_back(e);
}

Vertex::Vertex() {}

std::shared_ptr<HalfEdge> Vertex::find_edge_to(const std::shared_ptr<Vertex> &to) const {
//  if (
//      ((to->_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//          (_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4) || (
//          (_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//              (to->_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4)
//      ) {
//    std::cout << "our edge null opposite\n";
//  }
  for (const auto &e : this->_outgoing_edges) {
    if (not e.expired() and e.lock()->_end.lock() == to) {
      e.lock()->fix_opposite(); // костыль для поддержания корректности после delone refinement
      return e.lock();
    }
  }
//  if (
//      ((to->_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//          (_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4) || (
//          (_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//              (to->_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4)
//      ) {
//    std::cout << "our edge null opposite\n";
//  }
  return nullptr;
}

std::shared_ptr<HalfEdge> Vertex::get_edge_to(const std::shared_ptr<Vertex> &to) const {
//  if (
//      ((to->_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//          (_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4) || (
//          (_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//              (to->_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4)
//      ) {
//    std::cout << "our edge null opposite\n";
//  }
  for (const auto &e : this->_outgoing_edges) {
    if (not e.expired() and e.lock()->_end.lock() == to) {
      return e.lock();
    }
  }
//  if (
//      ((to->_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//          (_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4) || (
//          (_coordinates - Point({0.255303, 0.840519, 0.394945})).abs() < 1e-4 &&
//              (to->_coordinates - Point({0.018246, 1.030815, 0.331238})).abs() < 1e-4)
//      ) {
//    std::cout << "our edge null opposite\n";
//  }
  return nullptr;
}

std::istream &operator>>(std::istream &in, Vertex &v) {
  in >> v._coordinates;
  return in;
}
std::ostream &operator<<(std::ostream &out, const Vertex &v) {
  out << v._coordinates;
  return out;
}

std::shared_ptr<HalfEdge> Vertex::find_next_boundary_edge(const std::shared_ptr<HalfEdge> &prev) const {
  std::shared_ptr<HalfEdge> ans;
  for (const auto &e : this->_outgoing_edges) {
    if (not e.expired() and (not e.lock()->has_opposite())
        and (not ans or not prev or get_sin(*prev, *(e.lock())) > get_sin(
            *prev,
            *ans))) {
      ans = e.lock();
    }
  }
  return ans;
}

std::vector<std::shared_ptr<Vertex>> Vertex::neighbor_vertices() const {
  std::vector<std::shared_ptr<Vertex>> v;
  v.reserve(_outgoing_edges.size());
  for (const auto &e : this->_outgoing_edges) {
    if (not e.expired() and not e.lock()->_end.expired()) {
      v.push_back(e.lock()->_end.lock());
    }
  }
  return v;
}

std::vector<std::shared_ptr<Triangle>> Vertex::neighbor_triangles() const {
  std::vector<std::shared_ptr<Triangle>> v;
  v.reserve(_outgoing_edges.size());
  for (const auto &e : this->_outgoing_edges) {
    if (not e.expired() and not e.lock()->_owner.expired()) {
      v.push_back(e.lock()->_owner.lock());
    }
  }
  return v;
}

Vertex::operator Point() {
  return _coordinates;
}

Vector Vertex::get_normal() {
  Vector ans;
  for (const auto &t : neighbor_triangles()) {
    ans += t->n * t->area();
  }
  return ans.norm();
}
