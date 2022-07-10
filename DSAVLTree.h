// Owners - Vedant and Leland
// Edited by Leland and Vedant
//

#ifndef INC_22SU_SEARCH_ENGINE_DSAVLTREE_H
#define INC_22SU_SEARCH_ENGINE_DSAVLTREE_H
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
using std::string;

template<typename K, typename V>
class DSAVLTree {

private:
    class TreeNode {
        // private inner class - node
    public:
        K key;
        V value;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        int height = 0;
    };

    TreeNode* root;

    //private insert functions
    V& insert(TreeNode*& node, const K& k, const V& v);
    V& insert(TreeNode*& node, const K& k);
    // find
    bool find(TreeNode*& node, const K&);

    void Inorder_print(TreeNode* curr, int level) {
        if(curr != nullptr) {
            Inorder_print(curr -> left, level + 1);
            cout << curr->key << ": " << getHeight(curr) << endl;
            Inorder_print(curr -> right, level + 1);
        }
    }
    int getHeight(TreeNode* curr);
    // rotation helper functions for balancing
    void rotateWithRightChild(TreeNode*& k1);
    void rotateWithLeftChild(TreeNode*& k2);
    void doubleWithRightChild(TreeNode*& k1);
    void doubleWithLefTChild(TreeNode*& k3);
    void destroyTree(TreeNode*& node);

public:

    DSAVLTree(): root(nullptr) {}
    K& getRootKey();
    //public insert functions
    V& insert(const K&, const V&);
    V& insert(const K&);
    // overloaded [] operator - like a std map
    V& operator[](const K&);
    bool find(const K&);
    void print() { Inorder_print(root, 0); }
    // destructor
    ~DSAVLTree();
    int size = 0;
    // max for insert and setting heights
    int max(int a, int b) {
        if (a < b)
            return b;
        else
            return a;
    }
};

template<typename K, typename V>
V& DSAVLTree<K, V>::insert(const K& k, const V& v) {
    return insert(root, k, v);
}

template<typename K, typename V>
V& DSAVLTree<K, V>::insert(DSAVLTree<K, V>::TreeNode*& node, const K& k, const V&v) {

    V *temp; // pointer to value we want to return
    if (node == nullptr) {
        size++;
        //time to insert
        node = new DSAVLTree<K, V>::TreeNode();
        node->key = k;
        node->value = v;
        temp = &(node->value); // sets temp to the address of node->value

    }
    if (node->key < k) {
        temp = &(insert(node->right, k)); // temp is equal to the address of what gets returned by the recursive calls
    } else if (k < node->key) {
        temp = &(insert(node->left, k));
    } else {
        temp = &(node->value); // means we found a duplicate, want to return this value by reference

    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1; // sets heights of nodes after inserting
    // balancing logic

    if (getHeight(node->right) - getHeight(node->left) == 2) {
        if (node->right->key < k) {
            // case 4 - rotate with right child
            rotateWithRightChild(node);
        } else {
            // case 3 - double rotate with right child
            doubleWithRightChild(node);
        }
    }
    if (getHeight(node->left) - getHeight(node->right) == 2) {
        // either case 1 or case 2
        if (k < node->left->key) {
            // case 1 - rotate with left child
            rotateWithLeftChild(node);
        } else {
            // case 2 - double rotate with left child
            doubleWithLefTChild(node);
        }
    }
    // create reference to dereferenced pointer
    V &retVal = *temp;
    return retVal;

}

template<typename K, typename V>
int DSAVLTree<K, V>::getHeight(DSAVLTree::TreeNode *curr) {
    if (curr == nullptr){
        return -1;
    }
    else{
        return curr->height;
    }
}

template<typename K, typename V>
void DSAVLTree<K, V>::rotateWithRightChild(DSAVLTree::TreeNode *&k1) {
    TreeNode* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(getHeight(k1->left), getHeight(k1->right))+ 1;
    k2->height = max(getHeight(k2->right), k1->height) + 1;
    k1 = k2;
}

template<typename K, typename V>
void DSAVLTree<K, V>::rotateWithLeftChild(DSAVLTree::TreeNode *&k2) {
    TreeNode* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(getHeight(k2->left), getHeight(k2->right) + 1);
    k1->height = max(getHeight(k1->left), k2->height) + 1;
    k2 = k1;
}

template<typename K, typename V>
void DSAVLTree<K, V>::doubleWithRightChild(DSAVLTree::TreeNode *&k1) {
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template<typename K, typename V>
void DSAVLTree<K, V>::doubleWithLefTChild(DSAVLTree::TreeNode *&k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template<typename K, typename V>
void DSAVLTree<K, V>::destroyTree(DSAVLTree::TreeNode *&node) {
    if(node == nullptr){
        return;
    }
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

template<typename K, typename V>
DSAVLTree<K, V>::~DSAVLTree() {
    destroyTree(root);
    size = 0;
}

template<typename K, typename V>
V& DSAVLTree<K, V>::insert(DSAVLTree::TreeNode *&node, const K &k) {
    V *temp;
    if (node == nullptr) {
        size++;
        //time to insert
        node = new DSAVLTree<K, V>::TreeNode();
        node->key = k;
        temp = &(node->value);

    }
    if (node->key < k) {
        temp = &(insert(node->right, k));
    } else if (k < node->key) {
        temp = &(insert(node->left, k));
    } else {
        temp = &(node->value);

    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    if (getHeight(node->right) - getHeight(node->left) == 2) {
        if (node->right->key < k) {
            // case 4 - rotate with right child
            rotateWithRightChild(node);
        } else {
            // case 3 - double rotate with right child
            doubleWithRightChild(node);
        }
    }
    if (getHeight(node->left) - getHeight(node->right) == 2) {
        // either case 1 or case 2
        if (k < node->left->key) {
            // case 1 - rotate with left child
            rotateWithLeftChild(node);
        } else {
            // case 2 - double rotate with left child
            doubleWithLefTChild(node);
        }
    }
    V &retVal = *temp;
    return retVal;

}

template<typename K, typename V>
V &DSAVLTree<K, V>::insert(const K & k) {
    return insert(root, k);
}

template<typename K, typename V>
V &DSAVLTree<K, V>::operator[](const K & k) {
    return insert(root, k);
}

template<typename K, typename V>
bool DSAVLTree<K, V>::find(DSAVLTree::TreeNode *&node, const K & arg) {

    bool there = false;
    if(node == nullptr) {
        // prevents dereferencing nullptr - means that the key doesn't exist
        return false;
    }
    // found key
    if(node->key == arg) {
        there = true;
    } else if (node->key < arg) {
        there = find(node->right, arg);
    } else if(arg < node->key) {
        there = find(node->left, arg);
    }
    return there;
}

template<typename K, typename V>
bool DSAVLTree<K, V>::find(const K & arg) {
    return find(root, arg);
}

template<typename K, typename V>
K &DSAVLTree<K, V>::getRootKey() {
    return root->key;
}





#endif //INC_22SU_SEARCH_ENGINE_DSAVLTREE_H
