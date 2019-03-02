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

std::vector<Common::Location> King::GetAvailableMoves()
{
	std::vector<Common::Location> locs;
	return locs;
}
