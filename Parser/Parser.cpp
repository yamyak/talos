#include "stdafx.h"

#define PARSER_EXPORTS

#include <regex>
#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

Common::MoveRequest Parser::ParseMove(Common::Color color, Common::MiniBoard & board, std::string & moveString)
{
	Common::MoveRequest move;

	// queen side castling
	if (std::regex_search(moveString, std::regex("0-0-0")) ||
		std::regex_search(moveString, std::regex("O-O-O")))
	{
		move.type = Common::PieceType::KING;
		if (color == Common::Color::WHITE)
		{
			move.xOld = 4;
			move.yOld = 0;
			move.xNew = 2;
			move.yNew = 0;
		}
		else
		{
			move.xOld = 3;
			move.yOld = 0;
			move.xNew = 5;
			move.yNew = 0;
		}
	}
	// king side castling
	else if (std::regex_search(moveString, std::regex("0-0")) ||
		std::regex_search(moveString, std::regex("O-O")))
	{
		move.type = Common::PieceType::KING;
		if (color == Common::Color::WHITE)
		{
			move.xOld = 4;
			move.yOld = 0;
			move.xNew = 6;
			move.yNew = 0;
		}
		else
		{
			move.xOld = 3;
			move.yOld = 0;
			move.xNew = 1;
			move.yNew = 0;
		}
	}
	// king, queen, rook, bishop, knight, or pawn move
	else if(std::regex_search(moveString, std::regex("^[KQRBN]?[a-h]?[1-8]?x?[a-h][1-8]")))
	{
		switch (moveString.at(0))
		{
			case('K'):
			{
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				move = ParseMoveStringLocations(color, Common::PieceType::KING, board, moveString.substr(1), locs);
				break;
			}
			case('Q'):
			{
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				move = ParseMoveStringLocations(color, Common::PieceType::QUEEN, board, moveString.substr(1), locs);
				break;
			}
			case('R'):
			{
				const std::vector<std::pair<int, int>> locs = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				move = ParseMoveStringLocations(color, Common::PieceType::ROOK, board, moveString.substr(1), locs);
				break;
			}
			case('B'):
			{
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
				move = ParseMoveStringLocations(color, Common::PieceType::BISHOP, board, moveString.substr(1), locs);
				break;
			}
			case('N'):
			{
				const std::vector<std::pair<int, int>> locs = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
				move = ParseMoveStringLocations(color, Common::PieceType::KNIGHT, board, moveString.substr(1), locs);
				break;
			}
			default:
			{
				move = ParsePawnMove(color, board, moveString);
				break;
			}
		}
	}

	return move;
}

void Parser::FindPieceInSpots(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
	const std::vector<std::pair<int, int>> & locs, std::pair<int, int> conditions)
{
	for (const std::pair<int, int> & loc : locs)
	{
		int xLoc = move.xNew + loc.first;
		int yLoc = move.yNew + loc.second;

		if (Common::CheckIfOnBoard(xLoc, yLoc) &&
			board.data[xLoc][yLoc].occupied &&
			board.data[xLoc][yLoc].color == color &&
			board.data[xLoc][yLoc].type == move.type)
		{
			if ((conditions.first == -1 && conditions.second == -1) ||
				(conditions.first == -1 && conditions.second == yLoc) ||
				(conditions.second == -1 && conditions.first == xLoc))
			{
				move.xOld = xLoc;
				move.yOld = yLoc;
				break;
			}
		}
	}
}

void Parser::FindPieceOnLines(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
	const std::vector<std::pair<int, int>> & steps, std::pair<int, int> conditions)
{
	bool found = false;

	for (const std::pair<int, int> & step : steps)
	{
		int xLoc = move.xNew;
		int yLoc = move.yNew;

		bool blocked = false;

		while (!blocked && !found)
		{
			xLoc += step.first;
			yLoc += step.second;

			if (Common::CheckIfOnBoard(xLoc, yLoc))
			{
				if (board.data[xLoc][yLoc].occupied)
				{
					if (board.data[xLoc][yLoc].color == color &&
						board.data[xLoc][yLoc].type == move.type)
					{
						if ((conditions.first == -1 && conditions.second == -1) ||
							(conditions.first == -1 && conditions.second == yLoc) ||
							(conditions.second == -1 && conditions.first == xLoc))
						{
							move.xOld = xLoc;
							move.yOld = yLoc;
							found = true;
						}
					}
					else
					{
						blocked = true;
					}
				}
			}
			else
			{
				blocked = true;
			}
		}

		if (found)
		{
			break;
		}
	}
}

Common::MoveRequest Parser::ParsePawnMove(Common::Color color, Common::MiniBoard & board, std::string moveString)
{
	Common::MoveRequest move;
	move.type = Common::PieceType::PAWN;

	size_t xPos = moveString.find('x');
	if (xPos != std::string::npos)
	{
		move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
		move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
		move.xOld = GetIntegerFile(color, moveString.at(xPos - 1));
		move.yOld = -1;
		for (int j = move.yNew - 1; j >= 0; j--)
		{
			if (board.data[move.xOld][j].occupied && 
				board.data[move.xOld][j].color == color &&
				board.data[move.xOld][j].type == Common::PieceType::PAWN)
			{
				move.yOld = j;
				break;
			}
		}
		
		xPos += 3;
	}
	else
	{
		move.xNew = GetIntegerFile(color, moveString.at(0));
		move.yNew = GetIntegerRank(color, moveString.at(1));
		move.xOld = move.xNew;
		move.yOld = -1;
		for (int j = move.yNew - 1; j >= 0; j--)
		{
			if (board.data[move.xOld][j].occupied &&
				board.data[move.xOld][j].color == color &&
				board.data[move.xOld][j].type == Common::PieceType::PAWN)
			{
				move.yOld = j;
				break;
			}
		}

		xPos = 2;
	}

	if (std::regex_search(moveString, std::regex("[QRBN]")))
	{
		if (moveString.find('=') != std::string::npos)
		{
			xPos++;
		}

		switch (moveString.at(xPos))
		{
			case('Q'):
				move.pawnPromotion = Common::PieceType::QUEEN;
				break;
			case('R'):
				move.pawnPromotion = Common::PieceType::ROOK;
				break;
			case('B'):
				move.pawnPromotion = Common::PieceType::BISHOP;
				break;
			case('N'):
				move.pawnPromotion = Common::PieceType::KNIGHT;
				break;
			default:
				break;
		}
	}

	return move;
}

Common::MoveRequest Parser::ParseMoveStringLocations(Common::Color color, Common::PieceType type, Common::MiniBoard & board, 
	std::string moveString, const std::vector<std::pair<int, int>> & locs)
{
	Common::MoveRequest move;
	move.type = type;
	move.xOld = -1;
	move.yOld = -1;

	size_t xPos = moveString.find('x');

	if (std::regex_search(moveString, std::regex("[a-h][1-8]x?[a-h][1-8]")))
	{
		if (xPos != std::string::npos)
		{
			move.xOld = GetIntegerFile(color, moveString.at(xPos - 2));
			move.yOld = GetIntegerRank(color, moveString.at(xPos - 1));
			move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
			move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
		}
		else
		{
			move.xOld = GetIntegerFile(color, moveString.at(0));
			move.yOld = GetIntegerRank(color, moveString.at(1));
			move.xNew = GetIntegerFile(color, moveString.at(2));
			move.yNew = GetIntegerRank(color, moveString.at(3));
		}
	}
	else
	{
		std::pair<int, int> conditions{ 0, 0 };

		if (std::regex_search(moveString, std::regex("[a-h]x?[a-h][1-8]")))
		{
			if (xPos != std::string::npos)
			{
				move.xOld = GetIntegerFile(color, moveString.at(xPos - 1));
				move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
				move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
			}
			else
			{
				move.xOld = GetIntegerFile(color, moveString.at(0));
				move.xNew = GetIntegerFile(color, moveString.at(1));
				move.yNew = GetIntegerRank(color, moveString.at(2));
			}

			conditions.first = move.xOld;
			conditions.second = -1;
		}
		else if (std::regex_search(moveString, std::regex("[1-8]x?[a-h][1-8]")))
		{
			if (xPos != std::string::npos)
			{
				move.yOld = GetIntegerRank(color, moveString.at(xPos - 1));
				move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
				move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
			}
			else
			{
				move.yOld = GetIntegerRank(color, moveString.at(0));
				move.xNew = GetIntegerFile(color, moveString.at(1));
				move.yNew = GetIntegerRank(color, moveString.at(2));
			}

			conditions.first = -1;
			conditions.second = move.yOld;
		}
		else
		{
			if (xPos != std::string::npos)
			{
				move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
				move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
			}
			else
			{
				move.xNew = GetIntegerFile(color, moveString.at(0));
				move.yNew = GetIntegerRank(color, moveString.at(1));
			}

			conditions.first = -1;
			conditions.second = -1;
		}

		if (type == Common::PieceType::KING || type == Common::PieceType::KNIGHT)
		{
			FindPieceInSpots(board, move, color, locs, conditions);
		}
		else
		{
			FindPieceOnLines(board, move, color, locs, conditions);
		}
	}

	return move;
}

int Parser::GetIntegerFile(Common::Color color, char c)
{
	int file = 0;

	if (color == Common::Color::WHITE)
	{
		switch (c)
		{
			case('a'):
				file = 0;
				break;
			case('b'):
				file = 1;
				break;
			case('c'):
				file = 2;
				break;
			case('d'):
				file = 3;
				break;
			case('e'):
				file = 4;
				break;
			case('f'):
				file = 5;
				break;
			case('g'):
				file = 6;
				break;
			case('h'):
				file = 7;
				break;
			default:
				break;
		}
	}
	else
	{
		switch (c)
		{
			case('a'):
				file = 7;
				break;
			case('b'):
				file = 6;
				break;
			case('c'):
				file = 5;
				break;
			case('d'):
				file = 4;
				break;
			case('e'):
				file = 3;
				break;
			case('f'):
				file = 2;
				break;
			case('g'):
				file = 1;
				break;
			case('h'):
				file = 0;
				break;
			default:
				break;
		}
	}

	return file;
}

int Parser::GetIntegerRank(Common::Color color, char c)
{
	int rank = 0;

	if (color == Common::Color::WHITE)
	{
		switch (c)
		{
		case('1'):
			rank = 0;
			break;
		case('2'):
			rank = 1;
			break;
		case('3'):
			rank = 2;
			break;
		case('4'):
			rank = 3;
			break;
		case('5'):
			rank = 4;
			break;
		case('6'):
			rank = 5;
			break;
		case('7'):
			rank = 6;
			break;
		case('8'):
			rank = 7;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (c)
		{
		case('8'):
			rank = 0;
			break;
		case('7'):
			rank = 1;
			break;
		case('6'):
			rank = 2;
			break;
		case('5'):
			rank = 3;
			break;
		case('4'):
			rank = 4;
			break;
		case('3'):
			rank = 5;
			break;
		case('2'):
			rank = 6;
			break;
		case('1'):
			rank = 7;
			break;
		default:
			break;
		}
	}

	return rank;
}