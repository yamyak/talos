#include "stdafx.h"
#include "Bishop.h"

Bishop::Bishop(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 3;
	m_type = Common::PieceType::BISHOP;
}

Bishop::~Bishop()
{
}

std::vector<Common::Location> Bishop::GetAvailableMoves()
{
	std::vector<Common::Location> locs;
	return locs;
}
