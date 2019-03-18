#include "stdafx.h"

#include "Game.h"
#include "Common.h"

Game::Game()
{
}

Game::~Game()
{
}

// register each player with a color
void Game::RegisterPlayers(Player & p1, Player & p2)
{
	p1.SetColor(Common::Color::WHITE);
	p2.SetColor(Common::Color::BLACK);
}

// check if next player's king is in check mate
// update the winner flag if it is
bool Game::CheckGameStatus(Common::Color & currentColor)
{
	bool status = true;

	Common::Color nextColor  = (currentColor == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

	if (!CheckIfKingInCheck(nextColor))
	{

	}

	return status;
}

// request a move from the player
// check that the move is valid
// apply the move to the board
void Game::QueryPlayerForMove(Player & p, Common::Color color)
{
	std::vector<Common::PieceInfo> whiteInfo;
	std::vector<Common::PieceInfo> blackInfo;

	m_board.SetMode(color);

	// retrieve all the piece locations
	m_board.GetPieceLocations(whiteInfo, blackInfo);

	// query for move until valid move returned
	bool validMove = false;
	Common::MoveRequest move;
	while (!validMove)
	{
		// switch piece list based on color
		// friendly piece list first, enemy piece list second
		if (p.GetColor() == Common::Color::WHITE)
		{
			move = p.MakeMove(whiteInfo, blackInfo);
		}
		else
		{
			move = p.MakeMove(blackInfo, whiteInfo);
		}

		// check if the move is a valid one
		validMove = m_validator.CheckMoveRequest(move, color, m_board);

		// THIS NEEDS TO BE CHECKED AFTER THE MOVE IS APPLIED
		// AND REVERTED IF IT RETURNS A BAD RESULT
		validMove = validMove && CheckIfKingInCheck(color);
	}

	// apply the move to the board
	m_board.ApplyMove(move);
}

// transpose x, y coordinates in move request
void Game::TransposeMoveRequest(Common::MoveRequest & move)
{
	move.xOld = Common::BOARD_LENGTH - move.xOld;
	move.yOld = Common::BOARD_LENGTH - move.yOld;

	move.xNew = Common::BOARD_LENGTH - move.xNew;
	move.yNew = Common::BOARD_LENGTH - move.yNew;
}

// check if king is in check
bool Game::CheckIfKingInCheck(Common::Color color)
{
	bool valid = true;

	// get location of king
	std::pair<int, int> kingLoc = m_board.GetKingLocation(color);
	std::set<Common::PieceType> lethalTypes;
	
	// check if king is under attack from pawn
	// check 2 1-step forward diagonal to see if pawn there
	std::vector<int> locs = { kingLoc.first - 1, kingLoc.second + 1 };
	for (int x : locs)
	{
		// WRONG BECAUSE THE Y COORDINATE NEEDS TO BE KINGLOC + 1
		if (valid && Common::CheckIfOnBoard(x, kingLoc.second))
		{
			Piece * p = m_board.GetPiece(x, kingLoc.second);
			if (p != nullptr)
			{
				if (p->GetColor() != color && p->GetType() == Common::PieceType::PAWN)
				{
					valid = false;
				}
			}
		}
	}

	// check if king under attack from knight
	// iterate through all possible knight attack locations and see if knight there
	std::map<int, int>::const_iterator it = Common::KNIGHT_MOVES.begin();
	while (valid && it != Common::KNIGHT_MOVES.end())
	{
		int xLoc = kingLoc.first + it->first;
		int yLoc = kingLoc.second + it->second;

		if (Common::CheckIfOnBoard(xLoc, yLoc))
		{
			Piece * p = m_board.GetPiece(xLoc, yLoc);
			if (p != nullptr)
			{
				if (p->GetColor() != color && p->GetType() == Common::PieceType::KNIGHT)
				{
					valid = false;
				}
			}
		}
	}
	
	// check to see if 4 diagonals are clear of bishops and queens
	lethalTypes.insert(Common::PieceType::BISHOP);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, 1, 1, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, -1, 1, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, 1, -1, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, -1, -1, color, lethalTypes);

	// check to see if 4 straight lanes are clear of rooks and queens
	lethalTypes.clear();
	lethalTypes.insert(Common::PieceType::ROOK);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, 1, 0, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, -1, 0, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, 0, -1, color, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(m_board, kingLoc.first, kingLoc.second, 0, -1, color, lethalTypes);

	return valid;
}

int main()
{
	Game g;
	Algorithm a1;
	Player p1(&a1);
	Algorithm a2;
	Player p2(&a2);

	g.RegisterPlayers(p1, p2);

	Player * currentPlayer = &p1;
	Common::Color currentColor = Common::Color::WHITE;
	while (g.CheckGameStatus(currentColor))
	{
		g.QueryPlayerForMove(*currentPlayer, currentColor);
		
		currentPlayer = (currentPlayer == &p1) ? &p2 : &p1;
		currentColor = (currentColor == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;
	}

	return 0;
}