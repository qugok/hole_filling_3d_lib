#include "FastHoleFillingMethod.hpp"
#include "Hole.hpp"
#include "FillSmall.hpp"
#include "ModernAdvancingFront.hpp"
#include "DeloneLike.hpp"
#include "MinAreaTriangulation.hpp"
#include "StupidUmbrella.hpp"

void strategies::FastHoleFillingMethod::Process(const BaseFillingStrategy::Hole &h,
                                    const BaseFillingStrategy::MeshController &m) const {
  auto boundary = h.get_vertexes();
  if (boundary.size() <=low_border )
  {
    initial_triangulation::FillSmall small;
    small.ProcessHole(h, m);
    return;
  }
  else if (boundary.size() > top_border) {
    initial_triangulation::advancing_front::ModernAdvancingFront modern_advancing_front;
    auto v = modern_advancing_front.ProcessHole(h, m);
    if (fairing_big){
      fairing::StupidScaleDependentUmbrella scale_dependent_umbrella;
      scale_dependent_umbrella.Process(v, boundary, m);
    }
    return;
  }
  else {
    initial_triangulation::MinAreaTriangulation min_area_triangulation;
    refinement::DeloneLike delone_like(delone_limit);
    fairing::StupidHarmonicUmbrella harmonic_umbrella;
    auto inside_vertexes = min_area_triangulation.ProcessHole(h, m);
    delone_like.Process(inside_vertexes, boundary, m);
    harmonic_umbrella.Process(inside_vertexes, boundary, m);
  }
}
std::string strategies::FastHoleFillingMethod::get_all_params()  const  {
  std::ostringstream out;
  out << "FastHoleFillingMethod:\n\t";
  out << "low_border:\t" << low_border << "\n\t";
  out << "top_border:\t" << top_border << "\n\t";
  out << "delone_limit:\t" << delone_limit << "\n\t";
  out << "fairing_big:\t" << fairing_big << "\n";
  out << initial_triangulation::advancing_front::ModernAdvancingFront::get_static_params();
  return out.str();
}
