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
		info.x = Common::BOARD_LENGTH - info.x;
		info.y = Common::BOARD_LENGTH - info.y;
	}

	for (Common::PieceInfo & info : blackLocs)
	{
		info.x = Common::BOARD_LENGTH - info.x;
		info.y = Common::BOARD_LENGTH - info.y;
	}
}

// transpose x, y coordinates in move request
void Game::TransposeMoveRequest(Common::MoveRequest & move)
{
	move.xOld = Common::BOARD_LENGTH - move.xOld;
	move.yOld = Common::BOARD_LENGTH - move.yOld;

	move.xNew = Common::BOARD_LENGTH - move.xNew;
	move.yNew = Common::BOARD_LENGTH - move.yNew;
}

// verify if the move provided is a valid move
bool Game::CheckMoveRequest(Common::MoveRequest & move, Common::Color color)
{
	bool valid = true;

	// check if location coordinates are on the board
	valid = Common::CheckIfOnBoard(move.xNew, move.yNew) && Common::CheckIfOnBoard(move.xOld, move.yOld);

	// check that piece is actually moving and not sitting in 1 place
	valid = valid && (move.xNew != move.xOld || move.yNew != move.yOld);

	// check that there is a piece at the starting location
	Piece * moving = valid ? m_board.GetPiece(move.xOld, move.yOld) : nullptr;

	// check that correct piece is at the starting location
	valid = valid && moving != nullptr && moving->GetColor() == color && moving->GetType() == move.type;

	// if a piece is at the destination, check that it is an enemy piece
	Piece * target = valid ? m_board.GetPiece(move.xNew, move.yNew) : nullptr;
	if (target != nullptr)
	{
		valid = valid && target->GetColor() != color;
	}

	// check that move is valid for piece
	valid = valid && CheckMovePath(moving, move.xNew, move.yNew);
	// check that move path is clear of other pieces
	valid = valid && CheckPathForObstacles(moving, move.xNew, move.yNew);
	//
	valid = valid && CheckIfKingInCheck(color);

	return valid;
}

// check if any of the provided piece types are on the path defined
bool Game::CheckStraightPathForAggressors(int x, int y, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types)
{
	bool valid = true;

	int xStart = x + xStep;
	int yStart = y + yStep;

	// iterate through path using the single step defined
	// check to see if any of the piece types provided are on the path
	while (Common::CheckIfOnBoard(xStart, yStart))
	{
		Piece * p = m_board.GetPiece(xStart, yStart);
		if (p != nullptr)
		{
			if (p->GetColor() != color && types.find(p->GetType()) != types.end())
			{
				valid = false;
			}

			// if any piece found, stop searching
			// no more line of sight to starting point
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
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, 1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, -1, 1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, 1, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, -1, -1, color, lethalTypes);

	// check to see if 4 straight lanes are clear of rooks and queens
	lethalTypes.clear();
	lethalTypes.insert(Common::PieceType::ROOK);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, 1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, -1, 0, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, 0, -1, color, lethalTypes);
	valid = valid && CheckStraightPathForAggressors(kingLoc.first, kingLoc.second, 0, -1, color, lethalTypes);

	return valid;
}

// check that path from start location to destination is clear of any pieces
bool Game::CheckPathForObstacles(Piece * piece, int x, int y)
{
	bool valid = true;

	switch(piece->GetType())
	{
	case Common::PieceType::PAWN:
		// NEED TO REDO THIS PART
		if (y - piece->GetLocation().second > 1)
		{
			valid = m_board.GetPiece(x, y - 1) == nullptr;
		}
		break;
	case Common::PieceType::BISHOP:
	case Common::PieceType::ROOK:
	case Common::PieceType::QUEEN:
		// reduce x, y delta to most basic step
		int xStart = piece->GetLocation().first;
		int yStart = piece->GetLocation().second;
		int xDelta = x - xStart;
		int yDelta = y - yStart;
		xDelta = xDelta / std::abs(xDelta);
		yDelta = yDelta / std::abs(yDelta);
		// iterate forward from start to destination in steps
		// make sure no pieces are in that location
		xStart += xDelta;
		yStart += yDelta;
		while (xStart != x && yStart != y)
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
bool Game::CheckMovePath(Piece * piece, int x, int y)
{
	bool valid = false;

	// get the x, y changes
	int xDelta = x - piece->GetLocation().first;
	int yDelta = y - piece->GetLocation().second;

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
			Piece * p = m_board.GetPiece(x, y);
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