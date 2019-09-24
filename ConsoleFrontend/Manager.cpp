#include "pch.h"

#include "Manager.h"
#include "Display.h"
#include "Game.h"
#include "Parser.h"

const std::string VALID_MOVE_REQUEST = "Please provide a valid move: ";
const std::string INVALID_MOVE_FOUND = "That move was invalid, please provide a valid move: ";

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::Start()
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

		currentTurn = (currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;
	}

	std::string winner = (currentTurn == Common::Color::WHITE) ? "Black" : "White";
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
			if (tempBoard.data[i][j].occupied)
			{
				board.data[Common::BOARD_LENGTH - i - 1][Common::BOARD_LENGTH - j - 1].Update(tempBoard.data[i][j]);
			}
		}
	}
}
