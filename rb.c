#include <stdio.h>
#include <stdlib.h>

// Red-Black Tree Node
typedef struct Node {
    int data;
    char color; // 'R' for Red, 'B' for Black
    struct Node *left, *right, *parent;
} Node;

Node *root = NULL;

// Function Prototypes
Node* createNode(int data);
void rotateLeft(Node **root, Node *x);
void rotateRight(Node **root, Node *y);
void fixInsertion(Node **root, Node *z);
void insert(Node **root, int data);
void inorderTraversal(Node *root);

Node* minimum(Node* node);

Node* search(Node *root, int key);

// Create a new node
Node* createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = 'R'; // New node is always red
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Left rotate
void rotateLeft(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;

    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right rotate
void rotateRight(Node **root, Node *y) {
    Node *x = y->left;
    y->left = x->right;

    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;

    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Fix violations after insertion
void fixInsertion(Node **root, Node *z) {
    Node *parent = NULL;
    Node *grandparent = NULL;

    while ((z != *root) && (z->color != 'B') && (z->parent->color == 'R')) {
        parent = z->parent;
        grandparent = parent->parent;

        // Parent is left child of grandparent
        if (parent == grandparent->left) {
            Node *uncle = grandparent->right;

            // Case 1: Uncle is red, only recoloring
            if (uncle != NULL && uncle->color == 'R') {
                grandparent->color = 'R';
                parent->color = 'B';
                uncle->color = 'B';
                z = grandparent;
            } else {
                // Case 2: z is right child, rotate left
                if (z == parent->right) {
                    rotateLeft(root, parent);
                    z = parent;
                    parent = z->parent;
                }

                // Case 3: z is left child, rotate right
                rotateRight(root, grandparent);
                char tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                z = parent;
            }
        } else { // Parent is right child of grandparent
            Node *uncle = grandparent->left;

            // Case 1: Uncle is red, only recoloring
            if (uncle != NULL && uncle->color == 'R') {
                grandparent->color = 'R';
                parent->color = 'B';
                uncle->color = 'B';
                z = grandparent;
            } else {
                // Case 2: z is left child, rotate right
                if (z == parent->left) {
                    rotateRight(root, parent);
                    z = parent;
                    parent = z->parent;
                }

                // Case 3: z is right child, rotate left
                rotateLeft(root, grandparent);
                char tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                z = parent;
            }
        }
    }
    (*root)->color = 'B'; // Root should always be black
}

// Insert into Red-Black Tree
void insert(Node **root, int data) {
    Node *newNode = createNode(data);
    Node *y = NULL;
    Node *x = *root;

    while (x != NULL) {
        y = x;
        if (newNode->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newNode->parent = y;

    if (y == NULL) {
        *root = newNode;
    } else if (newNode->data < y->data) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    fixInsertion(root, newNode);
}

// In-order Traversal
void inorderTraversal(Node *root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%d(%c) ", root->data, root->color);
    inorderTraversal(root->right);
}

// Helper function to find the minimum node
Node* minimum(Node* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}
// Helper function to replace two nodes
void replaceNode(Node *u, Node *v) {
    if (u->parent == NULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Fix violations after deletion
void fixDeletion(Node **root, Node *x) {
    while (x != *root && x->color == 'B') {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            // Case 1: Sibling is red
            if (w->color == 'R') {
                w->color = 'B';
                x->parent->color = 'R';
                rotateLeft(root, x->parent);
                w = x->parent->right;
            }
            // Case 2: Both children of sibling are black
            if (w->left->color == 'B' && w->right->color == 'B') {
                w->color = 'R';
                x = x->parent;
            } else {
                // Case 3: Sibling's right child is black
                if (w->right->color == 'B') {
                    w->left->color = 'B';
                    w->color = 'R';
                    rotateRight(root, w);
                    w = x->parent->right;
                }
                // Case 4: Sibling's right child is red
                w->color = x->parent->color;
                x->parent->color = 'B';
                w->right->color = 'B';
                rotateLeft(root, x->parent);
                x = *root;
            }
        } else {
            Node *w = x->parent->left;
            // Case 1: Sibling is red
            if (w->color == 'R') {
                w->color = 'B';
                x->parent->color = 'R';
                rotateRight(root, x->parent);
                w = x->parent->left;
            }
            // Case 2: Both children of sibling are black
            if (w->right->color == 'B' && w->left->color == 'B') {
                w->color = 'R';
                x = x->parent;
            } else {
                // Case 3: Sibling's left child is black
                if (w->left->color == 'B') {
                    w->right->color = 'B';
                    w->color = 'R';
                    rotateLeft(root, w);
                    w = x->parent->left;
                }
                // Case 4: Sibling's left child is red
                w->color = x->parent->color;
                x->parent->color = 'B';
                w->left->color = 'B';
                rotateRight(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = 'B';
}

// Helper function to delete a node
Node* deleteNodeHelper(Node *root, Node *z) {
    Node *y = z;
    Node *x;
    char originalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        replaceNode(z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        replaceNode(z, z->left);
    } else {
        y = minimum(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != NULL) {
                x->parent = y;
            }
        } else {
            replaceNode(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        replaceNode(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (originalColor == 'B') {
        fixDeletion(&root, x);
    }

    return root;
}
Node* search(Node *root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }

    if (key < root->data) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

// Deletion function
Node* deleteNode(Node *root, int data) {
    Node *z = search(root, data);
    if (z == NULL) {
        printf("Node with data %d not found in the tree.\n", data);
        return root;
    }
    return deleteNodeHelper(root, z);
}


int main() {
    insert(&root, 20);
    insert(&root, 15);
    insert(&root, 25);
    insert(&root, 10);
    insert(&root, 5);
    insert(&root, 30);
    insert(&root, 35);

    printf("In-order Traversal before Deletion:\n");
    inorderTraversal(root);
    printf("\n");

    // Deleting a node
    root = deleteNode(root, 25);

    printf("In-order Traversal after Deletion of 25:\n");
    inorderTraversal(root);
    printf("\n");

    return 0;
}

