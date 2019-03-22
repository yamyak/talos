#include "stdafx.h"
#include "Board.h"
#include "Common.h"

Board::Board()
{
	// the board is set up so that it faces in the direction of the white pieces
	m_mode = Common::Color::WHITE;

	// add the pawns to the board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		m_matrix[i][1] = new Common::PieceInfo(Common::PieceType::PAWN, Common::Color::WHITE);
		m_matrix[i][6] = new Common::PieceInfo(Common::PieceType::PAWN, Common::Color::BLACK);
	}

	// add the rooks to the board
	m_matrix[0][0] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::WHITE);
	m_matrix[7][0] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::WHITE);

	m_matrix[0][7] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::BLACK);
	m_matrix[7][7] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::BLACK);

	// add the knights to the board
	m_matrix[1][0] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::WHITE);
	m_matrix[6][0] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::WHITE);

	m_matrix[1][7] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::BLACK);
	m_matrix[6][7] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::BLACK);

	// add the bishops to the board
	m_matrix[2][0] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::WHITE);
	m_matrix[5][0] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::WHITE);

	m_matrix[2][7] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::BLACK);
	m_matrix[5][7] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::BLACK);

	// add the queens to the board
	m_matrix[3][0] = new Common::PieceInfo(Common::PieceType::QUEEN, Common::Color::WHITE);
	m_matrix[4][7] = new Common::PieceInfo(Common::PieceType::QUEEN, Common::Color::BLACK);

	// add the kings to the board
	m_matrix[4][0] = new Common::PieceInfo(Common::PieceType::KING, Common::Color::WHITE);
	m_matrix[3][7] = new Common::PieceInfo(Common::PieceType::KING, Common::Color::BLACK);
}

Board::~Board()
{
}

int Board::Transpose(int val)
{
	return (m_mode == Common::Color::WHITE) ? val : (Common::BOARD_LENGTH - 1 - val);
}

Common::MiniBoard Board::GetBoard()
{
	Common::MiniBoard mini;

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			if (m_matrix[i][j] != nullptr)
			{
				int iTrans = Transpose(i);
				int jTrans = Transpose(j);
				mini.data[iTrans][jTrans] = new Common::PieceInfo(m_matrix[i][j]);
			}
		}
	}

	return mini;
}

void Board::SetMode(Common::Color color)
{
	m_mode = color;
}

// applys the move specified to board specified
void Board::ApplyMove(Common::MoveRequest & move, Common::PieceInfo * board[][Common::BOARD_LENGTH])
{
	int xOld = Transpose(move.xOld);
	int yOld = Transpose(move.yOld);
	int xNew = Transpose(move.xNew);
	int yNew = Transpose(move.yNew);

	// retrieve the piece to be moved
	Common::PieceInfo * p = board[xOld][yOld];
	// clear out the old piece location
	board[xOld][yOld] = nullptr;

	// check if new location is not empty
	if (board[xNew][yNew] != nullptr)
	{
		// retrieve the piece that will be killed and delete it
		Common::PieceInfo * dead = board[xNew][yNew];
		delete dead;
		dead = nullptr;
	}

	// move piece to new location
	board[xNew][yNew] = p;
}

// applys the move specified
void Board::ApplyMove(Common::MoveRequest & move)
{
	ApplyMove(move, m_matrix);
}