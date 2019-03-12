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
	// structures that hold the pieces split into opposing sides
	// makes it easier to provide players with their corresponding pieces
	std::vector<Piece *> m_blackPieces;
	std::vector<Piece *> m_whitePieces;
	// structure that holds the pieces as they are situated on the board
	// makes it easier to make moves and check for special cases
	std::vector<std::vector<Piece *>> m_matrix;

	// initialization function used when setting up the board
	// DON'T NEED TO SEND IN THE COLOR ANYMORE
	void AddPiece(Piece * piece, bool white);
};

