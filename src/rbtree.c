#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p -> nil = (node_t*)calloc(1, sizeof(node_t));
  p -> nil -> color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;

  if(y->left != t->nil){
    y->left->parent = x;
  }

  y->parent= x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x->parent->left == x){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left= x;
  x->parent =y;

}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;

  if(y->right != t->nil){
    y->right->parent = x;
  }

  y->parent= x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x->parent->left == x){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->right= x;
  x->parent =y;
}

void delete_node(rbtree *t, node_t *tmp){
  if(tmp != t-> nil){
    delete_node(t,tmp->left);
    delete_node(t,tmp->right);
    free(tmp);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void rb_insert_fixup(rbtree *t, node_t *z){
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else{
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    } 
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* new_node = (node_t *)calloc(1,sizeof(node_t));
  node_t* y = t->nil;
  node_t* x = t->root;
  
  while (x != t->nil){
    y = x;
    if (key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  new_node->parent = y;
  if (y == t->nil){
    t->root = new_node;
  }
  else if (key < y->key){
    y->left = new_node;
  }
  else { 
    y->right = new_node;
  }

  new_node->key = key;

  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  
  rb_insert_fixup(t,new_node);
  t->root->color = RBTREE_BLACK;
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }
  node_t *temp = t->root;

  while(temp != t->nil){
    if (temp->key > key){
      temp = temp -> left;
    }
    else if (temp->key < key){
      temp = temp -> right;
    }
    else{
      return temp;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t->root;
  while(temp->left != t->nil){
    temp = temp->left;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t->root;
  while(temp->right != t->nil){
    temp = temp->right;
  }
  return temp;
}

void delete_fixup(rbtree *t, node_t *x){
  while(x != t->root && x->color == RBTREE_BLACK){
    if (x== x->parent->left){
      node_t *w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
        if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
          w->color = RBTREE_RED;
          x = x->parent;
        }
        else{ 
          if(w->right->color == RBTREE_BLACK){
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t,w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          w->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          left_rotate(t,x->parent);
          x = t->root;
        }
    }
    else{
      node_t *w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
      }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      } 
    }
  }
  x->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  
  if (u->parent == t->nil){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if (p==NULL){
    return 0;
  }
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

  if (p->left == t->nil){
    x = p->right;
    rb_transplant(t,p,p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    rb_transplant(t,p,p->left);
  }
  else{
    y = p->right;
    while(y->left != t->nil){
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;
    if(y->parent == p){
      x->parent = y;
    }
    else{
      rb_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y_original_color == RBTREE_BLACK){
    delete_fixup(t,x);
  }
  free(p);
  return 0;
}

int rbtree_inorder(rbtree *t, key_t *arr, node_t *temp, int i){
  if(temp == t->nil){
    return i;
  }
  i =rbtree_inorder(t,arr,temp->left, i);
  arr[i++] = temp->key;
  i =rbtree_inorder(t,arr,temp->right, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *temp = t->root;

  rbtree_inorder(t, arr, temp, 0);
  return 0;
}
