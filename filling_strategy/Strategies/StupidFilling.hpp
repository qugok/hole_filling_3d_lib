#pragma once

#include <FillingStrategy.hpp>
#include "ClassicFillingStrategy.hpp"

namespace strategies {
/**
 * если точки три, то просто делает из них треугольник
 * иначе ставит центр масс и соединяет его со всеми точками
 * не рекомендуется использовать при размерах отверстия больше
 */
class StupidFilling : public ClassicFillingStrategy {
 public:
  StupidFilling();
};
}