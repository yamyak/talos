// TestHarness.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>

#include "Common.h"
#include "Game.h"
#include "Display.h"
#include "Parser.h"
#include "ConfigReader.h"
#include "ScriptReader.h"

bool MakeMove(Game & game, Parser & parser, Display & display, Common::MiniBoard & board, Common::Color & turn, std::string & moveStr, bool verbose)
{
	Common::MoveRequest move = parser.ParseMove(turn, board, moveStr);

	bool validMove = game.AttemptMove(turn, move, board);

	if (verbose && turn == Common::Color::WHITE)
	{
		display.DisplayBoard(Common::Color::WHITE, board);
	}

	board.TransposeBoard();

	if (verbose && turn == Common::Color::BLACK)
	{
		display.DisplayBoard(Common::Color::WHITE, board);
	}

	turn = (turn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

	validMove = validMove && game.CheckGameStatus(turn, board);

	return validMove;
}

int main(int argc, char** argv)
{
	std::string configPath;

	bool verbose = false;

	if (argc > 3 || argc < 2)
	{
		return -1;
	}
	else
	{
		configPath = argv[1];

		if (argc == 3 && std::string(argv[2]).compare("-v") == 0)
		{
			verbose = true;
		}
	}

	ConfigReader config(configPath);

	bool testStatus = true;

	for (std::string & file : config.GetFilePaths())
	{
		std::cout << "Script File: " << file << std::endl << std::endl;
		ScriptReader script(file);

		int count = 0;
		Common::Color currentTurn = Common::Color::WHITE;
		MoveQueue moves = script.GetQueue();

		Parser parser;
		Display display;
		Game game;
		Common::MiniBoard board;
		game.Initialize(board);

		if (verbose)
		{
			display.DisplayBoard(currentTurn, board);
		}

		for (std::pair<std::string, std::string> & turn : moves)
		{
			if (!turn.first.empty())
			{
				count++;
				if (!MakeMove(game, parser, display, board, currentTurn, turn.first, verbose))
				{
					break;
				}
			}

			if (!turn.second.empty())
			{
				count++;
				if (!MakeMove(game, parser, display, board, currentTurn, turn.second, verbose))
				{
					break;
				}
			}
		}

		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

		if (count != script.GetMoveCount())
		{
			std::cout << "Script Failed: Error at move " << count << std::endl;
			testStatus = false;
		}
		else if (currentTurn != script.GetWinner())
		{
			std::cout << "Script Failed: Incorrect winner " << ((currentTurn == Common::Color::WHITE) ? "White" : "Black") << std::endl;
			testStatus = false;
		}
		else
		{
			std::cout << "Script Passed" << std::endl;
		}

		std::cout << std::endl;
	}

	if (testStatus)
	{
		std::cout << "All Scripts Passed" << std::endl;
	}
	else
	{
		std::cout << "Script Failures Found" << std::endl;
	}

	return 0;
}
