// TestHarness.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Common.h"
#include "Game.h"
#include "Parser.h"
#include "ConfigReader.h"
#include "ScriptReader.h"

int main(int argc, char** argv)
{
	std::string cmdArgs;

	if (argc > 1)
	{
		return -1;
	}
	else if (argc == 1)
	{
		cmdArgs = argv[0];
	}

	ConfigReader config(cmdArgs);

	for (std::string & file : config.GetFilePaths())
	{
		ScriptReader script(file);
	}

	return 0;
}
