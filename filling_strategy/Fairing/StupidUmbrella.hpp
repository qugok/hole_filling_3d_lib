#pragma once
#include "FairingStrategy.hpp"
#include "functional"

namespace fairing {
/*
 * базовый класс для различных сглаживаний сетки основанных на идее приведения точки к взвешенной сумме её соседей
 * наследники отличаются функциями весов
 */
class StupidUmbrella : public FairingStrategy{
 protected:
  /**
   * @brief базовый конструктор для всех подобных методов
   * @param umbrella_operator - оператор, применяемый к вершине
   * @param count - колличество повторений применения сглаживания
   * @param iterative - как применять сглаживание - итеративно или ко всем вершинам сразу (как при домножении на матрицу)
   * @param include_boundary - прменять ли сглаживание к границе отверстия или только к точкам внутри
   */
  explicit StupidUmbrella(std::function<Vector(const std::shared_ptr<Vertex>&)>  umbrella_operator
                          , int count = 1
                          , bool iterative = true,
                          bool include_boundary = false);
 public:
  std::string get_all_params()  const override;

  void Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
               const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
               const BaseFillingStrategy::MeshController &m) const override;

 private:
  std::function<Vector(const std::shared_ptr<Vertex>&)> umbrella_operator;
  bool iterative, include_boundary;
  int count;
};
/*
 * используется функция весов w(v_i, v_j) = 1
 */
class StupidUniformUmbrella : public StupidUmbrella {
 public:
  explicit StupidUniformUmbrella(bool iterative = true, bool include_boundary = false);
  explicit StupidUniformUmbrella(int count, bool iterative = true, bool include_boundary = false);
  std::string get_all_params()  const override;
};

/*
 * используется функция весов w(v_i, v_j) = ||v_i-v_j||
 */
class StupidScaleDependentUmbrella : public StupidUmbrella {
 public:
  explicit StupidScaleDependentUmbrella(bool iterative = true, bool include_boundary = false);
  explicit StupidScaleDependentUmbrella(int count, bool iterative = true, bool include_boundary = false);
  std::string get_all_params()  const override;
};


/*
 *
 * используется функция весов w(v_i, v_j) это сумма ctg углов смотрящих на это ребро
 *     v_u
 *     /\
 *    /  \
 * v_i----v_j
 *    \  /
 *     \/
 *     v_d
 * используются ctg углов (v_i, v_d, v_j) и (v_i, v_u, v_j)
 *
 */
class StupidHarmonicUmbrella : public StupidUmbrella {
 public:
 public:
  explicit StupidHarmonicUmbrella(bool iterative = true, bool include_boundary = false);
  std::string get_all_params()  const override;
  explicit StupidHarmonicUmbrella(int count, bool iterative = true, bool include_boundary = false);
};
}