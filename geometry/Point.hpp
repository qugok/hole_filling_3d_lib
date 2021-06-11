#pragma once

#include <iostream>
#include <vector>
#include <tuple>

const double EPS = 1e-6;

struct Point {

  Point(double x = 0, double y = 0, double z = 0);

  template<class T>
  Point(T x, T y, T z = 0) :
      Point(static_cast<double>(x),
            static_cast<double>(y),
            static_cast<double>(z)) {}

  double x;
  double y;
  double z;

  [[nodiscard]] double abs() const;
  [[nodiscard]] Point norm() const;
  [[nodiscard]] double abs2() const;

  Point operator-() const;

};

std::ostream &operator<<(std::ostream &out, const Point &p);
std::istream &operator>>(std::istream &in, Point &p);

Point operator+(const Point &a, const Point &other);
Point operator+=(Point &a, const Point &other);
Point operator-(const Point &a, const Point &other);
Point operator-=(Point &a, const Point &other);
Point operator/(const Point &a, double number);
Point operator/=(Point &a, double number);
Point operator*(double number, const Point &other);
Point operator*(const Point &a, double number);
Point operator*=(Point &a, double number);
/**
 * inner (scalar) product
 * @param other
 * @return
 */
double operator*(const Point &a, const Point &other);
/**
 * cross product
 * @param other
 * @return
 */
Point operator&(const Point &a, const Point &other);
bool operator==(const Point &a, const Point &other);
bool operator!=(const Point &a, const Point &other);


namespace std {

template<>
struct hash<Point> {
  std::size_t operator()(const Point &p) const;
};
}

typedef Point Vector;
/*
 * скалярное произведение
 */
double inner_product(const Point &a, const Point &b);

/*
 * векторное произведение
 */
Vector cross_product(const Vector &a, const Vector &b);

Point barycenter(const std::vector<Point> &ps);

/*
 * abs синус угла между векторами
 */
double get_sin(const Vector &a, const Vector &b);

/*
 * угол между векторами
 * от 0 до pi
 */
double get_angle(const Vector &a, const Vector &b);

double get_cos(const Vector &a, const Vector &b);

double area(const Point &a, const Point &b, const Point &c);

Vector bisector(const Vector &a, const Vector &b);

std::tuple<Vector, Vector> trisector(const Vector &a, const Vector &b);

/*
 * возвращает две точки - ближайшую на 1 прямой ко 2 и ближайшую к 1 на 2
 */
std::tuple<Point, Point> nearest_points_of_lines(const Point &l1a,
                                                 const Point &l1b,
                                                 const Point &l2a,
                                                 const Point &l2b);

Vector projection(const Vector &v, const Vector &direction);

Vector anti_projection(const Vector &v, const Vector &direction);

bool point_on_segment(const Point &p, const Point &a, const Point &b);

// при обходе против часовой нормаль смотрит на зрителя (на верх)
Vector normal_to_triangle(const Point &a, const Point &b, const Point &c);