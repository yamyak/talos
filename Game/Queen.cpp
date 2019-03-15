#include "stdafx.h"
#include "Queen.h"

Queen::Queen(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 9;
	m_type = Common::PieceType::QUEEN;
}

Queen::~Queen()
{
}

std::vector<std::pair<int, int>> Queen::GetAvailableMoves()
{
	std::vector<std::pair<int, int>> locs;
	return locs;
}