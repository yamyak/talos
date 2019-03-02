#include "stdafx.h"
#include "Pawn.h"

Pawn::Pawn(int x, int y, Common::Color color) : Piece(x, y, color)
{
	m_points = 1;
	m_type = Common::PieceType::PAWN;
	m_firstMove = true;
}

Pawn::~Pawn()
{
}

std::vector<Common::Location> Pawn::GetAvailableMoves()
{
	std::vector<Common::Location> locs;

	for (int i = 1; i < 3; i++)
	{
		if (Common::CheckIfOnBoard(this->GetLocation().x, this->GetLocation().y + i))
		{
			if (i == 2 && !m_firstMove)
			{
				break;
			}

			Common::Location loc;
			loc.x = this->GetLocation().x;
			loc.y = this->GetLocation().y + i;

			Piece * p = m_board->GetPiece(loc);
			if (p != nullptr)
			{
				break;
			}

			locs.push_back(loc);
		}
	}

	if (m_firstMove && Common::CheckIfOnBoard(this->GetLocation().x - 1, this->GetLocation().y + 1))
	{
		Common::Location loc;
		loc.x = this->GetLocation().x - 1;
		loc.y = this->GetLocation().y + 1;

		locs.push_back(loc);
	}

	if (m_firstMove && Common::CheckIfOnBoard(this->GetLocation().x + 1, this->GetLocation().y + 1))
	{
		Common::Location loc;
		loc.x = this->GetLocation().x + 1;
		loc.y = this->GetLocation().y + 1;

		locs.push_back(loc);
	}

	return locs;
}

bool & Pawn::GetFirstMove()
{
	return m_firstMove;
}