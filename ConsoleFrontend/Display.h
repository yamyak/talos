#pragma once

#include <string>

#include "Common.h"

class Display
{
public:
	Display();
	~Display();

	bool DisplayBoard(Common::MiniBoard & board);
	Common::MoveRequest PromptUser(std::string prompt);
};