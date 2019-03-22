#pragma once

#include <utility>
#include "Common.h"

class Board
{
public:
	Board();
	~Board();

	Common::MiniBoard GetBoard();
	void ApplyMove(Common::MoveRequest & move);
	void ApplyMove(Common::MoveRequest & move, Common::PieceInfo * board[][Common::BOARD_LENGTH]);
	void SetMode(Common::Color color);

private:
	Common::Color m_mode;

	// structure that holds the pieces as they are situated on the board
	// makes it easier to make moves and check for special cases
	Common::PieceInfo * m_matrix[Common::BOARD_LENGTH][Common::BOARD_LENGTH];

	int Transpose(int val);
};

