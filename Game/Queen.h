#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(int x, int y, Common::Color color);
	~Queen();

	std::vector<std::pair<int, int>> GetAvailableMoves();
};

