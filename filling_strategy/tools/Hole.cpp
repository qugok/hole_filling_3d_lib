//
// Created by alex on 01.05.2021.
//

#include "Hole.hpp"

BaseFillingStrategy::Hole::~Hole() {
  if (boundary_start && boundary_start.use_count() == 2) {
    boundary_start->rm_next();
  }
}

BaseFillingStrategy::Hole::Hole() = default;
BaseFillingStrategy::Hole::Hole(const std::shared_ptr<HalfEdge> &e) {
  boundary_start = std::make_shared<Element>(e, nullptr);
  boundary_start->next = boundary_start;
}

std::vector<std::shared_ptr<Vertex>> BaseFillingStrategy::Hole::get_vertexes() const {
  std::vector<std::shared_ptr<Vertex>> ans;
  std::shared_ptr<Element> current = boundary_start;
  do {
    ans.push_back(current->elem->_start.lock());
    current = current->next;
  } while (current != boundary_start);
  return ans;
}
BaseFillingStrategy::Hole::Hole(const BaseFillingStrategy::Hole &h) {
  boundary_start = h.boundary_start;
}
BaseFillingStrategy::Hole::Iterator BaseFillingStrategy::Hole::begin() const {
  return BaseFillingStrategy::Hole::Iterator(boundary_start);
}
BaseFillingStrategy::Hole::Iterator BaseFillingStrategy::Hole::end() const {
  return BaseFillingStrategy::Hole::Iterator(boundary_start, false);
}
void BaseFillingStrategy::Hole::insert_next(const std::shared_ptr<HalfEdge> &e) {
  auto el = std::make_shared<Element>(e, boundary_start->next);
  boundary_start->next = el;
  boundary_start = el;
}

std::shared_ptr<BaseFillingStrategy::Hole::Element> BaseFillingStrategy::Hole::Element::set_next(const std::shared_ptr<HalfEdge> &e) {
  return next = std::make_shared<Element>(e, nullptr);
}
void BaseFillingStrategy::Hole::Element::rm_next() {
  next = nullptr;
}
BaseFillingStrategy::Hole::Element::Element(const std::shared_ptr<HalfEdge> &elem, const std::shared_ptr<Element> &next) {
  this->elem = elem;
  this->next = next;
}
