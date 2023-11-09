#include "lexer.h"

Lexer::Lexer(const char* source) {
	this->beg = source;
	this->line = 1;
}

std::vector<Token> Lexer::scan() {
	for (Token t = scanNextToken(); t.type != Token::TYPE::END; t = scanNextToken()) {
		if (t.type != Token::TYPE::UNIDENTIFIED) tokens.push_back(t);
		else ErrorHandler::error(t.line, "Unidentified token");
	}
	return tokens;
}

Token Lexer::scanNextToken() {
	switch (peek()) {
		case '\0':	return Token(Token::TYPE::END, beg, 1, line);
		case '\n':	line++; beg++; return scanNextToken();	
		case '(':	return Token(Token::TYPE::LEFT_PAREN, beg++, 1, line);	
		case ')':	return Token(Token::TYPE::RIGHT_PAREN, beg++, 1, line);	
		case '[':	return Token(Token::TYPE::LEFT_SQUARE, beg++, 1, line);	
		case ']':	return Token(Token::TYPE::RIGHT_SQUARE, beg++, 1, line);
		case '{':	return Token(Token::TYPE::LEFT_BRACE, beg++, 1, line);
		case '}':	return Token(Token::TYPE::RIGHT_BRACE, beg++, 1, line);
		case ',':	return Token(Token::TYPE::COMMA, beg++, 1, line);
		case '.':	return dotOrNumberLiteralOrTwoDots();
		case '-':	return minusOrMinusEqualOrMinusMinusOrNumberLiteral();
		case '+':	return plusOrPlusEqualOrPlusPlus();
		case ';':	return Token(Token::TYPE::SEMICOLON, beg++, 1, line);
		case '*':	return Token(Token::TYPE::STAR, beg++, 1, line);
		case '!':	return bangOrBangEqual();
		case '=':	return equalOrEqualEqual();
		case '<':	return lessOrLessEqualOrLeftShift();
		case '>':	return greaterOrGreaterEqualOrRightShift();
		case '/':	return slashOrCommentOrSlashEqual();
		case '%':	return percentOrPercentEqual();
		case '&':	return ampersandOrAmpersandAmpersand();
		case '|':	return pipeOrPipePipe();
		case ' ':
		case '\r':
		case '\t':	beg++; return scanNextToken();
		case '"':	return string();
		case '`':	return specialOperator();
		default:
			if (isDigit(peek())) return number();
			else if (isAlpha(peek())) return identifier();
			else return Token(Token::TYPE::UNIDENTIFIED, beg++, 1, line);
	}
}

char Lexer::advance() {
	return *beg++;
}

char Lexer::peek() {
	return *beg;
}

bool Lexer::isWhiteSpace(char c) {
	return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

bool Lexer::isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
	return isAlpha(c) || isDigit(c);
}

// type specific functions
Token Lexer::number(bool hasDot) {
	const char* start = beg;
	if (hasDot) start -= 1;
	while (isDigit(peek())) advance();		// goes on until non digit
	if (!hasDot && peek() == '.') {			// decimal point
		advance();
		while (isDigit(peek())) advance();
	} 
	if (peek() == 'e' || peek() == 'E') {	// scientific notation
		advance();
		if (peek() == '+' || peek() == '-') advance();
		while (isDigit(peek())) advance();			// throw error here?
	}
	return Token(Token::TYPE::NUMBER, start, beg, line);
}

Token Lexer::string() {
	const char* start = beg;
	advance();	// skip the opening "
	while (peek() != '"' && peek() != '\0') {
		if (peek() == '\n') line++;
		advance();
	}
	if (peek() == '\0') ErrorHandler::error(line, "Unterminated string");
	advance();	// skip the closing "
	return Token(Token::TYPE::STRING, start, beg, line);
}

Token Lexer::identifier() {
	const char* start = beg;
	while (isAlphaNumeric(peek())) advance();
	std::string_view lexeme(start, beg - start);

	if (lexeme == "and")		return Token(Token::TYPE::AND, start, beg, line);
	else if (lexeme == "as")	return Token(Token::TYPE::AS, start, beg, line);
	else if (lexeme == "break") return Token(Token::TYPE::BREAK, start, beg, line);
	else if (lexeme == "def")	return Token(Token::TYPE::DEF, start, beg, line);
	else if (lexeme == "else")	return Token(Token::TYPE::ELSE, start, beg, line);
	else if (lexeme == "false") return Token(Token::TYPE::FALSE, start, beg, line);
	else if (lexeme == "for")	return Token(Token::TYPE::FOR, start, beg, line);
	else if (lexeme == "if")	return Token(Token::TYPE::IF, start, beg, line);
	else if (lexeme == "in")	return Token(Token::TYPE::IN, start, beg, line);
	else if (lexeme == "or")	return Token(Token::TYPE::OR, start, beg, line);
	else if (lexeme == "print") return Token(Token::TYPE::PRINT, start, beg, line);
	else if (lexeme == "return")return Token(Token::TYPE::RETURN, start, beg, line);
	else if (lexeme == "true")	return Token(Token::TYPE::TRUE, start, beg, line);
	else if (lexeme == "while") return Token(Token::TYPE::WHILE, start, beg, line);
	else						return Token(Token::TYPE::IDENTIFIER, start, beg, line);
}

Token Lexer::slashOrCommentOrSlashEqual() {
	const char* start = beg;
	advance();
	if (peek() == '/') {	// single line comment
		while (peek() != '\n' && peek() != '\0') advance();
		return scanNextToken();
	} else if (peek() == '*') {	// multi line comment
		advance();
		while (peek() != '\0') {
			if (peek() == '\n') line++;
			else if (peek() == '*' && *(beg + 1) != '\0' && *(beg + 1) == '/') {
				advance(); advance();
				return scanNextToken();
			}
			advance();
		}
		ErrorHandler::error(line, "Unterminated multi line comment");
		return Token(Token::TYPE::UNIDENTIFIED, beg, 1, line);
	} else if (peek() == '=') {
		return Token(Token::TYPE::SLASH_EQUAL, start, beg++, line);
	} else {
		return Token(Token::TYPE::SLASH, start, beg, line);
	}
}

Token Lexer::lessOrLessEqualOrLeftShift() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::LESS_EQUAL, start, beg++, line);
	} else if (peek() == '<') {
		return Token(Token::TYPE::LEFT_SHIFT, start, beg++, line);
	} else {
		return Token(Token::TYPE::LESS, start, beg, line);
	}
}

Token Lexer::greaterOrGreaterEqualOrRightShift() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::GREATER_EQUAL, start, beg++, line);
	} else if (peek() == '>') {
		return Token(Token::TYPE::RIGHT_SHIFT, start, beg++, line);
	} else {
		return Token(Token::TYPE::GREATER, start, beg, line);
	}
}

Token Lexer::bangOrBangEqual() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::BANG_EQUAL, start, beg++, line);
	} else {
		return Token(Token::TYPE::BANG, start, beg, line);
	}
}

Token Lexer::equalOrEqualEqual() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::EQUAL_EQUAL, start, beg++, line);
	} else {
		return Token(Token::TYPE::EQUAL, start, beg, line);
	}
}

Token Lexer::plusOrPlusEqualOrPlusPlus() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::PLUS_EQUAL, start, beg++, line);
	} else if (peek() == '+') {
		return Token(Token::TYPE::PLUS_PLUS, start, beg++, line);
	} else {
		return Token(Token::TYPE::PLUS, start, beg, line);
	}
}

Token Lexer::minusOrMinusEqualOrMinusMinusOrNumberLiteral() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::MINUS_EQUAL, start, beg++, line);
	} else if (peek() == '-') {
		return Token(Token::TYPE::MINUS_MINUS, start, beg++, line);
	} else {
		return Token(Token::TYPE::MINUS, start, beg, line);
	}
}

Token Lexer::dotOrNumberLiteralOrTwoDots() {
	const char* start = beg;
	advance();
	if (isDigit(peek())) {
		return number(true);
	} else if (peek() == '.') {
		advance();
		return Token(Token::TYPE::DOT_DOT, start, beg++, line);
	} else {
		return Token(Token::TYPE::DOT, start, beg, line);
	}
}

Token Lexer::percentOrPercentEqual() {
	const char* start = beg;
	advance();
	if (peek() == '=') {
		return Token(Token::TYPE::PERCENT_EQUAL, start, beg++, line);
	} else {
		return Token(Token::TYPE::PERCENT, start, beg, line);
	}
}

Token Lexer::ampersandOrAmpersandAmpersand() {
	const char* start = beg;
	advance();
	if (peek() == '&') {
		return Token(Token::TYPE::AMPERSAND_AMPERSAND, start, beg++, line);
	} else {
		return Token(Token::TYPE::AMPERSAND, start, beg, line);
	}
}

Token Lexer::pipeOrPipePipe() {
	const char* start = beg;
	advance();
	if (peek() == '|') {
		return Token(Token::TYPE::PIPE_PIPE, start, beg++, line);
	} else {
		return Token(Token::TYPE::PIPE, start, beg, line);
	}
}

Token Lexer::specialOperator() {
	const char* start = beg;
	advance();	// skip the opening "
	while (peek() != '`' && peek() != '\0') {
		if (peek() == '\n') line++;
		advance();
	}
	if (peek() == '\0') ErrorHandler::error(line, "Unterminated backtick operator");
	advance();	// skip the closing `
	std::string_view lexeme(start, beg - start);

	// check if the current string is an operator
	if (lexeme == "`and`")	return Token(Token::TYPE::BIT_AND, start, beg, line);
	if (lexeme == "`or`")	return Token(Token::TYPE::BIT_OR, start, beg, line);
	if (lexeme == "`xor`")	return Token(Token::TYPE::BIT_XOR, start, beg, line);
	if (lexeme == "`mod`")	return Token(Token::TYPE::MOD, start, beg, line);
	if (lexeme == "`div`")	return Token(Token::TYPE::DIV, start, beg, line);

	ErrorHandler::error(line, "Unidentified backtick operator");
	return Token(Token::TYPE::UNIDENTIFIED, start, beg, line);
}


