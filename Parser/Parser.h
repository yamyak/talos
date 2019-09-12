#pragma once

#ifdef PARSER_EXPORTS
#define PARSER_API __declspec(dllexport)
#else
#define PARSER_API __declspec(dllimport)
#endif

#include <string>

#include "Common.h"

class Parser
{
public:
	PARSER_API Parser();
	PARSER_API ~Parser();

	PARSER_API Common::MoveRequest ParseMove(Common::Color color, Common::MiniBoard & board, std::string & moveString);
private:

};