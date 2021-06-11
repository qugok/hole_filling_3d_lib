//
// Created by alex on 09.03.2021.
//

#include "base.hpp"
HalfEdge::HalfEdge(const std::shared_ptr<Vertex> &start,
                   const std::shared_ptr<Vertex> &end,
                   const std::shared_ptr<HalfEdge> &opposite,
                   const std::shared_ptr<Triangle> &owner) : _start(start), _end(end), _opposite(opposite), _owner(owner){
}

bool HalfEdge::has_opposite() {
  return not _opposite.expired();
}
HalfEdge::operator Vector() const {
  Point a, b;
  if (not _start.expired()){
    a = _start.lock()->_coordinates;
  }
  if (not _end.expired()){
    b = _end.lock()->_coordinates;
  }
  return b - a;
}
HalfEdge::~HalfEdge() {
  if (has_opposite()){
    _opposite.lock()->_opposite.reset();
  }
}
void HalfEdge::fix_opposite() {
  if (has_opposite()) {
    _opposite.lock()->_opposite = shared_from_this();
  } else if (not _start.expired() and not _end.expired()){
    _opposite = _end.lock()->get_edge_to(_start.lock());
    if (not _opposite.expired()){
      _opposite.lock()->_opposite = shared_from_this();
    }
  }
}

namespace std{

std::size_t hash<HalfEdge>::operator()(const HalfEdge &p) const
  {
    // придумать нормальную хеш функцию
    Point a, b;
    if (not p._start.expired()){
      a = p._start.lock()->_coordinates;
    }
    return std::hash<Point>()(a);
//    return std::hash<>()(p._start) * std::hash<double>()(p.y) * std::hash<double>()(p.z)  ^ std::hash<double>()(p.y) * std::hash<double>()(p.x) + std::hash<double>()(p.x) ^ 1000000007;
  }
}