#pragma once

#include <string>
#include <vector>

#include "Common.h"

class ConfigReader
{
public:
	ConfigReader(std::string & configFile);
	~ConfigReader();

	std::vector<std::string> & GetFilePaths();

private:
	std::vector<std::string> m_fileNames;

	enum ConfigState
	{
		NONE,
		BASE_PATH,
		SCRIPTS,
		IGNORE
	};

	void ReadConfig(std::string & configFile);
};