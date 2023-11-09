#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexer.h"


int main(int argc, char** argv) {
	std::ifstream file(argv[1]);
	std::ostringstream sstream;
    sstream << file.rdbuf();
    const std::string str(sstream.str());
    const char* code_start = str.c_str();
	Lexer lex(code_start);
	auto tokens = lex.scan();
	for (auto& t : tokens) {
		std::cout << t.toString() << std::endl;
	}
}
