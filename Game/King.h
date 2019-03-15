#pragma once

#include "Piece.h"

class King : public Piece
{
public:
	King(int x, int y, Common::Color color);
	~King();

	std::vector<std::pair<int, int>> GetAvailableMoves();
};

