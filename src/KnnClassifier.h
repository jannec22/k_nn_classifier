#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

#include "Vector.h"
#include "definitions.h"

template <typename T = real>
class KnnClassifier {
  letter separator;
  const size_t k;
  vector<Vector<T>> training_set;
  string mode = "train";

 public:
  KnnClassifier(const size_t _k) : k(_k) {}

  void train(std::istream& stream) {
    string line;
    size_t count = 0;

    while (std::getline(stream, line)) {
      training_set.emplace_back(line, separator);
      // cout << training_set.back() << "\n";
      ++count;
    }
    cout << "trained with " << count << " test vectors\n";
  }

  void test(std::istream& stream) {
    string line;
    size_t failed = 0;
    size_t all = 0;

    while (std::getline(stream, line)) {
      Vector<T> test_vector(line, separator);
      string result = classify(test_vector);

      cout << test_vector.vector_class << " -> " << result << "\n";

      if (result != test_vector.vector_class) {
        ++failed;
      }
      ++all;
    }

    real percent = (failed * 100.0) / all;
    cout << "failed: " << percent << "%\n";
  }

  string classify(Vector<T>& test_vector) {

    /**
     * Description of the implementation:
     * the classify method tests one vector against all test vectors
     * it calculates the distance to each vector
     * and depending on it, it places a "tuple" in-place
     * into the sorted k-length array.
     * 
     * the array looks like that:
     * [(distance, class), ...]
     * eg:
     * [(1, xx), (2, zz), (INFINITY, )]
     * 
     * after that, it iterates through this array
     * and creates a map of scores like that:
     * {
     *  class: <number of occurances in the array>
     * } eg:
     * {
     *  xx: 1,
     *  zz: 1
     * }
     * 
     * then based on the score map it selects the first most frequent class
     * in the whole training set.
    */

    vector<std::pair<real, string>> closest_distance_set(
        k, std::pair<real, string>{INFINITY, ""});
    auto& last = closest_distance_set.back();

    for (auto&& v : training_set) {
      real dist = test_vector.distance(v);
      auto it = closest_distance_set.begin();

      // placing the (dist, class) tuple in the correct place
      if (last.second.empty()) {
        if (dist < last.first) {
          while (std::next(it) != closest_distance_set.end()) {
            bool current_not_empty_and_smaller =
                !it->second.empty() && it->first < dist;
            bool next_empty_or_smaller =
                std::next(it)->second.empty() || std::next(it)->first <= dist;

            if (current_not_empty_and_smaller || next_empty_or_smaller)
              ++it;
            else
              break;
          }

          it->first = dist;
          it->second = v.vector_class;
        }
      } else {
        while (std::next(it) != closest_distance_set.end()) {
          bool current_not_empty_and_smaller =
              !it->second.empty() && it->first < dist;
          bool next_empty_or_smaller =
              std::next(it)->second.empty() || std::next(it)->first <= dist;

          if (current_not_empty_and_smaller || next_empty_or_smaller)
            ++it;
          else
            break;
        }

        it->first = dist;
        it->second = v.vector_class;
      }
    }

    std::map<string, size_t> scores;

    // counting scores
    for (auto&& x : closest_distance_set) {
      if (x.first != INFINITY) {
        ++scores[x.second];
      }
    }
    // [(0.1, "ddd"), (2, "ddd"), (5, "dddd"), (7, "dddd")]
    // calculating first max
    auto max = std::max_element(
        scores.begin(), scores.end(),
        [](const std::pair<string, size_t> a,
           const std::pair<string, size_t> b) { return a.second < b.second; });

    return max->first;
  }

  KnnClassifier<T>& operator<<(std::istream& stream) {
    if (mode == "train") {
      train(stream);
    } else if (mode == "test") {
      test(stream);
    }

    return *this;
  }

  KnnClassifier<T>& operator<<(const letter& sep) {
    separator = sep;
    return *this;
  }

  KnnClassifier<T>& operator<<(const string& md) {
    cout << "[INFO] mode: " << md << "\n";
    mode = md;
    return *this;
  }
};