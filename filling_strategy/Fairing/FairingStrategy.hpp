#pragma once

#include <vector>
#include <memory>
#include <base.hpp>
#include <ImproveStrategy.hpp>

namespace fairing {
/*
 * базовый класс для сглаживания получившейся сетки
 */
class FairingStrategy : public ImproveStrategy {
 public:
  ~FairingStrategy() override = default;
};

}