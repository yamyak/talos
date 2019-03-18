#include "stdafx.h"
#include "Validator.h"

Validator::Validator()
{
}

Validator::~Validator()
{
}

// check that path from start location to destination is clear of any pieces
bool Validator::CheckPathForObstacles(Piece * piece, int x, int y, Board & board)
{
	bool valid = true;

	switch (piece->GetType())
	{
	case Common::PieceType::PAWN:
		// NEED TO REDO THIS PART
		if (y - piece->GetLocation().second > 1)
		{
			valid = board.GetPiece(x, y - 1) == nullptr;
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
			valid = board.GetPiece(xStart, yStart) == nullptr;
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
bool Validator::CheckMovePath(Piece * piece, int x, int y, Board & board)
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
		else if (xDeltaAbs == 1 && yDelta == 1)
		{
			// if moving diagonal 1 step, check that enemy piece exists at destination
			Piece * p = board.GetPiece(x, y);
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

// verify if the move provided is a valid move
bool Validator::CheckMoveRequest(Common::MoveRequest & move, Common::Color color, Board & board)
{
	bool valid = true;

	// check if location coordinates are on the board
	valid = Common::CheckIfOnBoard(move.xNew, move.yNew) && Common::CheckIfOnBoard(move.xOld, move.yOld);

	// check that piece is actually moving and not sitting in 1 place
	valid = valid && (move.xNew != move.xOld || move.yNew != move.yOld);

	// check that there is a piece at the starting location
	Piece * moving = valid ? board.GetPiece(move.xOld, move.yOld) : nullptr;

	// check that correct piece is at the starting location
	valid = valid && moving != nullptr && moving->GetColor() == color && moving->GetType() == move.type;

	// if a piece is at the destination, check that it is an enemy piece
	Piece * target = valid ? board.GetPiece(move.xNew, move.yNew) : nullptr;
	if (target != nullptr)
	{
		valid = valid && target->GetColor() != color;
	}

	// check that move is valid for piece
	valid = valid && CheckMovePath(moving, move.xNew, move.yNew, board);
	// check that move path is clear of other pieces
	valid = valid && CheckPathForObstacles(moving, move.xNew, move.yNew, board);

	return valid;
}

// check if any of the provided piece types are on the path defined
bool Validator::CheckStraightPathForAggressors(Board & board, int x, int y, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types)
{
	bool valid = true;

	int xStart = x + xStep;
	int yStart = y + yStep;

	// iterate through path using the single step defined
	// check to see if any of the piece types provided are on the path
	while (Common::CheckIfOnBoard(xStart, yStart))
	{
		Piece * p = board.GetPiece(xStart, yStart);
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