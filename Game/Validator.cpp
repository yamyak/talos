#include "stdafx.h"
#include "Validator.h"

#include <vector>

Validator::Validator()
{
}

Validator::~Validator()
{
}

// check that path from start location to destination is clear of any pieces
bool Validator::CheckPathForObstacles(Common::MoveRequest & move, Common::MiniBoard & board)
{
	bool valid = true;

	switch (move.type)
	{
	case Common::PieceType::PAWN:
		// if moved forward 2 units
		if (move.yNew - move.yOld == 2)
		{
			// make sure space 1 unit ahead is not occupied
			valid = board.data[move.xNew][move.yNew - 1] == nullptr;
		}
		break;
	case Common::PieceType::BISHOP:
	case Common::PieceType::ROOK:
	case Common::PieceType::QUEEN:
	{
		// reduce x, y delta to most basic step
		int xStart = move.xOld;
		int yStart = move.yOld;
		int xDelta = move.xNew - xStart;
		int yDelta = move.yNew - yStart;
		xDelta = xDelta / std::abs(xDelta);
		yDelta = yDelta / std::abs(yDelta);
		
		// start from first step on path to target
		xStart += xDelta;
		yStart += yDelta;

		// iterate forward from start to destination in steps
		while (valid && xStart != move.xNew && yStart != move.yNew)
		{
			// make sure no pieces are in that location
			valid = board.data[xStart][yStart] == nullptr;

			xStart += xDelta;
			yStart += yDelta;
		}
		break;
	}
	default:
		break;
	}

	return valid;
}

// check if king is under attack from any pawns
bool Validator::CheckPawnAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board)
{
	bool valid = true;

	// check 2 1-step forward diagonal to see if pawn there
	std::vector<int> locs = { kingLoc.first - 1, kingLoc.second + 1 };
	for (int x : locs)
	{
		// check if potential pawn location is on the board
		if (valid && Common::CheckIfOnBoard(x, kingLoc.second + 1))
		{
			// check if piece found at location, is opposing color, and a pawn
			Common::PieceInfo * p = board.data[x][kingLoc.second + 1];
			if (p != nullptr && p->color != color && p->type == Common::PieceType::PAWN)
			{
				valid = false;
			}
		}
	}

	return valid;
}

// check if king is under attack from any knights
bool Validator::CheckKnightAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board)
{
	bool valid = true;

	// vector holding all possible movements that a knight can make
	const std::vector<std::pair<int, int>> locs = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

	// iterate through all possible knight attack locations and see if knight there
	std::vector<std::pair<int, int>>::const_iterator it = locs.begin();
	while (valid && it != locs.end())
	{
		// calculate potential knight location
		int xLoc = kingLoc.first + it->first;
		int yLoc = kingLoc.second + it->second;

		// check if potential location is on board
		if (Common::CheckIfOnBoard(xLoc, yLoc))
		{
			// check if piece found at location, is opposing color, and a knight
			Common::PieceInfo * p = board.data[xLoc][yLoc];
			if (p != nullptr && p->color != color && p->type == Common::PieceType::KNIGHT)
			{
				valid = false;
			}
		}

		it++;
	}

	return valid;
}

// validate that move is a valid move for the piece moving
bool Validator::CheckMovePath(Common::MoveRequest & move, Common::MiniBoard & board)
{
	bool valid = false;

	// get the x, y changes
	int xDelta = move.xNew - move.xOld;
	int yDelta = move.yNew - move.yOld;

	// get the absolute x, y changes
	int xDeltaAbs = std::abs(xDelta);
	int yDeltaAbs = std::abs(yDelta);

	// validate the move based on the piece type
	switch (move.type)
	{
	case Common::PieceType::PAWN:
		// if the pawn moved diagonally
		if (xDeltaAbs == 1)
		{
			// if target location occupied, already checked that piece is enemy type
			// don't need to check it again
			Common::PieceInfo * p = board.data[move.xNew][move.yNew];
			
			// make sure only moved forward 1 unit and target is occupied
			valid = yDelta == 1 && p != nullptr;
		}
		else if(xDeltaAbs == 0)
		{
			// make sure either:
			//	1. moved forward 1 unit
			//	2. moved forward 2 units but previously at initial position
			valid = yDelta == 1 || (yDelta == 2 && move.yOld == 1);
		}
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

// verify if the move provided is a valid move
bool Validator::CheckMoveRequest(Common::Color color, Common::MoveRequest & move, Common::MiniBoard & board)
{
	bool valid = true;

	// check if location coordinates are on the board
	valid = Common::CheckIfOnBoard(move.xNew, move.yNew) && Common::CheckIfOnBoard(move.xOld, move.yOld);

	// check that piece is actually moving and not sitting in 1 place
	valid = valid && (move.xNew != move.xOld || move.yNew != move.yOld);

	// check that there is a piece at the starting location
	Common::PieceInfo * moving = valid ? board.data[move.xOld][move.yOld] : nullptr;

	// check that correct piece is at the starting location
	valid = valid && moving != nullptr && moving->color == color && moving->type == move.type;

	// if a piece is at the destination, check that it is an enemy piece
	Common::PieceInfo * target = valid ? board.data[move.xNew][move.yNew] : nullptr;
	if (target != nullptr)
	{
		valid = valid && target->color != color;
	}

	// check that move is valid for piece
	valid = valid && CheckMovePath(move, board);
	// check that move path is clear of other pieces
	valid = valid && CheckPathForObstacles(move, board);

	return valid;
}

// check if any of the provided piece types are on the path defined
bool Validator::CheckStraightPathForAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board, int xStep, int yStep, std::set<Common::PieceType> types)
{
	bool valid = true;

	// start on the first step of path to target
	int xStart = kingLoc.first + xStep;
	int yStart = kingLoc.second + yStep;

	// iterate through path using the single step defined
	while (Common::CheckIfOnBoard(xStart, yStart))
	{
		// check if piece found in current location on path
		Common::PieceInfo * p = board.data[xStart][yStart];
		if (p != nullptr)
		{
			// check to see if piece is opposing color and in piece type set parameter
			if (p->color != color && types.find(p->type) != types.end())
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