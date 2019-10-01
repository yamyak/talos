// TestHarness.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>

#include "Common.h"
#include "Game.h"
#include "Parser.h"
#include "ConfigReader.h"
#include "ScriptReader.h"

bool MakeMove(Game & game, Parser & parser, Common::MiniBoard & board, Common::Color & turn, std::string & moveStr)
{
	Common::MoveRequest move = parser.ParseMove(turn, board, moveStr);

	bool validMove = game.AttemptMove(turn, move, board);

	//board.TransposeBoard();
	turn = (turn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

	validMove = validMove && game.CheckGameStatus(turn, board);

	return validMove;
}

int main(int argc, char** argv)
{
	std::string cmdArgs;

	if (argc > 1)
	{
		return -1;
	}
	else if (argc == 1)
	{
		cmdArgs = argv[0];
	}

	ConfigReader config(cmdArgs);

	for (std::string & file : config.GetFilePaths())
	{
		std::cout << "Script File: " << file << std::endl;
		ScriptReader script(file);

		int count = 0;
		Common::Color currentTurn = Common::Color::WHITE;;
		MoveQueue moves = script.GetQueue();

		Parser parser;
		Game game;
		Common::MiniBoard board;
		game.Initialize(board);

		for (std::pair<std::string, std::string> & turn : moves)
		{
			count++;
			if (!MakeMove(game, parser, board, currentTurn, turn.first))
			{
				break;
			}

			count++;
			if (!MakeMove(game, parser, board, currentTurn, turn.second))
			{
				break;
			}
		}

		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

		if (count != script.GetMoveCount())
		{
			std::cout << "Script Failed: Error at move " << count << std::endl;
		}
		else if (currentTurn != script.GetWinner())
		{
			std::cout << "Script Failed: Incorrect winner " << ((currentTurn == Common::Color::WHITE) ? "White" : "Black") << std::endl;
		}
		else
		{
			std::cout << "Script Passed" << std::endl;
		}

		std::cout << std::endl;
	}

	return 0;
}
