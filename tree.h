#ifndef _TREE_H
#define _TREE_H
#include "statements.h"
#include "constants.h"
#include "symbol_table.h"

typedef struct NODE_T node_t;
typedef struct NODE_BIN_T node_bin_t;
typedef struct NODE_BIN_CONST_T node_bin_const_t;
typedef struct NODE_LOAD_STORE_T node_load_t;
typedef struct NODE_LOAD_STORE_T node_store_t;

enum NodeType
{
  NT_Iconst,  // int  ( iconst )
  NT_Bconst,  // char ( bconst )
  NT_Rconst,  // float ( rconst )
  NT_Cconst,  // char ( cconst )
  NT_Load,    // node_load_t ( load )
  NT_Store,   // node_store_t ( store )
  NT_Add,     // node_bin_t ( bin )
  NT_Sub,     // node_bin_t ( bin )
  NT_AddI,    // node_bin_const_t ( bin_const )
  NT_SubI     // node_bin_const_t ( bin_const )
};

struct NODE_LOAD_STORE_T
{
  node_t *address;
  node_t *data;
};

struct NODE_BIN_CONST_T
{
  int type; // VT_Cconst, VT_Bconst, VT_Iconst, VT_Rconst
  int op;
  node_t* left; // assume constants will always reside in the right child
};

struct NODE_BIN_T
{
  int op;
  node_t *left, *right;
};

struct NODE_T
{
  char *label;
  int type;
  union
  {
    node_bin_t bin;
    node_load_t load;
    node_store_t store;
    int iconst;
    char cconst;
    char bconst;
    float rconst;
  };
};


node_t *tree_generate_tree(statement_t *root, scope_t *scope);
node_t *tree_generate_node(statement_t *node, scope_t *scope);
#endif
