/**
 * @file BinaryTree.h
 */

#pragma once
#include <ostream>

/**
 * @class Node
 * @see Assignment.
 * Traverse through the nodes at level N-1 horizontally, to link the children,
 * which are at level N, horizontally.
 * Move to the next level down and repeat the above step until all levels have
 * been transformed.
 *
 * @note I was going to leave the original class Node untouched.
 * But since it was non-functional as is, why not clean things up
 * a little. No apologies.
 */
class Node
{
public:
    /// This would be a payload or pointer/reference to some kind of
    /// content in a real-world tree that had a true purpose.
    /// We will now use it as a test payload:
    /// The upper 16 bits will be the row of the node
    /// The lower 16 bits will be the column of the node.
    int data;

    Node *leftChild;
    Node *rightChild;

    /// before transformaton all rightSibling nodes are null
    /// After transformation, only the right most node in each level will have
    /// rightSibling null.
    /// Of course, root will have rightSibling null always.
    Node *rightSibling;

    /**
     * The constructor used by the test code. Create a node for a given
     * row and column in the tree.
     *
     * @param row The row in the tree where this node is expected to be inserted.
     * @param column The column in the tree where this node is expected to be inserted.
     */
    Node(unsigned int row, unsigned int column);

    static Node const sentinal;

    // Hide the default ctor, copy ctor and assignment operation
    // to protect ourselves from doing something stupid.
private:
    Node();
    Node(Node *node);
    Node& operator = (Node&);
};

/**
 * The assignment as described in the word document:
 * Traverse through the nodes at level N-1 horizontally, to link the children,
 * which are at level N, horizontally.
 * Move to the next level down and repeat the above step until all levels have
 * been transformed.
 *
 * @param root The tree to transform.
 */
void TransformBinaryTree(Node *root);

/**
 * Classical walking of a binary tree performing a function on each node
 * in sorted order (assuming a sorted binary tree).
 * Not part of requirements for this project but useful for test.
 *
 * @param root The root node to start iteration.
 * @param func The operation to perform on each node.
 */
void ForEachBinaryTree(Node const *root, void (*func)(Node const *node));
void ForEachBinaryTree(Node       *root, void (*func)(Node       *node));

/**
 * Remove and delete all node elements from the node specified (inclusive) and below.
 * This function frees all memory associated with these nodes.
 *
 * @warning If a node of a larger tree references the tree being deleted then
 * the attached node must be modified by hand (since the sub-tree does not
 * reference the outter tree).
 *
 * @param node The top level node to delete.
 */
void RemoveTree(Node *node);

/**
 * Output the Node class to an output stream.
 *
 * @param os The stream to print to.
 * @param node The node to print.
 *
 * @return std::ostream& The reference to the output stream that was printed upon..
 */
std::ostream& operator << (std::ostream& os, Node const *node);

/**
 * Get the Row value from the data member of the Node class.
 *
 * @param data The Node::data field to extract the row value from
 *
 * @return unsigned int The row value for this node.
 */
unsigned int NodeTestValueGetRow(int data);

/**
 * Get the Column value from the data member of the Node class.
 *
 * @param data The Node::data field to extract the column value from
 *
 * @return unsigned int The column value for this node.
 */
unsigned int NodeTestValueGetCol(int data);

