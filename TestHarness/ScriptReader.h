#pragma once

#include <string>
#include <vector>

#include "Common.h"

class ScriptReader
{
public:
	ScriptReader(std::string & filePath);
	~ScriptReader();

private:
	std::vector<std::string> m_moveQueue;
	Common::Color m_winner;
	int m_moveCount;
};