#pragma once
#include "string"

/*
 * базовый класс для всех стратегий всзаимодействия с отверстиями
 *
 * get_all_params() - возвращает все параметры которые влияют
 * на работу стратегии чтобы можно было это сохранить в файл
 */
class BaseFillingStrategy {
  friend class Mesh;
 public:
  virtual std::string get_all_params() const = 0;
 protected:
  struct Hole;
  class MeshController;
};