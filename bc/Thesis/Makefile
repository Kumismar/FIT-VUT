all:
	# cd src/lex && flex -o lex.yy.cc lex.l && sed -i '/#undef YY_DO_BEFORE_ACTION/d' lex.yy.h && sed -i '/#undef YY_NEW_FILE/d' lex.yy.h
	@[ -d build/ ] || mkdir build/ && cd build/ && cmake .. -G "Unix Makefiles" && cmake --build . -j10

gcc:
	@[ -d build/ ] || mkdir build/ && cd build/ && CC=gcc CXX=g++ cmake .. -G "Unix Makefiles" && cmake --build . -j10

clang:
	@[ -d build/ ] || mkdir build/ && cd build/ && CC=clang CXX=clang++ cmake .. -G "Unix Makefiles" && cmake --build . -j10

run:
	@[ -f build/src/Parser ] || echo "executable not build yet; 'make' first"
	@./build/src/Parser -f input.koubp -d -t

runtest:
	@[ -f build/test/ParserTest ] || echo "executable not built yet; 'make' first"
	@./build/test/ParserTest
	@bash test/test.sh

tree:
	@dot -Tpdf tree.dot -o tree.pdf

valgrind:
	@[ -f build/src/Parser ] || echo "executable not built yet; 'make' first"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./build/src/Parser -f input.koubp

valgrind_test:
	@[ -f build/test/ParserTest ] || echo "executable not built yet; 'make' first"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_test.log ./build/test/ParserTest

thesis:
	@cd doc/thesis && make

clean:
	@rm -rf build/ output.log tree.* valgrind* && cd doc/thesis && make clean
