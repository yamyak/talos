#include "pch.h"

#include "Manager.h"
#include "Display.h"
#include "Game.h"

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::Start()
{
	Display display;

	Game game;
	Common::MiniBoard board;

	game.Initialize(board);

	//while (game.CheckGameStatus(board))
	{
		TransposeBoard(board);
		display.DisplayBoard(Common::Color::BLACK, board);

		std::string promptMsg = "Please provide a valid move: ";
		bool validMove = false;
		bool first = true;
		while (!validMove)
		{
			Common::MoveRequest move = display.PromptUser(promptMsg);

			validMove = game.AttemptMove(move, board);

			if (first)
			{
				first = false;
				promptMsg = "Invalid Move. " + promptMsg;
			}
		}
	}

	std::string winner = (game.GetCurrentTurn() == Common::Color::WHITE) ? "Black" : "White";
	winner = "The " + winner + " player has won!";
	display.InformUser(winner);
}

void Manager::TransposeBoard(Common::MiniBoard & board)
{
	Common::MiniBoard tempBoard(board);
	board.Clear();

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			if (tempBoard.data[i][j] != nullptr)
			{
				board.data[Common::BOARD_LENGTH - i - 1][Common::BOARD_LENGTH - j - 1] = new Common::PieceInfo(tempBoard.data[i][j]);
			}
		}
	}
}
