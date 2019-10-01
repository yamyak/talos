#pragma once

#include <string>
#include <vector>
#include <utility>

#include "Common.h"

typedef std::vector<std::pair<std::string, std::string>> MoveQueue;

class ScriptReader
{
public:
	ScriptReader(std::string & filePath);
	~ScriptReader();

	Common::Color GetWinner();
	int GetMoveCount();
	MoveQueue GetQueue();

private:
	MoveQueue m_moveQueue;
	Common::Color m_winner;
	int m_moveCount;

	void ReadScript(std::string & filePath);
	void ParseMoves(std::string & script);
	void CleanComments(std::string & script);
	std::vector<std::string> TokenizeScript(std::string & script);
};