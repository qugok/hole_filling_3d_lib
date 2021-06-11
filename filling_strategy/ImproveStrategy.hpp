#pragma once
#include "BaseFillingStrategy.hpp"

/*
 * базовый класс для всех стратегий улучшения меша
 * и сглаживания и исправления
 * нужен чтобы их можно было применять не различая - как одну стратегию
 */
class ImproveStrategy : public BaseFillingStrategy {
 public:
  virtual void Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
                       const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
                       const BaseFillingStrategy::MeshController& m) const = 0;

  virtual ~ImproveStrategy() = default;
};

