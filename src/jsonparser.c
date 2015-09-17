/**
 * Copyright (c) 2015 Ryan McCullagh <me@ryanmccullagh.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <jsonparser.h>
#include <object.h>
#include <mutablestring.h>
#include <string.h>
#include <stdlib.h>

#include "json_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The global structure in this compiliation unit
 */
static json_parser_t* parser = NULL;
static json_error_t error;

static void dump_parser(json_parser_t* p)
{
	printf("json_parser_t(%p) {\n", (void *)p);
	printf("\ttext=%s\n", p->text);
	printf("\tlen=%zu\n", p->len);
	printf("\tpos=%zu\n", p->pos);
	printf("\tline=%zu\n", p->line);
	printf("\tlook=%c\n", p->look);
}

static void json_set_last_error(json_error_t* error) 
{
	error->message = malloc(strlen(parser->error_message) + 1);
	memcpy(error->message, parser->error_message, strlen(parser->error_message) + 1);
	error->column = parser->pos;
	error->line = parser->line;
	error->token = parser->look;
}

JSON_PARSER_API 
Object* json_parse(const char* text)
{
	if(text == NULL)
		return NULL;	

	Object* retval;
 	
	parser = json_parser_create(text);
	
	if(!parser)
		return NULL;

	retval = json_internal_parse(parser);

	json_set_last_error(&error);

	json_parser_destroy(parser);

	parser = NULL;

	return retval;
}

JSON_PARSER_API
void json_last_error(json_error_t* _error) 
{

	_error->message = malloc(strlen(error.message) + 1);
	memcpy(_error->message, error.message, strlen(error.message) + 1);
	free(error.message);
	error.message = NULL;
	_error->column = error.column;
	_error->line = error.line;
	_error->token = error.token;

	error.column = 0;
	error.line = 0;
	error.token = 'F';
}

#ifdef __cplusplus
}
#endif
