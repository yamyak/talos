#include "stdafx.h"

#define BACKEND_EXPORTS

#include "Game.h"
#include "Common.h"

Game::Game()
{
}

Game::~Game()
{
}

// check if king is in check
// returns false if king IS IN check
bool Game::CheckIfKingInCheck(Common::Color color, Common::MiniBoard & board)
{
	bool valid = true;

	// get location of king
	std::pair<int, int> kingLoc = GetKingLocation(color, board);
	
	// check for special case where king attacked by pawn
	valid = m_validator.CheckPawnAggressors(kingLoc, color, board);

	// check for special case where king attacked by knight
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

	// iterate through all locations on the board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		for (int j = 0; j < Common::BOARD_LENGTH; j++)
		{
			Common::PieceInfo & piece = board.data[i][j];

			// check if piece in location is a king of own color
			if (piece.occupied && piece.color == color && piece.type == Common::PieceType::KING)
			{
				loc.first = i;
				loc.second = j;
				break;
			}
		}
	}

	return loc;
}

// iterates in all directions provided and creates move requests to return
std::vector<Common::MoveRequest> Game::FindStraightLineMoves(Common::MiniBoard & board, int x, int y,
	const std::vector<std::pair<int, int>> & steps, Common::Color & color, Common::PieceType & type)
{
	std::vector<Common::MoveRequest> moves;

	// iterate through all directions
	for (const std::pair<int, int> & step : steps)
	{
		bool go = true;

		int xLoc = x;
		int yLoc = y;

		// loop while stepping forward
		while (go)
		{
			// move 1 step in current direction
			xLoc += step.first;
			yLoc += step.second;

			// check if current location is still on board
			if (Common::CheckIfOnBoard(xLoc, yLoc))
			{
				// if location empty or piece there is opposing color
				// create move request and add to list
				Common::PieceInfo & potential = board.data[xLoc][yLoc];
				if (!potential.occupied || potential.color != color)
				{
					Common::MoveRequest req;
					req.type = type;
					req.xOld = x;
					req.yOld = y;
					req.xNew = xLoc;
					req.yNew = yLoc;
					moves.push_back(req);

					// if opposing color piece found, then end the loop
					// can not move any further down this path
					if (potential.occupied)
					{
						go = false;
					}
				}
				else
				{
					// if same color piece found, then end the loop
					// can not move any further down this path
					go = false;
				}
			}
			else
			{
				// if no longer on game board, then end the loop
				// can not move any further down this path
				go = false;
			}
		}
	}

	return moves;
}

// creates move requests for valid moves provided
std::vector<Common::MoveRequest> Game::FindSpotMoves(Common::MiniBoard & board, int x, int y,
	const std::vector<std::pair<int, int>> & locs, Common::Color & color, Common::PieceType & type)
{
	std::vector<Common::MoveRequest> moves;

	// iterate through moves provided
	for (const std::pair<int, int> & loc : locs)
	{
		// check if target location is on the board
		if (Common::CheckIfOnBoard(x + loc.first, y + loc.second))
		{
			// if location empty or piece there is opposing color
			// create move request and add to list
			Common::PieceInfo & potential = board.data[x + loc.first][y + loc.second];
			if (!potential.occupied || potential.color != color)
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

// create list of all potential moves that piece in provided location can make
std::vector<Common::MoveRequest> Game::FindPotentialMoves(Common::MiniBoard & board, int x, int y)
{
	std::vector<Common::MoveRequest> moves;

	// get piece in location
	Common::PieceInfo & p = board.data[x][y];

	switch (p.type)
	{
	case Common::PieceType::KING:
	{
		// create list of all moves king can make and create move requests for them
		const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
		moves = FindSpotMoves(board, x, y, locs, p.color, p.type);
		break;
	}
	case Common::PieceType::QUEEN:
	{
		// create list of all directions queen can move in and iterate through those directions to create move requests
		const std::vector<std::pair<int, int>> steps = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p.color, p.type);
		break;
	}
	case Common::PieceType::PAWN:
	{
		// check if location 1 unit forward is on board
		if (Common::CheckIfOnBoard(x, y + 1))
		{
			// if location empty, create move request and add to list
			if (!board.data[x][y + 1].occupied)
			{
				Common::MoveRequest req;
				req.type = p.type;
				req.xOld = x;
				req.yOld = y;
				req.xNew = x;
				req.yNew = y + 1;
				moves.push_back(req);

				// check if first move and location 2 units forward is on board
				if (y == 1 && Common::CheckIfOnBoard(x, y + 2))
				{
					// if location empty, create move request and add to list
					if (!board.data[x][y + 2].occupied)
					{
						Common::MoveRequest req;
						req.type = p.type;
						req.xOld = x;
						req.yOld = y;
						req.xNew = x;
						req.yNew = y + 2;
						moves.push_back(req);
					}
				}
			}
		}

		// check 2 1-step forward diagonal to see if possible move
		const std::vector<int> d_moves = { -1, 1 };
		for (int m : d_moves)
		{
			// check if potential move location is on board
			if (Common::CheckIfOnBoard(x + m, y + 1))
			{
				// if location not empty and filled with opposing color piece
				// create move request and add to list
				Common::PieceInfo & diagonal_p = board.data[x + m][y + 1];
				if (diagonal_p.occupied && diagonal_p.color != p.color)
				{
					Common::MoveRequest req;
					req.type = p.type;
					req.xOld = x;
					req.yOld = y;
					req.xNew = x + m;
					req.yNew = y + 1;
					moves.push_back(req);
				}
			}
		}

		break;
	}
	case Common::PieceType::ROOK:
	{
		// create list of all directions rook can move in and iterate through those directions to create move requests
		const std::vector<std::pair<int, int>> steps = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p.color, p.type);
		break;
	}
	case Common::PieceType::BISHOP:
	{
		// create list of all directions bishop can move in and iterate through those directions to create move requests
		const std::vector<std::pair<int, int>> steps = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
		moves = FindStraightLineMoves(board, x, y, steps, p.color, p.type);
		break;
	}
	case Common::PieceType::KNIGHT:
	{
		// create list of all moves knight can make and create move requests for them
		const std::vector<std::pair<int, int>> locs = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
		moves = FindSpotMoves(board, x, y, locs, p.color, p.type);
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
	// update new location with piece information
	board.data[move.xNew][move.yNew].Update(board.data[move.xOld][move.yOld]);

	// clear out the old piece location
	board.data[move.xOld][move.yOld].occupied = false;
}

// initializes the game board with all pieces
bool Game::Initialize(Common::MiniBoard & board)
{
	m_currentTurn = Common::Color::WHITE;

	// add the pawns to the board
	for (int i = 0; i < Common::BOARD_LENGTH; i++)
	{
		board.data[i][1].Update(Common::PieceType::PAWN, Common::Color::WHITE);
		board.data[i][6].Update(Common::PieceType::PAWN, Common::Color::BLACK);
	}

	// add the rooks to the board
	board.data[0][0].Update(Common::PieceType::ROOK, Common::Color::WHITE);
	board.data[7][0].Update(Common::PieceType::ROOK, Common::Color::WHITE);

	board.data[0][7].Update(Common::PieceType::ROOK, Common::Color::BLACK);
	board.data[7][7].Update(Common::PieceType::ROOK, Common::Color::BLACK);

	// add the knights to the board
	board.data[1][0].Update(Common::PieceType::KNIGHT, Common::Color::WHITE);
	board.data[6][0].Update(Common::PieceType::KNIGHT, Common::Color::WHITE);

	board.data[1][7].Update(Common::PieceType::KNIGHT, Common::Color::BLACK);
	board.data[6][7].Update(Common::PieceType::KNIGHT, Common::Color::BLACK);

	// add the bishops to the board
	board.data[2][0].Update(Common::PieceType::BISHOP, Common::Color::WHITE);
	board.data[5][0].Update(Common::PieceType::BISHOP, Common::Color::WHITE);

	board.data[2][7].Update(Common::PieceType::BISHOP, Common::Color::BLACK);
	board.data[5][7].Update(Common::PieceType::BISHOP, Common::Color::BLACK);

	// add the queens to the board
	board.data[3][0].Update(Common::PieceType::QUEEN, Common::Color::WHITE);
	board.data[3][7].Update(Common::PieceType::QUEEN, Common::Color::BLACK);

	// add the kings to the board
	board.data[4][0].Update(Common::PieceType::KING, Common::Color::WHITE);
	board.data[4][7].Update(Common::PieceType::KING, Common::Color::BLACK);

	return true;
}

Common::Color Game::GetCurrentTurn()
{
	return m_currentTurn;
}

// attempts to apply the move request on the board provided
bool Game::AttemptMove(Common::MoveRequest & move, Common::MiniBoard & board)
{
	bool status = false;

	// check if the move is a valid one
	if (m_validator.CheckMoveRequest(m_currentTurn, move, board))
	{
		// apply the move to a test board
		Common::MiniBoard testBoard(board);
		ApplyMove(move, testBoard);

		// check if move puts own king in check
		if (CheckIfKingInCheck(m_currentTurn, testBoard))
		{
			// apply the move to the actual board
			ApplyMove(move, board);
			m_currentTurn = (m_currentTurn == Common::Color::WHITE) ? Common::Color::BLACK : Common::Color::WHITE;
			status = true;
		}
	}

	return status;
}

// check if player's king is in check mate
// returns false if the player's king IS IN checkmate
bool Game::CheckGameStatus(Common::MiniBoard & board)
{
	bool status = true;

	// check if king is in check
	// checks all potential moves for solution if king in check
	if (!CheckIfKingInCheck(m_currentTurn, board))
	{
		status = false;

		// iterate through all locations on board
		for (int i = 0; i < Common::BOARD_LENGTH; i++)
		{
			for (int j = 0; j < Common::BOARD_LENGTH; j++)
			{
				// make sure piece in location and is own color
				Common::PieceInfo & p = board.data[i][j];
				if (p.occupied && p.color == m_currentTurn)
				{
					// find all possible moves for current piece
					std::vector<Common::MoveRequest> moves = FindPotentialMoves(board, i, j);

					// iterate through all possible moves for current piece
					for (Common::MoveRequest & move : moves)
					{
						// apply each move to test board
						Common::MiniBoard & testBoard(board);
						ApplyMove(move, testBoard);

						// check if king is still in check
						if (CheckIfKingInCheck(m_currentTurn, testBoard))
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