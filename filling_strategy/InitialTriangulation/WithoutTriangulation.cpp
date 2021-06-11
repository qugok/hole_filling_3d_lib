//
// Created by alex on 11.05.2021.
//

#include "WithoutTriangulation.hpp"
#include "sstream"
std::vector<std::shared_ptr<Vertex>> initial_triangulation::WithoutTriangulation::ProcessHole(const TriangulationStrategy::Hole &h,
                                                            const BaseFillingStrategy::MeshController &m) const {
  return {};
}
std::string initial_triangulation::WithoutTriangulation::get_all_params()  const {
  std::ostringstream out;
  out << "WithoutTriangulation: ";
  return out.str();
}
