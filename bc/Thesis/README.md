# Bachelor's thesis

This repository contains source files for implementation output and thesis itself.
For final evaluation of thesis along with supervisor's and reviewer's report visit [this site](https://www.vut.cz/en/students/final-thesis/detail/153846).

The project can be compiled by invoking ```make``` in the project root directory.
You can choose explicitly between gcc and clang compilers using ```make gcc``` and ```make clang``` respectively.
You will need to have Make, CMake, and at least one of the compilers installed.
For running the parser, use ```make run```.
Feel free to change program options.
For a complete manual on how to run the program, add ```-h``` option in ```Makefile``` to ```run``` target and invoke ```make run```.
By default, the input file is *input.koubp* in project root dir, change it according to your preferences as well.
For AST visual representation of program in *input.koubp*, run ```make tree``` after ```make run``` and make sure the program was run with ```-t``` option.
It will convert *tree.dot* to *tree.pdf* in project root with program *dot*.

Tests are run with ```make runtest```, it has to be invoked after ```make```, which builds the testing executable as well.
If you want to check for memory leaks in either of the executables, run ```make valgrind``` or ```make valgrind_test``` for program and testing executable respectively.

Thesis can be built with ```make thesis```.
```make clean``` cleans the root directory from unnecessary files and directories left after building either program executable or thesis.
