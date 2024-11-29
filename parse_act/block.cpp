#include "block.h"
#include <parse/default/white_space.h>
#include <parse/default/new_line.h>
#include <parse/default/text.h>
#include <parse/default/number.h>
#include <parse/default/instance.h>

namespace parse_act {

block::block() {
	debug_name = "block";
}

block::block(tokenizer &tokens, void *data) {
	debug_name = "block";
	parse(tokens, data);
}

block::~block() {

}

void block::parse(tokenizer &tokens, void *data) {
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("int");
	tokens.expect("string");
	tokens.expect("real");
	tokens.expect("int_table");
	tokens.expect("string_table");
	tokens.expect("real_table");
	tokens.expect("begin");

	if (tokens.decrement(__FILE__, __LINE__, data)) {
		kind = tokens.next();
		bool isTable = (kind.find("_table") != string::npos);

		name = "";
		if (tokens.decrement(__FILE__, __LINE__, data)) {
			name = tokens.next();
		}

		if (kind == "begin") {
			tokens.increment(true);
			tokens.expect<parse::new_line>();

			tokens.increment(true);
			tokens.expect("end");

			tokens.increment(true);
			tokens.expect<section>();

			tokens.increment(true);
			tokens.expect<parse::new_line>();

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				sub.parse(tokens, data);
			}

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}

			if (tokens.decrement(__FILE__, __LINE__, data)) {
				tokens.next();
			}			
		} else {
			tokens.increment(true);
			tokens.expect<parse::number>();
			tokens.expect<parse::text>();

			while (tokens.decrement(__FILE__, __LINE__, data)) {
				if (tokens.found<parse::new_line>()) {
					break;
				}
				values.push_back(tokens.next());
				tokens.increment(true);
				if (isTable) {
					tokens.expect<parse::number>();
					tokens.expect<parse::text>();
				}
				tokens.expect<parse::new_line>();
			}
		}
	}

	tokens.syntax_end(this);
}

bool block::is_next(tokenizer &tokens, int i, void *data) {
	return tokens.is_next("begin", i)
		or tokens.is_next("int", i)
		or tokens.is_next("real", i)
		or tokens.is_next("string", i)
		or tokens.is_next("int_table", i)
		or tokens.is_next("real_table", i)
		or tokens.is_next("string_table", i);
}

void block::register_syntax(tokenizer &tokens) {
	if (!tokens.syntax_registered<block>())
	{
		tokens.register_syntax<block>();
		section::register_syntax(tokens);
		tokens.register_token<parse::number>();
		tokens.register_token<parse::text>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::white_space>(false);
		tokens.register_token<parse::new_line>();
	}
}

string block::to_string(string tab) const {
	string result = tab + kind + " " + name;

	if (kind == "begin") {
		result += "\n";
		result += sub.to_string(tab+"\t");
		result += tab + "end\n";
	} else {
		for (auto v = values.begin(); v != values.end(); v++) {
			result += " "  + *v;
		}
		result += "\n";
	}	
	
	return result;
}

parse::syntax *block::clone() const {
	return new block(*this);
}


}
