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

	// parse queen side castling
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
	// parse king side castling
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
	// parse king, queen, rook, bishop, knight, or pawn move
	else if(std::regex_search(moveString, std::regex("^[KQRBN]?[a-h]?[1-8]?x?[a-h][1-8]")))
	{
		// check if first character is piece symbol
		switch (moveString.at(0))
		{
			case('K'):
			{
				// create list of all possible king moves
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				// find move source and destination locations
				move = ParseMoveStringLocations(color, Common::PieceType::KING, board, moveString.substr(1), locs);
				break;
			}
			case('Q'):
			{
				// create list of all possible queen move directions
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				// find move source and destination locations
				move = ParseMoveStringLocations(color, Common::PieceType::QUEEN, board, moveString.substr(1), locs);
				break;
			}
			case('R'):
			{
				// create list of all possible rook move directions
				const std::vector<std::pair<int, int>> locs = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
				// find move source and destination locations
				move = ParseMoveStringLocations(color, Common::PieceType::ROOK, board, moveString.substr(1), locs);
				break;
			}
			case('B'):
			{
				// create list of all possible bishop move directions
				const std::vector<std::pair<int, int>> locs = { {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
				// find move source and destination locations
				move = ParseMoveStringLocations(color, Common::PieceType::BISHOP, board, moveString.substr(1), locs);
				break;
			}
			case('N'):
			{
				// create list of all possible knights moves
				const std::vector<std::pair<int, int>> locs = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };
				// find move source and destination locations
				move = ParseMoveStringLocations(color, Common::PieceType::KNIGHT, board, moveString.substr(1), locs);
				break;
			}
			default:
			{
				// find move source and destination locations
				move = ParsePawnMove(color, board, moveString);
				break;
			}
		}
	}

	return move;
}

// find source location that king or knight moved from
void Parser::FindPieceInSpots(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
	const std::vector<std::pair<int, int>> & locs, std::pair<int, int> conditions)
{
	// iterate through all possible backwards moves piece can make
	for (const std::pair<int, int> & loc : locs)
	{
		// find source of piece before current move
		int xLoc = move.xNew + loc.first;
		int yLoc = move.yNew + loc.second;

		// check that source location is valid
		if (Common::CheckIfOnBoard(xLoc, yLoc) &&
			board.data[xLoc][yLoc].occupied &&
			board.data[xLoc][yLoc].color == color &&
			board.data[xLoc][yLoc].type == move.type)
		{
			// check that this source location meets the conditions set
			if ((conditions.first == -1 && conditions.second == -1) ||
				(conditions.first == -1 && conditions.second == yLoc) ||
				(conditions.second == -1 && conditions.first == xLoc))
			{
				// update move source locations
				move.xOld = xLoc;
				move.yOld = yLoc;
				break;
			}
		}
	}
}

// find source location that queen, rook, or bishop moved from
void Parser::FindPieceOnLines(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
	const std::vector<std::pair<int, int>> & steps, std::pair<int, int> conditions)
{
	bool found = false;

	// iterate through all possible directions piece could have come from
	for (const std::pair<int, int> & step : steps)
	{
		// start at destination
		int xLoc = move.xNew;
		int yLoc = move.yNew;

		bool blocked = false;

		// while still a path backwards
		while (!blocked && !found)
		{
			// step back
			xLoc += step.first;
			yLoc += step.second;

			// check that location still on board
			if (Common::CheckIfOnBoard(xLoc, yLoc))
			{
				// if location is occupied, check piece
				if (board.data[xLoc][yLoc].occupied)
				{
					// check if location piece matches current move piece
					if (board.data[xLoc][yLoc].color == color &&
						board.data[xLoc][yLoc].type == move.type)
					{
						// check that this source location meets the conditions set
						if ((conditions.first == -1 && conditions.second == -1) ||
							(conditions.first == -1 && conditions.second == yLoc) ||
							(conditions.second == -1 && conditions.first == xLoc))
						{
							// update move source locations
							move.xOld = xLoc;
							move.yOld = yLoc;
							found = true;
						}
					}
					// piece doesn't match, this path can't followed any further
					else
					{
						blocked = true;
					}
				}

				// if location not occupied, continue to next on path
			}
			// off the board, this path can't followed any further
			else
			{
				blocked = true;
			}
		}

		// if source location found, done
		if (found)
		{
			break;
		}
	}
}

// fill out pawn move request source and destinations values based off of input string
Common::MoveRequest Parser::ParsePawnMove(Common::Color color, Common::MiniBoard & board, std::string moveString)
{
	Common::MoveRequest move;
	move.type = Common::PieceType::PAWN;

	// check if move string has an "x"
	size_t xPos = moveString.find('x');

	// set move locations based on whether move string has an "x"
	if (xPos != std::string::npos)
	{
		// fill out destination location and source column
		move.xNew = GetIntegerFile(color, moveString.at(xPos + 1));
		move.yNew = GetIntegerRank(color, moveString.at(xPos + 2));
		move.xOld = GetIntegerFile(color, moveString.at(xPos - 1));

		// search up to 1 below destination row for a valid pawn piece
		move.yOld = -1;
		for (int j = move.yNew - 1; j >= 0; j--)
		{
			if (board.data[move.xOld][j].occupied && 
				board.data[move.xOld][j].color == color &&
				board.data[move.xOld][j].type == Common::PieceType::PAWN)
			{
				// once found, use it's row value as the source row
				move.yOld = j;
				break;
			}
		}
		
		// move position forward to where pawn promotion character might be
		xPos += 3;
	}
	else
	{
		// fill out destination location and source column
		move.xNew = GetIntegerFile(color, moveString.at(0));
		move.yNew = GetIntegerRank(color, moveString.at(1));
		move.xOld = move.xNew;

		// search up to 1 below destination row for a valid pawn piece
		move.yOld = -1;
		for (int j = move.yNew - 1; j >= 0; j--)
		{
			if (board.data[move.xOld][j].occupied &&
				board.data[move.xOld][j].color == color &&
				board.data[move.xOld][j].type == Common::PieceType::PAWN)
			{
				// once found, use it's row value as the source row
				move.yOld = j;
				break;
			}
		}

		// move position to where pawn promotion character might be
		xPos = 2;
	}

	// check if pawn promotion character exists in move string
	if (std::regex_search(moveString, std::regex("[QRBN]")))
	{
		// if "=" character exists, move position up 1 character
		if (moveString.find('=') != std::string::npos)
		{
			xPos++;
		}

		// depending on character at pawn promotion character location
		// set the pawn promotion piece type
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

// fill out move request source and destinations values based off of input string
Common::MoveRequest Parser::ParseMoveStringLocations(Common::Color color, Common::PieceType type, Common::MiniBoard & board, 
	std::string moveString, const std::vector<std::pair<int, int>> & locs)
{
	Common::MoveRequest move;
	move.type = type;
	move.xOld = -1;
	move.yOld = -1;

	// check if move string has an "x"
	size_t xPos = moveString.find('x');

	// if move string has both row and column characters for source and destination
	if (std::regex_search(moveString, std::regex("[a-h][1-8]x?[a-h][1-8]")))
	{
		// set move locations based on whether move string has an "x"
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

		// if move string only has a column character for the source location
		if (std::regex_search(moveString, std::regex("[a-h]x?[a-h][1-8]")))
		{
			// set move locations based on whether move string has an "x"
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

			// set conditions to show that source column know but source row not known
			conditions.first = move.xOld;
			conditions.second = -1;
		}
		// if move string only has a row character for the source location
		else if (std::regex_search(moveString, std::regex("[1-8]x?[a-h][1-8]")))
		{
			// set move locations based on whether move string has an "x"
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

			// set conditions to show that source row know but source column not known
			conditions.first = -1;
			conditions.second = move.yOld;
		}
		// if move string does not have any source information
		else
		{
			// set move locations based on whether move string has an "x"
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

			// set conditions to show that no source information know
			conditions.first = -1;
			conditions.second = -1;
		}

		if (type == Common::PieceType::KING || type == Common::PieceType::KNIGHT)
		{
			// based off of conditions, find source location data for pieces that jump to locations
			FindPieceInSpots(board, move, color, locs, conditions);
		}
		else
		{
			// based off of conditions, find source location data for pieces that slide to locations
			FindPieceOnLines(board, move, color, locs, conditions);
		}
	}

	return move;
}

// get the x coordinate of move based of file character and current side
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

// get the y coordinate of move based of rank character and current side
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