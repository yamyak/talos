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

// given a side and the board layout, displays it on the console
bool Display::DisplayBoard(Common::Color currentSide, Common::MiniBoard & board)
{
	bool status = true;

	// convert all board locations into strings
	std::map<int, std::map<int, std::string>> boardText = ConvertBoardToText(board);

	// iterate through all board locations
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		// print the row key for the current row
		PrintLeftKey(currentSide, i);

		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			// print the current board location
			std::cout << std::setw(3) << boardText[i][j];
		}

		std::cout << std::endl;
	}

	// print all columns keys
	PrintBottomKey(currentSide);

	std::cout << std::endl;

	return status;
}

// print the column key based on what side has player perspective
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

// print the row key for board based on what side has player perspective
void Display::PrintLeftKey(Common::Color currentSide, int index)
{
	// black side is default
	int out = (currentSide == Common::Color::WHITE) ? (Common::BOARD_LENGTH - index) : (index + 1);
	std::cout << std::setw(2) << std::to_string(out);
}

// print message and get player input as a string
std::string Display::PromptUser(std::string & msg)
{
	std::cout << msg;

	std::string moveStr;
	std::cin >> moveStr;

	return moveStr;
}

// print message to console
void Display::InformUser(std::string & msg)
{
	std::cout << msg << std::endl;
}

// convert each coordinate location of the board into a text string
std::map<int, std::map<int, std::string>> Display::ConvertBoardToText(Common::MiniBoard & board)
{
	std::map<int, std::map<int, std::string>> output;

	// iterate along x-axis of board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		// flip board along x-axis
		int iT = Common::BOARD_LENGTH - i - 1;

		// iterate along y-axis of board
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			int jT = j;

			// if board location is unoccupied, set text to "00"
			if (!board.data[j][i].occupied)
			{
				output[iT][jT] = "00";
			}
			else
			{
				// get piece at board location
				Common::PieceInfo & p = board.data[j][i];

				// first letter is piece color
				output[iT][jT] = (p.color == Common::Color::WHITE) ? "W" : "B";

				// second letter is piece type
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

	// return 2d map of board
	return output;
}