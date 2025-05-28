#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct splaytree {
    int data;
    struct splaytree *left;
    struct splaytree *right;
} splaytree;

typedef struct pila {
    splaytree *base;
    splaytree *top;
    int tamaño;
    int curr;
} pila;

pila *crear_pila(int tamaño);
int add_pila(pila *pil, splaytree *objeto);
splaytree *eliminar_pila(pila *pil);
splaytree *create_node(int data);
void left_rotation(splaytree *node);
void right_rotation(splaytree *node);
void splay(splaytree *node, pila *pil);
int insert_node(splaytree *tree, int data);
void in_order_print(splaytree *root);
void print_tree(splaytree *root);
void print_tree_level_order(splaytree *root);

#endif

