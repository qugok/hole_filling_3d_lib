#pragma once

#include <FillingStrategy.hpp>

namespace strategies {

/*
 * класс позволяет создать стратегию где
 * сначала применится стратегия триангуляции переданная в a
 * потом по очереди применятся все стратегии улучшения (сглаживания или исправления)
 * переданные в векторе iss (ImproveStrategy sequence)
 *
 * вектор может быть пустой
 */
class SequenceFillingStrategy : public FillingStrategy {
  friend class Mesh;
 public:

  SequenceFillingStrategy(initial_triangulation::TriangulationStrategy* a,
                          std::vector<ImproveStrategy*>  iss);

  ~SequenceFillingStrategy() override;
 private:
  void Process(const Hole &h,
               const MeshController& m) const override;
 public:
  std::string get_all_params()  const override;
 private:

  initial_triangulation::TriangulationStrategy* a;
  std::vector<ImproveStrategy*> iss;
};
}
