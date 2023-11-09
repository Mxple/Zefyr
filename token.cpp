#include "token.h"
#include <iterator>

Token::Token(TYPE t, const char* beg, size_t len, int line)
	: type{t}, lexeme(beg, len), line{line} {}

Token::Token(TYPE t, const char* beg, const char* end, int line)
	: type{t}, lexeme(beg, std::distance(beg, end)), line{line} {}

std::string Token::toString() {
	switch (type) {
		case TYPE::LEFT_PAREN:		return std::string(lexeme) +"\t" + "LEFT_PAREN";
		case TYPE::RIGHT_PAREN:		return std::string(lexeme) +"\t" + "RIGHT_PAREN";
		case TYPE::LEFT_SQUARE:		return std::string(lexeme) +"\t" + "LEFT_SQUARE";
		case TYPE::RIGHT_SQUARE:	return std::string(lexeme) +"\t" + "RIGHT_SQUARE";
		case TYPE::LEFT_BRACE:		return std::string(lexeme) +"\t" + "LEFT_BRACE";
		case TYPE::RIGHT_BRACE:		return std::string(lexeme) +"\t" + "RIGHT_BRACE";
		case TYPE::COMMA:			return std::string(lexeme) +"\t" + "COMMA";
		case TYPE::DOT:				return std::string(lexeme) +"\t" + "DOT";
		case TYPE::MINUS:			return std::string(lexeme) +"\t" + "MINUS";
		case TYPE::PLUS:			return std::string(lexeme) +"\t" + "PLUS";
		case TYPE::SEMICOLON:		return std::string(lexeme) +"\t" + "SEMICOLON";
		case TYPE::SLASH:			return std::string(lexeme) +"\t" + "SLASH";
		case TYPE::STAR:			return std::string(lexeme) +"\t" + "STAR";
		case TYPE::BIT_AND:			return std::string(lexeme) +"\t" + "BIT_AND";
		case TYPE::BIT_OR:			return std::string(lexeme) +"\t" + "BIT_OR";
		case TYPE::BIT_XOR:			return std::string(lexeme) +"\t" + "BIT_XOR";
		case TYPE::BIT_NOT:			return std::string(lexeme) +"\t" + "BIT_NOT";
		case TYPE::BANG:			return std::string(lexeme) +"\t" + "BANG";
		case TYPE::BANG_EQUAL:		return std::string(lexeme) +"\t" + "BANG_EQUAL";
		case TYPE::EQUAL:			return std::string(lexeme) +"\t" + "EQUAL";
		case TYPE::EQUAL_EQUAL:		return std::string(lexeme) +"\t" + "EQUAL_EQUAL";
		case TYPE::GREATER:			return std::string(lexeme) +"\t" + "GREATER";
		case TYPE::GREATER_EQUAL:	return std::string(lexeme) +"\t" + "GREATER_EQUAL";
		case TYPE::LESS:			return std::string(lexeme) +"\t" + "LESS";
		case TYPE::LESS_EQUAL:		return std::string(lexeme) +"\t" + "LESS_EQUAL";
		case TYPE::PLUS_PLUS:		return std::string(lexeme) +"\t" + "PLUS_PLUS";
		case TYPE::MINUS_MINUS:		return std::string(lexeme) +"\t" + "MINUS_MINUS";
		case TYPE::PLUS_EQUAL:		return std::string(lexeme) +"\t" + "PLUS_EQUAL";
		case TYPE::MINUS_EQUAL:		return std::string(lexeme) +"\t" + "MINUS_EQUAL";
		case TYPE::SLASH_EQUAL:		return std::string(lexeme) +"\t" + "SLASH_EQUAL";
		case TYPE::STAR_EQUAL:		return std::string(lexeme) +"\t" + "STAR_EQUAL";
		case TYPE::LEFT_SHIFT:		return std::string(lexeme) +"\t" + "LEFT_SHIFT";
		case TYPE::RIGHT_SHIFT:		return std::string(lexeme) +"\t" + "RIGHT_SHIFT";
		case TYPE::DOT_DOT:			return std::string(lexeme) +"\t" + "DOT_DOT";
		case TYPE::IDENTIFIER:		return std::string(lexeme) +"\t" + "IDENTIFIER";
		case TYPE::STRING:			return std::string(lexeme) +"\t" + "STRING";
		case TYPE::NUMBER:			return std::string(lexeme) +"\t" + "NUMBER";
		case TYPE::AND:				return std::string(lexeme) +"\t" + "AND";
		case TYPE::ELSE:			return std::string(lexeme) +"\t" + "ELSE";
		case TYPE::FALSE:			return std::string(lexeme) +"\t" + "FALSE";
		case TYPE::DEF:				return std::string(lexeme) +"\t" + "DEF";
		case TYPE::FOR:				return std::string(lexeme) +"\t" + "FOR";
		case TYPE::IF:				return std::string(lexeme) +"\t" + "IF";
		case TYPE::OR:				return std::string(lexeme) +"\t" + "OR";
		case TYPE::PRINT:			return std::string(lexeme) +"\t" + "PRINT";
		case TYPE::RETURN:			return std::string(lexeme) +"\t" + "RETURN";
		case TYPE::TRUE:			return std::string(lexeme) +"\t" + "TRUE";
		case TYPE::WHILE:			return std::string(lexeme) +"\t" + "WHILE";
		case TYPE::IN:				return std::string(lexeme) +"\t" + "IN";
		case TYPE::AS:				return std::string(lexeme) +"\t" + "AS";
		case TYPE::BREAK:			return std::string(lexeme) +"\t" + "BREAK";
		case TYPE::UNIDENTIFIED:	return std::string(lexeme) +"\t" + "UNIDENTIFIED";
		case TYPE::END:				return std::string(lexeme) +"\t" + "END";
		case TYPE::PIPE:			return std::string(lexeme) +"\t" + "PIPE";
		case TYPE::QUESTION_MARK:	return std::string(lexeme) +"\t" + "QUESTION_MARK";
		case TYPE::COLON:			return std::string(lexeme) +"\t" + "COLON";
		case TYPE::PERCENT:			return std::string(lexeme) +"\t" + "PERCENT";
		case TYPE::TILDE:			return std::string(lexeme) +"\t" + "TILDE";
		case TYPE::AMPERSAND_EQUAL:	return std::string(lexeme) +"\t" + "AMPERSAND_EQUAL";
		case TYPE::AMPERSAND_AMPERSAND:	return std::string(lexeme) +"\t" + "AMPERSAND_AMPERSAND";
		case TYPE::PIPE_EQUAL:		return std::string(lexeme) +"\t" + "PIPE_EQUAL";
		case TYPE::PIPE_PIPE:		return std::string(lexeme) +"\t" + "PIPE_PIPE";
		default:					return std::string(lexeme) +"\t" + "UNKNOWN";
	}

}
