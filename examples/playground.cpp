#include "Mesh.hpp"
#include "ModernAdvancingFrontFilling.hpp"
#include "SequenceFillingStrategy.hpp"
#include "MinAreaTriangulation.hpp"
#include "AdvancingFront.hpp"
#include "FillSmall.hpp"
#include "DeloneLike.hpp"
#include "StupidUmbrella.hpp"

/*
 * здесь можно составить и опробовать свой
 * алгоритм из составных частей, доступных в библиотеке
 */
int main() {
  std::string readpath = "../models/for_test/1sphere_hole.stl";
  Mesh m = Mesh::read_from(readpath);


  // здесь можно собрать свою стратегию из различных частей
  // 1 - начальная триангуляция
  // 2 - улучшения триангуляции, применяемые последовательно
  strategies::SequenceFillingStrategy filling_strategy = strategies::SequenceFillingStrategy(
//      new initial_triangulation::advancing_front::ModernAdvancingFront(-1, sqrt(3.5)),
//      new initial_triangulation::advancing_front::AdvancingFront(-1, sqrt(3)),
      new initial_triangulation::MinAreaTriangulation(),
//          new initial_triangulation::FillSmall(),
      {
//              new fairing::StupidHarmonicUmbrella(3, true, true),
          new refinement::DeloneLike(100),
//              new fairing::StupidHarmonicUmbrella(3, true, false),
//              new fairing::StupidScaleDependentUmbrella(10, true, false),
//              new fairing::StupidScaleDependentUmbrella(3, true, true),
//              new fairing::StupidUniformUmbrella(10, true, false),
//              new fairing::StupidUniformUmbrella(1, true, true),
//              new refinement::DeloneLike(100),
      }
  );

  m.fillHolesWithStrategy(filling_strategy);

  std::string writepath = "../temp/playground.stl";
  Mesh::write_to(m, writepath);
  return 0;
}