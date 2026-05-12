#include <stdio.h>
#include "rbtree.h"

static rbtreeNode* rotate_left(rbTree *rbt, rbtreeNode* root) {
    rbtreeNode* right = root->right;
    if (root == rbt->root) {
        rbt->root = right;
    }

    right->parent = root->parent;
    if (root->parent && root->parent->left == root) {
        root->parent->left = right;
    } else if (root->parent) {
        root->parent->right = right;
    }

    root->right = right->left;
    if (right->left)
        right->left->parent = root;
    
    right->left = root;
    root->parent = right;

    return right;
}

static rbtreeNode* rotate_right(rbTree *rbt, rbtreeNode* root) {
    rbtreeNode* left = root->left;
    if (root == rbt->root) {
        rbt->root = left;
    }

    left->parent = root->parent;
    if (root->parent && root->parent->right == root) {
        root->parent->right = left;
    } else if (root->parent) {
        root->parent->left = left;
    }
    
    root->left = left->right;
    if (left->right)
        left->right->parent = root;
        
    left->right = root;
    root->parent = left;
    return left;
}

static void rbtree_insert_rebalance(rbTree* rbt, rbtreeNode* node) {
    if (!node->parent) {
        return;
    }
    if (node->parent->color == BLACK) {
        return;
    }
    rbtreeNode* parent = node->parent;
    rbtreeNode* grandfather = parent->parent;
    rbtreeNode* uncle = NULL;
    if (grandfather->right == parent) {
        uncle = grandfather->left;
    }
    else {
        uncle = grandfather->right;
    }
    if (uncle && uncle->color == RED) {
        grandfather->color = RED;
        parent->color = uncle->color = BLACK;
        rbtree_insert_rebalance(rbt, grandfather);
        return;
    }
    if (grandfather->left == parent) {
        if (parent->left == node) { // LL
            parent->color = BLACK;
            grandfather->color = RED;
            rotate_right(rbt, grandfather);
        }
        else { // LR
            node->color = BLACK;
            grandfather->color = RED;
            rotate_left(rbt, parent);
            rotate_right(rbt, grandfather);
        }
    }
    else {
        if (parent->left == node) { // RL
            node->color = BLACK;
            grandfather->color = RED;
            rotate_right(rbt, parent);
            rotate_left(rbt, grandfather);
        }
        else { // RR
            parent->color = BLACK;
            grandfather->color = RED;
            rotate_left(rbt, grandfather);
        }
    }
    return;
}

rbtreeNode* rbtree_insert(rbTree* rbt, size_t value) {
    rbtreeNode* cur = rbt->root;
    rbtreeNode* parent = NULL;
    while (cur) {
        parent = cur;
        if (value > cur->val) {
            cur = cur->right;
        }
        else if (value < cur->val) {
            cur = cur->left;
        }
        else {
            cur->count += 1;
            return rbt->root;
        }
    }
    rbtreeNode* node = (rbtreeNode*) malloc(sizeof(rbtreeNode));
    *node = (rbtreeNode) {
        .val = value,
        .color = RED,
        .count = 1,
        .left = NULL,
        .right = NULL,
        .parent = NULL
    };
    if (parent == NULL) {
        rbt->root = node;
        node->color = BLACK;
        return node;
    }
    if (parent->val > value) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    node->parent = parent;
    rbtree_insert_rebalance(rbt, node);
    rbt->root->color = BLACK;
    return rbt->root;
}

static rbtreeNode* successor(rbtreeNode* node) {
    rbtreeNode* cur = node->right;
    if (!cur) {
        return node;
    }
    while (cur->left != NULL) {
        cur = cur->left;
    }
    return cur;
}

static rbtreeNode* predecessor(rbtreeNode* node) {
    rbtreeNode* cur = node->left;
    if (!cur) {
        return node;
    }
    while (cur->right != NULL) {
        cur = cur->right;
    }
    return cur;
}

static void rbtree_delete_rebalance(rbTree* rbt, rbtreeNode* child, rbtreeNode* parent) {
    if (child) { // this child must be RED
        child->color = BLACK;
        return;
    }
    #define LEFT 1
    #define RIGHT 0
    rbtreeNode* brother = (child == parent->right) ? parent->left : parent->right;
    int brother_is_left = (child == parent->right) ? LEFT : RIGHT;
    if (brother->color == BLACK) {
        enum Color color = parent->color;
        rbtreeNode* left = brother->left;
        rbtreeNode* right = brother->right;
        if (left && left->color == RED) {
            if (brother_is_left) { // LL
                rbtreeNode* node = rotate_right(rbt, parent);
                node->color = color;
                if (node->left) node->left->color = BLACK;
                if (node->right) node->right->color = BLACK;
            }
            else { // RL
                rotate_right(rbt, brother);
                rbtreeNode* node = rotate_left(rbt, parent);
                node->color = color;
                if (node->left) node->left->color = BLACK;
                if (node->right) node->right->color = BLACK;
            }
        }
        else if (right && right->color == RED) {
            if (brother_is_left) { // LR
                rotate_left(rbt, brother);
                rbtreeNode* node = rotate_right(rbt, parent);
                node->color = color;
                if (node->left) node->left->color = BLACK;
                if (node->right) node->right->color = BLACK;
            }
            else { // RR
                rbtreeNode* node = rotate_left(rbt, parent);
                node->color = color;
                if (node->left) node->left->color = BLACK;
                if (node->right) node->right->color = BLACK;
            }
        }
        else if (!(left && left->color == RED) && !(right && right->color == RED)) { // NO RED
            if (parent->color == RED) {
                parent->color = BLACK;
                brother->color = RED;
            }
            else {
                brother->color = RED;
                rbtree_delete_rebalance(rbt, parent, parent->parent); 
            }
        }
    }
    else {
        brother->color = BLACK;
        parent->color = RED;
        if (brother_is_left) {
            rotate_right(rbt, parent);
            rbtree_delete_rebalance(rbt, parent->right, parent);
        }
        else {
            rotate_left(rbt, parent);
            rbtree_delete_rebalance(rbt, parent->left, parent);
        }
    }
    #undef LEFT
    #undef RIGHT
}

rbtreeNode* rbtree_delete(rbTree* rbt, size_t value) {
    rbtreeNode* cur = rbt->root;
    while (cur) {
        if (value > cur->val) {
            cur = cur->right;
        }
        else if (value < cur->val) {
            cur = cur->left;
        }
        else {
            if (cur->count > 1) {
                cur->count--;
                return rbt->root;
            }
            else break;
        }
    }
    if (cur == NULL) {
        return NULL;
    }

    rbtreeNode* del;
    rbtreeNode* child;
    rbtreeNode* parent;
    enum Color original_color = RED;
    if (cur->left == NULL || cur->right == NULL) {
        del = cur;
        child = del->left ? del->left : del->right;
    }
    else {
        rbtreeNode* suc = successor(cur);
        cur->val = suc->val;
        del = suc;
        child = del->right;
    }
    original_color = del->color;
    parent = del->parent;

    if (del->parent == NULL) {
        rbt->root = child;
    }
    else if (del == parent->left) {
        parent->left = child;
    }
    else {
        parent->right = child;
    }
    if (child) child->parent = parent;

    free(del);

    if (original_color == BLACK) {
        rbtree_delete_rebalance(rbt, child, parent);
    }

    if (rbt->root) 
        rbt->root->color = BLACK;
    return rbt->root;
}