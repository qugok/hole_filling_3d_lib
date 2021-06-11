#include "SequenceFillingStrategy.hpp"
#include "sstream"
#include "Hole.hpp"

strategies::SequenceFillingStrategy::SequenceFillingStrategy(initial_triangulation::TriangulationStrategy *a,
                                                 std::vector<ImproveStrategy *>  iss) : a(a), iss(std::move(iss)){}


strategies::SequenceFillingStrategy::~SequenceFillingStrategy() {
  delete a;
  for (auto i : iss){
    delete i;
  }
}
void strategies::SequenceFillingStrategy::Process(const BaseFillingStrategy::Hole &h,
                                      const BaseFillingStrategy::MeshController &m) const {
  auto boundary_vertexes = h.get_vertexes();
  auto vertexes_inside = a->ProcessHole(h, m);
  for (auto is : iss) {
    is->Process(vertexes_inside, boundary_vertexes, m);
  }
}
std::string strategies::SequenceFillingStrategy::get_all_params()  const {
  std::ostringstream out;
  out << "TriangulationStrategy:\n" << a->get_all_params() << "\n";
  for (auto is : iss){
    out << "ImproveStrategy:\n" << is->get_all_params() << "\n";
  }
  return out.str();

}
