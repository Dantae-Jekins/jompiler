// gcc neander_vm.c -o neander_vm.out && ./neander_vm.out neander_bin.mem

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/j_strings.h"
#include "../libs/j_transform.h"

#define NF (AC & 128)
#define ZF (!AC)

int main(int argc, char **argv)
{
    unsigned char PC = 0;
    unsigned char AC = 0;
    unsigned char MEM[256];
    unsigned char data = 128;

    printf("\nJEANDER running code\n");
    for (int i = 0; i < 256; i++)
        MEM[i] = 0;

    // leitor de argumentos
    for (int i = 1; i < argc; i++)
    {
        // arquivo a ser compilado
        if (argv[i][0] != '-')
        {
            FILE *fptr;
            fptr = fopen(argv[i], "r");
            char MEMTEMP[516];
            for (int j = 0; j < 516; j++)
            {
                MEMTEMP[j] = 0;
            }

            fread(MEMTEMP, 1, (516), fptr);
            for (int z = 4, j = 0; z < (516); z += 2, j++)
            {
                MEM[j] = MEMTEMP[z];
            }
            fclose(fptr);
        }

        // altera o program counter
        else if (str_match(argv[i], "-pc")) 
        {
            i++;
            if (i == argc)
            {
                printf("\nARGUMENTO FALTANDO");
                break;
            }
            

            int val = ret_uns(argv[i]);
            if ((val < 0) || (val > 255)) 
            {
                printf("\nFLAG INVÁLIDA PARA PC");
            } else {
                PC = val;
            }
        }

        // altera o acumulador
        else if (str_match(argv[i], "-ac")) {
            
            i++;
            if (i == argc)
            {
                printf("\nARGUMENTO FALTANDO");
                break;
            }

            int val = ret_uns(argv[i]);
            if ((val < 0) || (val > 255)) 
            {
                printf("\nFLAG INVÁLIDA PARA PC");
            } else {
                AC = val;
            }
        }
    }


    // printagem da memória
    int NOP_skip = 0;
    printf("\nCODE BLOCK ---\n");
    for (int i = 0; i < 256; i++)
    {
        if (i <= data)
        {
            switch (MEM[i])
            {
            case 0:

                if (NOP_skip == 0)
                {
                    printf("addr %.3d : NOP ---", i);
                    NOP_skip += 1;
                } 
                else if (NOP_skip == 1)
                {
                    printf(" .  .  .\n");
                    NOP_skip += 1;
                }
                break;

            case 16:
                printf("addr %.3d : STA %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 32:
                printf("addr %.3d : LDA %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 48:
                printf("addr %.3d : ADD %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 64:
                printf("addr %.3d : OR %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 80:
                printf("addr %.3d : AND %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 96:
                printf("addr %.3d : NOT", i);
                NOP_skip = 0;
                break;

            case 128:
                printf("addr %.3d : JMP %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 144:
                printf("addr %.3d : JN %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 160:
                printf("addr %.3d : JZ %u", i, MEM[i + 1]);
                NOP_skip = 0;
                break;

            case 240:
                printf("addr %.3d : HLT", i);
                NOP_skip = 0;
                break;

            default:
                printf("addr %.3d : %u", i, MEM[i]);
                NOP_skip = 0;
                break;
            }
            
            if (NOP_skip < 2)
            {
                printf("\n");
            }
        }

        else
        {
            if (data+1 == i) 
                printf("\nDATA BLOCK ---\n");

            printf("addr %.3d : %u\n", i, MEM[i]);
        }
    }

    int run = 1;
    while (run)
    {
        switch (MEM[PC])
        {
        case 16:
            MEM[MEM[PC + 1]] = AC;
            PC += 2;
            break;

        case 32:
            AC = MEM[MEM[PC + 1]];
            PC += 2;
            break;

        case 48:
            AC = AC + MEM[MEM[PC + 1]];
            PC += 2;
            break;

        case 64:
            AC = AC | MEM[MEM[PC + 1]];
            PC += 2;
            break;

        case 80:
            AC = AC & MEM[MEM[PC + 1]];
            PC += 2;
            break;

        case 96:
            AC = !AC;
            PC += 1;
            break;

        case 128:
            PC = MEM[PC + 1];
            break;

        case 144:
            if (NF)
            {
                PC = MEM[PC + 1];
            }
            PC += 2;
            break;

        case 160:
            if (ZF)
            {
                PC = MEM[PC + 1];
            }
            PC += 2;
            break;

        case 240:
            PC += 1;
            run = 0;
            break;

        default:
            PC += 1;
            break;
        }

        if (PC >= 255)
        {
            run = 0;
            break;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        if (i <= data)
        {
            switch (MEM[i])
            {
            case 0:
                printf("addr %.3d : NOP ---", i);
                break;

            case 16:
                printf("addr %.3d : STA %u", i, MEM[i + 1]);
                break;

            case 32:
                printf("addr %.3d : LDA %u", i, MEM[i + 1]);
                break;

            case 48:
                printf("addr %.3d : ADD %u", i, MEM[i + 1]);
                break;

            case 64:
                printf("addr %.3d : OR %u", i, MEM[i + 1]);
                break;

            case 80:
                printf("addr %.3d : AND %u", i, MEM[i + 1]);
                break;

            case 96:
                printf("addr %.3d : NOT", i);
                break;

            case 128:
                printf("addr %.3d : JMP %u", i, MEM[i + 1]);
                break;

            case 144:
                printf("addr %.3d : JN %u", i, MEM[i + 1]);
                break;

            case 160:
                printf("addr %.3d : JZ %u", i, MEM[i + 1]);
                break;

            case 240:
                printf("addr %.3d : HLT", i);
                break;

            default:
                printf("addr %.3d : %u", i, MEM[i]);
                break;
            }
        }

        else
        {
            printf("addr %.3d : %u", i, MEM[i]);
        }

        printf("    ");
        if (i % 1 == 0)
        {
            printf("\n");
        }
    }
}