#include "section.h"
#include "block.h"
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>
#include <parse/default/text.h>

namespace parse_act {

section::section() {
	debug_name = "section";
}

section::section(tokenizer &tokens, void *data) {
	debug_name = "section";
	parse(tokens, data);
}

section::~section() {

}

void section::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<block>();
	tokens.expect<parse::new_line>();

	tokens.increment(false);
	tokens.expect("include");

	while (tokens.decrement(__FILE__, __LINE__, data)) {
		tokens.next();
		tokens.increment(true);
		tokens.expect<parse::text>();

		if (tokens.decrement(__FILE__, __LINE__, data)) {
			includes.push_back(tokens.next());
		}
	}

	while (tokens.decrement(__FILE__, __LINE__, data)) {
		if (tokens.found<parse::new_line>()) {
			continue;
		} else {
			blocks.push_back(block(tokens, data));
		}

		tokens.increment(false);
		tokens.expect<block>();
		tokens.expect<parse::new_line>();
	}

	tokens.syntax_end(this);
}

bool section::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next<block>(i);
}

void section::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<section>())
	{
		tokens.register_syntax<section>();
		block::register_syntax(tokens);
		tokens.register_token<parse::text>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>();
	}
}

string section::to_string(string tab) const {
	string result = "";

	return result;
}

parse::syntax *section::clone() const {
	return new section(*this);
}

}
