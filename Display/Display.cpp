#include "stdafx.h"

#define DISPLAY_EXPORTS

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

	std::map<int, std::map<int, std::string>> boardText = ConvertBoardToText(currentSide, board);

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		PrintLeftKey(currentSide, i);

		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			std::cout << std::setw(3) << boardText[i][j];
		}

		std::cout << std::endl;
	}

	PrintBottomKey(currentSide);

	std::cout << std::endl;

	return status;
}

void Display::PrintBottomKey(Common::Color currentSide)
{
	if (currentSide == Common::Color::WHITE)
	{
		std::cout << std::setw(5) << " A";
		std::cout << std::setw(3) << " B";
		std::cout << std::setw(3) << " C";
		std::cout << std::setw(3) << " D";
		std::cout << std::setw(3) << " E";
		std::cout << std::setw(3) << " F";
		std::cout << std::setw(3) << " G";
		std::cout << std::setw(3) << " H";
	}
	else
	{
		std::cout << std::setw(5) << " H";
		std::cout << std::setw(3) << " G";
		std::cout << std::setw(3) << " F";
		std::cout << std::setw(3) << " E";
		std::cout << std::setw(3) << " D";
		std::cout << std::setw(3) << " C";
		std::cout << std::setw(3) << " B";
		std::cout << std::setw(3) << " A";
	}

	std::cout << std::endl;
}

void Display::PrintLeftKey(Common::Color currentSide, int index)
{
	int out = (currentSide == Common::Color::WHITE) ? (Common::BOARD_LENGTH - index) : (index + 1);
	std::cout << std::setw(2) << std::to_string(out);
}

std::string Display::PromptUser(std::string & msg)
{
	std::cout << msg;

	std::string moveStr;
	std::cin >> moveStr;

	return moveStr;
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
		int iT = Common::BOARD_LENGTH - i - 1;

		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			int jT = j;

			if (!board.data[j][i].occupied)
			{
				output[iT][jT] = "00";
			}
			else
			{
				Common::PieceInfo & p = board.data[j][i];

				output[iT][jT] = (p.color == Common::Color::WHITE) ? "W" : "B";

				switch (p.type)
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