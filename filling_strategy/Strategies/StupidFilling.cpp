#include "StupidFilling.hpp"

#include <FillSmall.hpp>
#include <WithoutRefinement.hpp>
#include <WithoutFairing.hpp>

strategies::StupidFilling::StupidFilling() : ClassicFillingStrategy(new initial_triangulation::FillSmall(),
                                                        new refinement::WithoutRefinement(),
                                                        new fairing::WithoutFairing()) {}
