#pragma once
#include <iostream>
#include <ostream>
#include <vector>
#include <string>

class ErrorHandler {
public:
	static bool hadError;
private:
	static void report(int line, std::string message) {
		std::cout << "Syntax Error: " << message << " at line " << line << std::endl;
		// hadError = true;
	}
public:
	static void error(int line, std::string message) {
		report(line, message);
	}

};
