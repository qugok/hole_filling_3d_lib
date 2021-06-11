#pragma once

#include <FillingStrategy.hpp>


namespace strategies {

/*
 * стратегия заполнения, предложенная в стастье
 * https://www.mdpi.com/2076-3417/10/3/969?type=check_update&version=2
 * (не совсем она, а исправленная с учетом недочетов в
 * минимальной триангуляции, делоне и наступательном фронте)
 * в зависимости от размера отверстия применяем разные алгоритмы
 * если вершин <= 6       -> FillSmall (центр масс)
 * если вершин > 6 и < 50 -> MinAreaTriangulation | 1 итерация DeloneLike | сглаживание StupidHarmonicUmbrella
 * если вершин > 50       -> ModernAdvancingFront
 *
 * настраиваемые параметры:
 * границы применения различных алгоритмов:                       low_border, top_border
 * число итераций DeloneLike в случае отверстия среднего размера: delone_limit
 * применение сглаживания к отверстию большого размера:           fairing_big
 */
class FastHoleFillingMethod : public FillingStrategy {
  static constexpr int low_border = 6;
  static constexpr int top_border = 50;
  static constexpr int delone_limit = 1;
  static constexpr bool fairing_big = false;

 public:
  FastHoleFillingMethod() = default;
  std::string get_all_params() const override;
 private:
  void Process(const Hole &h, const MeshController &m) const override;
};
}