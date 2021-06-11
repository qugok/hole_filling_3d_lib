#include <sstream>
#include "WithoutFairing.hpp"

void fairing::WithoutFairing::Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                      const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
                                      const BaseFillingStrategy::MeshController &m) const {

}
std::string fairing::WithoutFairing::get_all_params()  const {
  std::ostringstream out;
  out << "\t\tWithoutFairing:";
  return out.str();
}
