#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>
#include "parser.h"
#include "tokenizer.h"

JsonNode* parseValue(TokenList* list, int* index);

char* duplicateString(const char* src) {
	if (!src) return NULL;
	size_t len = strlen(src);
	char* dest = malloc(len + 1);
	if (dest) {
		strcpy(dest, src);
	}
   	return dest;
}

JsonNode* parseObject(TokenList* list, int* idx){
	//consume '{'
	++*idx;

	JsonNode* node = malloc(sizeof(JsonNode));
	node->type = OBJECT_VALUE;
	node->value.objectVal = malloc(sizeof(JsonObject));

	JsonObject* obj = node->value.objectVal;

	obj->capacity = 8;
	obj->count = 0;
	obj->pairs = malloc(sizeof(KeyValPair) * obj->capacity);

	while(*idx < list->count){
		//key has to be a string
		if(list->tokens[*idx].type != TOKEN_STRING){
			fprintf(stderr, "Json Parsing error: Json Key must be a string\n");
			freeJsonNode(node);
			return NULL;
		}

		char* key = duplicateString(list->tokens[*idx].value);
		++*idx;

		//next should be a semicolon
		if(*idx >= list->count || list->tokens[*idx].type != TOKEN_COLON){
			fprintf(stderr, "Json Parsing error: Expected ':' after key\n");
			free(key);
			freeJsonNode(node);
			return NULL;
		}

		++*idx;

		//parse val recursively
		JsonNode* value = parseValue(list, idx);
		if(!value){
			free(key);
			freeJsonNode(node);
			return NULL;
		}

		if(obj->count >= obj->capacity){
			obj->capacity *= 2;
			obj->pairs = realloc(obj->pairs, sizeof(KeyValPair) * obj->capacity);
		}

		obj->pairs[obj->count].key = key;
		obj->pairs[obj->count].value = value;
		obj->count++;

		//next , or }
		if(*idx >= list->count) break;

		if(list->tokens[*idx].type == TOKEN_COMMA){
			++*idx;
		}
		else if(list->tokens[*idx].type == TOKEN_CURLY_CLOSE){
			++*idx;
			return node;
		}
		else{
			fprintf(stderr, "Json Parsing error: Expected ',' or '}'\n" );
			freeJsonNode(node);
			return NULL;
		}
	}
	return node;
}

JsonNode* parseArray(TokenList* list, int* idx){
	++*idx;

	JsonNode* node = malloc(sizeof(JsonNode));
	node->type = ARRAY_VALUE;
	node->value.arrayVal = malloc(sizeof(JsonArray));

	JsonArray* arr = node->value.arrayVal;

	arr->capacity = 8;
    	arr->count = 0;
    	arr->elements = malloc(sizeof(JsonNode*) * arr->capacity);


	if(*idx < list->count && list->tokens[*idx].type == TOKEN_BRACKET_CLOSE){
		++*idx;
		return node;
	}

	while(*idx < list->count){
		JsonNode* element = parseValue(list, idx);
		if(!element){
			freeJsonNode(node);
			return NULL;
		}

		if(arr->count >= arr->capacity){
			arr->capacity *= 2;
			arr->elements = realloc(arr->elements, sizeof(JsonNode*) * arr->capacity);
		}

		arr->elements[arr->count++] = element;

		if(*idx >= list->count) break;

		if(list->tokens[*idx].type == TOKEN_COMMA){
			++*idx;
		}
		else if(list->tokens[*idx].type == TOKEN_BRACKET_CLOSE){
			++*idx;
			return node;
		}
		else{
			fprintf(stderr, "Json Parsing error: Expected ',' or ']' in array\n");
			freeJsonNode(node);
			return NULL;
		}
	}
	return node;
}

JsonNode* parseValue(TokenList* list, int* idx){
	if(*idx >= list->count) return NULL;

	Token t = list->tokens[*idx];
	JsonNode*  node = malloc(sizeof(JsonNode));

	switch (t.type) {
		case TOKEN_CURLY_OPEN:
			free(node);
			return parseObject(list, idx);
		case TOKEN_BRACKET_OPEN:
			free(node);
			return parseArray(list, idx);
		case TOKEN_STRING:
			node->type = STRING_VALUE;
			node->value.stringVal = duplicateString(t.value);
			++*idx;
			return node;
		case TOKEN_NUMBER:
			node->type = NUMBER_VALUE;
			node->value.numberVal = atof(t.value);
			++*idx;
			return node;
		case TOKEN_BOOLEAN:
			node->type = BOOLEAN_VALUE;
			node->value.booleanVal = (strcmp(t.value, "true") == 0) ? 1 : 0;
			++*idx;
			return node;
		case TOKEN_NULL:
			node->type = NULL_VALUE;
			++*idx;
			return node;
		default:
			fprintf(stderr, "Json Parsing error: Unexpected token type %d\n", t.type);
			free(node);
			return NULL;
	}
}

JsonNode* parse(TokenList* list) {
    int idx = 0;
    return parseValue(list, &idx);
}

void freeJsonNode(JsonNode* node){
	if(!node) return;

	switch (node->type) {
		case STRING_VALUE:
			free(node->value.stringVal);
			break;
		case OBJECT_VALUE:
			if (node->value.objectVal) {
                		JsonObject* obj = node->value.objectVal;
				for (int i = 0; i < obj->count; ++i) {
				    free(obj->pairs[i].key);
				    freeJsonNode(obj->pairs[i].value);
				}
				free(obj->pairs);
				free(obj);
		    	}
			break;
		case ARRAY_VALUE:
			if(node->value.arrayVal){
				JsonArray* arr = node->value.arrayVal;
				for(int i = 0; i < arr->count; i++){
					freeJsonNode(arr->elements[i]);
				}
				free(arr->elements);
				free(arr);
			}
			break;
		case NUMBER_VALUE:
		case BOOLEAN_VALUE:
		case NULL_VALUE:
			break;
			//stack values so no free needed
		
	}
	free(node);
}
