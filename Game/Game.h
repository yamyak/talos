#pragma once

#ifdef BACKEND_EXPORTS
#define BACKEND_API __declspec(dllexport)
#else
#define BACKEND_API __declspec(dllimport)
#endif

#include "Validator.h"

#include <vector>

class Game
{
public:
	BACKEND_API Game();
	BACKEND_API ~Game();

	BACKEND_API bool Initialize(Common::MiniBoard & board);
	BACKEND_API bool AttemptMove(Common::Color & color, Common::MoveRequest & move, Common::MiniBoard & board);
	BACKEND_API bool CheckGameStatus(Common::Color & color, Common::MiniBoard & board);

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