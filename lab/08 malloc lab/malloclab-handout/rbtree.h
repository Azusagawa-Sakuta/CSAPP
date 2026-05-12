#include <stddef.h>

enum Color {
    RED = 0, BLACK
};

typedef struct rbtreeNode {
    size_t val;
    size_t count;
    rbtreeNode* left;
    rbtreeNode* right;
    rbtreeNode* parent;
    enum Color color;
} rbtreeNode;

typedef struct {
    rbtreeNode* root;
} rbTree;

/*
 * Insert into rbtree.
 */
rbtreeNode* rbtree_insert(rbTree* rbt, size_t value);

/*
 * Delete from rbtree.
 */
rbtreeNode* rbtree_delete(rbTree* rbt, size_t value);