#pragma once

#include <FillingStrategy.hpp>
#include "ClassicFillingStrategy.hpp"

namespace strategies {
class ModernAdvancingFrontFilling : public ClassicFillingStrategy {
 public:
  ModernAdvancingFrontFilling();
};
}