#include "ModernAdvancingFrontFilling.hpp"

#include <ModernAdvancingFront.hpp>
#include <WithoutRefinement.hpp>
#include <WithoutFairing.hpp>

strategies::ModernAdvancingFrontFilling::ModernAdvancingFrontFilling()
: ClassicFillingStrategy(
    new initial_triangulation::advancing_front::ModernAdvancingFront(),
    new refinement::WithoutRefinement(),
    new fairing::WithoutFairing()) {}
