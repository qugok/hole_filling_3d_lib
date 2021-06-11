#pragma once

#include <TriangulationStrategy.hpp>
#include <RefinementStrategy.hpp>
#include <FairingStrategy.hpp>

/*
 * базовый класс для всех статегий заделывания отверстий
 * стратегию можно применить к мешу, позвав у него метод
 * fillHolesWithStrategy и передав туда объект стратегии
 *
 * стратегия - комплекстое решение
 * она занимается и начальной триангуляцией и улучшением потом
 */
class FillingStrategy : public BaseFillingStrategy {
  friend class Mesh;
 public:
  FillingStrategy() = default;
  FillingStrategy(const FillingStrategy& ) = delete;
  FillingStrategy& operator=(const FillingStrategy& ) = delete;
  FillingStrategy(FillingStrategy&& ) noexcept = delete;
  FillingStrategy& operator=(FillingStrategy&& ) noexcept = delete;

  virtual ~FillingStrategy() = default;

 private:
  virtual void Process(const Hole &h,
                       const MeshController& m) const = 0;

};
