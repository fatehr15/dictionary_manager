/* src.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "bst.h"
#include "queue.h"
#include "stack.h"


/* --- BST Operations --- */
static TreeNode *create_tnode (const char *word,const char *syn , const char *ant ){
    TreeNode *n =malloc(sizeof *n) ;
    n->word= strdup (word)  ;
    n->syn= strdup(syn) ;
    n->ant=strdup(ant) ;
    n->num_chars= strlen(word) ;
    n->num_vowels=count_vowels(word) ;
    n->left= NULL ;
    n->right= NULL ;
    return n ;
}

static TTree *init_tree(void) {
    TTree *tr = malloc (sizeof *tr) ;
    tr->root = NULL ;
    tr->size = 0 ;
    return tr ;
}

TTree *toTree(TStack *stk) {
    TTree *tr = init_tree() ;
    for (SNode *p = stk->top; p; p = p->next){
        tr = AddWordBST(tr, p->word, p->syn, p->ant) ;
    }
    return tr ;
}

TTree *fillTree(FILE *f) {
    TTree *tr =init_tree();
    char w[256],s[256],a[256];
    while (fscanf( f ,"%255s %255s %255s",w,s,a)==  3){
        tr = AddWordBST(tr, w, s, a) ;
    }
    return tr ;
}

TreeNode *getInfWordTree(TTree *tr, const char *word) {
    TreeNode *p = tr->root;
    while (p) {
        int cmp = strcmp(word, p->word);
        if (cmp == 0) return p;
        p = cmp < 0 ? p->left : p->right;
    }
    return NULL ;
}

static TreeNode *make_node(const char *word, const char *syn, const char *ant) {
    TreeNode *n = malloc(sizeof *n);
    n->word  = strdup(word);
    n->syn   = strdup(syn);
    n->ant   = strdup(ant);
    n->left = n->right = NULL;
    return n;
}

TreeNode *insert_node(TreeNode *root,const char *word,const char *syn,const char *ant,bool *added){
    if (!root) {
        *added = true;
        return make_node( word , syn , ant) ;
    }

    int cmp =strcmp(word , root->word );
    if (cmp < 0){
        root->left = insert_node(root->left , word , syn , ant , added ) ;
    }
    else if (cmp > 0) {
        root->right = insert_node( root->right , word , syn , ant , added );
    }
    else {
        free(root->syn ) ;
        free(root->ant ) ;
        root->syn = strdup(syn) ;
        root->ant = strdup(ant) ;
        *added = false ;
    }
    return root;
}

TTree *AddWordBST(TTree *tr, const char *word, const char *syne, const char *anton) {
    bool added = false;
    TreeNode *n = create_tnode(word, syne, anton);
    tr->root = insert_node(tr->root, n, &added);
    if (added) tr->size++;
    return tr;
}

static TreeNode *min_node(TreeNode *n) {
    while (n && n->left) n = n->left;
    return n;
}

static TreeNode *delete_node(TreeNode *root, const char *word, bool *deleted) {
    if (!root) return NULL;
    int cmp = strcmp(word, root->word);
    if (cmp < 0) root->left = delete_node(root->left, word, deleted);
    else if (cmp > 0) root->right = delete_node(root->right, word, deleted);
    else {
        *deleted = true;
        if (!root->left || !root->right) {
            TreeNode *tmp = root->left ? root->left : root->right;
            free(root->word);
            free(root->syn);
            free(root->ant);
            free(root);
            return tmp;
        } else {
            TreeNode *succ = min_node(root->right);
            free(root->word);
            free(root->syn);
            free(root->ant);
            root->word = strdup(succ->word);
            root->syn = strdup(succ->syn);
            root->ant = strdup(succ->ant);
            root->right = delete_node(root->right, succ->word, deleted);
        }
    }
    return root;
}

TTree *deleteWordBST(TTree *tr, const char *word) {
    bool deleted = false ;
    tr->root = delete_node(tr->root, word, &deleted);
    if (deleted) tr->size-- ;
    
    return tr ;
}

TTree *UpdateWordBST(TTree *tr, const char *word, const char *syne, const char *anton) {
    TreeNode *p = getInfWordTree(tr, word) ;

    if (p) {
        free(p->syn); free(p->ant);
        p->syn = strdup(syne); p->ant = strdup(anton);
        p->num_vowels = count_vowels(p->word);
    }

    return tr ;
}

void TraversalBSTinOrder(TTree *tr, void (*visit)(TreeNode*)) {
    if (!tr || !visit) return;

    static void recurse(TreeNode *n) {
        if (!n) return;
        recurse(n->left) ;
        visit(n) ;
        recurse(n->right) ;
    }

    recurse(tr->root) ;
}

void TraversalBSTpreOrder(TTree *tr, void (*visit)(TreeNode*)) {
    static void recurse(TreeNode *n) {
        if (!n) return;
        visit(n);
        recurse(n->left);
        recurse(n->right);
    }
    recurse(tr->root) ;
}

void TraversalBSTpostOrder(TTree *tr, void (*visit)(TreeNode*)) {
    static void recurse(TreeNode *n){
        if (!n) return ;
        recurse(n->left) ;
        recurse(n->right) ;
        visit(n) ;
    }
    recurse(tr->root) ;
}

void HighSizeBST(TTree *tr) {
    printf("Size: %d\n", tr->size) ;

    static int height(TreeNode *n){
        if (!n) return 0;
        int lh = height(n->left), rh = height(n->right) ;
        return (lh > rh ? lh : rh) + 1 ;
    }
    printf("Height: %d\n", height(tr->root));
}

TreeNode *LowestCommonAncestor(TTree *tr, const char *w1, const char *w2) {
    TreeNode *p = tr->root;
    while (p) {
        if (strcmp(w1, p->word) < 0 && strcmp(w2, p->word) < 0) p = p->left;
        else if (strcmp(w1, p->word) > 0 && strcmp(w2, p->word) > 0) p = p->right;
        else return p;
    }
    return NULL;
}

int CountNodesRanges(TTree *tr, int l, int h) {
    static int count_range(TreeNode *n) {
        if (!n) return 0;
        int cnt = 0;
        if (n->num_chars >= l && n->num_chars <= h) cnt++;
        if (n->num_chars > l) cnt += count_range(n->left);
        if (n->num_chars < h) cnt += count_range(n->right);
        return cnt;
    }
    return count_range(tr->root) ;
}

TreeNode *inOrderSuccessor(TTree *tr, const char *word) {
    TreeNode *cur = getInfWordTree(tr, word);
    if (!cur) return NULL;
    if (cur->right) return min_node(cur->right);
    TreeNode *succ = NULL;
    TreeNode *p = tr->root;
    while (p) {
        int cmp = strcmp(cur->word, p->word);
        if (cmp < 0) { succ = p; p = p->left; }
        else p = p->right;
    }
    return succ ;
}

TTree *BSTMirror(TTree *tr) {
    static TreeNode *mirror(TreeNode *n) {
        if (!n) return NULL;
        TreeNode *left = mirror(n->left);
        TreeNode *right = mirror(n->right);
        n->left = right;
        n->right = left;
        return n;
    }
    tr->root = mirror(tr->root);
    return tr;
}

static bool balanced(TreeNode *n, int *h) {
    if (!n) { *h = 0; return true; }
    int lh, rh;
    if (!balanced(n->left, &lh) || !balanced(n->right, &rh)) return false;
    *h = (lh > rh ? lh : rh) + 1;
    return abs(lh - rh) <= 1;
}

bool isBalancedBST(TTree *tr) {
    int h;
    return balanced(tr->root, &h);
}

TTree *BSTMerge(TTree *tr1, TTree *tr2) {
    // In-order traverse both into arrays, then merge sorted arrays, build balanced tree
    size_t n1 = tr1->size, n2 = tr2->size;
    TreeNode **arr1 = malloc(n1 * sizeof *arr1);
    TreeNode **arr2 = malloc(n2 * sizeof *arr2);
    size_t idx1 = 0, idx2 = 0;
    void collect(TreeNode *n, TreeNode **arr, size_t *i) {
        if (!n) return;
        collect(n->left, arr, i);
        arr[(*i)++] = n;
        collect(n->right, arr, i);
    }
    collect(tr1->root, arr1, &idx1);
    collect(tr2->root, arr2, &idx2);
    TreeNode **merged = malloc((n1+n2)*sizeof *merged);
    size_t i=0,j=0,k=0;
    while (i<n1 && j<n2) merged[k++] = strcmp(arr1[i]->word, arr2[j]->word)<0 ? arr1[i++] : arr2[j++];
    while (i<n1) merged[k++] = arr1[i++];
    while (j<n2) merged[k++] = arr2[j++];
    TTree *tr = init_tree(); tr->size = k;
    function<TreeNode*(int,int)> build = [&](int l, int r)->TreeNode* {
        if (l>r) return NULL;
        int m=(l+r)/2;
        TreeNode *n = merged[m];
        n->left = build(l,m-1);
        n->right= build(m+1,r);
        return n;
    };
    tr->root = build(0,k-1);
    free(arr1); free(arr2); free(merged);
    return tr;
}

int main() {
    return 0;
}
