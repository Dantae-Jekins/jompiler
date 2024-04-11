#ifndef J_STRINGS_H
#define J_STRINGS_H

// V 1.3  ALTERADO PARA JOMPILER

#include "./aloca.h"

// Retorna o tamano da string
int str_len(char *string)
{
    int i = 0;
    while(string[i]!='\0') 
      i++;

    return i;
}

void str_clr(char *str, int len) 
{
    for (int i = 0; i < len; i++)
      str[i] = 0;

    return;
}

// Retorna uma cópia de uma string. Necessário liberar a memoria alocada.
char *str_copy(char *string)
{
    int tam = str_len(string);
    char *aux = (char*)aloca(sizeof(int)*(1+tam));
    for (int i = 0; i <= tam; i++)
        aux[i] = string[i];
    
    aux[tam] = '\0'; //coloca ponto de parada
    return aux;
}

// Retorna str cortado entre start e end. Necessário liberar memória alocada
char *str_crop(unsigned int start, unsigned int end, char *str1)
{ 

  int size = end-start+1;
  char *str2 = (char*)aloca(sizeof(char)*(size));
  
  int i = 0;
  while((str1[start] != '\0') && (end > start))
  {
    str2[i++] = str1[start++];
  } 

  str2[size-1] = '\0'; // coloca ponto de parada
  return str2;
}

// Separa uma string. Necessário liberar a memória alocada.
// Retorna várias strings, termina com uma string NULL
char **str_split( char *str, char keyword)
{
  int len = str_len(str);

  // aloca um vetor auxiliar, quando o valor do auxiliar é diferente de 0, ele indica
  // o início (endereço do valor) da palavra e o seu final (valor naquele endereço).
  char *auxiliar = (char*) aloca(len);
  str_clr(auxiliar, len);
  int current_word_start_index  = 0;

  // conta ocorrências não repetidas das chaves
  int keyword_count   = 0;
  int repeated        = 1;
  int i;
  for (i = 0; i < len; i++) 
  {

    if (repeated == 1) 
    {
      if (str[i] != keyword)
      {
        current_word_start_index = i; // começo de uma palavra
        repeated = 0;
      }
    }

    else
    {
      if (str[i] == keyword)
      {
        auxiliar[current_word_start_index] = i;

        keyword_count += 1;
        repeated = 1;
      }
    }
  }

  // se ainda estava lendo uma palavra até o término da string, termina ela
  if (repeated == 0) 
  {
      auxiliar[current_word_start_index] = i;
      keyword_count += 1;
  }


  //  prepara vetor de retorno
  // keyword_count é igual a quantidade de palavras
  char **ret = aloca(sizeof(char*) * (keyword_count+1));
  ret[keyword_count] = 0;

  int ret_index = 0;
  for(i = 0; i < len; i++) 
  {
    if (auxiliar[i] != 0)
    {
      ret[ret_index] = str_crop(i, (int)auxiliar[i], str);
      ret_index ++;
    }
    i = auxiliar[i];
  }

  // retorna
  libera(auxiliar);
  return ret;
}


// Retorna duas strings somadas. Necessário liberar a memória alocada.
char *str_concac(char *str1, char *str2)
{
  // adquire os tamanhos
  int len1 = str_len(str1);
  int len2 = str_len(str2);
  int end = len1 + len2;

  // aloca
  char *ret = (char*)aloca(sizeof(char) * (end + 1));
  ret[end] = '\0';

  int i;
  // percorre str1 0 -> len1
  for (i = 0; i < len1; i++)
    ret[i] = str1[i]; 
  
  // percorre str2 i -> end
  for (int j = 0; i < end; i++, j++)
    ret[i] = str2[j];

  return ret;
}

// Move i elementos de str2 para str1
void str_mov(int i, char *str1, char *str2)
{
  int j = 0;
  while( j < i )
  {
    str1[j] = str2[j];
    j++;
  }  
}


// Verifica se str1 == str2
int str_match(char *str1, char *str2)
{
    int i = 0;
    do
    {
        if(str1[i] != str2[i])
            return 0; // retorna falso
        
    } while( (str1[i] != '\0') && (str2[i++] != '\0') );

    return 1;
}

// Verifica se até n caracteres str1 == str2
int str_match_number(char *str1, char *str2, int n)
{
    int i = 0;
    do
    {
        if(str1[i] != str2[i])
            return 0; // retorna falso
        
        if (i == n) {
          break;
        }
        
    } while( (str1[i] != '\0') && (str2[i++] != '\0') );

    return 1;
}

#endif
