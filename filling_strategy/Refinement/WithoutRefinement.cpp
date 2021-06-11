#include "WithoutRefinement.hpp"
#include "sstream"

void refinement::WithoutRefinement::Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                            const std::vector<std::shared_ptr<Vertex>> &hole_boundary, const MeshController& m) const {}
std::string refinement::WithoutRefinement::get_all_params()  const {

  std::ostringstream out;
  out << "WithoutRefinement: ";
  return out.str();
}
