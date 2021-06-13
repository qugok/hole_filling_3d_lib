#pragma once

#include <base.hpp>
#include <functional>
#include <BaseFillingStrategy.hpp>

namespace initial_triangulation {
/*
 * базовый класс для всех начальных триангуляций
 * предоставляет интерфейс заделывания отверстий:
 * по объекту отверстия и контроллеру меша заделывает отверстие
 * и возвращает новые вершины внутри отверстия для дальнейшей обработки
 */
class TriangulationStrategy : public BaseFillingStrategy {

 public:
  /*
   * по отверстию и контроллеру для mesh мы заделываем отверстие
   * @ return: вершины внутри заделанной дыры
   */
  virtual std::vector<std::shared_ptr<Vertex>> ProcessHole(
      const TriangulationStrategy::Hole &h,
      const MeshController &m) const = 0;

  virtual ~TriangulationStrategy() = default;
};
}