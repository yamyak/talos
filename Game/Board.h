#pragma once

#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"

class Board
{
public:
	Board();
	~Board();

	Common::Location GetKingLocation(Common::Color color);
	void GetPieceLocations(std::vector<Common::PieceInfo> & white, std::vector<Common::PieceInfo> & black);
	void ApplyMove(Common::MoveRequest & move);
	Piece * GetPiece(Common::Location loc);
	Piece * GetPiece(int x, int y);

private:
	std::vector<Piece *> m_blackPieces;
	std::vector<Piece *> m_whitePieces;
	std::vector<std::vector<Piece *>> m_matrix;

	void AddPiece(Piece * piece, bool white);
};

