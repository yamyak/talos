#pragma once

#include <map>

namespace Common
{
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

	struct PieceInfo
	{
		Location loc;
		PieceType type;
	};

	struct MoveRequest
	{
		PieceType type;
		Location oldLoc;
		Location newLoc;
	};

	const std::map<int, int> KNIGHT_MOVES = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

	inline bool CheckIfOnBoard(int x, int y) { return x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH; };
}