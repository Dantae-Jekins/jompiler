// gcc neander_compiler.c -o neander_compiler.out && ./neander_compiler.out neander_asm.asm

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/j_strings.h"
#include "../libs/j_transform.h"
#include "../libs/j_linkedList.h"

#define maxchar 16

int main(int argc, char **argv)
{
    // leitor de argumentos
    FILE *FPTR = 0;
     for (int i = 1; i < argc; i++)
    {
        // arquivo a ser compilado
        if (argv[i][0] != '-')
        {
            FPTR = fopen(argv[i], "r");
        }
    }

    if (FPTR == 0)
    {

        printf("\n NENHUM ARQUIVO SELECIONADO");
        return 0;
    }



    // variáveis para compilação em binário
    unsigned char MEM[256]; 
    unsigned char DATA_MEM[128];
    str_clr(DATA_MEM, 128);
    str_clr(MEM, 256);
    int index_mem_instruction = 0;
    int index_mem_highest_data = 0;
    list list_label = list_new();
    list list_placeholder = list_new();



    // leitura das instruções e labels
    char ch = getc(FPTR);
    char ln[maxchar];



    // variáveis para leitura de texto
    int index_ch = 0;
    int index_ln = 0;
    int stop = false;
    // Lê o arquivo
    while (ch != EOF && stop == false)
    {
        str_clr(ln, maxchar);
        printf("\n l:%03d:", index_ln);


        // Lê a linha pulando quebras
        // armazena o conteúdo em ln, aceitando no máximo maxchar caracteres
        if (ch == '\n')
            ch = getc(FPTR);

        while ((ch != '\n') && (ch != EOF))
        {
            if (index_ch >= maxchar)
            {
                stop = true;
                break;
            }

            ln[index_ch] = ch;
            ch = getc(FPTR);
            index_ch++;
        }

        if ((index_ch > maxchar) || stop)
        {
            printf(" ABORTANDO PROGRAMA: MAIS DE %d CARACTERES", maxchar);
            stop = true;
            break;
        }



        // split da palavra por espaços e trata word por word
        // trata palavra como instrução ou endereço.

        char **word = str_split(ln, ' ');
        int word_quantity;
        for (word_quantity = 0; word[word_quantity] != 0; word_quantity++)
        {
            printf(" %s",word[word_quantity]);
        }
            
        if (word_quantity > 3)
        {
            printf(" ABORTANDO PROGRAMA: MAIS DE 3 PALAVRAS");
            stop = true;
            break;
        }

        
        
        // constrói o binário parcialmente, 
        if (word_quantity == 1)
        {

            int word_len = str_len(word[0]);

            // FUNÇÃO NOT
            if (str_match(word[0], "NOT"))
            {
                printf(" \t -> 96");
                MEM[index_mem_instruction] = 96;
            }

            // FUNÇÃO HLT
            else if (str_match(word[0], "HLT"))
            {
                printf(" \t -> 240");
                MEM[index_mem_instruction] = 240;
            }

            // NOP
            else if (str_match(word[0], "NOP"))
            {
                printf(" \t -> 00");
                MEM[index_mem_instruction] = 0;
            }

            // LABEL
            else if (word_len > 1 && word[0][word_len - 1] == ':')
            {
                // TODO, LISTA DE LABELS, ADICIONAR ESSE ENDEREÇO A LABEL['word[0]']

                list_addFirst( &list_label, item_new(index_mem_instruction, str_crop(0, word_len-1, word[0])));
                MEM[index_mem_instruction] = 0;
                printf(" \t -> %d", index_mem_instruction);
            };
            index_mem_instruction += 1;
        }

        else if (word_quantity == 2)
        {
            
            // FUNÇÃO STA INDEX
            if (str_match(word[0], "STA"))
            {
                printf(" \t -> 16 DATA[%s]", word[1]);
                MEM[index_mem_instruction] = 16;
                MEM[index_mem_instruction+1] = ret_int(word[1]);
            }

            // FUNÇÃO LDA INDEX
            else if (str_match(word[0], "LDA"))
            {
                printf(" \t -> 32 DATA[%s]", word[1]);
                MEM[index_mem_instruction] = 32;
                MEM[index_mem_instruction+1] = ret_int(word[1]);
            }

            // FUNÇÃO ADD INDEX
            else if (str_match(word[0], "ADD"))
            {
                printf(" \t -> 48 DATA[%s]", word[1]);
                MEM[index_mem_instruction] = 48;
                MEM[index_mem_instruction+1] = ret_int(word[1]);
            }

            // FUNÇÃO OR INDEX
            else if (str_match(word[0], "OR"))
            {   
                printf(" \t -> 64 DATA[%s]", word[1]);
                MEM[index_mem_instruction] = 64;
                MEM[index_mem_instruction+1] = ret_int(word[1]);
            }
            
            // FUNÇÃO OR INDEX
            else if (str_match(word[0], "AND"))
            {   
                printf(" \t -> 80 DATA[%s]", word[1]);
                MEM[index_mem_instruction] = 80;
                MEM[index_mem_instruction+1] = ret_int(word[1]);
            }

            // FUNÇÃO JMP LABEL
            else if (str_match(word[0], "JMP"))
            {
                printf(" \t -> 128 LABEL['%s']", word[1]);
                MEM[index_mem_instruction] = 128;
                list_addFirst( &list_placeholder, item_new(index_mem_instruction+1, str_copy(word[1])));
            }

            // FUNÇÃO JN LABEL
            else if (str_match(word[0], "JN"))
            {
                printf(" \t -> 144 LABEL['%s']", word[1]);

                MEM[index_mem_instruction] = 144;
                list_addFirst( &list_placeholder, item_new(index_mem_instruction+1, str_copy(word[1])));
            }

            // FUNÇÃO JZ LABEL
            else if (str_match(word[0], "JZ"))
            {
                printf(" \t -> 160 LABEL['%s']", word[1]);
                MEM[index_mem_instruction] = 160;
                list_addFirst( &list_placeholder, item_new(index_mem_instruction+1, str_copy(word[1])));
            }

            index_mem_instruction += 2;
        }

        else if (word_quantity == 3)
        {
            // ATRIBUIÇÃO AO ENDEREÇO
            if (str_match(word[1], "<-"))
            {
                // recebe o valor 
                int addr = ret_int(word[0]);
                int val = ret_int(word[2]);
                
                if (addr > 128) 
                {
                    printf(" ABORTANDO PROGRAMA: ENDEREÇO DE ATRIBUIÇÃO INVÁLIDO (>128)");
                    stop = true;
                }

                else if (val > 255 || val < 0)
                {
                    printf(" ABORTANDO PROGRAMA: VALOR DE ATRIBUIÇÃO INVÁLIDO (>255 || <0)");
                    stop = true;
                }
                
                DATA_MEM[addr] = val;
                if (index_mem_highest_data < addr)
                    index_mem_highest_data = addr;
            }
        }

        for (int i = 0; i <= word_quantity; i++)
        {
            libera(word[i]);
        }

        libera(word);
        index_ch = 0;
        index_ln++;
    }   // end while

    fclose(FPTR);

    if (stop)
        return 0; // sem desalocar NADA AAA

    /**    printagem das árvores
    char *list_str = list_toString(list_label);
    printf("\n%s", list_str);
    libera(list_str);

    list_str = list_toString(list_placeholder);
    printf("\n%s", list_str);
    libera(list_str);*/
    
    // resolve as labels dependentes (placeholders)
    int labels_unresolved_count = 0;
    snode placeholder_node = list_placeholder.ini;
    while(placeholder_node != NULL) 
    {
        labels_unresolved_count += 1;
        struct Item item_placeholder = placeholder_node->item;
        placeholder_node = placeholder_node->next;

        snode label_node = list_label.ini;
        while (label_node != NULL)
        {
            struct Item item_label = label_node->item;
            if( str_match(item_placeholder.label, item_label.label))
            {

                MEM[item_placeholder.address] = item_label.address;
                labels_unresolved_count -= 1;
            }

            label_node = label_node->next;
        }
        
        if (labels_unresolved_count > 0)
        {
            printf("LABEL NÃO DEFINIDA: %s", item_placeholder.label);
            stop = true;
        }
           
    }
    if (stop)
        return 0;
        
    list_Free(&list_label);
    list_Free(&list_placeholder);



    // resolve a memória de dado e de instrução
    int index_mem_data_start = 255 - index_mem_highest_data;
    for (int i = index_mem_data_start, j = 0; i < 256; i++, j++)
    {
        MEM[i] = DATA_MEM[j];
    }

    for (int i = 0; i < 256; i++) 
    {
        int val = MEM[i];
        if ( (val == 16)
            || (val == 32)
            || (val == 48)
            || (val == 64)
            || (val == 80))
        {
            printf("\n %d : %d", MEM[i+1], index_mem_data_start);
            MEM[i+1] = MEM[i+1] + index_mem_data_start;
            i+=1;
        }

        // pula jumps para não interferirem em labels
        else if ( (val == 128)
            || (val == 144)
            || (val == 160))
        {
            i+=2;
        }
    }

    // grava o arquivo como .mem
    
    FPTR = fopen("./neander_bin.mem","w");
    char FILE_MEM[516];
    str_clr(FILE_MEM, 516);
    FILE_MEM[0] = 0x03;
    FILE_MEM[1] = 0x4e;
    FILE_MEM[2] = 0x44;
    FILE_MEM[3] = 0x52;

    for(int i = 0; i < 256; i+=1)
    {
        FILE_MEM[4+(2*i)] = MEM[i];   
    }
    fwrite(FILE_MEM, 1, 516, FPTR);
}