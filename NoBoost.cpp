#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "src/KnnClassifier.h"
#include "src/Vector.h"
#include "src/definitions.h"

int main(int ac, char** av) {
  size_t k = __k__;
  string train_set_filename = "__ts__";
  letter separator = ',';

  try {
    KnnClassifier<> cs{k};

    std::ifstream train_set_in_stream(train_set_filename);
    if (!train_set_in_stream) {
      throw std::runtime_error("could not open train-set file '" +
                               train_set_filename + "'");
    }
    // read stdin as test-set
    cs << separator  //
       << "train"    //
       << train_set_in_stream;

    cout << "[INFO] Using stdin as test-set. Press Ctrl+D to exit.\n";
    cs << "test"  //
       << std::cin;

  } catch (const exception& e) {
    cerr << e.what() << '\n';
  }
}
