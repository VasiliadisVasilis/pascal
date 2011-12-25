#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "constants.h"

scope_t *st_init(scope_t *scope)
{
  scope_t *ret = ( scope_t * ) calloc(1, sizeof(scope_t));
  ret->parent = scope;
  return ret;
}

scope_t *st_destroy(scope_t *scope)
{
  //TODO free the contents of the scope
  return scope->parent;
}

typedefs_entry_t* st_typedef_find(char *name, scope_t *scope)
{
  int i;

  while ( scope ) {
    for ( i = 0; i < scope->typedefs_size; i ++ )
      if ( !strcasecmp(scope->typedefs[i].name, name ) )
        return scope->typedefs + i;
    scope = scope->parent;
  }

  return NULL;
}

int st_typedef_register(typedefs_entry_t *entry, scope_t *scope)
{
  int i = 0;
  typedefs_entry_t *found = st_typedef_find(entry->name, scope);

  if ( found ) {
    printf("typedef %s is already defined\n", entry->name);
    return Failure;
  }

  switch ( entry->type )
  {
    case TT_Record:
      {
        for ( i = 0 ; i < entry->record.size; i ++ )
        {
          if ( entry->record.types[i].dataType == VT_User ) {
            if ( st_typedef_find(entry->record.types[i].userType
											, scope ) == 0 )
            {
              printf("st_typedef_register: field type %s of record "
									"%s is not defined\n",
                  entry->record.types[i].userType, entry->name);
            }
          }
        }
      }
      break;

    case TT_Array:
      {
        var_t *var;
        const_t *constant;
				limits_entry_t *l;
        for ( i = 0, l=entry->array.dims.limits
						; i < entry->array.dims.size; i ++, l++ )
        {
          if ( l->isRange )
          {
            if ( l->range.from.type == LT_Id )
            {
              var = st_var_find( l->range.from.id.id, scope );

              if ( var ) {
                if ( var->type.dataType != VT_Integer 
											&& var->type.dataType != VT_Char ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->range.from.id.id );
                  return Failure;
                }
                continue;
              }

              constant = st_const_find(l->range.from.id.id, scope);

              if ( constant ) {
                if ( constant->constant.type != VT_Cconst 
                    && constant->constant.type != VT_Iconst ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->range.from.id.id );
                  return Failure;
                }
              } else {
                printf("st_typedef_find: array's %s limit %s is not "
										"defined\n",
                    entry->name, l->range.from.id.id);
                return Failure;
              }
            }

            if ( l->range.to.type == LT_Id )
            {
              var = st_var_find( l->range.to.id.id, scope );

              if ( var ) {
                if ( var->type.dataType != VT_Integer 
										&& var->type.dataType != VT_Char ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->range.to.id.id );
                  return Failure;
                }
                continue;
              }

              constant = st_const_find( l->range.to.id.id, scope );

              if ( constant ) {
                if ( constant->constant.type != VT_Cconst 
                    && constant->constant.type != VT_Iconst ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->range.to.id.id );
                  return Failure;
                }
              } else {
                printf("st_typedef_find: array's %s limit %s is not "
										"defined\n",
                    entry->name, l->range.to.id.id);
                return Failure;
              }


            }

          }
          else
          {
            if ( l->limit.type == LT_Id )
            {

              var = st_var_find( l->limit.id.id, scope );

              if ( var ) {
                if ( var->type.dataType != VT_Integer 
										&& var->type.dataType != VT_Char ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->limit.id.id );
                  return Failure;
                }
                continue;
              }

              constant = st_const_find(l->limit.id.id, scope);

              if ( constant ) {
                if ( constant->constant.type != VT_Cconst 
                    && constant->constant.type != VT_Iconst ) {
                  printf("st_typedef_find: array's %s limit %s is not "
											"integer/char\n",
                      entry->name, l->limit.id.id );
                  return Failure;
                }
              } else {
                printf("st_typedef_find: array's %s limit %s is not "
										"defined\n",
                    entry->name, l->limit.id.id);
                return Failure;
              }


            }
          }
        }
      }
      break;

    case TT_List:
      printf("st_typedef_register: TT_List is not implemented\n");
      return Failure;
      break;

    case TT_Set:
      printf("st_typedef_register: TT_Set is not implemented\n");
      return Failure;
      break;

    case TT_Range:
      printf("st_typedef_register: TT_Range is not implemented\n");
      return Failure;
      break;
  }
	
	int size = st_get_type_size(VT_User, entry->name, scope);
	
	if ( size == 0 ) {
		printf("Type %s is incomplete\n", entry->name);
		return Failure;
	}

  scope->typedefs = ( typedefs_entry_t* ) realloc( scope->typedefs,
      sizeof(typedefs_entry_t) * ( scope->typedefs_size +1 ));
  scope->typedefs[ scope->typedefs_size ++ ] = *entry;
		
  printf("registered type %s size %d\n", entry->name, size);

	return Success;
}

var_t *st_var_define(char* id, data_type_t type, scope_t *scope)
{
  int i;

  for ( i=0; i < scope->vars_size; i++ ) {

    if ( !strcasecmp(id, scope->vars[i].id))
      break;
  }

  if ( scope->vars_size && i!=scope->vars_size ) {
    printf("Error: %s has already been defined\n",id );
    return 0;
  }

  scope->vars = ( var_t * ) realloc(scope->vars
					, sizeof(var_t) * (scope->vars_size+1) );
  scope->vars[ scope->vars_size ].id = id;
  scope->vars[ scope->vars_size ].pass = 0;
  scope->vars[ scope->vars_size ].reference = NULL;
  scope->vars[ scope->vars_size ].type = type;

  return scope->vars + (scope->vars_size++);
}

var_t* st_var_find(char *id, scope_t *scope)
{
  int i;

  if ( id == NULL )
    return NULL;

  for ( ; scope; scope = scope->parent ) {
    for ( i = 0; i < scope->vars_size; i ++ )
      if ( !strcasecmp(id, scope->vars[i].id) )
        return scope->vars + i;
  }

  return NULL;
}

func_t *st_func_find(char *id, scope_t *global)
{
  int i = 0;

  if ( id == 0 )
    return 0;

  for ( i = 0 ; i < global->funcs_size; i ++ ) {
    if ( strcasecmp(id, global->funcs[i].id) == 0 )
      return global->funcs + i;
  }
  return NULL;
}


func_t *st_func_define(char *id, data_type_t type, var_t *params,
		int size, scope_t *scope)
{
  int i;
  func_t *func = NULL;

  for ( i = 0; i < scope->funcs_size; i ++ ) {
    if ( !strcasecmp(id, scope->funcs[i].id) ) {
      printf("Function %s is already defined\n", id);
      return NULL;
    }
  }

  scope->funcs = ( func_t* ) realloc( scope->funcs
			, (scope->funcs_size+1) * ( sizeof(func_t)));
  func = scope->funcs + scope->funcs_size++;

  func->id = id;
  func->type = type;
  func->params = params;
  func->size = size;


  return func;
}

const_t* st_const_define(char *id, constant_t *constant, scope_t *scope)
{
  if ( st_const_find(id, scope) ) {
    printf("constdef %s is already defined\n", id);
    return NULL;
  }

  scope->consts = ( const_t* ) realloc( scope->consts, 
					(scope->consts_size+1) * sizeof(const_t));
  scope->consts[ scope->consts_size ].id = id;
  scope->consts[ scope->consts_size ].constant = *constant;

  return scope->consts + ( scope->consts_size++ );
}

const_t* st_const_find(char *id, scope_t *scope)
{
  int i;
  scope_t *p;

  for ( p = scope; p; p = p->parent )
    for ( i = 0; i < p->consts_size; i ++ )
      if ( !strcasecmp(id, p->consts[i].id) ) {
        return p->consts+i;
      }

  return NULL;
}


int st_get_type_size(int type, char* userType, scope_t *scope)
{
  typedefs_entry_t *p;
  switch (type) 
  {
    case VT_Integer: case VT_Iconst:
      return INTEGER_SIZE;
    case VT_Char:    case VT_Cconst:
      return CHAR_SIZE;
    case VT_Real:    case VT_Rconst:
      return REAL_SIZE;
    case VT_Boolean: case VT_Bconst:
      return BOOLEAN_SIZE;
    case VT_User:
      break;
    default:
      return 0;
  }

  // if we've made it this far we need to return the size for a 
  // user defined type.

  p = st_typedef_find(userType, scope);

  if ( p == NULL )
    return 0;
  
  switch ( p->type )
  {
    case TT_Record:
      
    break;

    case TT_Array:
    break;

    case TT_List:
    break;

    case TT_Set:
    break;

    case TT_Range:
    break;
  }

	return 0;
}

