#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(int x, int y, Common::Color color);
	~Bishop();

	std::vector<Common::Location> GetAvailableMoves();
};

