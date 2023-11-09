#pragma once
#include <string>
#include <string_view>

struct Token {
public:
	enum class TYPE {
		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN, LEFT_SQUARE, RIGHT_SQUARE,
		LEFT_BRACE, RIGHT_BRACE, COMMA, DOT,
		MINUS, PLUS, SEMICOLON, SLASH, STAR,
		QUESTION_MARK, COLON, PERCENT, TILDE,
		AMPERSAND, PIPE,						// ampersand maybe for reference


		// Special operators
		BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,		// might not be 1 char
		MOD, DIV,
		
		// One or two character tokens.
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL,
		PLUS_PLUS, MINUS_MINUS, PLUS_EQUAL, MINUS_EQUAL,
		SLASH_EQUAL, STAR_EQUAL,
		LEFT_SHIFT, RIGHT_SHIFT,
		AMPERSAND_EQUAL, AMPERSAND_AMPERSAND,
		PIPE_EQUAL, PIPE_PIPE,
		PERCENT_EQUAL,
		DOT_DOT,	// for range operator

		// Literals.
		IDENTIFIER, STRING, NUMBER,

		// Keywords.
		AND, ELSE, FALSE, DEF, FOR, IF, OR,
		PRINT, RETURN, TRUE, WHILE, IN, AS, BREAK,

		UNIDENTIFIED, END
	};

	TYPE type;					// specifies the type of token
	std::string_view lexeme;	// the view of the text of the token
	int line;

	Token(TYPE t, const char* beg, size_t len, int line);
	Token(TYPE t, const char* beg, const char* end, int line);

	std::string toString();
};
