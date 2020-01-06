// ConsoleFrontend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Common.h"
#include "Display.h"
#include "Game.h"
#include "Parser.h"

const std::string VALID_MOVE_REQUEST = "Please provide a valid move: ";
const std::string INVALID_MOVE_FOUND = "That move was invalid, please provide a valid move: ";

// main function that continually runs until game is won by 1 side
int main()
{
	// create all game components
	Display display;
	Parser parser;
	Game game;
	Common::MiniBoard board;

	// initialize the game with the board
	game.Initialize(board);

	// start the game with the white side
	Common::Color currentTurn = Common::Color::WHITE;

	// iterate between both sides of the game
	// continue until current side is in check mate
	while (game.CheckGameStatus(currentTurn, board))
	{
		// display the board
		display.DisplayBoard(currentTurn, board);

		std::string promptMsg = VALID_MOVE_REQUEST;
		bool validMove = false;

		// prompt the player until a valid move is provide
		while (!validMove)
		{
			// prompt the player and get the input returned
			std::string moveString = display.PromptUser(promptMsg);

			// convert the input string into a move request
			Common::MoveRequest move = parser.ParseMove(currentTurn, board, moveString);

			// attempt to play the move and return status of attempt
			// if move is valid, it is played
			validMove = game.AttemptMove(currentTurn, move, board);

			// prepare prompt string is case of invalid move
			promptMsg = INVALID_MOVE_FOUND;
		}

		// tranpose the board in preparation of displaying from opposite player perspective
		board.TransposeBoard();

		// switch the next player
		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;
	}

	// create a winner output string and display it
	std::string winner = (currentTurn == Common::Color::WHITE) ? "Black" : "White";
	winner = "The " + winner + " player has won!";
	display.InformUser(winner);

	return 0;
}
