#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>

namespace parse_act
{

struct block;

struct section : parse::syntax
{
	section();
	section(tokenizer &tokens, void *data = NULL);
	~section();

	vector<string> includes;
	vector<block> blocks;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}
