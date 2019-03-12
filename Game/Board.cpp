#include "stdafx.h"
#include "Board.h"
#include "Common.h"

Board::Board()
{
	// resizes the piece matrix to the correct sizes
	m_matrix.resize(Common::BOARD_LENGTH);
	for (std::vector<Piece *> & v : m_matrix)
	{
		v.resize(Common::BOARD_LENGTH);
	}

	// the board is set up so that it faces in the direction of the white pieces

	// add the pawns to the board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		Pawn * p = new Pawn(i, 1, Common::Color::WHITE);
		AddPiece(p, true);

		p = new Pawn(i, 6, Common::Color::BLACK);
		AddPiece(p, false);
	}

	// add the rooks to the board
	Rook * r = new Rook(0, 0, Common::Color::WHITE);
	AddPiece(r, true);
	r = new Rook(7, 0, Common::Color::WHITE);
	AddPiece(r, true);

	r = new Rook(0, 7, Common::Color::BLACK);
	AddPiece(r, false);
	r = new Rook(7, 7, Common::Color::BLACK);
	AddPiece(r, false);

	// add the knights to the board
	Knight * n = new Knight(1, 0, Common::Color::WHITE);
	AddPiece(n, true);
	n = new Knight(6, 0, Common::Color::WHITE);
	AddPiece(n, true);

	n = new Knight(1, 7, Common::Color::BLACK);
	AddPiece(n, false);
	n = new Knight(6, 7, Common::Color::BLACK);
	AddPiece(n, false);

	// add the bishops to the board
	Bishop * b = new Bishop(2, 0, Common::Color::WHITE);
	AddPiece(b, true);
	b = new Bishop(5, 0, Common::Color::WHITE);
	AddPiece(b, true);

	b = new Bishop(2, 7, Common::Color::BLACK);
	AddPiece(b, false);
	b = new Bishop(5, 7, Common::Color::BLACK);
	AddPiece(b, false);

	// add the queens to the board
	Queen * q = new Queen(3, 0, Common::Color::WHITE);
	AddPiece(q, true);
	q = new Queen(4, 7, Common::Color::BLACK);
	AddPiece(q, false);

	// add the kings to the board
	King * k = new King(4, 0, Common::Color::WHITE);
	AddPiece(k, true);
	k = new King(3, 7, Common::Color::BLACK);
	AddPiece(k, false);
}

Board::~Board()
{
}

// adds the pointer to the piece to both the player list and the piece matrix
void Board::AddPiece(Piece * piece, bool white)
{
	if (white)
	{
		m_whitePieces.push_back(piece);
	}
	else
	{
		m_blackPieces.push_back(piece);
	}

	Common::Location loc = piece->GetLocation();
	m_matrix[loc.x][loc.y] = piece;
}

// returns the x, y coordinates to the king piece for the specified color
Common::Location Board::GetKingLocation(Common::Color color)
{
	std::vector<Piece *> * pieces;

	pieces = (color == Common::Color::WHITE) ? &m_whitePieces : &m_blackPieces;

	Common::Location loc;

	// iterates through all the pieces for the specified color
	// trying to find the king piece
	for (Piece * p : *pieces)
	{
		if (p->GetType() == Common::PieceType::KING)
		{
			loc = p->GetLocation();
		}
	}

	return loc;
}

// populates 2 lists of piece x, y coordinates, one for black and one for white
void Board::GetPieceLocations(std::vector<Common::PieceInfo> & white, std::vector<Common::PieceInfo> & black)
{
	for (Piece * p : m_whitePieces)
	{
		Common::PieceInfo info;
		info.type = p->GetType();
		info.loc = p->GetLocation();

		white.push_back(info);
	}

	for (Piece * p : m_blackPieces)
	{
		Common::PieceInfo info;
		info.type = p->GetType();
		info.loc = p->GetLocation();

		black.push_back(info);
	}
}

// applys the move specified
void Board::ApplyMove(Common::MoveRequest & move)
{
	// retrieve the piece to be moved
	Piece * p = m_matrix[move.oldLoc.x][move.oldLoc.y];
	// clear out the old piece location
	m_matrix[move.oldLoc.x][move.oldLoc.y] = nullptr;

	// check if new location is not empty
	if (m_matrix[move.newLoc.x][move.newLoc.y] != nullptr)
	{
		// retrieve the piece that will be killed and delete it
		Piece * dead = m_matrix[move.newLoc.x][move.newLoc.y];
		delete dead;
		dead = nullptr;

		// iterate through both arrays to remove the null pointer from the deleted piece
		// COULD POTENTIALLY DO THIS BASED ON MOVING PIECE COLOR
		std::vector<Piece *>::iterator pIter = m_blackPieces.begin();
		while(pIter != m_blackPieces.end())
		{
			if (*pIter == nullptr)
			{
				m_blackPieces.erase(pIter);
				break;
			}
		}

		pIter = m_whitePieces.begin();
		while (pIter != m_whitePieces.end())
		{
			if (*pIter == nullptr)
			{
				m_whitePieces.erase(pIter);
				break;
			}
		}
	}

	// move piece to new location and update its internal coordinates
	m_matrix[move.newLoc.x][move.newLoc.y] = p;
	p->SetLocation(move.newLoc.x, move.newLoc.y);
}

// get the piece at the coordinates in the specified location obj
Piece * Board::GetPiece(Common::Location loc)
{
	return m_matrix[loc.x][loc.y];
}

// get the piece at the specified x, y coordinates
Piece * Board::GetPiece(int x, int y)
{
	return m_matrix[x][y];
}