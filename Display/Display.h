#pragma once

#ifdef DISPLAY_EXPORTS
#define DISPLAY_API __declspec(dllexport)
#else
#define DISPLAY_API __declspec(dllimport)
#endif

#include <string>
#include <map>

#include "Common.h"

class Display
{
public:
	DISPLAY_API Display();
	DISPLAY_API ~Display();

	DISPLAY_API bool DisplayBoard(Common::Color currentSide, Common::MiniBoard & board);
	DISPLAY_API Common::MoveRequest PromptUser(std::string & msg);
	DISPLAY_API void InformUser(std::string & msg);

private:
	std::map<int, std::map<int, std::string>> ConvertBoardToText(Common::Color currentSide, Common::MiniBoard & board);
	void PrintBottomKey(Common::Color currentSide);
	void PrintLeftKey(Common::Color currentSide, int index);
};