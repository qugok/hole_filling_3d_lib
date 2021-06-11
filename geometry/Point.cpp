//#include <cmath>
#include <valarray>
#include "Point.hpp"

double inner_product(const Point &a, const Point &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 * векторное произведение
 */
Vector cross_product(const Vector &a, const Vector &b) {
  return Vector(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}
Point barycenter(const std::vector<Point> &ps) {
  Point ans;
  for (auto p : ps) {
    ans += p;
  }
  return ans / ps.size();
}

double get_angle(const Vector &a, const Vector &b) {
  double angle = asin(get_sin(a, b));
  auto temp = get_cos(a, b);
  if (get_cos(a, b) <= 0)
    angle = M_PI - angle;
  return angle;
}

double get_sin(const Vector &a, const Vector &b) {
  if (a == Point() || b == Point()){
    return 0;
  }
  return (a & b).abs() / a.abs() / b.abs();
}
double get_cos(const Vector &a, const Vector &b) {
  if (a == Point() || b == Point()){
    return 0;
  }
  return (a * b) / a.abs() / b.abs();
}

double area(const Point &a, const Point &b, const Point &c) {
  return cross_product(b - a, c - a).abs() / 2;
}

Vector bisector(const Vector &a, const Vector &b) {
  return (a.norm() + b.norm()).norm();
}

std::tuple<Vector, Vector> trisector(const Vector &a, const Vector &b) {
  Vector na = a.norm();
  Vector nb = b.norm();
  double angle = get_angle(a, b);
  if (angle < EPS){
    return {na, na};
  }
  Vector vl = (na * (sin(angle * 2 / 3) / sin(angle / 3)) + nb).norm();
  Vector vr = (nb * (sin(angle * 2 / 3) / sin(angle / 3)) + na).norm();
  return {vl, vr};
}

Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

Point Point::operator-() const {
  return Point(-x, -y, -z);
}

Point operator+(const Point &a, const Point &b) {
  return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}
std::ostream &operator<<(std::ostream &out, const Point &p) {
  out << p.x << " " << p.y << " " << p.z;
  return out;
}

std::istream &operator>>(std::istream &in, Point &p) {
  in >> p.x >> p.y >> p.z;
  return in;
}

bool operator==(const Point &a, const Point &b) {
  return std::abs(a.x - b.x) < EPS and std::abs(a.y - b.y) < EPS and std::abs(a.z - b.z) < EPS;
}

double operator*(const Point &a, const Point &other) {
  return inner_product(a, other);
}
Point operator&(const Point &a, const Point &other) {
  return cross_product(a, other);
}
Point operator/(const Point &a, double number) {
  return Point(a.x / number, a.y / number, a.z / number);
}
Point operator*(const Point &a, double number) {
  return Point(a.x * number, a.y * number, a.z * number);
}
Point operator*(double number, const Point &other) {
  return other * number;
}
Point operator+=(Point &a, const Point &other) {
  return a = a + other;
}
Point operator-=(Point &a, const Point &other) {
  return a = a - other;
}
Point operator/=(Point &a, double number) {
  return a = a / number;
}
Point operator*=(Point &a, double number) {
  return a = a * number;
}
double Point::abs() const {
  return sqrt(x * x + y * y + z * z);
}
double Point::abs2() const {
  return x * x + y * y + z * z;
}
bool operator!=(const Point &a, const Point &other) {
  return not(a == other);
}
Point Point::norm() const {
  return *this / this->abs();
}
Point operator-(const Point &a, const Point &other) {
  return Point(a.x - other.x, a.y - other.y, a.z - other.z);
}

namespace std {
std::size_t hash<Point>::operator()(const Point &p) const {
  // ODO придумать нормальную хеш функцию
  return std::hash<int>()(p.x * 1000) * std::hash<int>()(p.y * 1000) * std::hash<int>()(p.z * 1000)
      ^ std::hash<int>()(p.y * 1000) * std::hash<int>()(p.x * 1000) + std::hash<int>()(p.x * 1000) ^ 1000000007;
}
}

std::tuple<Point, Point> nearest_points_of_lines(const Point& l1a, const Point& l1b, const Point& l2a, const Point& l2b){
  // направляющий вектор прямой l1
  Vector l1 = l1a - l1b;
  // направляющий вектор прямой l2
  Vector l2 = l2a - l2b;
  // вектор от точки на 2 прямой до точки на 1 прямой
  Vector ab = l1a - l2b;
  // направление кратчайшего соединяющего отрезка
  Vector norm = cross_product(l1, l2).norm();
  // вектор кратчайшего соединяющего отрезка (и с расстоянием) (от 2 до 1)
  Vector abnorm = projection(ab, norm);
  //проекция точки l2b на плочкость, параллельную обоим прямым и содержащую первую прямую
  Point b1pl = l2b + abnorm;
  // проекция b1pl на 1 прямую
  Point b1pll1 = l1a + projection(b1pl - l1a, l1);
  // вектор, соотв этой проекции
  Vector proj = b1pll1 - b1pl;
  // ближайшая к 1 прямой точка на 2 прямой
  Point near2 = l2b + anti_projection(proj, l2);
  Point near1 = near2 + abnorm;
  return {near1, near2};
}

Vector projection(const Vector& v, const Vector& direction){
  return direction.norm() * v.abs() * get_cos(v, direction);
}

Vector anti_projection(const Vector& v, const Vector& direction){
  return direction.norm() * v.abs() / get_cos(v, direction);
}

bool point_on_segment(const Point& p, const Point& a, const Point& b){
  return (a + projection(p - a, b - a) - p).abs() < EPS and (p-a).abs() + (b-p).abs() - (a-b).abs() < EPS;
}


Vector normal_to_triangle(const Point &a, const Point &b, const Point &c){
  return cross_product(b-a, c-a).norm();
}