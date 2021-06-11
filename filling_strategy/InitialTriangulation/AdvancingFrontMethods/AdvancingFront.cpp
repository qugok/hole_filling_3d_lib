// /home/alex/mipt/1c/diploma/The_advancing_front_mesh_generation_meth.pdf
// https://www.academia.edu/40278413/The_advancing_front_mesh_generation_method_revisited

#include "AdvancingFront.hpp"
#include "FillSmall.hpp"
#include "tools/MeshController.hpp"
#include <tools/Hole.hpp>
#include <cmath>

initial_triangulation::advancing_front::AdvancingFront::AdvancingFront(size_t limit, double coef)
    : base::BaseAdvancingFront(limit, coef) {}

std::vector<std::shared_ptr<Vertex>> initial_triangulation::advancing_front::AdvancingFront::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const TriangulationStrategy::MeshController &m) const {

  Front f(h, m, new_edge_coef);
  return process_hole_front(h, m, f);
}

initial_triangulation::advancing_front::AdvancingFront::Front::Front(
    const TriangulationStrategy::Hole &h,
    const TriangulationStrategy::MeshController &m,
    double new_edge_coef)
    : BaseFront(h, m, new_edge_coef) {}
