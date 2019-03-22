#include "stdafx.h"
#include "Player.h"

Player::Player(Algorithm * a)
{
	m_alg = a;
}

Player::~Player()
{
}

void Player::SetColor(Common::Color color)
{
	m_color = color;
}

Common::Color Player::GetColor()
{
	return m_color;
}

Common::MoveRequest Player::MakeMove(Common::MiniBoard board)
{
	return m_alg->CalculateMove(board);
}