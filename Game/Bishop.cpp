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

std::vector<std::pair<int, int>> Bishop::GetAvailableMoves()
{
	std::vector<std::pair<int, int>> locs;
	return locs;
}
