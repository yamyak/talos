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
		PieceType type;
		Color color;

		PieceInfo(PieceType t, Color c) : type(t), color(c) {}
		PieceInfo(PieceInfo * p) : type(p->type), color(p->color) {}
	};

	// portable board that holds status of the game board
	// passed in with all commands
	struct MiniBoard
	{
		PieceInfo * data[BOARD_LENGTH][BOARD_LENGTH];

		MiniBoard() 
		{
			for (int i = 0; i < BOARD_LENGTH; i++)
			{
				for (int j = 0; j < BOARD_LENGTH; j++)
				{
					data[i][j] = nullptr;
				}
			}
		}

		MiniBoard(MiniBoard * b) : MiniBoard()
		{
			for (int i = 0; i < BOARD_LENGTH; i++)
			{
				for (int j = 0; j < BOARD_LENGTH; j++)
				{
					if (b->data[i][j] != nullptr)
					{
						data[i][j] = new Common::PieceInfo(b->data[i][j]);
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
					if (data[i][j] != nullptr)
					{
						delete data[i][j];
						data[i][j] = nullptr;
					}
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