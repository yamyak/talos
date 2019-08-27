#pragma once

#include <string>
#include <map>

#include "Common.h"

class Display
{
public:
	Display();
	~Display();

	bool DisplayBoard(Common::Color currentSide, Common::MiniBoard & board);
	Common::MoveRequest PromptUser(std::string & msg);
	void InformUser(std::string & msg);

private:
	std::map<int, std::map<int, std::string>> ConvertBoardToText(Common::Color currentSide, Common::MiniBoard & board);
	void PrintBottomKey(Common::Color currentSide);
	void PrintLeftKey(Common::Color currentSide, int index);
};