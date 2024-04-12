#include <stdio.h>
#include <stdarg.h>
#include "./lexer.h"
#include "../../libs/j_transform.h"

#define INPUT_SIZE 32
#define OUTPUT_SIZE 1024
#define RESULT_ADDR 30
#define next_Token token=get_Token(&lexer)
struct Lexer lexer;
struct Token token;
char output[OUTPUT_SIZE];
size_t last_written = 0;

void op_ini();
void op_0(int data);
void op_1(int data);
void op_Get_Number(int data);

int main(int argc, char **argv) 
{
    char *name_file_output = "neander.asm";

    // leitor de argumentos
    FILE *FPTR = 0;
    for (int i = 1; i < argc; i++)
    {
        // arquivo a ser compilado
        if (argv[i][0] != '-')
        {
            FPTR = fopen(argv[i], "r");
        }

        // define saída
        else if (str_match(argv[i], "-o"))
        {
            int arg_size = 1;
            if (argc <= i+arg_size)
            {
                printf("Argumento inválido\n");
                return 1;
            }

            i++;
            name_file_output = argv[i];
        }
    }

    if (FPTR == 0)
    {
        printf("\n NENHUM ARQUIVO SELECIONADO");
        return 0;
    }

    char text[INPUT_SIZE];
    str_clr(text, INPUT_SIZE);
    fgets(text, INPUT_SIZE-1, FPTR);
    fclose(FPTR);

    printf("\n Parsing: \n %s", text);

    lexer = new_Lexer(text);
    printf("\n Tokens identificados:\n { ");
    for(int i = 0; i < MAXTOKENS; i++)
    {
        if (lexer.token[i].type == 0x0) 
            break;
        printf("%s, ", lexer.token[i].string);
    }
    printf("END }\n");

    printf("\n Começando construção de assembly\n");
    str_clr(output, OUTPUT_SIZE);
    op_ini();
    
    FPTR = fopen(name_file_output,"w+");
    fwrite(output, 1, last_written+1, FPTR);
    fclose(FPTR);
}



void append_To_Output(char *format, ...)
{
    va_list args;
    va_start(args, format);
    last_written += vsnprintf(output+last_written, (OUTPUT_SIZE-last_written-2), format, args);
}



void op_ini()
{
    token = get_Token(&lexer);
    if (token.type == 0x0)
    {
        return;
    }
    op_0(0);
    if (token.type != 0x0)
    {   
        printf("\n\n ERRO! -------------------------");
        printf("\n Término inconclusível com token: %s\n\n", token.string);
    }

    else
    {
        append_To_Output("%d <- 0\n", RESULT_ADDR);
        append_To_Output("LDA 0\n");
        append_To_Output("STA %d\n", RESULT_ADDR);
        printf("\n Término correto com tokens tratados:\n Arquivo gerado: \n\n%s",output);
        return;
    }

}



void op_0(int data)
{   
    op_1(data);
    
    if (token.type == TYPE_PLUS)
    {
        next_Token;
        
        op_1(data+1);

        printf(" Add: num[%d] = num[%d] + num[%d]\n", data, data, data+1);
        append_To_Output("LDA %d\n", data);
        append_To_Output("ADD %d\n", data+1);
        append_To_Output("STA %d\n\n", data);
        
    }

    else if (token.type == TYPE_MINUS)
    {
        next_Token;

        op_1(data+2);
        
        append_To_Output("%d <- 1\n", data+1);
        append_To_Output("LDA %d\n", data+2);
        append_To_Output("NOT\n");
        append_To_Output("ADD %d\n", data+1);
        append_To_Output("ADD %d\n", data);
        append_To_Output("STA %d\n\n", data);
    }

    return;
}



void op_1(int data) 
{
    op_2(data);

    if (token.type == TYPE_MULT)
    {
        next_Token;
        op_2(data+3);

        append_To_Output("%d <- 255\n", data+1);
        append_To_Output("%d <- 0\n", data+2);
        append_To_Output("label%d_start: \n", data);
        append_To_Output("LDA %d\n", data);
        append_To_Output("JZ label%d_end \n", data);
        append_To_Output("ADD %d\n", data+1);
        append_To_Output("STA %d\n", data);
        append_To_Output("LDA %d\n", data+2);
        append_To_Output("ADD %d\n", data+3);
        append_To_Output("STA %d\n", data+2);
        append_To_Output("JMP label%d_start \n", data);
        append_To_Output("label%d_end: \n", data);
        append_To_Output("LDA %d\n", data+2);
        append_To_Output("STA %d\n", data);
        
    }

    else if (token.type == TYPE_DIV)
    {
        next_Token;
        op_2(data+1);
    }

    return;
}

void op_2(int data)
{
    op_Get_Number(data);

    if (token.type == TYPE_OPENP)
    {
        next_Token;
        op_0(data);

        if (token.type != TYPE_CLOSEP)
        {
            printf("\n BLOCO NÃO FECHADO \n");
            return;
        }

        next_Token;
        return;
    }
    return;
}

void op_Get_Number(int data)
{
    if (token.type == TYPE_NUMBER)
    {
        int value = ret_uns(token.string);
        next_Token;
        printf(" Num: %d alocado na memória em %d\n", value, data);
        append_To_Output("%d <- %d \n", data, value);

    }
    return;
}