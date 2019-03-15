#pragma once

#include "Piece.h"

class Knight : public Piece
{
public:
	Knight(int x, int y, Common::Color color);
	~Knight();

	std::vector<std::pair<int, int>> GetAvailableMoves();
};

