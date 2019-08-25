#include "pch.h"
#include "Display.h"

#include <iostream>
#include <iomanip>

Display::Display()
{
}

Display::~Display()
{
}

bool Display::DisplayBoard(Common::Color currentSide, Common::MiniBoard & board)
{
	bool status = true;

	std::cout << std::endl;

	std::map<int, std::map<int, std::string>> boardText = ConvertBoardToText(currentSide, board);

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			std::cout << std::setw(3) << boardText[i][j];
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

	return status;
}

Common::MoveRequest Display::PromptUser(std::string & msg)
{
	Common::MoveRequest move;

	std::cout << msg;

	std::string moveStr;
	std::cin >> moveStr;

	return move;
}

void Display::InformUser(std::string & msg)
{
	std::cout << msg << std::endl;
}

std::map<int, std::map<int, std::string>> Display::ConvertBoardToText(Common::Color currentSide, Common::MiniBoard & board)
{
	std::map<int, std::map<int, std::string>> output;

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		int iT = (currentSide == Common::Color::WHITE) ? Common::BOARD_LENGTH - i - 1 : i;

		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			int jT = (currentSide == Common::Color::BLACK) ? Common::BOARD_LENGTH - j - 1 : j;

			if (board.data[j][i] == nullptr)
			{
				output[iT][jT] = "00";
			}
			else
			{
				Common::PieceInfo * p = board.data[j][i];

				output[iT][jT] = (p->color == Common::Color::WHITE) ? "W" : "B";
				
				switch (p->type)
				{
					case Common::PieceType::KING:
						output[iT][jT] += "K";
						break;
					case Common::PieceType::QUEEN:
						output[iT][jT] += "Q";
						break;
					case Common::PieceType::KNIGHT:
						output[iT][jT] += "N";
						break;
					case Common::PieceType::BISHOP:
						output[iT][jT] += "B";
						break;
					case Common::PieceType::ROOK:
						output[iT][jT] += "R";
						break;
					case Common::PieceType::PAWN:
						output[iT][jT] += "P";
						break;
					default:
						break;
				}
			}
		}
	}

	return output;
}