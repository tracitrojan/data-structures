#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdexcept>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::invalid_argument;
using std::length_error;
using std::max;

enum traversal_order {preorder, inorder, postorder};

template <typename E>
class AVLTree;  // Let node class know AVL exists.

template <typename E>
class ANode {
  E data;
  int height = 0;
  ANode<E>* parent = nullptr;
  ANode<E>* left = nullptr;
  ANode<E>* right = nullptr;
  friend class AVLTree<E>;
};


template <typename E>
class AVLTree {
private:
  ANode<E>* root;
  int num_of_nodes;

  // Trinode restructuring
  bool is_balanced(ANode<E>* &);  // Check if subtree is balanced
  void rebalance(ANode<E>* &);
  ANode<E>* tallest_grandchild(ANode<E>* &ptr);  // Find the tallest grandchild of node

  // Helper functions to perform recursion.
  int height_helper(ANode<E>*);
  ANode<E>* insert_helper(ANode<E>* &, const E &);
  ANode<E>* remove_node(ANode<E>* &);
  ANode<E>* remove_helper(ANode<E>* &, const E &);
  E& retrieve_helper(ANode<E>* &, const E &);
  void print_helper(ANode<E>*, traversal_order);
  void remove_all_helper(ANode<E>* &);

  void expand_external(ANode<E>*);  // add external nodes
  bool is_external(ANode<E>*);
  void set_height(ANode<E>*);  // Set height of node.
  bool is_leaf_node(ANode<E>*);  // Check if node it a leaf node
  bool only_one_child(ANode<E>*);  // Check if node has only one child node
  ANode<E>* get_min_ptr(ANode<E>*);  // Find the pointer that points to the
                                   // minimum value.

public:
  AVLTree();
  ~AVLTree();

  bool is_empty();
  int size();
  int height();

  void insert(const E &);
  void remove(const E &);
  E& retrieve(const E &);

  void print(traversal_order);
  void remove_all();
};

template <typename E>
AVLTree<E>::AVLTree() {
  root = nullptr;
  num_of_nodes = 0;
}

template <typename E>
AVLTree<E>::~AVLTree() {
  remove_all_helper(root);
}

template <typename E>
bool AVLTree<E>::is_empty() {
  // Returns true if tree is empty, else false.
  return root == nullptr;
}

template <typename E>
void AVLTree<E>::expand_external(ANode<E>* ptr) {
  ptr->left = new ANode<E>;
  ptr->right = new ANode<E>;
  ptr->left->parent = ptr;
  ptr->right->parent = ptr;
}

template <typename E>
bool AVLTree<E>::is_external(ANode<E>* ptr) {
  if (ptr->right == nullptr && ptr->left == nullptr)
    return true;
  return false;
}

template <typename E>
bool AVLTree<E>::is_leaf_node(ANode<E>* ptr) {
  // Check it passed in pointer points to a leaf node.
  if (is_external(ptr->left) && is_external(ptr->right))
    return true;
  return false;
}

template <typename E>
bool AVLTree<E>::only_one_child(ANode<E>* ptr) {
  // Check if passed in pointer points to a node with only one child.
  if (is_external(ptr->left) && !is_external(ptr->right))
    return true;
  if (is_external(ptr->right) && !is_external(ptr->left))
    return true;
  return false;
}

template <typename E>
ANode<E>* AVLTree<E>::get_min_ptr(ANode<E>* ptr) {
  // Traverse the passed in pointer to the minimum value of the tree/subtree.
  if (ptr == nullptr || is_external(ptr))
    throw invalid_argument("cannot find minimum value; is null or external");

  if (is_external(ptr->left))
    return ptr;

  return get_min_ptr(ptr->left);
}

template <typename E>
int AVLTree<E>::size() {
  // Returns the number of nodes in the tree.
  return num_of_nodes;
}

template <typename E>
void AVLTree<E>::set_height(ANode<E>* ptr) {
  // Sets the height of the node by adding one to the height of the child with the
  // largest height.
    ptr->height = max(ptr->left->height, ptr->right->height) + 1;
}

template <typename E>
int AVLTree<E>::height() {
  // Check height of tree (how many nodes, not edges).
  return root->height;
}

template <typename E>
bool AVLTree<E>::is_balanced(ANode<E>* &ptr) {
  // Checks if the subtree is balanced by comparing the heights of its child
  // nodes. If it finds that the hight difference is greater than 1, then it
  // returns false; true otherwise.
  int bal = ptr->left->height - ptr->right->height;
  return ((bal >= -1) && (bal <= 1));
}

template <typename E>
ANode<E>* AVLTree<E>::tallest_grandchild(ANode<E>* &ptr) {
  // Finds the tallest grandchild of the passed in node. First it compares the
  // heights of the childs, the the heights of the child's child.
  ANode<E>* lchild = ptr->left;
  ANode<E>* rchild = ptr->right;

  if (lchild->height >= rchild->height) {
    if (lchild->left->height >= lchild->right->height)
      return lchild->left;
    else
      return lchild->right;
  }

  else {
    if (rchild->right->height >= rchild->left->height)
      return rchild->right;
    else
      return rchild->left;
  }
}

template <typename E>
void AVLTree<E>::rebalance(ANode<E>* &ptr) {
  // Checks to see if the tree needs any rebalancing. If it does it calls
  // restructure.
  ANode<E>* z = ptr;
  while (z != root) {
    // Check for balancing all the way up until the root.
    z = z->parent;
    set_height(z);

    if (!is_balanced(z)) {
      // If unbalanced, find tallest grandchild, perform trinode restructure,
      // and adjust the heights.
      ANode<E>* x = tallest_grandchild(z);
      // z = restructure(x);
      set_height(z->left);
      set_height(z->right);
      set_height(z);
    }
  }
}

template <typename E>
ANode<E>* AVLTree<E>::insert_helper(ANode<E>* &ptr, const E &elem) {
  // Recursively traverse until you reach a leaf node's child node (nullptr). Once
  // reached, allocate a new node and replace the nullptr to point to that new node.

  if (num_of_nodes == 0) {
    // If tree is empty, create root node.;
    ptr = new ANode<E>;
    ptr->data = elem;
    expand_external(ptr);
    return ptr;
  }

  if (is_external(ptr)) {
    // Add new node to tree.
    ptr->data = elem;
    expand_external(ptr);
    return ptr;
  }

  // Traverse
  if (elem < ptr->data) {
    ANode<E>* child = insert_helper(ptr->left, elem);
    if (ptr->left == child)
      child->parent = ptr;
    return child;
  }
  else if (elem > ptr->data) {
    ANode<E>* child = insert_helper(ptr->right, elem);
    if (ptr->right == child)
      child->parent = ptr;
    return child;
  }
  else if (elem == ptr->data)
    throw invalid_argument("value is already in tree");
}

template <typename E>
void AVLTree<E>::insert(const E &elem) {
  // To remove an element. Call to insert helper.
  ANode<E>* ptr = insert_helper(root, elem);
  num_of_nodes++;
  set_height(ptr);
  rebalance(ptr);
}

template <typename E>
ANode<E>* AVLTree<E>::remove_node(ANode<E>* &ptr) {
  // Remove node that the passed in pointer points to. There are three cases
  // that have to be taken cared of: The node that needs to be removed (1) is
  // a leaf node, (2) has only one child, or (3) has two child nodes. Returns
  // A pointer to where the node was removed.

  if (is_leaf_node(ptr)) {
    // If leaf node, link one of its external nodes with the parent node, then
    // delete the leaf node and the other external node that was not used. If
    // It is root then delete everything.

    if (ptr == root) {
      delete ptr->right;
      delete ptr->left;
      delete ptr;
      ptr = nullptr;
      return ptr;
    }
    ANode<E>* gparent = ptr->parent;
    ANode<E>* extern_node = ptr->left;

    if (gparent->left == ptr)
      gparent->left = extern_node;
    else gparent->right = extern_node;
    extern_node->parent = gparent;

    delete ptr->right;
    delete ptr;

    return extern_node;
  }

  else if (only_one_child(ptr)) {
    // If node only has one child node then (depending on whether it is a right
    // child or left child) set the pointer to point to the child node and
    // delete the node that the pointer previously pointed to.

    if (is_external(ptr->left)) {
      // Change ptr to point to its right child and delete the node that it
      // previously pointed to.
      ANode<E>* to_remove = ptr;
      ptr = ptr->right;
      ptr->parent = to_remove->parent;
      delete to_remove->left;  // delete external node
      delete to_remove;
      return ptr;
    }
    else if (is_external(ptr->right)) {
      // Preform the same as above but with opposite child.
      ANode<E>* to_remove = ptr;
      ptr = ptr->left;
      ptr->parent = to_remove->parent;
      delete to_remove->right;  // delete external node
      delete to_remove;
      return ptr;
    }
  }

  // Case in which there are two children.
  else {
    // Find the minimum value of the subtree with ptr->right as root, then
    // replace ptr's data with the minimum value. Finally, delete the node that
    // originally had the minimum value.
    ANode<E>* min = get_min_ptr(ptr->right);
    ptr->data = min->data;
    return remove_helper(ptr->right, min->data);
  }
}

template <typename E>
ANode<E>* AVLTree<E>::remove_helper(ANode<E>* &ptr, const E &elem) {
  // Traverses ptr to the node that needs to be deleted. Assigns new child's
  // parent. Returns pointer that points to where the deleted node use to be,
  // unless it was a leaf node that was removed; in that case it returns a
  // pointer to it's parent.
  if (is_external(ptr))
    throw invalid_argument("value not in tree");

  if (ptr->data == elem)
    return remove_node(ptr);
  else if (elem < ptr->data)
    return remove_helper(ptr->left, elem);
  else if (elem > ptr->data)
    return remove_helper(ptr->right, elem);
}

template <typename E>
void AVLTree<E>::remove(const E &elem) {
  // To remove an element. Call to remove helper.
  if (is_empty())
    throw length_error("tree is empty, nothing to remove");

  ANode<E>* ptr = remove_helper(root, elem);
  num_of_nodes--;
  rebalance(ptr);
};

template <typename E>
E& AVLTree<E>::retrieve_helper(ANode<E>* &ptr, const E &elem) {
  // Traverses to the node the matches elem. Returns that value.
  if (is_external(ptr))
    throw invalid_argument("value not in tree");

  if (ptr->data == elem)
    return ptr->data;
  else if (elem < ptr->data)
    return retrieve_helper(ptr->left, elem);
  else if (elem > ptr->data)
    return retrieve_helper(ptr->right, elem);
}

template <typename E>
E& AVLTree<E>::retrieve(const E &elem) {
  // Retrieve the node's data that matches elem. Call to retrieve_helper.
  if (is_empty())
    throw length_error("tree is empty, nothing to retrieve");
  return retrieve_helper(root, elem);
}

template <typename E>
void AVLTree<E>::print_helper(ANode<E>* ptr, traversal_order order) {
  // Performs preorder, inorder, and postorder traversal.
  if (!is_external(ptr)) {
    // Pop off the recursive stack if reached external.

    if (order == preorder) {
      cout << ptr->data << " ";
      print_helper(ptr->left, preorder);
      print_helper(ptr->right, preorder);
    }
    else if (order == inorder) {
      print_helper(ptr->left, inorder);
      cout << ptr->data << " ";
      print_helper(ptr->right, inorder);
    }
    else if (order == postorder) {
      print_helper(ptr->left, postorder);
      print_helper(ptr->right, postorder);
      cout << ptr->data << " ";
    }
  }
}

template <typename E>
void AVLTree<E>::print(traversal_order order) {
  // Print values in the tree in the given order (preorder, inorder, or postorder).
  if (is_empty())
    throw length_error("tree is empty, nothing to print");

  print_helper(root, order);
  cout.flush();  // Flush output buffer to avoid data from not appearing on console.
}

template <typename E>
void AVLTree<E>::remove_all_helper(ANode<E>* &ptr) {
  // Traverses the tree in postorder order and removes the nodes one at a time.
  // Includes external nodes.
  if (ptr != nullptr) {
    // Pop of the recursive stack if reached nullptr.
    remove_all_helper(ptr->left);
    remove_all_helper(ptr->right);
    delete ptr;
    ptr = nullptr;
  }
}

template <typename E>
void AVLTree<E>::remove_all() {
  // Remove all nodes from tree. Call to remove_helper.
  if (is_empty())
    throw length_error("tree is empty, nothing to remove");

  remove_all_helper(root);
  num_of_nodes = 0;
}

#endif