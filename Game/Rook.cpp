#include "stdafx.h"
#include "Rook.h"

Rook::Rook(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 5;
	m_type = Common::PieceType::ROOK;
}

Rook::~Rook()
{
}

std::vector<Common::Location> Rook::GetAvailableMoves()
{
	std::vector<Common::Location> locs;
	return locs;
}