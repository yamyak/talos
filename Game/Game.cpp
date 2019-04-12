#include "stdafx.h"

#include "Game.h"
#include "Common.h"

Game::Game()
{
}

Game::~Game()
{
}

// check if king is in check
bool Game::CheckIfKingInCheck(Common::Color color, Common::MiniBoard & board)
{
	bool valid = true;

	// get location of king
	std::pair<int, int> kingLoc = GetKingLocation(color, board);
	
	valid = m_validator.CheckPawnAggressors(kingLoc, color, board);

	valid = valid && m_validator.CheckKnightAggressors(kingLoc, color, board);

	std::set<Common::PieceType> lethalTypes;
	
	// check to see if 4 diagonals are clear of bishops and queens
	lethalTypes.insert(Common::PieceType::BISHOP);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, 1, 1, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, -1, 1, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, 1, -1, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, -1, -1, lethalTypes);

	// check to see if 4 straight lanes are clear of rooks and queens
	lethalTypes.clear();
	lethalTypes.insert(Common::PieceType::ROOK);
	lethalTypes.insert(Common::PieceType::QUEEN);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, 1, 0, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, -1, 0, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, 0, 1, lethalTypes);
	valid = valid && m_validator.CheckStraightPathForAggressors(kingLoc, color, board, 0, -1, lethalTypes);

	return valid;
}

// returns the x, y coordinates to the king piece for the specified color
std::pair<int, int> Game::GetKingLocation(Common::Color color, Common::MiniBoard & board)
{
	std::pair<int, int> loc;

	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			Common::PieceInfo * ptr = board.data[i][j];

			if (ptr != nullptr && ptr->color == color && ptr->type == Common::PieceType::KING)
			{
				loc.first = i;
				loc.second = j;
			}
		}
	}

	return loc;
}

std::vector<Common::MoveRequest> Game::FindStraightLineMoves(Common::MiniBoard & board, int x, int y,
	const std::vector<std::pair<int, int>> & steps, Common::Color & color, Common::PieceType & type)
{
	std::vector<Common::MoveRequest> moves;

	for (const std::pair<int, int> & step : steps)
	{
		bool go = true;

		while (go)
		{
			int xLoc = x + step.first;
			int yLoc = y + step.second;

			if (Common::CheckIfOnBoard(xLoc, yLoc))
			{
				Common::PieceInfo * potential = board.data[xLoc][yLoc];

				if (potential == nullptr || potential->color != color)
				{
					Common::MoveRequest req;
					req.type = type;
					req.xOld = x;
					req.yOld = y;
					req.xNew = xLoc;
					req.yNew = yLoc;
					moves.push_back(req);

					if (potential != nullptr)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	return moves;
}

std::vector<Common::MoveRequest> Game::FindSpotMoves(Common::MiniBoard & board, int x, int y,
	const std::vector<std::pair<int, int>> & locs, Common::Color & color, Common::PieceType & type)
{
	std::vector<Common::MoveRequest> moves;

	for (const std::pair<int, int> & loc : locs)
	{
		if (Common::CheckIfOnBoard(x + loc.first, y + loc.second))
		{
			Common::PieceInfo * potential = board.data[x + loc.first][y + loc.second];

			if (potential == nullptr || potential->color != color)
			{
				Common::MoveRequest req;
				req.type = type;
				req.xOld = x;
				req.yOld = y;
				req.xNew = x + loc.first;
				req.yNew = y + loc.second;
				moves.push_back(req);
			}
		}
	}
	
	return moves;
}

std::vector<Common::MoveRequest> Game::FindPotentialMoves(Common::MiniBoard & board, int x, int y)
{
	std::vector<Common::MoveRequest> moves;

	Common::PieceInfo * p = board.data[x][y];

	switch (p->type)
	{
	case Common::PieceType::KING:
	{
		const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
		moves = FindSpotMoves(board, x, y, locs, p->color, p->type);
		break;
	}
	case Common::PieceType::QUEEN:
	{
		const std::vector<std::pair<int, int>> steps = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p->color, p->type);
		break;
	}
	case Common::PieceType::PAWN:
		break;
	case Common::PieceType::ROOK:
	{
		const std::vector<std::pair<int, int>> steps = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p->color, p->type);
		break;
	}
	case Common::PieceType::BISHOP:
	{
		const std::vector<std::pair<int, int>> steps = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p->color, p->type);
		break;
	}
	case Common::PieceType::KNIGHT:
	{
		const std::vector<std::pair<int, int>> locs = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
		moves = FindSpotMoves(board, x, y, locs, p->color, p->type);
		break;
	}
	default:
		break;
	}

	return moves;
}

// applys the move specified to board specified
void Game::ApplyMove(Common::MoveRequest & move, Common::MiniBoard & board)
{
	// retrieve the piece to be moved
	Common::PieceInfo * p = board.data[move.xOld][move.yOld];
	// clear out the old piece location
	board.data[move.xOld][move.yOld] = nullptr;

	// check if new location is not empty
	if (board.data[move.xNew][move.yNew] != nullptr)
	{
		// retrieve the piece that will be killed and delete it
		Common::PieceInfo * dead = board.data[move.xNew][move.yNew];
		delete dead;
		dead = nullptr;
	}

	// move piece to new location
	board.data[move.xNew][move.yNew] = p;
}

bool Game::Initialize(Common::MiniBoard & board)
{
	// add the pawns to the board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		board.data[i][1] = new Common::PieceInfo(Common::PieceType::PAWN, Common::Color::WHITE);
		board.data[i][6] = new Common::PieceInfo(Common::PieceType::PAWN, Common::Color::BLACK);
	}

	// add the rooks to the board
	board.data[0][0] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::WHITE);
	board.data[7][0] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::WHITE);

	board.data[0][7] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::BLACK);
	board.data[7][7] = new Common::PieceInfo(Common::PieceType::ROOK, Common::Color::BLACK);

	// add the knights to the board
	board.data[1][0] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::WHITE);
	board.data[6][0] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::WHITE);

	board.data[1][7] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::BLACK);
	board.data[6][7] = new Common::PieceInfo(Common::PieceType::KNIGHT, Common::Color::BLACK);

	// add the bishops to the board
	board.data[2][0] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::WHITE);
	board.data[5][0] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::WHITE);

	board.data[2][7] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::BLACK);
	board.data[5][7] = new Common::PieceInfo(Common::PieceType::BISHOP, Common::Color::BLACK);

	// add the queens to the board
	board.data[3][0] = new Common::PieceInfo(Common::PieceType::QUEEN, Common::Color::WHITE);
	board.data[4][7] = new Common::PieceInfo(Common::PieceType::QUEEN, Common::Color::BLACK);

	// add the kings to the board
	board.data[4][0] = new Common::PieceInfo(Common::PieceType::KING, Common::Color::WHITE);
	board.data[3][7] = new Common::PieceInfo(Common::PieceType::KING, Common::Color::BLACK);

	return true;
}

bool Game::AttemptMove(Common::Color & color, Common::MoveRequest & move, Common::MiniBoard & board)
{
	bool status = false;

	// check if the move is a valid one
	if (m_validator.CheckMoveRequest(color, move, board))
	{
		Common::MiniBoard testBoard(board);
		ApplyMove(move, testBoard);

		if (CheckIfKingInCheck(color, testBoard))
		{
			// apply the move to the board
			ApplyMove(move, board);
			status = true;
		}
	}

	return status;
}

// check if next player's king is in check mate
// update the winner flag if it is
bool Game::CheckGameStatus(Common::Color & color, Common::MiniBoard & board)
{
	bool status = true;

	if (!CheckIfKingInCheck(color, board))
	{
		status = false;

		// iterate through all same color pieces on the board
		for (int i = 0; i < Common::BOARD_LENGTH; i++)
		{
			for (int j = 0; j < Common::BOARD_LENGTH; j++)
			{
				Common::PieceInfo * p = board.data[i][j];
				if (p != nullptr && p->color == color)
				{
					// find all possible moves for each piece
					std::vector<Common::MoveRequest> moves = FindPotentialMoves(board, i, j);

					for (Common::MoveRequest & move : moves)
					{
						Common::MiniBoard & testBoard(board);
						// apply each move to test board and check for check again
						ApplyMove(move, testBoard);

						// if way out found, return true
						if (CheckIfKingInCheck(color, testBoard))
						{
							return true;
						}
					}
				}
			}
		}
	}

	return status;
}

int main()
{
	return 0;
}