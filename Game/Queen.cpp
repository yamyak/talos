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

std::vector<Common::Location> Queen::GetAvailableMoves()
{
	std::vector<Common::Location> locs;
	return locs;
}