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

std::vector<std::pair<int, int>> Rook::GetAvailableMoves()
{
	std::vector<std::pair<int, int>> locs;
	return locs;
}