#pragma once

#include <set>
#include "Common.h"
#include "Piece.h"

class Validator
{
public:
	Validator();
	~Validator();

	bool CheckMoveRequest(Common::MoveRequest & move, Common::Color color, Board & board);
	bool CheckStraightPathForAggressors(Board & board, int x, int y, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types);

private:
	bool CheckMovePath(Piece * piece, int x, int y, Board & board);
	
	bool CheckPathForObstacles(Piece * piece, int x, int y, Board & board);
};

