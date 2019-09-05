#pragma once

#include <string>

#include "Common.h"

class Parser
{
public:
	Parser();
	~Parser();

	Common::MoveRequest ParseMove(Common::Color color, Common::MiniBoard & board, std::string & moveString);
private:
	
};