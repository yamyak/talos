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
bool Game::CheckGameStatus(Common::Color color)
{
	bool status = true;

	m_board.SetMode(color);

	// retrieve all the piece locations
	Common::MiniBoard mini = m_board.GetBoard();

	if (!CheckIfKingInCheck(color, mini))
	{
		status = false;

		// iterate through all same color pieces on the board
		for (int i = 0; i < Common::BOARD_LENGTH; i++)
		{
			for (int j = 0; j < Common::BOARD_LENGTH; j++)
			{
				Common::PieceInfo * p = mini.data[i][j];
				if (p != nullptr && p->color == color)
				{
					// find all possible moves for each piece
					std::vector<Common::MoveRequest> moves = FindPotentialMoves(mini, i, j);

					for (Common::MoveRequest & move : moves)
					{
						Common::MiniBoard & testBoard(mini);
						// apply each move to test board and check for check again
						m_board.ApplyMove(move, testBoard.data);

						// if way out found, return true
						if (CheckIfKingInCheck(color, testBoard))
						{
							return true;
						}
					}
				}
			}
		}
		
		// if no way out found, set the winner and return false
		if (!status)
		{
			m_winner = color == Common::Color::WHITE ? Common::Color::BLACK : Common::Color::WHITE;
		}
		
	}

	return status;
}

// request a move from the player
// check that the move is valid
// apply the move to the board
void Game::QueryPlayerForMove(Player & p)
{
	// retrieve all the piece locations
	Common::MiniBoard mini = m_board.GetBoard();

	// query for move until valid move returned
	bool validMove = false;
	Common::MoveRequest move;
	while (!validMove)
	{
		move = p.MakeMove(mini);

		// check if the move is a valid one
		validMove = m_validator.CheckMoveRequest(move, p.GetColor(), mini);

		Common::MiniBoard testBoard(mini);
		if (validMove)
		{
			m_board.ApplyMove(move, testBoard.data);
		}
		
		validMove = validMove && CheckIfKingInCheck(p.GetColor(), mini);
	}

	// apply the move to the board
	m_board.ApplyMove(move);
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

int main()
{
	Game g;
	Algorithm a1;
	Player p1(&a1);
	Algorithm a2;
	Player p2(&a2);

	g.RegisterPlayers(p1, p2);

	Player * currentPlayer = &p1;
	while (g.CheckGameStatus(currentPlayer->GetColor()))
	{
		g.QueryPlayerForMove(*currentPlayer);
		
		currentPlayer = (currentPlayer == &p1) ? &p2 : &p1;
	}

	// return the game winner

	return 0;
}