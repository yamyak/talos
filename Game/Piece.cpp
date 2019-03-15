#include "stdafx.h"
#include "Piece.h"

Piece::Piece(int x, int y, Common::Color color, Board * board)
{
	m_x = x;
	m_y = y;

	m_color = color;
	m_alive = true;
	m_points = 0;

	m_board = board;
}

Piece::~Piece()
{
}

std::pair<int, int> Piece::GetLocation()
{
	std::pair<int, int> loc(m_x, m_y);
	return loc;
}

Common::Color Piece::GetColor()
{
	return m_color;
}

Common::PieceType Piece::GetType()
{
	return m_type;
}

void Piece::SetLocation(int x, int y)
{
	m_location.x = x;
	m_location.y = y;
}