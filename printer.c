#include <assert.h>
#include "printer.h"

void print_instruction(node_t *node, FILE* output);

void print_instruction_add(node_t *left, node_t *right, reg_t *reg, FILE* output)
{
	if ( right == NULL ) {
		// this add is used as a move
		switch(left->type) {
			case NT_Iconst:
				fprintf(output, "addi %s, $0, %d\n", left->reg.name, left->iconst);
			break;

      case NT_Bconst:
        fprintf(output, "addi %s, $0, %d\n", left->reg.name, left->bconst);
      break;

			default:
				assert(0 && "Unhandled type");
		}
	} else {

    print_instruction(left, output);
    
    if ( right->type == NT_Iconst ) {
      fprintf(output, "addi %s, %s, %d\n", reg->name, left->reg.name, right->iconst);
    } else {
      fprintf(output, "add %s, %s, %s\n", reg->name, left->reg.name, right->reg.name);
    }
  }
}

void print_instruction_store(reg_t *start, int offset, node_t *data, FILE* output)
{
  print_instruction(data, output);
  fprintf(output, "sw %s, %d(%s)\n", data->reg.name, offset, start->name);
}

void print_instruction_tree(node_list_t *tree, FILE* output)
{
	node_list_t *c;

	for ( c=tree; c!=NULL; c=c->next)
		print_instruction(c->node, output);
}

void print_instruction_less_than(node_t *left, node_t *right, reg_t* reg, FILE *output)
{
  fprintf(output, "slt %s, %s, %s\n", reg->name, left->reg.name, right->reg.name);
}

void print_instruction_load(reg_t *start, int offset, reg_t *dest, FILE *output)
{
  fprintf(output, "lw %s, %d(%s)\n", dest->name, offset, start->name);
}

void print_instruction(node_t *node, FILE* output)
{
	if ( node == NULL )
		return;
	
	if ( node->label )
		fprintf(output, "%s : ", node->label );

  printf("#\t\tnode type: %d\n", node->type);
	
	switch ( node->type ) {
		case NT_Add:
			print_instruction_add(node->bin.left, node->bin.right, &node->reg, output);
		break;

    case NT_Bconst:
		case NT_Iconst:
			print_instruction_add(node, NULL, NULL, output);
		break;
		
    case NT_Load:
      print_instruction_load(&node->load.reg, node->load.offset, &node->reg, output);
    break;

		case NT_Store:
			print_instruction_store(&node->store.reg, node->store.offset, node->store.data, output);
		break;

    case NT_LessThan:
      print_instruction_less_than(node->bin.left, node->bin.right, &node->reg, output);
    break;
    
    case NT_If:
      print_instruction(node->_if.condition, output);
      if ( node->_if._true)
        print_instruction_tree(node->_if._true, output);
      if ( node->_if._false)
        print_instruction_tree(node->_if._false, output);
    break;

		default:
			printf("%d\n", node->type);
			assert(0 && "Unhandled node type" );
	}
}


