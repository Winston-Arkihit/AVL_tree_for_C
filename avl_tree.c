#include <stdio.h>
#include <stdlib.h>

enum knot_errors{
    NULL_NODE = -1,
};

typedef struct KNOT{
    int knot_value;
    int height;
    struct KNOT *pointer_to_left; //pointer to left knot
    struct KNOT *pointer_to_right; //pointer to right knot

} knot;

knot* create_knot(int value) {
    knot* new_node = (knot*)malloc(sizeof(knot));
    new_node->knot_value = value;
    new_node->height = 0; // height = 0, because no height
    new_node->pointer_to_left = NULL;
    new_node->pointer_to_right = NULL;
    return new_node;
}

int get_height(knot* node) {
    if (node == NULL){
        return NULL_NODE;
    }
    return node->height;
}

int get_node_balance(knot *node){
    if (node == NULL){
        return NULL_NODE;
    }
    else{
        int difference = get_height(node->pointer_to_left) - get_height(node->pointer_to_right);
        return difference;
    }
}

knot* small_right_rotate(knot* a) {
    // 1. remember parts of swap (according to wiki)
    knot* b = a->pointer_to_left;
    knot* C = b->pointer_to_right;

    // 2. change tree
    a->pointer_to_left = C;    // subtree C goes to node a
    b->pointer_to_right = a;   // node a becomes right son of b

    // 3. update "a" height
    int left_a = get_height(a->pointer_to_left);
    int right_a = get_height(a->pointer_to_right);
    a->height = (left_a > right_a ? left_a : right_a) + 1;

    // 4. update "b" height
    int left_b = get_height(b->pointer_to_left);
    int right_b = get_height(b->pointer_to_right);
    b->height = (left_b > right_b ? left_b : right_b) + 1;

    // return b, because it is our new HIGH of current part nodes
    return b;
}

knot* small_left_rotate(knot* a) {
    // 1. remember parts of swap (according to wiki)
    knot* b = a->pointer_to_right;
    knot* C = b->pointer_to_left;

    // 2. change tree
    a->pointer_to_right = C;    // subtree C goes to node a
    b->pointer_to_left = a;     // node a becomes right son of b

    // 3. update "a" height
    int left_a = get_height(a->pointer_to_left);
    int right_a = get_height(a->pointer_to_right);
    a->height = (left_a > right_a ? left_a : right_a) + 1;

    // 4. update "b" height
    int left_b = get_height(b->pointer_to_left);
    int right_b = get_height(b->pointer_to_right);
    b->height = (left_b > right_b ? left_b : right_b) + 1;

    // return b, because it is our new HIGH of current part nodes
    return b;
}

knot* insert_knot(knot* node, int value) {
    if (node == NULL) {
    return create_knot(value);
    }

    if (value < node->knot_value) {
        node->pointer_to_left = insert_knot(node->pointer_to_left, value);
    } else if (value > node->knot_value) {
        node->pointer_to_right = insert_knot(node->pointer_to_right, value);
    }
    // height update
    int left_h = get_height(node->pointer_to_left);
    int right_h = get_height(node->pointer_to_right);
    node->height = (left_h > right_h ? left_h : right_h) + 1;
    int balance = get_node_balance(node);
    if (balance > 1){
        return small_right_rotate(node);
    }
    else if (balance < -1){
        return small_left_rotate;
    }

    return node; // return changed node

}

void free_tree(knot* node) {
    if (node == NULL) return NULL_NODE; // nothing to delete
    // recursive all left
    free_tree(node->pointer_to_left);
    // recursive all right
    free_tree(node->pointer_to_right);
    // kill "BEGIN" node
    free(node);
}

// helper function to find minimum node in subtree
knot* find_min_knot(knot* node) {
    knot* current = node;
    while (current->pointer_to_left != NULL) {
        current = current->pointer_to_left;
    }
    return current;
}

// delete only one node and pull up its children
knot* delete_knot_only(knot* node, int value) {
    // 1. recursive search for node
    if (node == NULL) {
        return node; // node not found
    }

    if (value < node->knot_value) {
        node->pointer_to_left = delete_knot_only(node->pointer_to_left, value);
    } 
    else if (value > node->knot_value) {
        node->pointer_to_right = delete_knot_only(node->pointer_to_right, value);
    } 
    
    // 2. node found SO start delete and pull up process
    else {
        // CASE A & B: node has ONE child or NO children
        if ((node->pointer_to_left == NULL) || (node->pointer_to_right == NULL)) {
            
            // check which child exists and remember it
            knot *child_to_pull_up = node->pointer_to_left ? node->pointer_to_left : node->pointer_to_right;

            // if node has no children
            if (child_to_pull_up == NULL) {
                free(node);
                node = NULL;  // parent pointer becomes NULL
            } 
            // if node has one child
            else { 
                *node = *child_to_pull_up; // copy child data to current node
                free(child_to_pull_up);    // free old child node memory
            }
        } 
        // CASE C: node has TWO children
        else {
            // find min node in right subtree
            knot* successor = find_min_knot(node->pointer_to_right);
            node->knot_value = successor->knot_value;
            node->pointer_to_right = delete_knot_only(node->pointer_to_right, successor->knot_value);
        }
    }

    return node; // return changed subtree
}
