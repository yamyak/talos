#include "stdafx.h"
#include "Knight.h"

Knight::Knight(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 3;
	m_type = Common::PieceType::KNIGHT;
}

Knight::~Knight()
{
}

std::vector<std::pair<int, int>> Knight::GetAvailableMoves()
{
	std::vector<std::pair<int, int>> locs;
	return locs;
}
