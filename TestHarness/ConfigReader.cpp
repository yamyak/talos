#include "pch.h"

#include <iostream>
#include <fstream>

#include "ConfigReader.h"

ConfigReader::ConfigReader(std::string & configFile)
{
	ReadConfig(configFile);
}

ConfigReader::~ConfigReader()
{
}

void ConfigReader::ReadConfig(std::string & configFile)
{
	std::ifstream myFile;
	myFile.open(configFile, std::ios::in);

	if (myFile.is_open())
	{
		ConfigState state = NONE;
		std::string basePath;

		std::string line;
		while (std::getline(myFile, line))
		{
			if (line.empty())
			{
				continue;
			}

			if (line.compare("[BASE_PATH]") == 0)
			{
				state = BASE_PATH;
				continue;
			}
			else if (line.compare("[SCRIPTS]") == 0)
			{
				state = SCRIPTS;
				continue;
			}
			else if (line.compare("[IGNORE]") == 0)
			{
				state = IGNORE;
				continue;
			}

			switch (state)
			{
				case BASE_PATH:
				{
					basePath = line;
					break;
				}
				case SCRIPTS:
				{
					m_fileNames.push_back(basePath + "\\" + line);
					break;
				}
				case IGNORE:
				{
					break;
				}
				default:
					break;
			}
		}

		myFile.close();
	}
}

std::vector<std::string> & ConfigReader::GetFilePaths()
{
	return m_fileNames;
}