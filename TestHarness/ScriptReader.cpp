#include "pch.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

#include "ScriptReader.h"

ScriptReader::ScriptReader(std::string & filePath)
{
	ReadScript(filePath);
}

ScriptReader::~ScriptReader()
{
}

void ScriptReader::ReadScript(std::string & filePath)
{
	std::ifstream myFile;
	myFile.open(filePath, std::ios::in);

	bool movesFound = false;

	std::string script = "";

	if (myFile.is_open())
	{
		std::string line;
		while (std::getline(myFile, line))
		{
			if (movesFound)
			{
				script += " ";
				script += line;
			}

			if (std::regex_match(line, std::regex("^[Result")))
			{
				std::smatch match;
				std::regex_search(line, match, std::regex("[01]-[01]"));
				std::string m = match[0];

				if (m.compare("1-0") == 0)
				{
					m_winner = Common::Color::WHITE;
				}
				else if (m.compare("0-1") == 0)
				{
					m_winner = Common::Color::BLACK;
				}
			}
			else if (std::regex_match(line, std::regex("^[PlyCount")))
			{
				std::smatch match;
				std::regex_search(line, match, std::regex("[0-9]+"));
				m_moveCount = stoi(match[0]);
			}
			else if (std::regex_match(line, std::regex("^1\.")))
			{
				movesFound = true;

				script += line;
			}
		}

		myFile.close();
	}

	ParseMoves(script);
}

void ScriptReader::ParseMoves(std::string & script)
{
	CleanComments(script);

	std::vector<std::string> tokens = TokenizeScript(script);

	bool first = true;

	std::pair<std::string, std::string> move;

	for (std::string & tok : tokens)
	{
		if (std::regex_match(tok, std::regex("^[0-9]+\.")))
		{
			std::size_t period = tok.find('.');
			if (period != tok.size() - 1)
			{
				tok = tok.substr(period + 1);
			}
			else
			{
				continue;
			}
		}
		
		if (first)
		{
			move.first = tok;
			first = false;
		}
		else
		{
			move.second = tok;
			m_moveQueue.push_back(move);
			first = true;
		}
	}
}

std::vector<std::string> ScriptReader::TokenizeScript(std::string & script)
{
	std::vector<std::string> tokens;
	std::stringstream ss(script);

	std::string tok;

	while (getline(ss, tok, ' '))
	{
		tokens.push_back(tok);
	}

	return tokens;
}

void ScriptReader::CleanComments(std::string & script)
{
	bool cleaned = false;

	while (!cleaned)
	{
		std::string::iterator it1 = script.begin();
		while (it1 != script.end())
		{
			bool erased = false;

			if (*it1 == '{')
			{
				std::string::iterator it2 = it1;
				while (it2 != script.end())
				{
					if (*it2 == '}')
					{
						script.erase(it1, it2);
						erased = true;
						break;
					}

					it2++;
				}
			}

			if (erased)
			{
				break;
			}

			it1++;
		}

		if (it1 == script.end())
		{
			cleaned = true;
		}
	}
}

Common::Color ScriptReader::GetWinner()
{
	return m_winner;
}

int ScriptReader::GetMoveCount()
{
	return m_moveCount;
}

MoveQueue ScriptReader::GetQueue()
{
	return m_moveQueue;
}