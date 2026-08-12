#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

TokenList tokenize(const char* jsonString){
	TokenList list;
	list.capacity = 256; // initial guess
	list.count = 0;
	list.tokens = malloc(sizeof(Token) * list.capacity);

	int i = 0;
	while (jsonString[i] != '\0') {
		char c = jsonString[i];
		
		if(c == ' ' || c == '\n' || c == '\r' || c == '\t'){
			i++;
			continue;
		}

		if(list.count >= list.capacity){
			list.capacity *= 2;
			list.tokens = realloc(list.tokens, sizeof(Token) * list.capacity);
		}

		if(c == '{'){
			list.tokens[list.count].type = TOKEN_CURLY_OPEN;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}

		
		else if(c == '}'){
			list.tokens[list.count].type = TOKEN_CURLY_CLOSE;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}


		else if(c == ':'){
			list.tokens[list.count].type = TOKEN_COLON;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}
		

		else if(c == ','){
			list.tokens[list.count].type = TOKEN_COMMA;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}


		else if(c == '['){
			list.tokens[list.count].type = TOKEN_BRACKET_OPEN;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}


		else if(c == ']'){
			list.tokens[list.count].type = TOKEN_BRACKET_CLOSE;
			list.tokens[list.count].value = NULL;
			list.count++;
			i++;
		}

		else if(c == '"'){
			i++;
			int start = i;
			
			while(jsonString[i] != '"' && jsonString[i] != '\0'){
				i++;
			}

			int strLen = i - start;

			char* extractedStr = malloc(strLen + 1);

			strncpy(extractedStr, &jsonString[start], strLen);
			extractedStr[strLen] = '\0';

			list.tokens[list.count].type = TOKEN_STRING;
			list.tokens[list.count].value = extractedStr;
			list.count++;

			i++;
		}

		else if(isdigit(c) || c == '-'){
			int start = i;

			while(isdigit(jsonString[i])|| jsonString[i] == '.' 
				|| jsonString[i] == '-' || jsonString[i] == 'e' || jsonString[i] == 'E'){
				i++;
			}

			int numLen = i - start;
			char* extractedNum = malloc(numLen + 1);
			strncpy(extractedNum, &jsonString[start], numLen);
			extractedNum[numLen] = '\0';

			list.tokens[list.count].type = TOKEN_NUMBER;
			list.tokens[list.count].value = extractedNum;
			list.count++;
		}
		
		else if (isalpha(c)) {
		    int start = i;
		    
		    while (isalpha(jsonString[i])) {
			i++;
		    }
		    
		    int wordLen = i - start;
		    char* extractedWord = malloc(wordLen + 1);
		    strncpy(extractedWord, &jsonString[start], wordLen);
		    extractedWord[wordLen] = '\0';
		    
		    if (strcmp(extractedWord, "true") == 0 || strcmp(extractedWord, "false") == 0) {
			list.tokens[list.count].type = TOKEN_BOOLEAN;
			list.tokens[list.count].value = extractedWord;
			list.count++;
		    } 
		    else if (strcmp(extractedWord, "null") == 0) {
			list.tokens[list.count].type = TOKEN_NULL;
			list.tokens[list.count].value = extractedWord;
			list.count++;
		    } 
		    else {
			free(extractedWord);
		    }
		}
		else {
			i++;
		}

	}

	return list;
}

void freeTokenList(TokenList* list){
	for(int i = 0; i < list->count; i++){
		if(list->tokens[i].value != NULL){
			free(list->tokens[i].value);
		}
	}

	free(list->tokens);
}
