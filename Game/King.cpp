#include "stdafx.h"
#include "King.h"

King::King(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 100;
	m_type = Common::PieceType::KING;
}

King::~King()
{
}

std::vector<std::pair<int, int>> King::GetAvailableMoves()
{
	std::vector<std::pair<int, int>> locs;
	return locs;
}
