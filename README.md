## How to compile / run (Linux)

```bash
$ ./compile
$ ./a.out < test-sets/test-set.csv --k <k-value> --train-set train-sets/train-set.csv
$ # or
$ ./a.out < test-sets/abcde.csv --k <k-value> --train-set train-sets/abcde.csv
$ # if no boost
$ ./compile no-boost <k> <path/to/train-set>
$ ./a.out < test-sets/test-set.csv
```

```
THIS PROGRAM USES BOOST TO PARSE ARGUMENTS AND EXPECTS BOOST program_options headers IN /usr/local/include/boost
IF YOU DONT HAVE BOOST, USE compile no-boost k tes-set.csv
```