There are 6 input files in here, and output files that correspond to them.
Input files:
  spec-1.txt: From the project spec, page 1
  spec-1-no.txt: spec-1.txt with one B changed to a wall so no solution txists
  spec-A.txt: From the project spec, appendix A
  spec-A-no.txt: From the project spec, appendix A
  small.txt: 3 colors, height 20, width 20
  big.txt: 24 colors, height 899, width 1000

The "small" and "big" files were produced by the same program that was used to
produce many of the test cases that are on the autograder machine.

The input files that have no solution have only one corresponding output file,
since the flags used do not change the output. The other output files have
a name that indicates their input file, and which flags were used. For example,
out-spec-A-queue-list.txt was the spec-A.txt input file run with the --queue
and --output list flags.

I ran each of these input files on a CAEN server. All of the "spec" tests ran
in under 0.02 seconds (as reported by the time command). The small.txt file took
0.031 seconds. The big.txt took about 1.6 seconds total (0.993 user, 0.593 system).
