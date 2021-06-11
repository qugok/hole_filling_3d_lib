#pragma once

#include <BaseFillingStrategy.hpp>
#include <base.hpp>

/*
 * класс для работы с отверстием
 * его можно построить
 * основные методы: получить список вершин отверстия
 *                  проитерироваться во полуребрам, составляющим отверстие
 */
struct BaseFillingStrategy::Hole {
  /**
   * hole предполагается зацикленной - после последнего ребра снова идет первое
   */
  struct Element {
    Element(const std::shared_ptr<HalfEdge> &elem, const std::shared_ptr<Element> &next);
    std::shared_ptr<Element> set_next(const std::shared_ptr<HalfEdge> &e);
    void rm_next();

    std::shared_ptr<HalfEdge> elem;
    std::shared_ptr<Element> next;
  };

  struct Iterator;
  Iterator begin() const;
  Iterator end() const ;

  Hole();
  Hole(const std::shared_ptr<HalfEdge> &e);
  Hole(const Hole &h);
  void insert_next(const std::shared_ptr<HalfEdge> &e);

  [[nodiscard]] std::vector<std::shared_ptr<Vertex>> get_vertexes() const;

  ~Hole();

  std::shared_ptr<Element> boundary_start;
};

struct BaseFillingStrategy::Hole::Iterator {
  using iterator_category = std::forward_iterator_tag;
  using pointer           = std::shared_ptr<HalfEdge> *;
  using reference         = std::shared_ptr<HalfEdge> &;

  Iterator(const std::shared_ptr<Element> &elem, bool first = true) : cur(elem), first(first) {}

  reference operator*() const { return (cur->elem); }
  pointer operator->() { return &(cur->elem); }

  Iterator& operator++() { first = false; cur = cur->next; return *this; }

  Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

  friend bool operator==(const Iterator &a, const Iterator &b) { return a.cur == b.cur && a.first == b.first; };
  friend bool operator!=(const Iterator &a, const Iterator &b) { return not (a == b); };

 private:

  std::shared_ptr<Element> cur;
  bool first;
};