#pragma once

#include <utility>
#include <vector>

#include "Board.h"
#include "Common.h"

class Piece
{
public:
	Piece(int x, int y, Common::Color color, Board * board);
	~Piece();

	std::pair<int, int> GetLocation();
	Common::Color GetColor();
	Common::PieceType GetType();
	void SetLocation(int x, int y);
	virtual std::vector<std::pair<int, int>> GetAvailableMoves()
	{
		std::vector<std::pair<int, int>> v;
		return v;
	};

protected:
	int m_points;
	Common::PieceType m_type;
	Board * m_board;

private:
	Common::Color m_color;
	int m_x;
	int m_y;
	bool m_alive;
};

