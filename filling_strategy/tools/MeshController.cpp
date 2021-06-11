#include "MeshController.hpp"
#include <Mesh.hpp>

BaseFillingStrategy::MeshController::MeshController(Mesh &m) : m(m) {}

void BaseFillingStrategy::MeshController::remove_triangle(const std::shared_ptr<Triangle> &t) const {
  m.remove_triangle(t);
}
std::shared_ptr<Triangle> BaseFillingStrategy::MeshController::add_triangle(
    const std::shared_ptr<Vertex> &a,
    const std::shared_ptr<Vertex> &b,
    const std::shared_ptr<Vertex> &c,
    Vector n) const {
  return m.add_triangle(a, b, c, n);
}
void BaseFillingStrategy::MeshController::get_structure(std::vector<std::shared_ptr<Vertex>> &vertices,
                                                        std::vector<std::tuple<size_t,
                                                                               size_t,
                                                                               size_t>> &triangles) const {
  m.get_structure(vertices, triangles);
}
