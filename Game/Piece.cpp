#include "stdafx.h"
#include "Piece.h"

Piece::Piece(int x, int y, Common::Color color, Board * board)
{
	Common::Location loc;
	loc.x = x;
	loc.y = y;
	m_location = loc;

	m_color = color;
	m_alive = true;
	m_points = 0;

	m_board = board;
}

Piece::~Piece()
{
}

Common::Location Piece::GetLocation()
{
	return m_location;
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