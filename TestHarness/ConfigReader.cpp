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

// read in the config file
void ConfigReader::ReadConfig(std::string & configFile)
{
	// open the config file
	std::ifstream myFile;
	myFile.open(configFile, std::ios::in);

	if (myFile.is_open())
	{
		ConfigState state = NONE;
		std::string basePath;

		// iterate through file reading line by line
		std::string line;
		while (std::getline(myFile, line))
		{
			// skip empty lines
			if (line.empty())
			{
				continue;
			}

			// if line contents is a state flag, 
			// set current state of reader and skip to next line
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

			// based on current state, parse line
			switch (state)
			{
				// set the base path of input script files
				case BASE_PATH:
				{
					basePath = line;
					break;
				}
				// add new input script file to list of input files
				case SCRIPTS:
				{
					// concatenate filename with base path
					m_fileNames.push_back(basePath + "\\" + line);
					break;
				}
				// don't have to record ignore script files
				// only in config file so there's a place for them
				case IGNORE:
				{
					break;
				}
				default:
					break;
			}
		}

		// close the files
		myFile.close();
	}
}

// return the list of input script paths
std::vector<std::string> & ConfigReader::GetFilePaths()
{
	return m_fileNames;
}