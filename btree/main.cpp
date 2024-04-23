/**
 * main.cpp
 * Test application for string reversal for Synapse.
 */

#include <cstdint>
#include <iostream>
#include "BinaryTree.h"

static bool const print_debug = false;

using namespace std;

static void AddNodeToTree(Node *root, Node *node)
{
    static unsigned int const left = 0;

    unsigned int const row = NodeTestValueGetRow(node->data);
    unsigned int const col = NodeTestValueGetCol(node->data);

    // Shift The direction bit over the value of the
    // column telling the iterator which way to turn,
    // left (0) or right (1) as we iterate down the tree.
    // The goal is to point node_iter at the parent node
    // to which we will attach this node as a child node.
    Node *node_iter = root;
    unsigned int direction = 1U << (row - 1);
    for ( ; direction != 1U; direction >>= 1) {
        if ((direction & col) == left) {
            node_iter = node_iter->leftChild;
        } else {
            node_iter = node_iter->rightChild;
        }
    }

    if ((direction & col) == left) {
        node_iter->leftChild = node;
    } else {
        node_iter->rightChild = node;
    }
}

static Node* TestTreeCreate(unsigned int depth)
{
    unsigned int row = 0;
    unsigned int col = 0;
    Node *root = new Node(row, col);

    // Iterate through the depth.
    // The root node sits at row 0; so begin with 1 for all other nodes to add.
    for (row = 1; row < depth; ++row) {
        // The width of a full tree layer is 2 ^ depth;
        unsigned int const width = 1U << row;

        // Iterate over the width of this layer of the test tree.
        for (col = 0; col < width; ++col) {
            Node *node = new Node(row, col);
            AddNodeToTree(root, node);
        }
    }

    return root;
}

void NodePrint(Node const *node)
{
    cout << node;
    cout << " -> ";
    cout << node->rightSibling;
    cout << endl;
}

static void TreePrint(Node const *root)
{
    ForEachBinaryTree(root, NodePrint);
    cout << endl;
}

/// @todo Encapsulate in a test class.
static bool test_result = false;

static void CheckBinaryTreeSiblingOrder(Node *node)
{
    test_result = true;

    for ( ; test_result && (node->rightSibling != &Node::sentinal); node = node->rightSibling) {

        unsigned int const row = NodeTestValueGetRow(node->data);
        unsigned int const col = NodeTestValueGetCol(node->data);

        unsigned int const row_sibling = NodeTestValueGetRow(node->rightSibling->data);
        unsigned int const col_sibling = NodeTestValueGetCol(node->rightSibling->data);

        test_result = (row == row_sibling) && (col < col_sibling);
        if (!test_result) {
            cout << "inconsistent:";
            cout << "(" << row << ", " << col_sibling << ")" << " ";
            cout << "(" << row << ", " << col_sibling << ")" << endl;
        }
    }
}

/**
 * For every node in the tree check to see that it results
 * in a sibling list for which each link to the right has
 * an increasing value in the column value and the same
 * value in the row value.
 *
 * @param root The tree root.
 *
 * @return bool true if pass, false if fail.
 */
static bool CheckBinaryTree(Node *root)
{
    ForEachBinaryTree(root, CheckBinaryTreeSiblingOrder);
    return test_result;
}

// Check a full tree, 4 deep (no deletions).
bool test_0()
{
    Node *root = TestTreeCreate(4);
    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 0:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);

    return result;
}

// Check a tree 4 deep, with the 3 left most nodes deleted.
// Subtree located at (2,0) is removed prior to transformation.
bool test_1()
{
    Node *root = TestTreeCreate(4);

    Node *remove = root->leftChild->leftChild;
    root->leftChild->leftChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 1:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 4 deep, with the 3 middle lower nodes deleted
// Subtree located at (2,1) is removed prior to transformation.
bool test_2()
{
    Node *root = TestTreeCreate(4);

    Node *remove = root->leftChild->rightChild;
    root->leftChild->rightChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 2:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 4 deep, with the 3 middle lower nodes deleted
// Subtree located at (2,2) is removed prior to transformation.
bool test_3()
{
    Node *root = TestTreeCreate(4);

    Node *remove = root->rightChild->leftChild;
    root->rightChild->leftChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 3:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 4 deep, with the 3 middle right most nodes deleted
// Subtree located at (2,3) is removed prior to transformation.
bool test_4()
{
    Node *root = TestTreeCreate(4);

    Node *remove = root->rightChild->rightChild;
    root->rightChild->rightChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 4:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 4 deep, removing all inner nodes.
// Subtrees located at (2,1) and (2,2) are removed prior to transformation.
bool test_5()
{
    Node *root = TestTreeCreate(4);

    Node *remove = root->leftChild->rightChild;
    root->leftChild->rightChild = 0;
    RemoveTree(remove);

    remove = root->rightChild->leftChild;
    root->rightChild->leftChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 5:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 1 deep, just the root node.
bool test_6()
{
    Node *root = TestTreeCreate(1);
    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 6:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

// Check a tree 6 deep
// Subtrees located at (2,1), (3,4), (4,11) are removed prior to transformation.
bool test_7()
{
    Node *root = TestTreeCreate(6);

    Node *remove = root->leftChild->rightChild;                     // (2,1)
    root->leftChild->rightChild = 0;
    RemoveTree(remove);

    remove = root->rightChild->leftChild->leftChild;                // (3,4)
    root->rightChild->leftChild->leftChild = 0;
    RemoveTree(remove);

    remove = root->rightChild->leftChild->rightChild->rightChild;   // (4,11)
    root->rightChild->leftChild->rightChild->rightChild = 0;
    RemoveTree(remove);

    TransformBinaryTree(root);

    if (print_debug) {
        cout << "tree 7:" << endl;
        TreePrint(root);
    }

    bool const result = CheckBinaryTree(root);
    RemoveTree(root);
    return result;
}

int main(int argc, char *argv[])
{
    bool result = false;
    result = test_0();
    cout << "test 0: " << result << endl;

    result = test_1();
    cout << "test 1: " << result << endl;

    result = test_2();
    cout << "test 2: " << result << endl;

    result = test_3();
    cout << "test 3: " << result << endl;

    result = test_4();
    cout << "test 4: " << result << endl;

    result = test_5();
    cout << "test 5: " << result << endl;

    result = test_6();
    cout << "test 6: " << result << endl;

    result = test_7();
    cout << "test 7: " << result << endl;
}
