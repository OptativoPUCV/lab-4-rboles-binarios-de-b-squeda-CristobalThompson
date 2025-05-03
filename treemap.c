#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap* new = (TreeMap *) malloc(sizeof(TreeMap));
    if (new == NULL) exit(1);
    new->root = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *aux = tree->root;
    while(aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            return;
        }
        if (aux->right == NULL && tree->lower_than(key, aux->pair->key) == 0){
            aux->right = createTreeNode(key, value);
            aux->right->parent = aux;
            tree->current = aux->right;
            return;
        }
        if(aux->left == NULL && tree->lower_than(key, aux->pair->key) == 1){
            aux->left = createTreeNode(key, value);
            aux->left->parent = aux;
            tree->current = aux->left;
            return;
        }

        if (tree->lower_than(key, aux->pair->key) == 0)
            aux = aux->right;
        else if (tree->lower_than(key, aux->pair->key) == 1)
            aux = aux->left;
    }
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL) x = x->left;
    return x;
}

//0 es izq mayor a der, 1 contrario
void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* aux = node;
    TreeNode* parent = node->parent;
    if (node->left == NULL && node->right == NULL){

        if (tree->lower_than(node->pair->key, parent->pair->key) == 0) parent->right = NULL;
        else parent->left = NULL;
        
        free(aux);
    }
    else if(node->left != NULL && node->right != NULL){
        TreeNode* minimo = minimum(node->right);
        node->pair = minimo->pair;
        removeNode(tree, minimo);
    }
    else{
        if (node->left != NULL){

            if (tree->lower_than(node->pair->key, parent->pair->key) == 0) parent->right = node->left;
            else parent->left = node->left;
            node->left->parent = parent;
            free(aux);
        }
        else{
            if (tree->lower_than(node->pair->key, parent->pair->key) == 0) parent->right = node->right;
            else parent->left = node->right;
            node->right->parent = parent;
            free(aux);
        }
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    if (aux == NULL) return NULL;
    while(aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key) == 0)
            aux = aux->right;
        else if (tree->lower_than(key, aux->pair->key) == 1)
            aux = aux->left;
    }
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return (minimum(tree->root))->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
