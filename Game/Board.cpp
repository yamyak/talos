#include "stdafx.h"
#include "Board.h"
#include "Common.h"

Board::Board()
{
	m_matrix.resize(Common::BOARD_LENGTH);
	for (std::vector<Piece *> & v : m_matrix)
	{
		v.resize(Common::BOARD_LENGTH);
	}

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		Pawn * p = new Pawn(i, 1, Common::Color::WHITE);
		AddPiece(p, true);

		p = new Pawn(i, 6, Common::Color::BLACK);
		AddPiece(p, false);
	}

	Rook * r = new Rook(0, 0, Common::Color::WHITE);
	AddPiece(r, true);
	r = new Rook(7, 0, Common::Color::WHITE);
	AddPiece(r, true);

	r = new Rook(0, 7, Common::Color::BLACK);
	AddPiece(r, false);
	r = new Rook(7, 7, Common::Color::BLACK);
	AddPiece(r, false);

	Knight * n = new Knight(1, 0, Common::Color::WHITE);
	AddPiece(n, true);
	n = new Knight(6, 0, Common::Color::WHITE);
	AddPiece(n, true);

	n = new Knight(1, 7, Common::Color::BLACK);
	AddPiece(n, false);
	n = new Knight(6, 7, Common::Color::BLACK);
	AddPiece(n, false);

	Bishop * b = new Bishop(2, 0, Common::Color::WHITE);
	AddPiece(b, true);
	b = new Bishop(5, 0, Common::Color::WHITE);
	AddPiece(b, true);

	b = new Bishop(2, 7, Common::Color::BLACK);
	AddPiece(b, false);
	b = new Bishop(5, 7, Common::Color::BLACK);
	AddPiece(b, false);

	Queen * q = new Queen(3, 0, Common::Color::WHITE);
	AddPiece(q, true);
	q = new Queen(4, 7, Common::Color::BLACK);
	AddPiece(q, false);

	King * k = new King(4, 0, Common::Color::WHITE);
	AddPiece(k, true);
	k = new King(3, 7, Common::Color::BLACK);
	AddPiece(k, false);
}

Board::~Board()
{
}

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

Common::Location Board::GetKingLocation(Common::Color color)
{
	std::vector<Piece *> * pieces;

	pieces = (color == Common::Color::WHITE) ? &m_whitePieces : &m_blackPieces;

	Common::Location loc;

	for (Piece * p : *pieces)
	{
		if (p->GetType() == Common::PieceType::KING)
		{
			loc = p->GetLocation();
		}
	}

	return loc;
}

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

void Board::ApplyMove(Common::MoveRequest & move)
{
	Piece * p = m_matrix[move.oldLoc.x][move.oldLoc.y];
	m_matrix[move.oldLoc.x][move.oldLoc.y] = nullptr;

	if (m_matrix[move.newLoc.x][move.newLoc.y] != nullptr)
	{
		Piece * dead = m_matrix[move.newLoc.x][move.newLoc.y];
		delete dead;
		dead = nullptr;

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

	m_matrix[move.newLoc.x][move.newLoc.y] = p;
	p->SetLocation(move.newLoc.x, move.newLoc.y);
}

Piece * Board::GetPiece(Common::Location loc)
{
	return m_matrix[loc.x][loc.y];
}

Piece * Board::GetPiece(int x, int y)
{
	return m_matrix[x][y];
}