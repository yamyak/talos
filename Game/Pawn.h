#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(int x, int y, Common::Color color);
	~Pawn();

	std::vector<std::pair<int, int>> GetAvailableMoves();
	bool & GetFirstMove();

private:
	bool m_firstMove;
};

