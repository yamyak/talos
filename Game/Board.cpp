#include "stdafx.h"
#include "Board.h"
#include "Common.h"

Board::Board()
{
	m_mode = Common::Color::WHITE;

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

int Board::Transpose(int val)
{
	return Common::BOARD_LENGTH - 1 - val;
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

	std::pair<int, int> loc = piece->GetLocation();
	m_matrix[loc.first][loc.second] = piece;
}

// returns the x, y coordinates to the king piece for the specified color
std::pair<int, int> Board::GetKingLocation(Common::Color color)
{
	std::vector<Piece *> * pieces;

	pieces = (color == Common::Color::WHITE) ? &m_whitePieces : &m_blackPieces;

	std::pair<int, int> loc;

	// iterates through all the pieces for the specified color
	// trying to find the king piece
	for (Piece * p : *pieces)
	{
		if (p->GetType() == Common::PieceType::KING)
		{
			loc = p->GetLocation();
		}
	}

	loc.first = (m_mode == Common::Color::WHITE) ? loc.first : Transpose(loc.first);
	loc.second = (m_mode == Common::Color::WHITE) ? loc.second : Transpose(loc.second);

	return loc;
}

// populates 2 lists of piece x, y coordinates, one for black and one for white
void Board::GetPieceLocations(std::vector<Common::PieceInfo> & white, std::vector<Common::PieceInfo> & black)
{
	for (Piece * p : m_whitePieces)
	{
		Common::PieceInfo info;
		info.type = p->GetType();
		std::pair<int, int> loc = p->GetLocation();
		info.x = (m_mode == Common::Color::WHITE) ? loc.first : Transpose(loc.first);
		info.y = (m_mode == Common::Color::WHITE) ? loc.second : Transpose(loc.second);

		white.push_back(info);
	}

	for (Piece * p : m_blackPieces)
	{
		Common::PieceInfo info;
		info.type = p->GetType();
		std::pair<int, int> loc = p->GetLocation();
		info.x = (m_mode == Common::Color::WHITE) ? loc.first : Transpose(loc.first);
		info.y = (m_mode == Common::Color::WHITE) ? loc.second : Transpose(loc.second);

		black.push_back(info);
	}
}

// applys the move specified
void Board::ApplyMove(Common::MoveRequest & move)
{
	int xOld = (m_mode == Common::Color::WHITE) ? move.xOld : Transpose(move.xOld);
	int yOld = (m_mode == Common::Color::WHITE) ? move.yOld : Transpose(move.yOld);
	int xNew = (m_mode == Common::Color::WHITE) ? move.xNew : Transpose(move.xNew);
	int yNew = (m_mode == Common::Color::WHITE) ? move.yNew : Transpose(move.yNew);

	// retrieve the piece to be moved
	Piece * p = m_matrix[xOld][yOld];
	// clear out the old piece location
	m_matrix[xOld][yOld] = nullptr;

	// check if new location is not empty
	if (m_matrix[xNew][yNew] != nullptr)
	{
		// retrieve the piece that will be killed and delete it
		Piece * dead = m_matrix[xNew][yNew];
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
	m_matrix[xNew][yNew] = p;
	p->SetLocation(xNew, yNew);
}

// get the piece at the specified x, y coordinates
Piece * Board::GetPiece(int x, int y)
{
	int xVal = (m_mode == Common::Color::WHITE) ? x : Transpose(x);
	int yVal = (m_mode == Common::Color::WHITE) ? y : Transpose(y);
	return m_matrix[xVal][yVal];
}