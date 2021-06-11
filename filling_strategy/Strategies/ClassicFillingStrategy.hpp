#pragma once

#include <FillingStrategy.hpp>

namespace strategies {
/*
 * класс позволяет создать стратегию где
 * сначала применится стратегия триангуляции переданная в a
 * потом применится исправление переденное в b
 * потом применится сглаживание переданное в c
 */
class ClassicFillingStrategy : public FillingStrategy {

 public:
  ClassicFillingStrategy(initial_triangulation::TriangulationStrategy* a,
                         refinement::RefinementStrategy* b,
                         fairing::FairingStrategy* c);
  std::string get_all_params()  const  override;

  ~ClassicFillingStrategy() override;
 private:
  void Process(const Hole &h,
               const MeshController& m) const override;

  initial_triangulation::TriangulationStrategy* a;
  refinement::RefinementStrategy* b;
  fairing::FairingStrategy* c;
};

}