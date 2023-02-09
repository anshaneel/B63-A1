/*
   AVL tree implementation.

   Author: A. Tafliovich
   Based heavily on materials developed by F. Estrada.
*/

#include "AVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.
 */
int height(AVL_Node* node){

    if (node == NULL){
        return 0;
    }

    return max(height(node -> left), height(node -> right)) + 1;

}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void update_height(AVL_Node* node) {

    if (node == NULL){
      return;
    }

  int height_left = 0, height_right = 0;

  if (node -> left != NULL){
      height_left = node -> left -> height;
  }
  if (node -> right != NULL){
      height_right = node -> right -> height;
  }

  node -> height = max(height_left, height_right) + 1;

  }

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 of node is NULL.
 */
int balance_factor(AVL_Node* node) {

   if (node == NULL){
    return 0;
  }

  int height_left = 0, height_right = 0;

  if (node -> left != NULL){
      height_left = node -> left -> height;
  }
  if (node -> right != NULL){
      height_right = node -> right -> height;
  }

  return height_left - height_right;

}

/* Returns the result of performing the corresponding rotation in the AVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
AVL_Node* right_rotation(AVL_Node* node){

    AVL_Node* y = node;
    AVL_Node* x = node -> left;

    y -> left = x -> right;
    x -> right = y;

    update_height(x);
    update_height(y);

    return x;

}
// single rotations: left/counter-clockwise
AVL_Node* left_rotation(AVL_Node* node){

    AVL_Node* y = node;
    AVL_Node* x = node -> right;

    y -> right = x -> left;
    x -> left = y;

    update_height(x);
    update_height(y);

    return x;

}
// double rotation: right/clockwise then left/counter-clockwise
AVL_Node* right_left_rotation(AVL_Node* node){

    node -> right = right_rotation(node -> right);
    return left_rotation(node);
    
}
// double rotation: left/counter-clockwise then right/clockwise
AVL_Node* left_right_rotation(AVL_Node* node){

    node -> left = left_rotation(node -> left);
    return right_rotation(node);

}

AVL_Node* rotation(AVL_Node* node){

    int balance = balance_factor(node);

    if (balance == 0 || balance == 1 || balance == -1){
        return node;
    }
    
      if (balance > 1){
        if (balance_factor(node -> left) < 0){
            return right_left_rotation(node);
        }
        else {
            return right_rotation(node);
        }
    }
    if (balance < -1){
        if (balance_factor(node -> right) > 0){
            return left_right_rotation(node);
        }
        else {
           return left_rotation(node);
        }
    }

}

/* Returns the successor node of 'node'. */
AVL_Node* successor(AVL_Node* node){

    AVL_Node* current = node -> right;

    while (current -> left != NULL){
        current = current -> left;
    }

    return current;

  }

/* Creates and returns an AVL tree node with key 'key', value 'value', height
 * of 1, and left and right subtrees NULL.
 */
AVL_Node* create_node(int key, void* value){

  AVL_Node *new_node = (AVL_Node*) malloc(sizeof(AVL_Node));
  new_node -> left = NULL;
  new_node -> right = NULL;
  new_node -> height = 1;
  new_node -> key = key;
  new_node -> value = value;

  return new_node;

}

/*************************************************************************
 ** Provided functions
 *************************************************************************/
void print_tree_inorder_(AVL_Node* node, int offset) {
  if (node == NULL) return;
  print_tree_inorder_(node->right, offset + 1);
  printf("%*s %d [%d]\n", offset, "", node->key, node->height);
  print_tree_inorder_(node->left, offset + 1);
}

void print_tree_inorder(AVL_Node* node) {
  print_tree_inorder_(node, 0);
}

void delete_tree(AVL_Node* node) {
  if (node == NULL) return;
  delete_tree(node->left);
  delete_tree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

AVL_Node* search(AVL_Node* node, int key) {

  if (node == NULL){
    return node;
  }
  if (node -> key == key){
    return node;
  }
  if (node -> key < key){
    return search(node -> right, key);
  }
  else {
    return search(node -> left , key);
  }
}

AVL_Node* insert(AVL_Node* node, int key, void* value) {

  if (node == NULL){
      return create_node(key, value);
  }
  if (node -> key == key){
      return node;
  }
  if (node -> key > key){
      node -> left = insert(node -> left, key, value);
  }
  else {
      node -> right = insert(node -> right, key, value);
  }

  update_height(node);
  node = rotation(node);

  return node;

}

AVL_Node* delete(AVL_Node* node, int key) {

  if (node == NULL){
    return node;
  }

  if (node -> key > key){
    node -> left = delete(node -> left, key);
  }
  else if (node -> key < key){
    node -> right = delete(node -> right, key);
  }
  else {

    AVL_Node* tmp;

    if (node -> left == NULL){
      tmp = node -> right;
      free(node);
      return tmp;
    }

    if (node -> right == NULL){
      tmp = node -> left;
      free(node);
      return tmp;
    }

    tmp = successor(node);

    node -> key = tmp -> key;
    node -> value = tmp -> value;

    node -> right = delete(node -> right, tmp -> key);
  }

  update_height(node);
  node = rotation(node);

  return node;

}
