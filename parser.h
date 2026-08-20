#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef enum {
	//types of data a node can hold
	OBJECT_VALUE,
	ARRAY_VALUE,
	STRING_VALUE,
	NUMBER_VALUE,
	BOOLEAN_VALUE,
	NULL_VALUE
} JsonType;

struct JsonObject;
struct JsonArray;

typedef struct JsonNode {
	JsonType type;

	//since only one value will ever be stored in a node, we can use union
	union {
		char* stringVal;
		double numberVal;
		int booleanVal;
		struct JsonObject* objectVal;
		struct JsonArray* arrayVal;
	} value;
} JsonNode;

typedef struct {
	char* key;
	JsonNode* value;
} KeyValPair;

typedef struct JsonObject {
	KeyValPair* pairs;
	int count;
	int capacity;
} JsonObject;

typedef struct JsonArray {
	//an array of pointers to different nodes
	JsonNode** elements;
	int count;
	int capacity;
} JsonArray;

JsonNode* parse(TokenList* list);
void freeJsonNode(JsonNode* node);

#endif
