#pragma once

#include <parse/parse.h>
#include <parse/syntax.h>
#include "section.h"

namespace parse_act
{

struct block : parse::syntax
{
	block();
	block(tokenizer &tokens, void *data = NULL);
	~block();

	string kind;
	string name;
	vector<string> values;
	section sub;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}
