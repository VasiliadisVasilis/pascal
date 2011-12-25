#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "expressions.h"


node_t *tree_generate_node(statement_t *node, scope_t *scope);
node_t *tree_generate_address(variable_t *var);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, expression_t *expr, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);



node_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
	node_t *node, *cur;
	statement_t *p;

	if ( root == NULL )
		return NULL;

	node = tree_generate_node( root, scope );

	if ( node == NULL )
		return NULL;

	for ( p = root->next; p ; p = p->next ) {
		cur = tree_generate_node( p, scope );

		if ( cur == NULL )
			return NULL;

		node->next = cur;
		cur->prev  = node;
		node = cur;
	}

	return node;
}

node_t *tree_generate_address(variable_t *var)
{
	/*
		 Traverse var, and accumulate offsets based on the types of each
		 @child in var ( see variable_t in bison_union.h ) then add the base
		 address and return it as a icosnt type node_t.
	 */
	return NULL;
}

node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	ret->store.address = address;
	return ret;
}

node_t *tree_generate_store(variable_t *var, expression_t *expr, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	ret->store.address = address;
	return ret;
}

node_t *tree_generate_load(variable_t *var, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Load;
	ret->load.address = address;
	return ret;
}

node_t *tree_generate_sconst(char *string)
{
	return NULL;
}

node_t *tree_generate_value( expression_t *expr, scope_t *scope)
{
	node_t *node = NULL;

	switch ( expr->type )
	{
		case ET_Constant:
			node = ( node_t* ) calloc(1, sizeof(node_t));

			switch ( expr->constant.type )
			{
				case VT_Bconst:
					node->type = NT_Bconst;
					node->bconst = expr->constant.bconst;
					break;

				case VT_Iconst:
					node->type = NT_Iconst;
					node->iconst = expr->constant.iconst;
					break;

				case VT_Rconst:
					node->type = NT_Rconst;
					node->rconst = expr->constant.rconst;
					break;

				case VT_Cconst:
					node->type = NT_Cconst;
					node->cconst = expr->constant.cconst;
					break;
			}
			break;

		case ET_Set:
			break;

		case ET_Not:
			node = ( node_t* ) calloc(1, sizeof(node_t));
			node->type = NT_Not;
			node->not = tree_generate_value( expr->notExpr, scope );
			break;

		case ET_Variable:
			node = tree_generate_load( expr->variable, scope);
			break;

		case ET_Call:

			break;

		case ET_Binary:
			{
				switch ( expr->binary.op )
				{
					case AddopM:
					case MuldivandopDiv:
					case MuldivandopM:
					case MuldivandopMod:
					case AddopP: {
												 expression_t *e_left, *e_right;
												 node_t *left, *right;

												 // have constants always on the right site ( might prove usefull )
												 if ( expr->binary.left->type == ET_Constant ) {
													 e_right= expr->binary.left;
													 e_left = expr->binary.right;
												 } else {
													 e_left = expr->binary.left;
													 e_right = expr->binary.right;
												 }

												 left = tree_generate_value( e_left, scope );
												 right = tree_generate_value( e_right, scope );

												 node = ( node_t * ) calloc(1, sizeof(node_t));
												 switch ( expr->binary.op )
												 {
													 case AddopP:
														 node->type = NT_Add;
														 break;

													 case AddopM:
														 node->type = NT_Sub;
														 break;

													 case MuldivandopDiv:
														 node->type = NT_Div;
														 break;

													 case MuldivandopMod:
														 node->type = NT_Mod;
														 break;

													 case MuldivandopM:
														 node->type = NT_Mult;
														 break;
												 }

												 node->bin.left = left;
												 node->bin.right = right;
											 } 
											 break;
				}
			}
	}

	return node;
}

node_t *tree_generate_node(statement_t *node, scope_t *scope)
{  
	node_t *_node = NULL;


	if ( node == NULL )
		return NULL;

	printf("node_type: %s\n", statement_type_to_string(node));

	switch ( node->type )
	{
		case ST_Assignment:
			{
				node_t *var, *data;

				if ( node->assignment.type == AT_Expression ) {
					var = tree_generate_store( node->assignment.var, node->assignment.expr
							, scope);
					data = tree_generate_value( node->assignment.expr, scope );
				} else {
					var = tree_generate_store_str(node->assignment.var
							, node->assignment.string, scope);
					data = tree_generate_sconst( node->assignment.string );
				}

				_node = calloc (1, sizeof(node_t));
				_node->type = NT_Store;
				_node->load.address = var;
				_node->load.data = data;
				break;
			}

		case ST_If:
		{
			node_t *_true, *_false, *condition, *join;

			if ( node->join )
				join = tree_generate_node(node->join, scope);

			condition = tree_generate_value(node->_if.condition, scope);
			//TODO: jump based on the condition value

			if ( node->_if._true )
				_true = tree_generate_value(node->_if.condition, scope);
		}
			break;

		case ST_While:
			break;

		case ST_For:
			break;

		case ST_With:
			break;

		case ST_Call:
			break;

		case ST_Case:
			break;
	}

	return _node;
}

