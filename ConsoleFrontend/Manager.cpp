#include "pch.h"

#include "Manager.h"
#include "Display.h"
#include "Game.h"

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::Start()
{
	Display display;

	Game game;
	Common::MiniBoard board;

	game.Initialize(board);
}
