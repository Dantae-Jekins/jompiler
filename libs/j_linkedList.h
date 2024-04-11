#ifndef J_LINKEDLIST_H
#define J_LINKEDLIST_H

// V 1.3 - Específico para o projeto aloca

#include<stdbool.h>
#include "./j_strings.h"
#include "./aloca.h"
#include "./j_transform.h"


// Estrutura do conteúdo de um node
// considerando number como uma chave de registro (ID)
struct Item
{
  int address;
  char *label;
};


// Estrutura node
struct Node
{
    struct Node *next;
    struct Node *prev;
    struct Item item;
} typedef * snode; 


// Estrura da lista ligada
typedef struct 
{
    snode ini;
    snode end;
    int len;
} list;


// -------------------------------------------------------------- //
// FUNÇÕES SUJEITAS A MUDANÇA, NECESSÁRIO ADAPTAR DE PROJETO PARA PROJETO

// Cria um novo item
// mudando os conteúdos de struct Item, se torna necessário
// também mudar esta função
struct Item item_new( int addr, char *label)
{
  struct Item ret;
  ret.address = addr;
  ret.label = label;
  return ret;
}


// Função para comparar itens, 
// mudando os conteúdos de struct Item, se torna necessário
// também mudar esta função.
bool __item_Compare(struct Item i1, struct Item i2)
{
  if ((i1.address == i2.address) && (i1.address == i2.address))
    return true;

  return false;
}


// Função para transformar itens em string,
// mudando os conteúdos de struct Item, se torna necessário 
// também mudar esta função
char * __item_toString(struct Item i1)
{
  char *aux1;
  char *aux2;
  aux1 = ret_str(i1.address);
  aux2 = str_concac((char *)"(", aux1);

  libera(aux1);
  aux1 = str_concac(aux2,(char *)", ");
  
  libera(aux2);
  aux2 = str_concac(aux1, i1.label);

  libera(aux1);
  aux1 = str_concac(aux2, (char *)")");
  
  return aux1;
}


// Função que retorna uma cópia de item,
// necessário saber um pouco mais sobre C para usar isto corretamente
struct Item __item_Copy(struct Item value)
{
  // criação
  struct Item ret;

  // ret = value; só após C90 
  ret.address = value.address;
  ret.label = str_copy(value.label);

  return ret;
}


// Destrói o item se algo estiver alocado
// obviamente consome qualquer coisa alocada de dentro
int item_destroy(struct Item target) {
  if (target.label != NULL) {
    libera(target.label);
    return 1;
  }
  return 0;
}


// Transforma a lista em uma string, útil para printagem
// É necessário liberar a memória
char *list_toString( list target)
{

  // Checa se está vazia
  if (target.ini == NULL)
  {
    return str_copy((char *)"{ null }");
  }

  else
  {
    // prepara o retorno
    char *ret = (char *)"{ ";
    char *value = NULL;

    // 2 strings para possibilitar libera
    char *str1 = str_copy( ret );
    char *str2 = NULL;

    // percorre
    snode This = target.ini;
    while (1)
    {
      // libera value
      if ( value != NULL )
        libera(value);

      value = __item_toString(This->item);

      // concatena e libera a string anterior
      str2 = str_concac(str1, value);
      libera(str1);
      str1 = str2;

      // vê se é o fim
      if (This->next == NULL)
      {
        str2 = str_concac(str1, (char *)" }");
        libera(str1);
        libera(value);
        return str2;
      }

      // se não adiciona uma vírgula
      str2 = str_concac(str1, (char *)", ");
      libera(str1);
      str1 = str2;

      This = This->next;
    }
  } 
}






// ------------------------------------------------------------- //
/// FUNÇÕES NÃO SUJEITAS A MUDANÇA, NÃO TOQUE, SÓ EM CASO DE BUG

//Cria uma nova lista e retorna
list list_new()
{
  list ret;
  ret.len = 0;
  ret.ini = NULL;
  ret.end = NULL;
  return ret;
}


// Adiciona um elemento ao inicio da lista
// é necessário liberar a memória
void list_addFirst(list * target, struct Item value)
{
  snode new_n = (snode) aloca(sizeof( struct Node ));
  new_n->item = __item_Copy(value);
  new_n->prev = NULL;
  new_n->next = NULL;

  if ( target->len == 0 )
  {
    target->ini = new_n;
    target->end = new_n;
    target->len = 1;
    return;
  }

  target->len += 1;
  
  snode aux = target->ini;
  target->ini = new_n;

  new_n->next = aux;
  aux->prev = new_n;
}


// Adiciona um elemento ao final da lista
// é necessário liberar a memória
void list_addLast(list * target, struct Item value)
{
  snode new_n = (snode) aloca(sizeof( struct Node ));
  new_n->item = __item_Copy(value);
  new_n->prev = NULL;
  new_n->next = NULL;

  if ( target->len == 0 )
  {
    target->ini = new_n;
    target->end = new_n;
    target->len = 1;
    return;
  }

  target->len += 1;
  
  snode aux = target->end;
  target->end = new_n;
  
  aux->next = new_n;
  new_n->prev = aux;
}


// Função para remover um nó com um nó como parâmetro
// O nó DEVE existir na lista, não é uma função de busca
// A cadeia de caracteres é consumida !!
void list_nodeRemove(list *target, snode node)
{
  item_destroy(node->item);
  snode parent = node->prev;
  snode child = node->next;
  
  // faz o antecessor se conectar ao sucessor
  if (parent != NULL)
    parent->next = child;

  else
    target->ini = child;

  
  // faz o sucessor se conectar ao antecessor
  if (child != NULL)
    child->prev = parent;

  else
    target->end = parent;

  target->len -= 1;
  libera(node);
}


// Função para remover um nó com uma estrutura como parâmetro
// Retorna true se foi removido com sucesso
bool list_itemRemove( list * target, struct Item value) 
{
  // checa o tamanho da lista
  if (target->len == 0) return false; 
  
  if (target->len == 1 ) 
  {
    if (__item_Compare( target->ini->item, value))
    {
      list_nodeRemove( target, target->ini );
      return true;
    }
    else return false;
  }

  // exitem mais de dois elementos:
  snode parent = (snode) target;
  while(parent->next != NULL)
  {
    if (__item_Compare( target->ini->item, value))
    {
      list_nodeRemove( target, parent->next);
      return true;
    }

    // movimenta
    parent = parent->next;
  } 
  
  return false;
}


// Retorna a quantidade de elementos da lista
int list_Count( list target )
{
  if (target.ini == NULL)
    return 0;
  
  snode current = target.ini;
  int count = 1;
  while ( current->next != NULL )
  {
    current = current->next;
    count += 1;
  }

  return count;
}


// Limpa a lista liberando toda sua memória
void list_Free( list *target)
{
  snode node = target->ini;
  snode aux;
  while ( node != NULL )
  {
    aux = node->next;
    list_nodeRemove(target, node);
    node = aux;
  }
}

#endif
