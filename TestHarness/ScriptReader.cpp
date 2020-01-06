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

// read in the recorded game script
void ScriptReader::ReadScript(std::string & filePath)
{
	// open the script file
	std::ifstream myFile;
	myFile.open(filePath, std::ios::in);

	bool movesFound = false;

	std::string script = "";

	// check if script file is open
	if (myFile.is_open())
	{
		std::string line;
		// iterate through getting lines from script file
		while (std::getline(myFile, line))
		{
			// check if flag set
			if (movesFound)
			{
				// add new moves line to overall total moves line and continue
				script += " ";
				script += line;
				continue;
			}

			// check if line matches results line in script file
			if (std::regex_search(line, std::regex("^\\[Result")))
			{
				// search line for part matching the final result and pull it out
				std::smatch match;
				std::regex_search(line, match, std::regex("[01]-[01]"));
				std::string m = match[0];

				// based off final result string, set expected winner of game
				if (m.compare("1-0") == 0)
				{
					m_winner = Common::Color::WHITE;
				}
				else if (m.compare("0-1") == 0)
				{
					m_winner = Common::Color::BLACK;
				}
			}
			// check if line matches # of moves line in script file
			else if (std::regex_search(line, std::regex("^\\[PlyCount")))
			{
				// search line for part matching a number and pull it out
				std::smatch match;
				std::regex_search(line, match, std::regex("[0-9]+"));
				// set the expected number of moves played in game
				m_moveCount = stoi(match[0]);
			}
			// check if line matches beginning of moves list in script file
			else if (std::regex_search(line, std::regex("^1\\.")))
			{
				// set flag
				movesFound = true;

				// start adding moves lines to 1 total moves line
				script += line;
			}
		}

		// close file
		myFile.close();
	}

	// parse total moves line
	ParseMoves(script);
}

// parse all moves in script
void ScriptReader::ParseMoves(std::string & script)
{
	// remove comments from move string
	CleanComments(script);

	// split move string into individual move strings
	std::vector<std::string> tokens = TokenizeScript(script);

	bool first = true;

	// pairs of white and black moves
	std::pair<std::string, std::string> move;

	// iterate through all tokens
	for (std::string & tok : tokens)
	{
		// check if token is a move index or move
		if (std::regex_search(tok, std::regex("^[0-9]+\\.")))
		{
			// find "." character in token
			std::size_t period = tok.find('.');
			// check if period is not at end of token
			// means no space between move index and move string
			if (period != tok.size() - 1)
			{
				// only use everything after period
				tok = tok.substr(period + 1);

				// check if move periods exist at start of string due to ellipses 
				// means move covered multiple lines or have comment in between
				while (tok.at(0) == '.')
				{
					// remove first character (".") from string
					tok = tok.substr(1);
				}
			}
			else
			{
				// if simply a move index without move, ignore and move on
				continue;
			}
		}
		
		// check if odd numbered move (white move)
		if (first)
		{
			// add to first part of a pair and add to list of moves
			move.first = tok;
			m_moveQueue.push_back(move);
			first = false;
		}
		else
		{
			// if even number move (black move), add to latest pair as second part
			m_moveQueue[m_moveQueue.size() - 1].second = tok;
			first = true;
		}
	}
}

// split up move string into individual moves
std::vector<std::string> ScriptReader::TokenizeScript(std::string & script)
{
	// convert string into stringstream
	std::vector<std::string> tokens;
	std::stringstream ss(script);

	std::string tok;

	// tokenize string using spaces
	while (getline(ss, tok, ' '))
	{
		// check if token exists
		if (!tok.empty())
		{
			// as long as token is not final result token, add to token list
			if (!std::regex_search(tok, std::regex("0-1")) &&
				!std::regex_search(tok, std::regex("1-0")))
			{
				tokens.push_back(tok);
			}
		}
	}

	return tokens;
}

// remove any comments from the move line
void ScriptReader::CleanComments(std::string & script)
{
	bool cleaned = false;

	// continue until line end reached
	while (!cleaned)
	{
		// start at beginning of line
		std::string::iterator it1 = script.begin();

		// continue until line end reached
		while (it1 != script.end())
		{
			bool erased = false;

			// check if comment start found
			if (*it1 == '{')
			{
				// save start point
				std::string::iterator it2 = it1;
				// continue until line end reached
				while (it2 != script.end())
				{
					// check if comment end found
					if (*it2 == '}')
					{
						// erase portion of line that is comment
						script.erase(it1, it2 + 1);
						// set flag stating that part of string erased
						erased = true;
						break;
					}

					// iterate through string until comment end found
					it2++;
				}
			}

			// if erased portion of string in this iteration of loop
			// break out of this loop and start from beginning again
			if (erased)
			{
				break;
			}

			// iterate through string until comment start found
			it1++;
		}

		// if end of string found, done and can exit loop
		if (it1 == script.end())
		{
			cleaned = true;
		}
	}
}

// return the winner color
Common::Color ScriptReader::GetWinner()
{
	return m_winner;
}

// return the number of moves script has
int ScriptReader::GetMoveCount()
{
	return m_moveCount;
}

// return the moves
MoveQueue ScriptReader::GetQueue()
{
	return m_moveQueue;
}