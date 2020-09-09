#ifndef SCANNER_H
#define SCANNER_H

#include "TokenType.hpp"
#include <iostream>
#include <string>
#include <list>
#include <map>

/*
  Module Name: ScannerC
  Function Description:
    The scanner works its way through the source code,
    adding tokens, until it runs out of characters.
    When it's done, it appends one final "end of file"
    token.
*/

class ScannerC
{
private:
    std::string sourceCode;

    // All the tokens generated from source code.
    std::list<TokenC> tokens;

    /*
      The three fields help to keep track of where
      in the source code we are.

      The "start" and "current" fields are offsets in the string
      -- the first character in the crrent lexeme being scanned,
      and the character we're currently considering.

      The "line" field tracks what source code line "current" is
      on, so we can produce tokens that know their location.
    */
    int start;
    int current;
    int line;
    static std::map<std::string, TokenTypeE> keyWords;

public:
    // Store the source code.
    ScannerC(std::string sourceCode);

    std::list<TokenC> scanAllTokens();

private:
    void scanOneToken();
    bool isAtEnd();
    char advance();
    void addToken(TokenTypeE type);
    void addToken(TokenTypeE type, double literal);
    void addToken(TokenTypeE type, std::string literal);
    void string();
    bool isDigit(char ch);
    void number();
    bool isAlpha(char ch);
    bool isAlphaOrNumeric(char ch);
    void identifier();
    bool nextMatch(char expected);
    char peek();
    char peekNext();
};

#endif  // SCANNER_H