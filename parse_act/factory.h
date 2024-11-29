#pragma once

#include "section.h"

namespace parse_act
{

parse::syntax *produce(tokenizer &tokens, void *data)
{
	return new section(tokens, data);
};

string factory_type()
{
	return "[" + section().debug_name + "]";
}

}
