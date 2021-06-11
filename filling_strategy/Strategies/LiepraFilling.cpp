#include "LiepraFilling.hpp"
#include "sstream"
#include "MinAreaTriangulation.hpp"
#include "DeloneLike.hpp"

std::string strategies::LiepraFilling::get_all_params()  const {
  std::ostringstream out;
  out << ClassicFillingStrategy::get_all_params();
  out << "LiepraFilling";
  return out.str();
}

strategies::LiepraFilling::LiepraFilling(fairing::FairingStrategy *fairing_strategy)
: ClassicFillingStrategy(new initial_triangulation::MinAreaTriangulation(), new refinement::DeloneLike(), fairing_strategy){}
