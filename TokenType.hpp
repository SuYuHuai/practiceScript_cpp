#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <string>
#include <iosfwd>
#include <sstream>

enum TokenTypeE
{
    /*
      Single-character tokens.
      ()  {}  ,  .  -  +  ;  /  *
    */
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    /*
      One or two character tokens.
      !  !=  =  ==  >  >=  <  <=
    */
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    /*
      Literals.
    */
    IDENTIFIER, STRING, NUMBER,

    /*
      Keywords.
    */
    VAR, NIL,
    AND, OR,
    IF, ELSE, FOR, WHILE,
    TRUE, FALSE,
    CLASS, SUPER, THIS,
    FUN, RETURN,  
    PRINT,

    /*
        End of file.
    */
    ENDOF
};

class TokenC
{
private:
    TokenTypeE type;
    std::string lexeme;
    double numLiteral;        // Literal value of Number literals.
    std::string strLiteral;   // Literal value of String literals.
    int line;

public:
    // Constructor for number literals.
    TokenC(TokenTypeE type, std::string lexeme, double literal, int line)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->numLiteral = literal;
        this->line = line;
    }

    // Constructor fot string litetrals.
    TokenC(TokenTypeE type, std::string lexeme, std::string literal, int line)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->strLiteral = literal;
        this->line = line;
    }

    std::string toString()
    {
        std::stringstream buffer;

        if (type == NUMBER)
        {
            buffer << type << " " << lexeme << " " << numLiteral;
        }
        else
        {
            buffer << type << " " << lexeme << " " << strLiteral;
        }

        std::string str = buffer.str();

        return str;
    }
};

#endif  // TOKEN_TYPE_HPP