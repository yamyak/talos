#pragma once

#include <map>

namespace Common
{
	// size of the side of a board
	const int BOARD_LENGTH = 8;

	struct Location
	{
		int x;
		int y;
	};

	enum PieceType
	{
		PAWN,
		KNIGHT,
		BISHOP,
		ROOK,
		QUEEN,
		KING
	};

	enum Color
	{
		WHITE,
		BLACK
	};

	// structure provided to player object from the board
	// contains the x, y coordinates of a piece
	struct PieceInfo
	{
		Location loc;
		PieceType type;
	};

	// structure provided to board from the player
	// contains the move the player wishes to make
	struct MoveRequest
	{
		PieceType type;
		Location oldLoc;
		Location newLoc;
	};

	// map holding all possible movements that a knight can make
	// THIS DOES NOT WORK LIKE THIS
	const std::map<int, int> KNIGHT_MOVES = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

	// function to check if provided x,y coordinates are located within board limits
	inline bool CheckIfOnBoard(int x, int y) { return x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH; };
}