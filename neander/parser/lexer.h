#include <stdint.h>
#include <stdbool.h>
#include "../../libs/aloca.h"
#include "../../libs/j_strings.h"

#define MAXTOKENS 32


/// símbolos terminais da gramática
/// 0x00 None
/// 0x01 Number
/// 0x02 Menos -
/// 0x03 Mais  +
/// 0x04 Vezes *
/// 0x05 Div   /
/// 0x06 Parentesis1 (
/// 0x07 Parentesis2 )
#define TYPE_NUMBER 0x01
#define TYPE_MINUS  0x02
#define TYPE_PLUS   0x03
#define TYPE_MULT   0x04
#define TYPE_DIV    0x05
#define TYPE_OPENP  0x06
#define TYPE_CLOSEP 0x07

struct Token
{
    char *string;
    uint8_t type;
};



struct Lexer 
{
    struct Token token[MAXTOKENS];
    int index;
};



bool is_Digit(char symbol) 
{
    if (symbol >= '0' && symbol <= '9')
    {
        return true;
    }

    return false;
}



struct Lexer new_Lexer(char *input) 
{
    
    struct Lexer ret;
    ret.index = 0;

    // limpa
    for (int i = 0; i < MAXTOKENS; i++)
    {
        ret.token[i].string = 0x0;
        ret.token[i].type   = 0x0;
    }

    // preenche j token
    for (int i = 0, token_index = 0; input[i] != 0;)
    {
        if (input[i] == ' ' || input[i] == '\n')
        {
            i += 1;
        }

        else if (is_Digit(input[i]))
        {
            int digit_index = 1;
            int start_index = i;
            i += 1;

            while (is_Digit(input[i]))
            {
                digit_index += 1;
                i += 1;
            }

            ret.token[token_index].type = 0x01; 
            ret.token[token_index].string = str_crop(start_index, start_index+digit_index, input);
            token_index += 1;
        }

        else if (input[i] == '-')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x02; // usar defines para facilitar?
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;
        }
        
        else if (input[i] == '+')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x03;
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;            
        }
        
        else if (input[i] == '*')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x04;
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;
        }
        
        else if (input[i] == '/')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x05;
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;            
        }
        
        else if (input[i] == '(')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x06;
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;            
        }
        
        else if (input[i] == ')')
        {
            int digit_index = 1;
            ret.token[token_index].type = 0x07;
            ret.token[token_index].string = str_crop(i, i+digit_index, input);
            token_index += 1;
            i +=1;            
        }
    }

    return ret;
}



struct Token get_Token( struct Lexer *lexer)
{
    if (lexer->index >= MAXTOKENS || (lexer->token[lexer->index].type == 0x0))
    {
        struct Token ret;
        ret.type = 0x0;
        ret.string = 0;
        return ret;
    }

    lexer->index += 1;
    return lexer->token[lexer->index-1];
}



void free_Lexer( struct Lexer lexer)
{
    // limpa
    for (int i = 0; i < MAXTOKENS; i++)
    {
        if ( lexer.token[i].string != 0x0 )
        {
            libera(lexer.token[i].string);
            lexer.token[i].string = 0x0;
        }

        lexer.token[i].type = 0x0;
    }
}