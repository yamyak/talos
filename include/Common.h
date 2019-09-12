#pragma once

#include <map>

namespace Common
{
	// size of the side of a board
	const int BOARD_LENGTH = 8;

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

	// structure that hold piece info
	struct PieceInfo
	{
		bool occupied;
		PieceType type;
		Color color;

		PieceInfo() : occupied(false) {}

		void Update(PieceInfo & p)
		{
			occupied = true;
			type = p.type;
			color = p.color;
		}

		void Update(PieceType t, Color c)
		{
			occupied = true;
			type = t;
			color = c;
		}
	};

	// portable board that holds status of the game board
	// passed in with all commands
	struct MiniBoard
	{
		PieceInfo data[BOARD_LENGTH][BOARD_LENGTH];

		MiniBoard() {}

		MiniBoard(MiniBoard & b)
		{
			for (int i = 0; i < BOARD_LENGTH; i++)
			{
				for (int j = 0; j < BOARD_LENGTH; j++)
				{
					if (b.data[i][j].occupied)
					{
						data[i][j].Update(b.data[i][j]);
					}
				}
			}
		}

		~MiniBoard()
		{
			Clear();
		}

		void Clear()
		{
			for (int i = 0; i < BOARD_LENGTH; i++)
			{
				for (int j = 0; j < BOARD_LENGTH; j++)
				{
					data[i][j].occupied = false;
				}
			}
		}
	};

	// structure provided to backend from the player
	// contains the move the player wishes to make
	struct MoveRequest
	{
		PieceType type;
		int xOld;
		int yOld;
		int xNew;
		int yNew;

		MoveRequest() {}
	};

	// function to check if provided x,y coordinates are located within board limits
	inline bool CheckIfOnBoard(int x, int y) { return x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH; };
}