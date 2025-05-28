#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct splaytree {
    int data;
    struct splaytree *left;
    struct splaytree *right;
} splaytree;



splaytree *create_node(int data);
void left_rotation(splaytree *node);
void right_rotation(splaytree *node);
void splay(splaytree *node);
splaytree * insert_node(splaytree *tree, int data);
void in_order_print(splaytree *root);
void print_tree(splaytree *root);
void print_tree_level_order(splaytree *root);

#endif

