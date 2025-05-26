#include <stdio.h>
#include <stdlib.h>

typedef struct pila
{
    int *base;
    int *top;
    int tamaño;
}pila;

pila * crear_pila(int tamaño){
    pila *new_pila;
    new_pila = (pila *) malloc(sizeof(pila));
    new_pila->base = (int *) malloc(tamaño * sizeof(int));
    new_pila->tamaño = tamaño;
    new_pila->top = new_pila->base;
    return(new_pila);
}

int añadir_pila(pila * pil, int objeto){
    if (pil->top < pil->base + pil->tamaño){
        *(pil->top) = objeto; pil->top +=1;
        return 0;
    }
    else 
        return 1;
}

void print_top(pila *pil){
    printf("%d", *(pil->top - 1));
}

int eliminar_pila(pila *pil){
    pil->top -= 1;
    return(*(pil->top));
}
