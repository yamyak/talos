#pragma once

#include "Board.h"
#include "Validator.h"
#include "Player.h"

class Game
{
public:
	Game();
	~Game();

	void RegisterPlayers(Player & p1, Player & p2);
	bool CheckGameStatus(Common::Color color);
	void QueryPlayerForMove(Player & p);

private:
	Board m_board;
	Validator m_validator;

	// flag to show the color of the winner
	Common::Color m_winner;

	bool CheckIfKingInCheck(Common::Color color, Common::MiniBoard & board);
	std::pair<int, int> GetKingLocation(Common::Color color, Common::MiniBoard & board);
	std::vector<Common::MoveRequest> FindPotentialMoves(Common::MiniBoard & board, int x, int y);
	std::vector<Common::MoveRequest> FindStraightLineMoves(Common::MiniBoard & board, int x, int y, 
		const std::vector<std::pair<int, int>> & steps, Common::Color & color, Common::PieceType & type);
	std::vector<Common::MoveRequest> FindSpotMoves(Common::MiniBoard & board, int x, int y,
		const std::vector<std::pair<int, int>> & locs, Common::Color & color, Common::PieceType & type);
};

