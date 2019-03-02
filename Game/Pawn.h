#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(int x, int y, Common::Color color);
	~Pawn();

	std::vector<Common::Location> GetAvailableMoves();
	bool & GetFirstMove();

private:
	bool m_firstMove;
};

