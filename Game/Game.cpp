#include "stdafx.h"

#include "Game.h"
#include "Common.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::RegisterPlayers(Player & p1, Player & p2)
{
	p1.SetColor(Common::Color::WHITE);
	p2.SetColor(Common::Color::BLACK);
}

bool Game::CheckGameStatus(Common::Color & currentColor)
{
	bool status = true;

	Common::Color nextColor  = (currentColor == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;

	if (!CheckIfKingInCheck(nextColor))
	{

	}

	return status;
}

void Game::QueryPlayerForMove(Player & p, Common::Color color)
{
	std::vector<Common::PieceInfo> whiteInfo;
	std::vector<Common::PieceInfo> blackInfo;

	m_board.GetPieceLocations(whiteInfo, blackInfo);
	if (p.GetColor() == Common::Color::BLACK)
	{
		TransposeLocations(whiteInfo, blackInfo);
	}

	bool validMove = false;
	Common::MoveRequest move;
	while (!validMove)
	{
		if (p.GetColor() == Common::Color::WHITE)
		{
			move = p.MakeMove(whiteInfo, blackInfo);
		}
		else
		{
			move = p.MakeMove(blackInfo, whiteInfo);
			TransposeMoveRequest(move);
		}

		validMove = CheckMoveRequest(move, color);
	}

	m_board.ApplyMove(move);
}

void Game::TransposeLocations(std::vector<Common::PieceInfo> & whiteLocs, std::vector<Common::PieceInfo> & blackLocs)
{
	for (Common::PieceInfo & info : whiteLocs)
	{
		info.loc.x = Common::BOARD_LENGTH - info.loc.x;
		info.loc.y = Common::BOARD_LENGTH - info.loc.y;
	}

	for (Common::PieceInfo & info : blackLocs)
	{
		info.loc.x = Common::BOARD_LENGTH - info.loc.x;
		info.loc.y = Common::BOARD_LENGTH - info.loc.y;
	}
}

void Game::TransposeMoveRequest(Common::MoveRequest & move)
{
	move.oldLoc.x = Common::BOARD_LENGTH - move.oldLoc.x;
	move.oldLoc.y = Common::BOARD_LENGTH - move.oldLoc.y;

	move.newLoc.x = Common::BOARD_LENGTH - move.newLoc.x;
	move.newLoc.y = Common::BOARD_LENGTH - move.newLoc.y;
}

bool Game::CheckMoveRequest(Common::MoveRequest & move, Common::Color color)
{
	bool valid = true;

	valid = Common::CheckIfOnBoard(move.newLoc.x, move.newLoc.y) && Common::CheckIfOnBoard(move.oldLoc.x, move.oldLoc.y);

	valid = valid && (move.newLoc.x != move.oldLoc.x || move.newLoc.y != move.oldLoc.y);

	Piece * moving = valid ? m_board.GetPiece(move.oldLoc) : nullptr;

	valid = valid && moving != nullptr && moving->GetColor() == color && moving->GetType() == move.type;

	Piece * target = valid ? m_board.GetPiece(move.newLoc) : nullptr;

	if (target != nullptr)
	{
		valid = valid && target->GetColor() != color;
	}

	valid = valid && CheckMovePath(moving, move.newLoc);
	valid = valid && CheckPathForObstacles(moving, move.newLoc);
	valid = valid && CheckIfKingInCheck(color);

	return valid;
}

bool Game::CheckStraightPathForAgressors(Common::Location start, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types)
{
	bool valid = true;

	int xStart = start.x + xStep;
	int yStart = start.y + yStep;

	while (Common::CheckIfOnBoard(xStart, yStart))
	{
		Piece * p = m_board.GetPiece(xStart, yStart);
		if (p != nullptr)
		{
			if (p->GetColor() != color && types.find(p->GetType()) != types.end())
			{
				valid = false;
			}

			break;
		}

		xStart += xStep;
		yStart += yStep;
	}

	return valid;
}

bool Game::CheckIfKingInCheck(Common::Color color)
{
	bool valid = true;

	Common::Location kingLoc = m_board.GetKingLocation(color);
	std::set<Common::PieceType> lethalTypes;
	
	std::vector<int> locs = { kingLoc.x - 1, kingLoc.x + 1 };
	for (int x : locs)
	{
		if (valid && Common::CheckIfOnBoard(x, kingLoc.y))
		{
			Piece * p = m_board.GetPiece(x, kingLoc.y);
			if (p != nullptr)
			{
				if (p->GetColor() != color && p->GetType() == Common::PieceType::PAWN)
				{
					valid = false;
				}
			}
		}
	}

	std::map<int, int>::const_iterator it = Common::KNIGHT_MOVES.begin();
	while (valid && it != Common::KNIGHT_MOVES.end())
	{
		int xLoc = kingLoc.x + it->first;
		int yLoc = kingLoc.y + it->second;

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
	
	lethalTypes.insert(Common::PieceType::BISHOP);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && CheckStraightPathForAgressors(kingLoc, 1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, -1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, 1, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, -1, -1, color, lethalTypes);

	lethalTypes.clear();
	lethalTypes.insert(Common::PieceType::ROOK);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && CheckStraightPathForAgressors(kingLoc, 1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, -1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, 0, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAgressors(kingLoc, 0, -1, color, lethalTypes);

	return valid;
}

bool Game::CheckPathForObstacles(Piece * piece, Common::Location target)
{
	bool valid = true;

	switch(piece->GetType())
	{
	case Common::PieceType::PAWN:
		if (target.y - piece->GetLocation().y > 1)
		{
			valid = m_board.GetPiece(target.x, target.y - 1) == nullptr;
		}
		break;
	case Common::PieceType::BISHOP:
	case Common::PieceType::ROOK:
	case Common::PieceType::QUEEN:
		int xStart = piece->GetLocation().x;
		int yStart = piece->GetLocation().y;
		int xDelta = target.x - xStart;
		int yDelta = target.y - yStart;
		xDelta = xDelta / std::abs(xDelta);
		yDelta = yDelta / std::abs(yDelta);
		xStart += xDelta;
		yStart += yDelta;
		while (xStart != target.x && yStart != target.y)
		{
			valid = m_board.GetPiece(xStart, yStart) == nullptr;
			if (!valid)
			{
				break;
			}
			xStart += xDelta;
			yStart += yDelta;
		}
		break;
	default:
		break;
	}

	return valid;
}

bool Game::CheckMovePath(Piece * piece, Common::Location target)
{
	bool valid = false;

	int xDelta = target.x - piece->GetLocation().x;
	int yDelta = target.y - piece->GetLocation().y;

	int xDeltaAbs = std::abs(xDelta);
	int yDeltaAbs = std::abs(yDelta);

	switch (piece->GetType())
	{
	case Common::PieceType::PAWN:
		Pawn * pawn = (Pawn *)piece;
		bool & firstMove = pawn->GetFirstMove();

		if (xDeltaAbs == 0)
		{
			valid = yDelta == 1 || (firstMove && yDelta == 2);
		}
		else if(xDeltaAbs == 1 && yDelta == 1)
		{
			Piece * p = m_board.GetPiece(target);
			valid = p != nullptr && p->GetColor() != piece->GetColor();
		}

		firstMove = valid ? false : firstMove;
		break;
	case Common::PieceType::BISHOP:
		valid = xDeltaAbs == yDeltaAbs && xDeltaAbs != 0;
		break;
	case Common::PieceType::KNIGHT:
		valid = (xDeltaAbs == 1 && yDeltaAbs == 2) || (xDeltaAbs == 2 && yDeltaAbs == 1);
		break;
	case Common::PieceType::ROOK:
		valid = (xDeltaAbs != 0 && yDeltaAbs == 0) || (xDeltaAbs == 0 && yDeltaAbs != 1);
		break;
	case Common::PieceType::QUEEN:
		valid = (xDeltaAbs == yDeltaAbs && xDeltaAbs != 0) || (xDeltaAbs != 0 && yDeltaAbs == 0) || (xDeltaAbs == 0 && yDeltaAbs != 1);
		break;
	case Common::PieceType::KING:
		valid = (xDeltaAbs == 0 && yDeltaAbs == 1) || (xDeltaAbs == 1 && yDeltaAbs == 0) || (xDeltaAbs == 1 && yDeltaAbs == 1);
		break;
	default:
		break;
	}

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