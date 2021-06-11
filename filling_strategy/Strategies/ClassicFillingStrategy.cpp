#include "ClassicFillingStrategy.hpp"
#include "sstream"
#include "Hole.hpp"

strategies::ClassicFillingStrategy::ClassicFillingStrategy(initial_triangulation::TriangulationStrategy *a,
                                               refinement::RefinementStrategy *b,
                                               fairing::FairingStrategy *c)
    : a(a), b(b), c(c) {}
void strategies::ClassicFillingStrategy::Process(const Hole &h,
                                     const MeshController& m) const{
  auto boundary_vertexes = h.get_vertexes();
  auto vertexes_inside = a->ProcessHole(h, m);
  b->Process(vertexes_inside, boundary_vertexes, m);
  c->Process(vertexes_inside, boundary_vertexes, m);
}

strategies::ClassicFillingStrategy::~ClassicFillingStrategy() {
  delete a;
  delete b;
  delete c;
}
std::string strategies::ClassicFillingStrategy::get_all_params()  const  {
  std::ostringstream out;
  out << "TriangulationStrategy:\n" << a->get_all_params() << "\n";
  out << "RefinementStrategy:\n" << b->get_all_params() << "\n";
  out << "FairingStrategy:\n" << c->get_all_params() << "\n";
  return out.str();
}