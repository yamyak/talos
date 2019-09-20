#pragma once

#ifdef PARSER_EXPORTS
#define PARSER_API __declspec(dllexport)
#else
#define PARSER_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

#include "Common.h"

class Parser
{
public:
	PARSER_API Parser();
	PARSER_API ~Parser();

	PARSER_API Common::MoveRequest ParseMove(Common::Color color, Common::MiniBoard & board, std::string & moveString);

private:
	Common::MoveRequest ParsePawnMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	Common::MoveRequest ParseKingMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	Common::MoveRequest ParseQueenMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	Common::MoveRequest ParseRookMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	Common::MoveRequest ParseBishopMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	Common::MoveRequest ParseKnightMove(Common::Color color, Common::MiniBoard & board, std::string moveString);
	
	int GetIntegerFile(Common::Color color, char c);
	int GetIntegerRank(Common::Color color, char c);
	void FindPieceInSpots(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
		const std::vector<std::pair<int, int>> & locs, std::pair<int, int> conditions);

};