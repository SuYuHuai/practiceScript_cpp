#ifndef YU_HUAI_START_H
#define YU_HUAI_START_H

/*
  YuHuai is a scripting language, which means
  it executes directly from source code.
*/

#include "Scanner.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class StartC
{
public:
    StartC();
    void start(int argc, char* args);

private:
    void runFile(const std::string filePath);
    void runPrompt();
    void run(std::string srcCode);

    /*
      We'll use this to ensure we don't try to execute
      code that has a known error.
    */
    bool hadError;
    void error(int line, std::string message);
    void report(int line, std::string where, std::string message);
};

#endif  // YU_HUAI_START_H