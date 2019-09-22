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
	Common::MoveRequest ParseMoveStringLocations(Common::Color color, Common::PieceType type, Common::MiniBoard & board,
		std::string moveString, const std::vector<std::pair<int, int>> & locs);

	void FindPieceInSpots(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
		const std::vector<std::pair<int, int>> & locs, std::pair<int, int> conditions);
	void FindPieceOnLines(Common::MiniBoard & board, Common::MoveRequest & move, Common::Color & color,
		const std::vector<std::pair<int, int>> & steps, std::pair<int, int> conditions);

	int GetIntegerFile(Common::Color color, char c);
	int GetIntegerRank(Common::Color color, char c);
};