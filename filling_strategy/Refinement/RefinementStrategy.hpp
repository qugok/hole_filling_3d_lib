#pragma once

#include <vector>
#include <memory>
#include <base.hpp>
#include <ImproveStrategy.hpp>

namespace refinement {
/*
 * базовый класс для уточнения сетки после алгоритмов триангуляции
 */
class RefinementStrategy : public ImproveStrategy {
 public:
  ~RefinementStrategy() override = default;
};
}
