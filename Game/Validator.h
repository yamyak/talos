#pragma once

#include "Common.h"

#include <set>

class Validator
{
public:
	Validator();
	~Validator();

	bool CheckMoveRequest(Common::Color color, Common::MoveRequest & move, Common::MiniBoard & board);
	bool CheckIfKingInCheck(std::pair<int, int> loc, Common::Color color, Common::MiniBoard & board);

private:
	bool CheckMovePath(Common::MoveRequest & move , Common::MiniBoard & board);
	bool CheckPathForObstacles(Common::Color color, Common::MoveRequest & move, Common::MiniBoard & board);
	bool CheckStraightPathForAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board, int xStep, int yStep, std::set<Common::PieceType> types);
	bool CheckPawnAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board);
	bool CheckKnightAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board);
};

