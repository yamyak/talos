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
};

