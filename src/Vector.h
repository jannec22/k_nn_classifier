#pragma once
#include <iostream>
#include <sstream>
#include <cmath>

#include "definitions.h"

template <typename T>
struct Vector {
  string vector_class;
  vector<T> coords;

  Vector(string&, letter&);

  T distance(const Vector&) const;
};

template <>
real Vector<real>::distance(const Vector& other) const {
  if (other.coords.size() != coords.size()) return INFINITY;
  real sum = 0;

  for (size_t i = 0; i < coords.size(); ++i) {
    sum += (other.coords[i] - coords[i]) * (other.coords[i] - coords[i]);
  }

  return std::sqrt(sum);
}

template <>
Vector<real>::Vector(string& str, letter& separator) {
  std::stringstream ss(str);

  std::getline(ss, vector_class, ',');

  real coord;
  while (ss >> coord) {
    coords.push_back(coord);
    ss >> separator;
  }
}

std::ostream& operator<<(std::ostream& stream, const Vector<real>& v) {
  stream << v.vector_class << " (";
  bool first = true;

  for (auto x : v.coords) {
    if (!first) {
      stream << ", ";
    } else {
      first = false;
    }

    stream << x;
  }

  stream << ")";

  return stream;
}