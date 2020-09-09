#include "Start.h"

StartC::StartC()
{
    hadError = false;
}

/*
  All the start of the YuHuai script.
  Begin to scan a file or enter prompt mode.
*/
void StartC::start(int argc, char* args)
{
    if (argc > 1)
    {
        // More than 1 argument, print the guide of usage.
        std::cout << "Usage: YuHuai [sript]" << std::endl;
    }
    else if (argc == 1)
    {
        /*
          When you run YuHuai from the command line and
          give it a path to a file, it reads the file
          and executes it.
        */
        std::cout << "Script File path: " << args << std::endl;

        std::string srcFilePath(args);
        runFile(srcFilePath);
    }
    else
    {
        runPrompt();
    }
}

/*
  Read the script source file and execute it
  according to the file path.
*/
void StartC::runFile(const std::string filePath)
{
    std::ifstream inFile;
    std::stringstream buffer;
    std::string srcCode;

    /*
      Read all the source code from the script file
      according to the filePath.
    */
    inFile.open(filePath.c_str(), std::ios::in);
    buffer << inFile.rdbuf();

    srcCode = buffer.str();
    inFile.close();

    // Run the source code.
    run(srcCode);

    if (hadError)
    {
        std::cerr << "Error happend after run source code.";
        exit(65);
    }
}

/*
    When you fire up YuHuai without any arguments,
    it will drop you into a prompt where you can
    enter and execute code one line at a time.
*/
void StartC::runPrompt()
{
    std::string srcCode;

    for ( ; ;)
    {
        std::cout << "> ";
        std::getline(std::cin, srcCode);

        // EOF (ctrl+D) is entered.
        if (srcCode.empty())
        {
            std::cout << "Exit: Ctrl + D is entered." << std::endl;
            break;
        }
        else
        {
            // Run the source code.
            run(srcCode);

            /*
              We need to reset this flag in the interactive
              loop. If the user makes a mistake, it shouldn't
              kill their entire session.
            */
            hadError = false;
        }
    }
}

/*
  Run the source code.
*/
void StartC::run(std::string srcCode)
{
/*     std::istringstream inString(srcCode);
    std::string token;

    while (!inString.eof())
    {
        inString >> token;
        std::cout << token << std::endl;
    } */

    ScannerC scanner(srcCode);
    std::list<TokenC> tokens = scanner.scanAllTokens();

    for (auto i = tokens.begin(); i != tokens.end(); ++i)
    {
        std::cout << (*i).toString() << std::endl;
    }
}

/*
  This tells users some syntax error occurred on
  a given line.
*/
void StartC::error(int line, std::string message)
{
    std::string where;
    report(line, where, message);
}

void StartC::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << "] Error"
              << where << ": " << message << std::endl;
    hadError = true;
}