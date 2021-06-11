#pragma once
#include <functional>
#include <memory>
#include <base.hpp>

namespace fairing {

/*
 * инструменты для использования при сглаживании
 */
namespace tools {

/*
 * прибавить к каждой вершине соотв ей вектор
 */
void add(const std::vector<std::shared_ptr<Vertex>> &vertices, const std::vector<Vector> &delta);

/*
 * взвешенный umbrella оператор
 * по функции веса и вершине возвращает значение на которое её необходимо сдвинуть
 */
Vector weighted_umbrella_operator(
    const std::function<double(const std::shared_ptr<Vertex> &, const std::shared_ptr<Vertex> &)> &weight,
    const std::shared_ptr<Vertex> &v);

/*
 * взвешенный umbrella оператор вторго порядка. не используется
 */
Vector second_order_weighted_umbrella_operator(
    const std::function<double(const std::shared_ptr<Vertex> &, const std::shared_ptr<Vertex> &)> &weight,
    const std::shared_ptr<Vertex> &v);
}

/*
 * функции веса
 */
namespace weight_funcs {
// w(v_i, v_j) = 1
double uniform_weight_func(const std::shared_ptr<Vertex> & a, const std::shared_ptr<Vertex> & b);
// w(v_i, v_j) = ||v_i-v_j||
double scale_dependent_weight_func(const std::shared_ptr<Vertex> & a, const std::shared_ptr<Vertex> & b);
// w(v_i, v_j) это сумма ctg углов смотрящих на это ребр
double harmonic_weight_func(const std::shared_ptr<Vertex> & a, const std::shared_ptr<Vertex> & b);
}

// просто общедоступные методы

Point uniform_umbrella_operator(const std::shared_ptr<Vertex> &v);
Point scale_dependent_umbrella_operator(const std::shared_ptr<Vertex> &v);
Point harmonic_umbrella_operator(const std::shared_ptr<Vertex> &v);

}