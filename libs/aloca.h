#ifndef J_ALOCA
#define J_ALOCA

#include <stdio.h>
#include <stdlib.h>

// C99
#define BUFFERSIZE 8192
#define CONTROLSIZE BUFFERSIZE/8

char buffer[BUFFERSIZE];
char control[CONTROLSIZE];


/// Descobre o tamanho necessário para o header
size_t _byte_headersize(size_t size) {
  size_t ret = 1;
  while(size > 255) {
    //  possível bug?
    //  divisão de inteiros perde precisão, divisões 
    //  consecutivas aumentam os erros de maneira exponencial.
    size /= 256;
    ret++;
  }

  return ret;
}


/// Mapeia um endereço do buffer ao de controle
/// Se o bit do buffer é 1, o endereço no buffer
/// está sendo reservado para controle
char _get_controlbuffer(size_t addr) {
  size_t index = addr / 8;
  size_t bit = addr % 8;
  return control[index] & (1 << bit);
}


/// Seta um bit do buffer de controle, mapeando ele
/// através de addr, com shifting e a or
void _set_controlbuffer(size_t addr) {
  size_t index = addr / 8;
  size_t bit = addr % 8;
  control[index] = control[index] | (1 << bit);
}


// Limpa um bit do buffer de controle, mapeando ele
// através de addr, com shifting e a nand
void _clr_controlbuffer(size_t addr) {
  size_t index = addr / 8;
  size_t bit = addr % 8;
  control[index] = control[index] & ~(1 << bit);
}


/// Aloca um espaço em buffer
/// Retorna um ponteiro ao primeiro endereço
/// ou NULL em caso de falha
void *aloca(size_t size) {
  if (size == 0) return NULL;
  
  // calcula informações sobre espaço necessário
  size_t cntsize = 0; // contador de espaço vazio 
  size_t headersize = _byte_headersize(size);
  size_t blocksize = size+headersize;
  
  // procura por um espaço necessário
  for(size_t i = 0; i < BUFFERSIZE; i++) {
    
    // Se não é um byte de controle
    if (_get_controlbuffer(i) == 0) {
      cntsize += 1;

      //rastreou espaço suficiente
      if (cntsize == blocksize) {
        size_t start = i - blocksize + 1;  
        for (size_t j = 0; j < headersize; j++)
          _set_controlbuffer(start+j);
        
        // now i am become death the destroyer of worlds 1
        char *stream = (char *) &size;
        for (size_t cnt = 0; cnt < headersize; cnt++) {
          buffer[start + cnt] = stream[cnt];
        }

        // now i am become death the destroyer of worlds 2
        return &buffer[i - size + 1];
      }
    } 

    // Se é um byte de controle
    else {
      cntsize = 0; // reseta contador
      size_t cnt = 0;
      size_t jumpsize = 0;
      char stream[] = {0, 0, 0, 0, 0, 0, 0, 0}; 

      // registra todos os bytes de header
      while(_get_controlbuffer(i)) {
        stream[cnt] = buffer[i];
        i++;
        cnt++;
      }

      // pula a quantidade de bytes já reservados
      // now i am become death the destroyer of worlds 3
      jumpsize = *((size_t *) stream);
      i += jumpsize - 1; //-1 pois o i é incrementado no final do while
    }
  }

  return NULL;
}



void libera(void *ptr) {
  size_t i = (char *)ptr - (char *)&buffer[0];
  if (i >= BUFFERSIZE) return;
  
  // movimenta ao header
  i--;
  while(_get_controlbuffer(i)) {
    _clr_controlbuffer(i);
    
    if (i != 0) i--; 
  }
}

#endif
