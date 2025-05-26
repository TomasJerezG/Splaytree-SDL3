#include <stdio.h>
#include <stdlib.h>

typedef struct splaytree
{
    int data;
    struct splaytree *left;
    struct splaytree *right;
}splaytree;


typedef struct pila
{
    splaytree *base;
    splaytree *top;
    int tamaño;
}pila;

pila * crear_pila(int tamaño){
    pila *new_pila;
    new_pila = (pila *) malloc(sizeof(pila));
    new_pila->base = (splaytree *) malloc(tamaño * sizeof(splaytree));
    new_pila->tamaño = tamaño;
    new_pila->top = new_pila->base;
    return(new_pila);
}

int add_pila(pila * pil, splaytree *objeto){
    if (pil->top < pil->base + pil->tamaño){
        pil->top = objeto; pil->top +=1;
        return 0;
    }
    else 
        return 1;
}

splaytree * eliminar_pila(pila *pil){
    pil->top -= 1;
    return(pil->top);
}


splaytree *create_node(int data){
    splaytree *new_node;
    new_node = (splaytree*) malloc(sizeof(splaytree));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
}

void left_rotation(splaytree *node){
    splaytree *old_node = node;
    node->data = old_node->right->data;
    node->right = old_node->right->right;
    node->left = old_node;
    node->left->right = old_node->right;
}

void right_rotation(splaytree *node){
    splaytree *old_node = node;
    node->data = old_node->left->data;
    node->left = old_node->left->left;
    node->right = old_node;
    node->right->left = old_node->left;
}

void splay(splaytree *node, pila * pil){

    
}
int insert_node(splaytree * tree, int data){
    splaytree * current;
    current = tree;
    pila *pill;
    pill = crear_pila(100);
    while (current != NULL && (current->left !=NULL || current->right !=NULL))
    {
        add_pila(pill, current);
        if (current->data <data)
        {
            current = current->right;
        }
        else if (current->data > data)
        {
            current = current->left;
        }
        else
        {
            return -1;
        }
    }
    splaytree *new_node = create_node(data);
    if (current == NULL)
    {
        current = eliminar_pila(pill);
        
    }
    if (current->data < data)
    {
        current->right = new_node;
        return 1;
    }
    if (current->data > data)
    {
        current->left = new_node;
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

    // Create a simple queue using an array
    splaytree *queue[1000];  // Adjust size as needed
    int front = 0, rear = 0;

    queue[rear++] = root;  // Enqueue root

    while (front < rear) {
        int nodes_in_level = rear - front;
        
        while (nodes_in_level--) {
            splaytree *current = queue[front++];  // Dequeue
            printf("%d ", current->data);

            if (current->left != NULL)
                queue[rear++] = current->left;  // Enqueue left child
            if (current->right != NULL)
                queue[rear++] = current->right;  // Enqueue right child
        }
        printf("\n");  // New line after each level
    }
}
