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

// 
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

	// retrieve all the piece locations
	m_board.GetPieceLocations(whiteInfo, blackInfo);
	// if the player is black, need to transpose piece location to fit black perspective
	if (p.GetColor() == Common::Color::BLACK)
	{
		TransposeLocations(whiteInfo, blackInfo);
	}

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
			// transpose move information if black player
			TransposeMoveRequest(move);
		}

		// check if the move is a valid one
		validMove = CheckMoveRequest(move, color);
	}

	// apply the move to the board
	m_board.ApplyMove(move);
}

// transpose all piece x, y coordinates
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

// transpose x, y coordinates in move request
void Game::TransposeMoveRequest(Common::MoveRequest & move)
{
	move.oldLoc.x = Common::BOARD_LENGTH - move.oldLoc.x;
	move.oldLoc.y = Common::BOARD_LENGTH - move.oldLoc.y;

	move.newLoc.x = Common::BOARD_LENGTH - move.newLoc.x;
	move.newLoc.y = Common::BOARD_LENGTH - move.newLoc.y;
}

// verify if the move provided is a valid move
bool Game::CheckMoveRequest(Common::MoveRequest & move, Common::Color color)
{
	bool valid = true;

	// check if location coordinates are on the board
	valid = Common::CheckIfOnBoard(move.newLoc.x, move.newLoc.y) && Common::CheckIfOnBoard(move.oldLoc.x, move.oldLoc.y);

	// check that piece is actually moving and not sitting in 1 place
	valid = valid && (move.newLoc.x != move.oldLoc.x || move.newLoc.y != move.oldLoc.y);

	// check that there is a piece at the starting location
	Piece * moving = valid ? m_board.GetPiece(move.oldLoc) : nullptr;

	// check that correct piece is at the starting location
	valid = valid && moving != nullptr && moving->GetColor() == color && moving->GetType() == move.type;

	// if a piece is at the destination, check that it is an enemy piece
	Piece * target = valid ? m_board.GetPiece(move.newLoc) : nullptr;
	if (target != nullptr)
	{
		valid = valid && target->GetColor() != color;
	}

	// check that move is valid for piece
	valid = valid && CheckMovePath(moving, move.newLoc);
	// check that move path is clear of other pieces
	valid = valid && CheckPathForObstacles(moving, move.newLoc);
	//
	valid = valid && CheckIfKingInCheck(color);

	return valid;
}

bool Game::CheckStraightPathForAggressors(Common::Location start, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types)
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

// check if king is in check
bool Game::CheckIfKingInCheck(Common::Color color)
{
	bool valid = true;

	// get location of king
	Common::Location kingLoc = m_board.GetKingLocation(color);
	std::set<Common::PieceType> lethalTypes;
	
	// check if king is under attack from pawn
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
	valid = valid && CheckStraightPathForAggressors(kingLoc, 1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, -1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, 1, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, -1, -1, color, lethalTypes);

	lethalTypes.clear();
	lethalTypes.insert(Common::PieceType::ROOK);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && CheckStraightPathForAggressors(kingLoc, 1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, -1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, 0, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc, 0, -1, color, lethalTypes);

	return valid;
}

// check that path from start location to destination is clear of any pieces
bool Game::CheckPathForObstacles(Piece * piece, Common::Location target)
{
	bool valid = true;

	switch(piece->GetType())
	{
	case Common::PieceType::PAWN:
		// NEED TO REDO THIS PART
		if (target.y - piece->GetLocation().y > 1)
		{
			valid = m_board.GetPiece(target.x, target.y - 1) == nullptr;
		}
		break;
	case Common::PieceType::BISHOP:
	case Common::PieceType::ROOK:
	case Common::PieceType::QUEEN:
		// reduce x, y delta to most basic step
		int xStart = piece->GetLocation().x;
		int yStart = piece->GetLocation().y;
		int xDelta = target.x - xStart;
		int yDelta = target.y - yStart;
		xDelta = xDelta / std::abs(xDelta);
		yDelta = yDelta / std::abs(yDelta);
		// step forward from start to destination
		// make sure no pieces are in that location
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

// validate that move is a valid move for the piece moving
bool Game::CheckMovePath(Piece * piece, Common::Location target)
{
	bool valid = false;

	// get the x, y changes
	int xDelta = target.x - piece->GetLocation().x;
	int yDelta = target.y - piece->GetLocation().y;

	// get the absolute x, y changes
	int xDeltaAbs = std::abs(xDelta);
	int yDeltaAbs = std::abs(yDelta);

	// validate the move based on the piece type
	switch (piece->GetType())
	{
	case Common::PieceType::PAWN:
		Pawn * pawn = (Pawn *)piece;
		// check if pawn's first move
		bool & firstMove = pawn->GetFirstMove();

		if (xDeltaAbs == 0)
		{
			// if not moving diagonal, check that 1 or 2 steps (first move if 2)
			valid = yDelta == 1 || (firstMove && yDelta == 2);
		}
		else if(xDeltaAbs == 1 && yDelta == 1)
		{
			// if moving diagonal 1 step, check that enemy piece exists at destination
			Piece * p = m_board.GetPiece(target);
			valid = p != nullptr && p->GetColor() != piece->GetColor();
		}
		// update first move flag
		firstMove = valid ? false : firstMove;
		break;
	case Common::PieceType::BISHOP:
		// check that x and y absolute change is the same and nonzero for bishop
		valid = xDeltaAbs == yDeltaAbs && xDeltaAbs != 0;
		break;
	case Common::PieceType::KNIGHT:
		// check that absolute change of x and y are 1 and 2 or 2 and 1 (L shaped) for knight
		valid = (xDeltaAbs == 1 && yDeltaAbs == 2) || (xDeltaAbs == 2 && yDeltaAbs == 1);
		break;
	case Common::PieceType::ROOK:
		// check that either absolute x change is nonzero or absolute y change but not both for rook
		valid = (xDeltaAbs != 0 && yDeltaAbs == 0) || (xDeltaAbs == 0 && yDeltaAbs != 0);
		break;
	case Common::PieceType::QUEEN:
		// check that either bishop or rook rules apply
		valid = (xDeltaAbs == yDeltaAbs && xDeltaAbs != 0) || (xDeltaAbs != 0 && yDeltaAbs == 0) || (xDeltaAbs == 0 && yDeltaAbs != 1);
		break;
	case Common::PieceType::KING:
		// check that king move only 1 step in any direction
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