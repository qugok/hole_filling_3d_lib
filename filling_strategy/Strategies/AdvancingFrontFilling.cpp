
#include <AdvancingFront.hpp>
#include <WithoutRefinement.hpp>
#include <WithoutFairing.hpp>

#include "AdvancingFrontFilling.hpp"

strategies::AdvancingFrontFilling::AdvancingFrontFilling() : ClassicFillingStrategy(
    new initial_triangulation::advancing_front::AdvancingFront(),
    new refinement::WithoutRefinement(),
    new fairing::WithoutFairing()) {}
