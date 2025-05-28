#include <stdio.h>
#include <stdlib.h>
#include "splaytree.h"


splaytree *create_node(int data){
    splaytree *new_node;
    new_node = (splaytree*) malloc(sizeof(splaytree));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
}

splaytree* leftRotate(splaytree* x) {
    splaytree* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

splaytree* rightRotate(splaytree* x) {
    splaytree* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

splaytree*  splay_node(splaytree *root, int key) {
    if (root == NULL || root->data == key)
        return root;

    if (key < root->data) {
        if (root->left == NULL)
            return root;

        // Zig-Zig (Left Left)
        if (key < root->left->data) {
            root->left->left = splay_node(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->data) {
            root->left->right = splay_node(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    } else {
        if (root->right == NULL)
            return root;

        // Zag-Zig (Right Left)
        if (key < root->right->data) {
            root->right->left = splay_node(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->right->data) {
            root->right->right = splay_node(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

splaytree * insert_node(splaytree * tree, int data){
    splaytree * current;
    current = tree;
    splaytree * last = tree;
    while (current != NULL && (current->left !=NULL || current->right !=NULL))
    {
        if (current->data <data)
        {
            last = current;
            current = current->right;
        }
        else if (current->data > data)
        {
            last = current;
            current = current->left;
        }
        else
        {
            return NULL;
        }
    }
    splaytree *new_node = create_node(data);
    if (current == NULL)
    {
        current = last;
        
    }
    if (current->data < data)
    {
        current->right = new_node;
        splaytree *new_tree = splay_node(tree, data);
        return new_tree;
    }
    if (current->data > data)
    {
        current->left = new_node;
        splaytree *new_tree = splay_node(tree, data);
        return new_tree;
    }
}

void in_order_print(splaytree *root) {
    if (root != NULL) {
        in_order_print(root->left);
        printf("%d ", root->data);
        in_order_print(root->right);
    }
}

void print_tree(splaytree *root) {
    printf("Tree elements (in-order): ");
    in_order_print(root);
    printf("\n");
}

void print_tree_level_order(splaytree *root) {
    if (root == NULL) {
        printf("Tree is empty!\n");
        return;
    }

    splaytree *queue[1000];  
    int front = 0, rear = 0;

    queue[rear++] = root;  

    while (front < rear) {
        int nodes_in_level = rear - front;
        
        while (nodes_in_level--) {
            splaytree *current = queue[front++];  
            printf("%d ", current->data);

            if (current->left != NULL)
                queue[rear++] = current->left;  
            if (current->right != NULL)
                queue[rear++] = current->right;  
        }
        printf("\n");  
    }
}