#pragma once

#include "Common.h"

class Manager
{
public:
	Manager();
	~Manager();

	void Start();

private:
	void TransposeBoard(Common::MiniBoard & board);
};

