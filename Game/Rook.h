#pragma once

#include "Piece.h"

class Rook : public Piece
{
public:
	Rook(int x, int y, Common::Color color);
	~Rook();

	std::vector<std::pair<int, int>> GetAvailableMoves();
};

