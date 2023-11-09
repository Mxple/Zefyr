#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "error_handler.h"

/**
 * Scans the input file and produces tokens
 */
class Lexer { 
private:
	const char* beg = nullptr;	// the source code
	std::vector<Token> tokens;

	int line;

	// helper functions
	Token scanNextToken();
	char advance();
	char peek();
	bool isWhiteSpace(char c);
	bool isDigit(char c);
	bool isAlpha(char c);	// underscore is considered alpha
	bool isAlphaNumeric(char c);

	// type specific functions
	Token number(bool hasDot = false);
	Token string();
	Token identifier();
	Token slashOrCommentOrSlashEqual();
	Token lessOrLessEqualOrLeftShift();
	Token greaterOrGreaterEqualOrRightShift();
	Token bangOrBangEqual();
	Token equalOrEqualEqual();
	Token plusOrPlusEqualOrPlusPlus();
	Token minusOrMinusEqualOrMinusMinusOrNumberLiteral();
	Token dotOrNumberLiteralOrTwoDots();
	Token percentOrPercentEqual();
	Token ampersandOrAmpersandAmpersand();
	Token pipeOrPipePipe();
	Token specialOperator();


public:
	Lexer(const char* source);
	std::vector<Token> scan();

};
