// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <cassert>

using namespace std;

// copy constructor for BSTMap
BSTMap::BSTMap(const BSTMap &bst) {
  // Create a new node with data from the source BST's root
  Node *tempNode = new Node(nullptr, nullptr, bst.root->data);
  this->root = tempNode;
  copyConstructorHelper(this->root, bst.root);
}

// Helper function for the copy constructor
void BSTMap::copyConstructorHelper(Node *nodeOne, Node *nodeTwo) {
  // Return if the source node has no left or right child
  if (nodeTwo->left == nullptr && nodeTwo->right == nullptr) {
    return;
  }
  // If the source node has a left child, create a new node with its data and
  // set it as the left child of the destination node
  if (nodeTwo->left != nullptr) {
    Node *tempNode = new Node(nullptr, nullptr, nodeTwo->left->data);
    nodeOne->left = tempNode;
    copyConstructorHelper(nodeOne->left, nodeTwo->left);
  }
  // If the source node has a right child, create a new node with its data and
  // set it as the right child of the destination node
  if (nodeTwo->right != nullptr) {
    Node *tempNode = new Node(nullptr, nullptr, nodeTwo->right->data);
    nodeOne->right = tempNode;
    copyConstructorHelper(nodeOne->right, nodeTwo->right);
  }
}

// Constructor for BSTMap that takes an array of length n
// and creates a tree with minimum height using a helper function similar to
// rebalance.
BSTMap::BSTMap(const vector<value_type> &v) {
  vector<value_type> nodes = v;
  size_t num = nodes.size() - 1;
  int numInt = static_cast<int>(num);
  treeConstructionHelper(root, nodes, 0, numInt);
}

BSTMap::Node::Node(Node *left, Node *right, value_type &pair) {
  this->data = pair;
  this->left = left;
  this->right = right;
}

// destructor
BSTMap::~BSTMap() { clear(); }

// delete all nodes in tree
void BSTMap::clear() {
  clearHelper(root);
  this->root = nullptr;
  totalMapNodes = 0;
}

void BSTMap::clearHelper(Node *node) {
  // Base case
  if (node == nullptr) {
    return;
  }
  clearHelper(node->left);
  clearHelper(node->right);
  delete node;
  node = nullptr;
}

// true if no nodes in BST
bool BSTMap::empty() const { return (root == nullptr); }

// Number of nodes in BST
int BSTMap::size() const { return totalMapNodes; }

// true if item is in BST
bool BSTMap::contains(const key_type &key) const {
  mapped_type map;
  return containsHelper(map, this->root, key);
}

// helper function for the contains function
// If the key is found in the current node, update the mapped value and return
// true Recursively call containsHelper on the left and right child of the node
// to search for the key in the subtrees
bool BSTMap::containsHelper(mapped_type &map, Node *node,
                            const key_type &key) const {
  // Base case
  if (node == nullptr) {
    return false;
  }
  if (node->data.first == key) {
    map = node->data.second;
    return true;
  }
  return containsHelper(map, node->left, key) ||
         containsHelper(map, node->right, key);
}

// If k matches the key returns a reference to its value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const key_type &key) {

  // If the tree is empty, create a new node with the given key as the root
  if (this->root == nullptr) {
    value_type newValue;
    newValue.first = key;
    Node *newNode = new Node(nullptr, nullptr, newValue);
    this->root = newNode;
    return this->root->data.second;
  }

  // Traverse the tree to find the node with the given key or the appropriate
  // position to insert a new node with the given key
  Node *curr = this->root;
  while (curr != nullptr) {

    if (curr->data.first == key) {
      return curr->data.second;
    }

    if (key < curr->data.first) {

      if (curr->left == nullptr) {
        value_type newValue;
        newValue.first = key;
        Node *newNode = new Node(nullptr, nullptr, newValue);
        curr->left = newNode;
        return curr->left->data.second;
      }
      curr = curr->left;
    } else {
      if (curr->right == nullptr) {
        value_type newValue;
        newValue.first = key;
        Node *newNode = new Node(nullptr, nullptr, newValue);
        curr->right = newNode;
        return curr->right->data.second;
      }
      curr = curr->right;
    }
  }
  exit(1);
}

// retrieves all of the necessary values into the vector using a key_type
vector<BSTMap::value_type> BSTMap::getAll(const key_type &key) const {
  vector<value_type> v;
  getAllHelper(this->root, key, v);
  return v;
}

// helper function for the getAll function
void BSTMap::getAllHelper(Node *node, const key_type &key,
                          vector<value_type> &v) const {
  if (node == nullptr) {
    return;
  }
  getAllHelper(node->left, key, v);
  getAllHelper(node->right, key, v);
  if (node->data.first.find(key) != string::npos) {
    v.push_back(node->data);
  }
}

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const { return getHeight(this->root); }

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const Node *n) {
  if (n == nullptr) {
    return 0;
  }
  int left = getHeight(n->left);
  int right = getHeight(n->right);
  if (right < left) {
    return left + 1;
  }
  return right + 1;
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const string &k) const {
  if (this->contains(k)) {
    return 1;
  }
  return 0;
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const value_type &value)) const {
  this->inOrderHelper(this->root, visit);
}

// helper function for the inorder function
void BSTMap::inOrderHelper(Node *node,
                           void visit(const value_type &value)) const {
  if (node == nullptr) {
    return;
  }
  inOrderHelper(node->left, visit);
  visit(node->data);
  inOrderHelper(node->right, visit);
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const value_type &value)) const {
  preOrderHelper(root, visit);
}

// helper function for the preOrder function
void BSTMap::preOrderHelper(Node *node,
                            void visit(const value_type &value)) const {
  if (node == nullptr) {
    return;
  }
  visit(node->data);
  preOrderHelper(node->left, visit);
  preOrderHelper(node->right, visit);
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const value_type &item)) const {
  postOrderHelper(root, visit);
}

// helper function for the postOrder function
void BSTMap::postOrderHelper(Node *node,
                             void visit(const value_type &item)) const {
  if (node == nullptr) {
    return;
  }
  postOrderHelper(node->left, visit);
  postOrderHelper(node->right, visit);
  visit(node->data);
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  if (root == nullptr) {
    return;
  }
  vector<value_type> nodes;
  rebalanceHelper(nodes, root);
  this->clear();
  treeConstructionHelper(this->root, nodes, 0,
                         static_cast<int>(nodes.size()) - 1);
}

// helper function for the rebalance function, puts the node in
// inorder traversal
void BSTMap::rebalanceHelper(vector<value_type> &nodes, Node *node) {
  if (node == nullptr) {
    return;
  }
  rebalanceHelper(nodes, node->left);
  nodes.push_back(node->data);
  rebalanceHelper(nodes, node->right);
}

// helper function for the rebalance function, creates a tree from the vector
// nodesToVector created
void BSTMap::treeConstructionHelper(Node *tempNode, vector<value_type> &node,
                                    int begin, int end) {
  if (begin == end || end < 0) {
    (*this)[node[begin].first] = node[begin].second;
    return;
  }
  int middle = begin + static_cast<int>(end - begin) / 2;
  if (end - begin != 1) {
    (*this)[node[middle].first] = node[middle].second;
    treeConstructionHelper(tempNode, node, begin, middle - 1);
    treeConstructionHelper(tempNode, node, middle + 1, end);
  } else {
    (*this)[node[begin].first] = node[begin].second;
    if (begin != end) {
      (*this)[node[end].first] = node[end].second;
    }
  }
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  if (this->root == nullptr && other.root == nullptr) {
    return true;
  }
  if (this->root == nullptr || other.root == nullptr) {
    return false;
  }
  return operatorDoubleEqualHelper(root, other.root);
}

// helper function for the operator== function
bool BSTMap::operatorDoubleEqualHelper(Node *nodeOne, Node *nodeTwo) const {
  if (nodeOne == nullptr && nodeTwo == nullptr) {
    return true;
  }
  if (nodeOne == nullptr || nodeTwo == nullptr) {
    return false;
  }
  return (nodeOne->data == nodeTwo->data) &&
         (operatorDoubleEqualHelper(nodeOne->left, nodeTwo->left)) &&
         (operatorDoubleEqualHelper(nodeOne->right, nodeTwo->right));
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const {
  return (!(*this == other));
}