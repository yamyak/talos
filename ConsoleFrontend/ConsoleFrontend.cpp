// ConsoleFrontend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Common.h"
#include "Display.h"
#include "Game.h"
#include "Parser.h"

const std::string VALID_MOVE_REQUEST = "Please provide a valid move: ";
const std::string INVALID_MOVE_FOUND = "That move was invalid, please provide a valid move: ";

int main()
{
	Display display;
	Parser parser;
	Game game;
	Common::MiniBoard board;

	game.Initialize(board);

	Common::Color currentTurn = Common::Color::WHITE;
	while (game.CheckGameStatus(currentTurn, board))
	{
		display.DisplayBoard(currentTurn, board);

		std::string promptMsg = VALID_MOVE_REQUEST;
		bool validMove = false;
		while (!validMove)
		{
			std::string moveString = display.PromptUser(promptMsg);

			Common::MoveRequest move = parser.ParseMove(currentTurn, board, moveString);

			validMove = game.AttemptMove(currentTurn, move, board);

			promptMsg = INVALID_MOVE_FOUND;
		}

		board.TransposeBoard();

		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;
	}

	std::string winner = (currentTurn == Common::Color::WHITE) ? "Black" : "White";
	winner = "The " + winner + " player has won!";
	display.InformUser(winner);

	return 0;
}
