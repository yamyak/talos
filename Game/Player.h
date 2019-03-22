#pragma once

#include <vector>
#include "Common.h"
#include "Algorithm.h"

class Player
{
public:
	Player(Algorithm * a);
	~Player();

	void SetColor(Common::Color color);
	Common::Color GetColor();
	Common::MoveRequest MakeMove(Common::MiniBoard board);

private:
	Common::Color m_color;
	Algorithm * m_alg;
};

