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
	bool CheckGameStatus(Common::Color & currentColor);
	void QueryPlayerForMove(Player & p, Common::Color color);

private:
	Board m_board;
	Validator m_validator;

	// flag to show the color of the winner
	Common::Color m_winner;

	void TransposeMoveRequest(Common::MoveRequest & move);
	bool CheckIfKingInCheck(Common::Color color);
};

