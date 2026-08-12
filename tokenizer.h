#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
	TOKEN_CURLY_OPEN,
	TOKEN_CURLY_CLOSE,
	TOKEN_BRACKET_OPEN,
	TOKEN_BRACKET_CLOSE,
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_COMMA,
	TOKEN_COLON,
	TOKEN_BOOLEAN,
	TOKEN_NULL,
	TOKEN_EOF
} TokenType;

typedef struct {
	TokenType type;
	char* value; //holds the actaual text
} Token;

typedef struct {
	Token* tokens; //points to allocated array of tokens
	int count;	//how many tokens currently in
	int capacity;	//how many allowed
} TokenList;

TokenList tokenize(const char* jsonString);
void freeTokenList(TokenList* list);
#endif
