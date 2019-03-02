#pragma once

#include <vector>

#include "Board.h"
#include "Common.h"

class Piece
{
public:
	Piece(int x, int y, Common::Color color, Board * board);
	~Piece();

	Common::Location GetLocation();
	Common::Color GetColor();
	Common::PieceType GetType();
	void SetLocation(int x, int y);
	virtual std::vector<Common::Location> GetAvailableMoves()
	{
		std::vector<Common::Location> v;
		return v;
	};

protected:
	int m_points;
	Common::PieceType m_type;
	Board * m_board;

private:
	Common::Color m_color;
	Common::Location m_location;
	bool m_alive;
};

