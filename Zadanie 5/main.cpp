#include <iostream>

using namespace std;

enum Color
{
    RED,
    BLACK
};

struct Node
{
    int data;
    Color color;
    Node *left, *right, *parent;

    Node(int data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree
{
private:
    Node *root;

    void rotateLeft(Node *&ptr)
    {
        Node *rightChild = ptr->right;
        ptr->right = rightChild->left;

        if (ptr->right)
            ptr->right->parent = ptr;

        rightChild->parent = ptr->parent;

        if (!ptr->parent)
            root = rightChild;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = rightChild;
        else
            ptr->parent->right = rightChild;

        rightChild->left = ptr;
        ptr->parent = rightChild;
    }

    void rotateRight(Node *&ptr)
    {
        Node *leftChild = ptr->left;
        ptr->left = leftChild->right;

        if (ptr->left)
            ptr->left->parent = ptr;

        leftChild->parent = ptr->parent;

        if (!ptr->parent)
            root = leftChild;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = leftChild;
        else
            ptr->parent->right = leftChild;

        leftChild->right = ptr;
        ptr->parent = leftChild;
    }

    void fixViolation(Node *&ptr)
    {
        Node *parent = nullptr;
        Node *grandparent = nullptr;

        while (ptr != root && ptr->color == RED && ptr->parent->color == RED)
        {
            parent = ptr->parent;
            grandparent = parent->parent;

            if (parent == grandparent->left)
            {
                Node *uncle = grandparent->right;

                if (uncle && uncle->color == RED)
                {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    ptr = grandparent;
                }
                else
                {
                    if (ptr == parent->right)
                    {
                        rotateLeft(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
            else
            {
                Node *uncle = grandparent->left;

                if (uncle && uncle->color == RED)
                {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    ptr = grandparent;
                }
                else
                {
                    if (ptr == parent->left)
                    {
                        rotateRight(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void preorderPrint(Node *ptr)
    {
        if (ptr == nullptr)
            return;
        cout << ptr->data << " ";
        preorderPrint(ptr->left);
        preorderPrint(ptr->right);
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(const int &data)
    {
        Node *newNode = new Node(data);
        Node *parent = nullptr;
        Node *ptr = root;

        while (ptr)
        {
            parent = ptr;
            if (data < ptr->data)
                ptr = ptr->left;
            else
                ptr = ptr->right;
        }

        newNode->parent = parent;
        if (!parent)
            root = newNode;
        else if (data < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;

        fixViolation(newNode);
    }

    void preorder()
    {
        preorderPrint(root);
        cout << endl;
    }
};

int main()
{
    RedBlackTree rbTree;
    int n;

    cout << "Podaj liczbe elementow: ";
    cin >> n;

    cout << "Podaj " << n << " liczb calkowitych:\n";
    for (int i = 0; i < n; ++i)
    {
        int num;
        cin >> num;
        rbTree.insert(num);
    }

    cout << "Porzadek preorder BST:\n";
    rbTree.preorder();

    return 0;
}
