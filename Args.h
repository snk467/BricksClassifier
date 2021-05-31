#pragma once

#include<string>

class Args
{
public:
	static const std::string dataDir;
	static const std::string modelsDir;
	static const std::string outPath;
	static bool calibration;
	static bool verbose;
	static bool showOutput;
private:
	Args(){ } 
};