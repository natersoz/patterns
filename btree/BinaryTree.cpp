/**
 * @file BinaryTree.cpp
 */

#include <cstdint>
#include <iostream>
#include <iomanip>
#include "BinaryTree.h"

using namespace std;

Node const Node::sentinal = Node(UINT16_MAX, UINT16_MAX);

Node::Node(unsigned int row, unsigned int column) :
    data(0), leftChild(0), rightChild(0), rightSibling(0)
{
    data = (row & UINT16_MAX);
    data <<= 16;
    data |= (column & UINT16_MAX);
}

static Node *FindLeftMostChild(Node *node, unsigned int depth)
{
    while (depth > 0) {
        if (node->leftChild) {
            node = node->leftChild;
            depth -= 1;
        } else if (node->rightChild) {
            node = node->rightChild;
            depth -= 1;
        } else {
            node = 0;       // No sibling found, return null.
            break;
        }
    }

    return node;
}

#if 0
static bool volatile break_point = false;

static void Breakpoint()
{
    break_point = true;
}
#endif

// node is the parent seeking the sibling for its child.
// Note: node's right sibling pointer is set to its
// parent when making this function call.
static Node* FindRightSiblingForChild(Node *node)
{
    // Since we are finding the sibling for a child, and we are passing
    // in the child's parent node, the depth begins at value '1'.
    // That is: the parent is one up from the child.
    unsigned int depth = 0;
    Node *siblingFound = 0;

    // Iterate up to the next parent which has a right child
    // but not past the root.
    while (node->rightSibling && !siblingFound) {
        Node const *node_prev = node;
        node = node->rightSibling;          // Iterate up the tree.
        depth += 1;

        if (node->rightChild && (node->rightChild != node_prev)) {
            // Since we are referencing the rigth node when making the
            // call we are pre-decrementing the depth by 1; thus depth - 1.
            siblingFound = FindLeftMostChild(node->rightChild, depth - 1);
        }
    }

    return siblingFound? siblingFound : const_cast<Node*>(&Node::sentinal);
}

void TransformBinaryTree(Node *root)
{
    Node *node = root;
    while (node) {
        // If the right sibling pointer is set to null then it means
        // we have yet to iterate into that node.
        // We are overloading the right sibling pointer as the pointer
        // to the parent on the iteration into the children.
        // This allows us to iterate up from the children back to the parent.
        // i.e, node-rs = 0 becomes node->rs = parent.
        if (node->leftChild && !node->leftChild->rightSibling) {
            node->leftChild->rightSibling = node;
            node = node->leftChild;
        } else if (node->rightChild && !node->rightChild->rightSibling) {
            node->rightChild->rightSibling = node;
            node = node->rightChild;
        } else {
            Node *parent = node->rightSibling;
            node->rightSibling = FindRightSiblingForChild(node);
            node = parent;
        }
    }
}

void ForEachBinaryTree(Node const *root, void (* func)(Node const *node))
{
    if (root->leftChild) {
        ForEachBinaryTree(root->leftChild, func);
    }

    func(root);

    if (root->rightChild) {
        ForEachBinaryTree(root->rightChild, func);
    }
}

void ForEachBinaryTree(Node *root, void (* func)(Node *node))
{
    if (root->leftChild) {
        ForEachBinaryTree(root->leftChild, func);
    }

    func(root);

    if (root->rightChild) {
        ForEachBinaryTree(root->rightChild, func);
    }
}

static void DeleteNode(Node *node)
{
    delete node;
}

void RemoveTree(Node *node)
{
    ForEachBinaryTree(node, DeleteNode);
}

std::ostream& operator << (std::ostream& os, Node const* node)
{
    if (node == &Node::sentinal) {
        os << "sentinal";
    } else if (node) {
        os << "(";
        os << setbase(10) << setw(3) << NodeTestValueGetRow(node->data);
        os << ", ";
        os << setbase(10) << setw(3) << NodeTestValueGetCol(node->data);
        os << ")";
    } else {
        os << "null";
    }

    return os;
}

unsigned int NodeTestValueGetRow(int32_t data)
{
    return (static_cast<unsigned int>(data) >> 16) & UINT16_MAX;
}

unsigned int NodeTestValueGetCol(int32_t data)
{
    return (static_cast<unsigned int>(data) >>  0) & UINT16_MAX;
}

