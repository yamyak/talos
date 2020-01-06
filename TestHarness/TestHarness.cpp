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

// make a move in the game
bool MakeMove(Game & game, Parser & parser, Display & display, Common::MiniBoard & board, Common::Color & turn, std::string & moveStr, bool verbose)
{
	// parse the move string
	Common::MoveRequest move = parser.ParseMove(turn, board, moveStr);

	// attempt the move on the game board
	bool validMove = game.AttemptMove(turn, move, board);

	// display game board if verbose mode
	// do this before transposing if currently white move
	if (verbose && turn == Common::Color::WHITE)
	{
		// always display board from white perspective for stable view
		display.DisplayBoard(Common::Color::WHITE, board);
	}

	// transpose the board
	board.TransposeBoard();

	// display game board if verbose mode
	// do this after transposing if currently black move
	// in order to check white perspective
	if (verbose && turn == Common::Color::BLACK)
	{
		// always display board from white perspective for stable view
		display.DisplayBoard(Common::Color::WHITE, board);
	}

	// switch player turn
	turn = (turn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

	// check that checkmate not reached
	validMove = validMove && game.CheckGameStatus(turn, board);

	return validMove;
}

// main loop to run test harness
int main(int argc, char** argv)
{
	std::string configPath;

	bool verbose = false;

	// if program doesn't have 2 or 3 arguments, fail
	if (argc > 3 || argc < 2)
	{
		return -1;
	}
	else
	{
		// read in config file path
		configPath = argv[1];

		// if third argument exist and is "-v", set verbose mode flag
		if (argc == 3 && std::string(argv[2]).compare("-v") == 0)
		{
			verbose = true;
		}
	}

	// read in and parse config files
	ConfigReader config(configPath);

	bool testStatus = true;

	// iterate through all move script files listed in config file
	for (std::string & file : config.GetFilePaths())
	{
		// read in and parse current move script file
		std::cout << "Script File: " << file << std::endl << std::endl;
		ScriptReader script(file);

		// initialize this game run
		int count = 0;
		Common::Color currentTurn = Common::Color::WHITE;
		// get moves
		MoveQueue moves = script.GetQueue();

		// initialize game components
		Parser parser;
		Display display;
		Game game;
		Common::MiniBoard board;
		game.Initialize(board);

		// if verbose mode, display the board 
		if (verbose)
		{
			display.DisplayBoard(currentTurn, board);
		}

		// iterate through all white/black move pairs
		for (std::pair<std::string, std::string> & turn : moves)
		{
			// if white move exists
			if (!turn.first.empty())
			{
				// increment number of moves played
				count++;
				// attempt to make the move
				if (!MakeMove(game, parser, display, board, currentTurn, turn.first, verbose))
				{
					// if move failed, stop running the script
					break;
				}
			}

			// if white move exists
			if (!turn.second.empty())
			{
				// increment number of moves played
				count++;
				// attempt to make the move
				if (!MakeMove(game, parser, display, board, currentTurn, turn.second, verbose))
				{
					// if move failed, stop running the script
					break;
				}
			}
		}

		// switch player back to expected winner side
		// since MakeMove function automatically switches it after move made
		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

		// check if moves made equal number expected in script
		if (count != script.GetMoveCount())
		{
			// report failure due to issue at last move made
			std::cout << "Script Failed: Error at move " << count << std::endl;
			testStatus = false;
		}
		// check if winner is expected winner in script
		else if (currentTurn != script.GetWinner())
		{
			// report failure due to wrong winner
			std::cout << "Script Failed: Incorrect winner " << ((currentTurn == Common::Color::WHITE) ? "White" : "Black") << std::endl;
			testStatus = false;
		}
		else
		{
			// report script run successfully
			std::cout << "Script Passed" << std::endl;
		}

		std::cout << std::endl;
	}

	// check if all scripts passed and report
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
