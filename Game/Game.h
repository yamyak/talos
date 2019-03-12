#pragma once

#include <set>
#include "Board.h"
#include "Player.h"

class Game
{
public:
	Game();
	~Game();

	void RegisterPlayers(Player & p1, Player & p2);
	bool CheckGameStatus(Common::Color & currentColor);
	void QueryPlayerForMove(Player & p, Common::Color color);

private:
	Board m_board;

	// flag to show the color of the winner
	Common::Color m_winner;

	void TransposeLocations(std::vector<Common::PieceInfo> & whiteLocs, std::vector<Common::PieceInfo> & blackLocs);
	void TransposeMoveRequest(Common::MoveRequest & move);
	bool CheckMoveRequest(Common::MoveRequest & move, Common::Color color);
	bool CheckMovePath(Piece * piece, Common::Location target);
	bool CheckStraightPathForAggressors(Common::Location start, int xStep, int yStep, Common::Color color, std::set<Common::PieceType> types);
	bool CheckPathForObstacles(Piece * piece, Common::Location target);
	bool CheckIfKingInCheck(Common::Color color);
};

