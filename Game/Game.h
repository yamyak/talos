#pragma once

#include "Validator.h"

#include <vector>

class Game
{
public:
	Game();
	~Game();

	bool Initialize(Common::MiniBoard & board);
	bool AttemptMove(Common::Color & color, Common::MoveRequest & move, Common::MiniBoard & board);
	bool CheckGameStatus(Common::Color & color, Common::MiniBoard & board);

private:
	Validator m_validator;

	bool CheckIfKingInCheck(Common::Color color, Common::MiniBoard & board);
	std::pair<int, int> GetKingLocation(Common::Color color, Common::MiniBoard & board);
	std::vector<Common::MoveRequest> FindPotentialMoves(Common::MiniBoard & board, int x, int y);
	std::vector<Common::MoveRequest> FindStraightLineMoves(Common::MiniBoard & board, int x, int y, 
	const std::vector<std::pair<int, int>> & steps, Common::Color & color, Common::PieceType & type);
	std::vector<Common::MoveRequest> FindSpotMoves(Common::MiniBoard & board, int x, int y,
	const std::vector<std::pair<int, int>> & locs, Common::Color & color, Common::PieceType & type);
	void ApplyMove(Common::MoveRequest & move, Common::MiniBoard & board);
};

