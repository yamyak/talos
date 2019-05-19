#pragma once

#include "Game.h"

using namespace System;

namespace BackendInterface {
	public ref class BackendWrapper
	{
	public:
		BackendWrapper();

		bool InitializeWrapper();
		bool AttemptMoveWrapper();
		bool CheckGameStatusWrapper();

	private:
		Game * m_game;
	};
}
