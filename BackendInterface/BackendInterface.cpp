#include "stdafx.h"
#include <iostream>
#include "Game.h"

#include "BackendInterface.h"

BackendInterface::BackendWrapper::BackendWrapper()
{
	m_game = new Game();
}

bool BackendInterface::BackendWrapper::InitializeWrapper()
{
	return true;
}

bool BackendInterface::BackendWrapper::AttemptMoveWrapper()
{
	return true;
}

bool BackendInterface::BackendWrapper::CheckGameStatusWrapper()
{
	return true;
}

int main(array<System::String ^> ^args)
{
	BackendInterface::BackendWrapper wrapper;

	bool stat1 = wrapper.InitializeWrapper();

	bool stat2 = wrapper.AttemptMoveWrapper();

	bool stat3 = wrapper.CheckGameStatusWrapper();

	System::Console::WriteLine(L"Managed Code");
	std::cout << "Native Code" << std::endl;
	return 0;
}
