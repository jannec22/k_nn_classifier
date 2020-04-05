#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "src/KnnClassifier.h"
#include "src/Vector.h"
#include "src/definitions.h"

void print_help(po::options_description& desc) { cout << desc << "\n"; }

void prepare_args_desc(po::options_description& desc) {
  desc.add_options()                                        //
      ("help", "produce help message")                      //
      ("k", po::value<size_t>(), "set k value")             //
      ("test-set", po::value<string>(), "test set file")    //
      ("train-set", po::value<string>(), "train set file")  //
      ("separator", po::value<letter>(),
       "separator of the fields in train set. default ','");
}

bool test_args(po::variables_map& map) {
  if (map.count("help")) return false;

  if (!map.count("k")) {
    cerr << "[ERR] no k given.\n";
    return false;
  }

  if (!map.count("train-set")) {
    cerr << "[ERR] no train set given.\n";
    return false;
  }

  if (!map.count("separator")) {
    cout << "[INFO] no separator, using ','\n";
  }

  if (!map.count("test-set")) {
    cout << "[WARN] no test-set, skipping testing\n";
  }

  return true;
}

int main(int ac, char** av) {
  size_t k;
  bool read_test_file = false;
  string test_set_filename;
  string train_set_filename;
  letter separator = ',';

  try {
    po::options_description desc("Available options");
    prepare_args_desc(desc);

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (test_args(vm)) {
      k = vm["k"].as<size_t>();
      train_set_filename = vm["train-set"].as<string>();

      KnnClassifier<> cs{k};

      std::ifstream train_set_in_stream(train_set_filename);
      if (!train_set_in_stream) {
        throw std::runtime_error("could not open train-set file '" +
                                 test_set_filename + "'");
      }

      if (vm.count("separator")) {
        separator = vm["separator"].as<letter>();
      }

      if (vm.count("test-set")) {
        test_set_filename = vm["test-set"].as<string>();
        std::ifstream test_set_stream(test_set_filename);

        if (!test_set_stream) {
          throw std::runtime_error("could not open test-set file '" +
                                   test_set_filename + "'");
        }

        cs << separator            //
           << "train"              //
           << train_set_in_stream  //
           << "test"               //
           << test_set_stream;
      } else {
        // read stdin as test-set
        cs << separator  //
           << "train"    //
           << train_set_in_stream;

        cout << "[INFO] Using stdin as test-set. Press Ctrl+D to exit.\n";
        cs << "test"  //
           << std::cin;
      }

    } else {
      print_help(desc);
      return 1;
    }

  } catch (const exception& e) {
    cerr << e.what() << '\n';
  }
}
