#pragma once

#include <FillingStrategy.hpp>
#include "ClassicFillingStrategy.hpp"

namespace strategies {
/*
 * классическая стратегия наступательного фронта без всяких улучшений
 */
class AdvancingFrontFilling : public ClassicFillingStrategy {
 public:
  AdvancingFrontFilling();
};

}
