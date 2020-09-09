#include "Scanner.h"

    ScannerC::ScannerC(std::string sourceCode)
    {
        this->sourceCode = sourceCode;
        start = 0;
        current = 0;
        line = 1;
    }

    /*
      Scan source code and store all the tokens.
    */
    std::list<TokenC> ScannerC::scanAllTokens()
    {
        while (!isAtEnd())
        {
            /*
              We are at the beginning of the next lexeme.
              Each turn of the loop, we scan a sigle token.
            */
            start = current;
            scanOneToken();
        }

        tokens.push_back(TokenC(TokenTypeE::ENDOF, "", 0/*TODO*/, line));

        return tokens;
    }

    // Maximal Munch rule.
    // It sancs it that way even though doing so leads to
    // a syntax error later in the parser.
    /*
      Generate one token a time.
    */
    void ScannerC::scanOneToken()
    {
        // Consume next character.
        char ch = advance();

        switch(ch)
        {
            // Sigle-character tokens.
            case '(':
                addToken(LEFT_PAREN);
                break;

            case ')':
                addToken(RIGHT_PAREN);
                break;

            case '{':
                addToken(LEFT_BRACE);
                break;

            case '}':
                addToken(RIGHT_BRACE);
                break;

            case ',':
                addToken(COMMA);
                break;

            case '.':
                addToken(DOT);
                break;

            case '-':
                addToken(MINUS);
                break;

            case '+':
                addToken(PLUS);
                break;

            case ';':
                addToken(SEMICOLON);
                break;

            case '*':
                addToken(STAR);
                break;
            
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespaces.
                break;

            case '\n':
                line++;
                break;

            // One or two character tokens.
            case '!':
                addToken(nextMatch('=') ? BANG_EQUAL : BANG);
                break;
            
            case '=':
                addToken(nextMatch('=') ? EQUAL_EQUAL : EQUAL);
                break;
            
            case '<':
                addToken(nextMatch('=') ? LESS_EQUAL : LESS);
                break;

            case '>':
                addToken(nextMatch('=') ? GREATER_EQUAL : GREATER);
                break;
            
            case '/':
                if (nextMatch('/'))
                {
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd())
                    {
                        advance();
                    }
                }
                else
                {
                    // Just a slash.
                    addToken(SLASH);
                }
                break;
            
            // String literals.
            case '"':
                string();
                break;

            /*
              Numeric token, identifier token, or some
              characters dont't use in our script.
            */
            default:
                if (isDigit(ch))
                {
                    number();
                }
                else if (isAlpha(ch))
                {
                    /*
                      We assume any lexeme starting with a letter
                      or underscore (_) is an identifier.
                    */
                    identifier();
                }
                else
                {
                    /*
                      Some charcters don't use in our script,
                      like @#^. Just discard them silently.
                    */
                    std::cerr << "Unexpected character.";
                    //PanGu.error(line, "Unexpected character.");
                    std::cerr << "Line " << line
                              << "Unkonw character " << ch << std::endl;
                
                }
                break;
        }
    }

    bool ScannerC::isAtEnd()
    {
        return (current >= sourceCode.length());
    }

    /*
      This method consumes the next character in the
      source file and returns it.
    */
    char ScannerC::advance()
    {
        current++;

        return sourceCode[current - 1];
    }

    /*
      Add non-literal tokens.
    */
    void ScannerC::addToken(TokenTypeE type)
    {
        addToken(type, 0/*TODO*/);
    }

    /*
      Number literal.
      "addToken" grabs the text of the current lexeme
      and creates a new token for it.
    */
    void ScannerC::addToken(TokenTypeE type, double literal)
    {
        // Extract the text value of the lexeme.
        std::string text = sourceCode.substr(start, current - start);

        tokens.push_back(TokenC(type, text, literal, line));

        return;
    }

    /*
      String literal.
      "addToken" grabs the text of the current lexeme
      and creates a new token for it.
    */
    void ScannerC::addToken(TokenTypeE type, std::string literal)
    {
        // Extract the text value of the lexeme.
        std::string text = sourceCode.substr(start, current - start);

        tokens.push_back(TokenC(type, text, literal, line));

        return;
    }

    /*
      Add a string literal token.
    */
    void ScannerC::string()
    {
        while (peek() != '"' && isAtEnd())
        {
            if (peek() == '\n')
                line++;

            advance();
        }

        // Unterminated string.
        if (isAtEnd())
        {
            std::cerr << "Unterminated string";
            //PanGu.error(line, "Unterminated string");
            return;
        }

        // The closing " is detected.
        advance();

        // Trim the surrounding quotes.
        std::string value = sourceCode.substr(start+1, current - start - 2);
        addToken(STRING, value);
    }

    /*
      Test the character is a digit or not.
    */
    bool ScannerC::isDigit(char ch)
    {
        return (ch >= '0' && ch <= '9');
    }

    /*
      Add a numberic literal token.
    */
    void ScannerC::number()
    {
        while (isDigit(peek()))
            advance();

        // Look for a fractional part.
        if (peek() == '.' && isDigit(peekNext()))
        {
            // Consume the ".".
            advance();

            while (isDigit(peek()))
                advance();
        }

        std::string numberString = sourceCode.substr(start, current - start);

        addToken(NUMBER, std::stod(numberString.c_str()));
    }

    /*
      Test the character is a letter or not.
    */
    bool ScannerC::isAlpha(char ch)
    {
        return (ch >= 'a' && ch <= 'z') ||
               (ch >= 'A' && ch <= 'Z') ||
               (ch == '_');
    }

    /*
      Test the character is alpha or numeric or not.
    */
    bool ScannerC::isAlphaOrNumeric(char ch)
    {
        return isAlpha(ch) || isDigit(ch);
    }

    /*
      Add an identifier token.
    */
    void ScannerC::identifier()
    {
        while (isAlphaOrNumeric(peek()))
            advance();

        // See if the identifier is a reserved word.
        std::string text = sourceCode.substr(start, current - start);
        std::map<std::string, TokenTypeE>::iterator pos = keyWords.find(text);

        // Default we think this is a user-defined identifier.
        TokenTypeE type = IDENTIFIER;

        if (pos != keyWords.end())
            type = pos->second;     // This identifier is a keyword.

        addToken(type);
    }

    /*
      Test the next character.
    */
    bool ScannerC::nextMatch(char expected)
    {
        if (isAtEnd())
            return false;
        
        if (sourceCode[current] != expected)
        {
            return false;
        }

        /*
          Only consume the current character if
          it's what we're looking for.
        */
        current++;

        return true;
    }

    /*
      Return the current unconsumed character, but dose
      not consume it.
    */
    char ScannerC::peek()
    {
        if (isAtEnd())
            return '\0';
        
        return sourceCode[current];
    }

    /*
      Retunr the character behind "currnet".
    */
    char ScannerC::peekNext()
    {
        if (current + 1 >= sourceCode.length())
            return '\0';
        
        return sourceCode[current + 1];
    }

std::map<std::string, TokenTypeE> ScannerC::keyWords = {
       {"and",    AND},
       {"class",  CLASS},
       {"else",   ELSE},
       {"false",  FALSE},
       {"for",    FOR},
       {"fun",    FUN},
       {"if",     IF},
       {"nil",    NIL},
       {"or",     OR},
       {"print",  PRINT},
       {"return", RETURN},
       {"super",  SUPER},
       {"this",   THIS},
       {"true",   TRUE},
       {"var",    VAR},
       {"while",  WHILE}
};