#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "register.h"

reg_file_t rf_saved, rf_temp, rf_fsaved, rf_ftemp;
long int post_number = 0;
int stmt_id = 0;
extern int unique_id;
node_t **printer;

void start_reg_mem(node_list_t *start);
void assign_mem_to_reg(node_list_t *start);

void delete_global_pointers(){
	int i;
	for(i = 0 ; i < post_number ; i++ ){
			nodes[i] = NULL;
			printer[i] = NULL;
	}

	free(printer);
	free(nodes);
}

void assign_nodes_tree(node_t *start);

void rg_add(reg_file_t* rf, reg_t *reg)
{
	rf->regs = ( reg_t* ) realloc(rf->regs
				, ( rf->size+1 ) * sizeof(reg_t) );
	rf->regs[ rf->size++ ] = *reg;
}

reg_t* give_reg_id(reg_file_t *rf, int id){
	if(id>=rf->size){
		printf("fatal error\n line 24");
		exit(0);
	}
	else{
		return(&(rf->regs[id]));
	}
}


reg_t rg_get_zero()
{
  reg_t ret;
  ret.name = "$0";
  ret.id = 0;
  ret.in_use = 0;

  return ret;
}

reg_t rg_allocate()
{
  reg_t ret;
  static int unique = 1;
  ret.id = unique;
  ret.name = (char*)malloc(sizeof(char)*30);
  sprintf(ret.name, "$%d", ret.id);

  unique++;
  return ret;
}

void rg_init()
{
	// Initialize temporary variables
	// info taken from 
	// http://msdn.microsoft.com/en-us/library/ms253512(v=vs.80).aspx


	
	reg_t v2 = { .name="$g1", .id= 1, .in_use=0 };
	reg_t v0 = { .name="$g2", .id= 2, .in_use=0 };
	reg_t v1 = { .name="$g3", .id= 3, .in_use=0 };
	reg_t a0 = { .name="$g4", .id= 4, .in_use=0 };
	reg_t a1 = { .name="$g5", .id= 5, .in_use=0 };
	reg_t a2 = { .name="$g6", .id= 6, .in_use=0 };
	reg_t a3 = { .name="$g7", .id= 7, .in_use=0 };
	
	reg_t t0 = { .name="$g8", .id= 8, .in_use=0 };
	reg_t t1 = { .name="$g9", .id= 9, .in_use=0 };
	reg_t t2 = { .name="$g10", .id=10, .in_use=0 };
	reg_t t3 = { .name="$g11", .id=11, .in_use=0 };
	reg_t t4 = { .name="$g12", .id=12, .in_use=0 };
	reg_t t5 = { .name="$g13", .id=13, .in_use=0 };
	reg_t t6 = { .name="$g14", .id=14, .in_use=0 };
	reg_t t7 = { .name="$g15", .id=15, .in_use=0 };
	
	reg_t s0 = { .name="$g16", .id=16, .in_use=0 };
	reg_t s1 = { .name="$g17", .id=17, .in_use=0 };
	reg_t s2 = { .name="$g18", .id=18, .in_use=0 };
	reg_t s3 = { .name="$g19", .id=19, .in_use=0 };
	reg_t s4 = { .name="$g20", .id=20, .in_use=0 };
	reg_t s5 = { .name="$g21", .id=21, .in_use=0 };
	reg_t s6 = { .name="$g22", .id=22, .in_use=0 };
	reg_t s7 = { .name="$g23", .id=23, .in_use=0 };
	
	reg_t t8 = { .name="$g24", .id=24, .in_use=0 };
	reg_t t9 = { .name="$g25", .id=25, .in_use=0 };
	reg_t t10 = { .name="$g26", .id=26, .in_use=0 };
	reg_t t11 = { .name="$g27", .id=27, .in_use=0 };
	reg_t t12 = { .name="$g28", .id=28, .in_use=0 };
	reg_t t13 = { .name="$g29", .id=29, .in_use=0 };
	reg_t t14 = { .name="$g30", .id=30, .in_use=0 };
	reg_t t15 = { .name="$g31", .id=31, .in_use=0 };
	
	reg_t f0  = { .name="$f0", .id=32, .in_use=0 };
	reg_t f4  = { .name="$f4", .id=36, .in_use=0 };
	reg_t f6  = { .name="$f6", .id=38, .in_use=0 };
	reg_t f8  = { .name="$f8", .id=40, .in_use=0 };
	reg_t f10 = { .name="$f10", .id=42, .in_use=0 };
	reg_t f16 = { .name="$f16", .id=48, .in_use=0 };
	reg_t f18 = { .name="$f18", .id=50, .in_use=0 };

	reg_t f20 = { .name="$f20", .id=52, .in_use=0 };
	reg_t f22 = { .name="$f22", .id=54, .in_use=0 };
	reg_t f24 = { .name="$f24", .id=56, .in_use=0 };
	reg_t f26 = { .name="$f26", .id=58, .in_use=0 };
	reg_t f28 = { .name="$f28", .id=60, .in_use=0 };
	reg_t f30 = { .name="$f30", .id=62, .in_use=0 };

	memset(&rf_saved, 0, sizeof(reg_file_t));
	memset(&rf_temp, 0, sizeof(reg_file_t));

	memset(&rf_fsaved, 0, sizeof(reg_file_t));
	memset(&rf_ftemp, 0, sizeof(reg_file_t));
	
	rg_add(&rf_saved, &s0);
	rg_add(&rf_saved, &s1);
	rg_add(&rf_saved, &s2);
	rg_add(&rf_saved, &s3);
	rg_add(&rf_saved, &s4);
	rg_add(&rf_saved, &s5);
	rg_add(&rf_saved, &s6);
	rg_add(&rf_saved, &s7);

	rg_add(&rf_saved,&t0);
	rg_add(&rf_saved,&t1);
	rg_add(&rf_saved,&t2);
	rg_add(&rf_saved,&t3);
	rg_add(&rf_saved,&t4);
	rg_add(&rf_saved,&t5);
	rg_add(&rf_saved,&t6);
	rg_add(&rf_saved,&t7);
	rg_add(&rf_saved,&t8);
	rg_add(&rf_saved,&t9);
	rg_add(&rf_saved,&t10);
	rg_add(&rf_saved,&t11);
	rg_add(&rf_saved,&t12);
	rg_add(&rf_saved,&t13);
	rg_add(&rf_saved,&t14);
	rg_add(&rf_saved,&t15);	
	
	rg_add(&rf_saved,&v0);
	rg_add(&rf_saved,&v1);
	rg_add(&rf_saved,&v1);
	rg_add(&rf_saved,&v2);
	
	rg_add(&rf_saved,&a0);
	rg_add(&rf_saved,&a1);
	rg_add(&rf_saved,&a2);
	rg_add(&rf_saved,&a3);

	rg_add(&rf_fsaved, &f0);
	rg_add(&rf_fsaved, &f4);
	rg_add(&rf_fsaved, &f6);
	rg_add(&rf_fsaved, &f8);
	rg_add(&rf_fsaved, &f10);
	rg_add(&rf_fsaved, &f16);
	rg_add(&rf_fsaved, &f18);
	
	rg_add(&rf_ftemp,&f20);
	rg_add(&rf_ftemp,&f22);
	rg_add(&rf_ftemp,&f24);
	rg_add(&rf_ftemp,&f26);
	rg_add(&rf_ftemp,&f28);
	rg_add(&rf_ftemp,&f30);

}

void givepostnumbers_tree(node_list_t *start){
  node_list_t *c;
  for(c = start; c!=NULL ; c = c->next){
		c->id = stmt_id;
		stmt_id++;
		printf("stmt\n");
    givepostnumbers(c->node);
	}
  return;
}
void check_father(node_t *start);

void check_father_list(node_list_t *start){
	node_list_t *c;
	for(c = start ;  c!=NULL ; c= c->next)
		check_father(c->node);
}

void check_father(node_t *start){
  if(!start)
    return ;


 switch(start->type){
		case NT_LessThani:
		case NT_Subi:
		case NT_Ori:
		case NT_Addi: {
			if(start->bin_semi.left->parent[0] != start){
				printf("ARXIDIA  father\n");
			}
			check_father(start->bin_semi.left);
			break;
		}
		case NT_Iconst: 
		case NT_Bconst:
		case NT_Rconst:
		case NT_Cconst:
		case NT_Lui:{
			break;
		}
		case NT_Load:
			if(start->load.address&&start->load.address->parent[0] != start){
				printf("ARXIDIA  father\n");
			}
			check_father(start->load.address);
			break;
		case NT_Store:
			if(start->store.data->parent[0] != start)
				printf("ARXIDIA  father\n");
					
				check_father(start->store.data);

			break;
		case NT_LessThan:
		case NT_Add: 
		case NT_Sub:
		case NT_Div:
		case NT_Mult:
		case NT_Mod:{
			
			if(start->bin.left->parent[0] != start)
				printf("ARXIDIA  father\n");
					
			if(start->bin.right->parent[0] != start)
				printf("ARXIDIA  father\n");
					
				check_father(start->bin.right);
				check_father(start->bin.left);
			break;
		}
		case NT_String:
		case NT_Not: 
		case NT_If:
		case NT_Jump:
			break;
			
		case NT_BranchZ:{
			break;
		}
		case NT_While:
			break;
		case NT_For:
			break;
		case NT_Nop:
			break;
		default:
			printf("type %d\n", start->type);
			assert(0 && "Unhandled type in tree");
		}

  return;
}









void givepostnumbers(node_t *start){
  if(!start)
    return ;

  if ( start->post != -1 )
    return;

  switch (start->type){
		case NT_LessThani:
    case NT_Subi:
    case NT_Ori:
    case NT_Addi: {
      givepostnumbers(start->bin_semi.left);
      start->post= post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
//			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
		}
		case NT_Lui:
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
      //propably constant nodes do not need post number id cause are going to be converted in opi instructions
      start->post= post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
//			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_Load:{
      if ( start->load.address )
        givepostnumbers(start->load.address);
    //  givepostnumbers(start->load.data); load does not have data.
      start->post = post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
// 			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_Store:{
      givepostnumbers(start->store.data); 

      if ( start->store.address )
        givepostnumbers(start->store.address);

      start->post = post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
// 			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:{
      givepostnumbers(start->bin.left);
      givepostnumbers(start->bin.right);
      start->post = post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
// 			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
      break;
    case NT_Not: //Not yes seen a valid implementation
      break;
    case NT_If:{
			start->_if.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_if.branch);
			printf("IF\n");
      givepostnumbers_tree(start->_if._true);
			printf("ELSE\n");
      givepostnumbers_tree(start->_if._false);
			printf("END\n");
      break;
    }
    case NT_Jump:
			start->post= post_number++;
			start->time = start->post; 
			start->scheduled = 0;
			start->reg = NULL;
      break;
    case NT_BranchZ:{
      givepostnumbers(start->branchz.condition);
			start->post = post_number++;
// 			start->num_parents = 0;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_LessThan:{
      givepostnumbers(start->bin.left);
      givepostnumbers(start->bin.right);
			start->post = post_number++;
			start->time = start->post;
			start->scheduled = 0;
			start->reg = NULL;
// 			start->num_parents = 0;
      printf("\t %ld\n",start->post);
      break;
    }
    case NT_While:{
			start->_while.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_while.branch);
			printf("WHILE\n");
      givepostnumbers_tree(start->_while.loop);
			printf("END\n");
      break;
    }
    case NT_For:{
			start->_for.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_for.init);
      givepostnumbers(start->_for.branch);
      printf("FOR\n");
			givepostnumbers_tree(start->_for.loop);
			printf("END\n");
      break;
    }
    
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}




void init_reg_lives(){
		reg_nodes_rep = (int *) malloc (post_number*sizeof(int));
		nodes = (node_t**) calloc (post_number,sizeof(node_t*));
		printer = (node_t**) malloc (post_number*sizeof(node_t*));
		if(!(nodes&&reg_nodes_rep)){
			printf("NOT ENOUGH MEMORY\n");
			exit(0);
		}
}

void assign_nodes_list(node_list_t *start){
  node_list_t *c;
  for(c = start; c!=NULL ; c = c->next){
    assert(c->node);
    assign_nodes_tree(c->node);
	}
  return;
}

void assign_nodes_tree(node_t *start){
  //static int id = 0;
  if ( !start )
    return;
  
  if ( nodes[start->post] == start )
    return;

  
  switch (start->type){
		case NT_LessThani:
    case NT_Subi:
    case NT_Ori:
    case NT_Addi: {
      assign_nodes_tree(start->bin_semi.left);
      nodes[start->post]=start;
      break;
    }
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:
		case NT_Lui:{
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld) (%p :: %p)\n", id++, start->post,
      //    nodes[start->post]->parent, start->parent);
      break;
    }
    case NT_Load:{
      assign_nodes_tree(start->load.address);
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_Store:{
      assign_nodes_tree(start->load.data); 

      if ( start->load.address )
        assign_nodes_tree(start->load.address);

      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_Add: 
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:{
      assign_nodes_tree(start->bin.left);
      assign_nodes_tree(start->bin.right);

      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_String:
      break;
    case NT_Not: //Not yes seen a valid implementation
      break;
    case NT_If:{
      assign_nodes_tree(start->_if.branch);
      assign_nodes_list(start->_if._true);
      assign_nodes_list(start->_if._false);
      break;
    }
    case NT_Jump:
			 nodes[start->post] = start;
      break;
    case NT_BranchZ:{
      assign_nodes_tree(start->branchz.condition);
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_LessThan:{
      assign_nodes_tree(start->bin.left);
      assign_nodes_tree(start->bin.right);
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_While:{
      assign_nodes_tree(start->_while.branch);
      assign_nodes_list(start->_while.loop);
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_For:{
      assign_nodes_tree(start->_for.init);
      assign_nodes_tree(start->_for.branch);
			assign_nodes_list(start->_for.loop);
      nodes[start->post] = start;
      // printf("Visiting %d (post: %ld)\n", id++, start->post);
      break;
    }
    case NT_Nop:
 #warning NT_Nops are not stored into nodes[]
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }


  return;
}


int cmp_lives(const void *a, const void *b){
	const node_t **_a = (const node_t**)a;
	const node_t **_b = (const node_t**)b;
	int i;
	int max1,max2;
	if(!*_a || !((*_a)->parent[0]))
		return 1;
	if(!*_b || !((*_b)->parent[0]))
		return -1;

	max1=0;
	max2=0;
	for(i = 0 ; i < (*_a)->num_parents; i++){
		if( (*_a)->parent[i] &&   (*_a)->parent[i]->time > max1);
			max1 = (*_a)->parent[i]->time ;
	}
	for (i = 0; i < (*_b)->num_parents ; i++){
	if( (*_b)->parent[i] &&   (*_b)->parent[i]->time > max2);
			max2 = (*_b)->parent[i]->time ;	
	}
	
	return max2-max1 ;
}

void print_nodes(){
	int i;
	for(i = 0 ; i < post_number ; i++){
		printf("(%d)\t: %ld \t",i,nodes[i]->time);
		if(!(nodes[i]->parent[0]))
			printf(" 0 \n");
		else
			printf(" %ld (%ld)\n",nodes[i]->parent[0]->time - nodes[i]->time, nodes[i]->parent[0]->time);
			
	}
}

int compute_life(node_t *node){
	int i;
	int max ;
	assert(node);
	max = node->parent[0]->time;
	for(i = 1 ; i < node->num_parents ; i++){
		if(node->parent[i] &&max < node->parent[i]->time){
			max = node->parent[i]->time;
		}
	}
	return max - node->time;
}

int give_reg(unsigned long int time, unsigned long int life){
	int i;
	int j;
	unsigned int temp;
	for(i =  0 ; i < 32 ; i++){
		temp = 1;
		temp = temp & (reg_nodes_rep[time] >> i);
		if(!(temp&1)){
			j = time;
			
			while ((!((reg_nodes_rep[j] >> i)&1)) && j < time+life){
				j++;
			}
			if (j == time+life  ){
				temp = 1<<i;
				for(j = time ; j < time+life ; j++)
					reg_nodes_rep[j] = reg_nodes_rep[j]|temp;
				return i;
			}
		}
	}
	return -1;
}


int find_reg(node_t *cur_node){
	assert(cur_node);
	unsigned long int time = cur_node->time;
	unsigned long int life  ;
	if(!(cur_node->parent[0])){
		life = 1;
		printf("%ld\n",life);
	}
  else {
		life = compute_life (cur_node) ;
    if ( life<= 0 )  {
      printf("WRONG post: %ld %ld\n", cur_node->time,life);
      printf("  child is : %d: \n", cur_node->type);
      printf("i am : %p\n", cur_node);
    }
  }




	return give_reg(time,life);
	
}

void give_regs(){
	int k,i;

	for(i = 0 ; i < post_number ; i++)
    printf("i: %d %p\n", i, nodes[i]);
  
  printf("****\n");
	memcpy(printer,nodes,post_number*sizeof(node_t*));
	qsort(nodes,post_number,sizeof(node_t*),cmp_lives);
	
	
	for(i = 0; i < post_number ; i++){
		printf(""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN"\n",
			BYTETOBINARY(reg_nodes_rep[i]>>24),BYTETOBINARY(reg_nodes_rep[i]>>16) , BYTETOBINARY(reg_nodes_rep[i]>>8) , BYTETOBINARY(reg_nodes_rep[i]));
	}
	
/*
	for(i = 0 ; i < post_number ; i++)
    printf("i: %d %p\n", i, nodes[i]);
*/

	for(i = 0 ; i < post_number ; i++){
		if(nodes[i]->reg == NULL){
			k = find_reg(nodes[i]);
			if(k == -1){
				printf("split occasion not yet implemented\n");
			}
			else{
				nodes[i]->reg = give_reg_id(&rf_saved, k);
			}
		}
		else{
			printf("AHHAHAHHAHA \t%ld %s\n",nodes[i]->time,nodes[i]->reg->name);
		}
	}
	print_nodes();
	for(i = 0; i < post_number ; i++){
		printf(""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN"\n",
			BYTETOBINARY(reg_nodes_rep[i]>>24),BYTETOBINARY(reg_nodes_rep[i]>>16) , BYTETOBINARY(reg_nodes_rep[i]>>8) , BYTETOBINARY(reg_nodes_rep[i]));
	}
}


void print_code(){
		int i;
		for(i = 0 ; i < post_number ; i++ ){

			if(printer[i] -> label )
				printf("%s:  ",printer[i]->label);

			
			switch(printer[i]->type){
				case NT_Iconst:{
					printf("%ld addi %s, $0, %d\n",printer[i]->post,printer[i]->reg->name,printer[i]->iconst );
					break;
				} 
				case NT_Bconst:
				case NT_Rconst:
				case NT_Cconst:{
					break;
				}
				case NT_Load:{
						if(printer[i]->load.address){
							printf( "%ld lw   %s, %d(%s)\n",printer[i]->post, printer[i]->reg->name, printer[i]->load.offset, 
								printer[i]->load.address->reg->name);
						}
						else{
							printf("%ld lw   %s, %d($0)\n",printer[i]->post,  printer[i]->reg->name, printer[i]->load.offset);
						}
					break;
				}
				case NT_Store:{
						if(printer[i]->store.address){
							printf("%ld sw   %s, %d(%s)\n",printer[i]->post, printer[i]->store.data->reg->name, printer[i]->store.offset, 
								printer[i]->store.address->reg->name);
						}
						else{
							printf("%ld sw   %s, %d($0)\n",printer[i]->post,  printer[i]->store.data->reg->name, printer[i]->store.offset);
						}
					break;
				}
				case NT_Add:{
					if(printer[i]->bin.right->type == NT_Iconst){
						printf("%ld addi %s, %s, %d\n",printer[i]->post, printer[i]->reg->name,	printer[i]->bin.left->reg->name, printer[i]->bin.right->iconst);
					}
					else if(printer[i]->bin.left->type == NT_Iconst){
						printf("%ld addi %s, %s, %d\n",printer[i]->post, printer[i]->reg->name,	printer[i]->bin.right->reg->name, printer[i]->bin.left->iconst);
					}
					else{
						printf("%ld add %s, %s, %s\n",printer[i]->post, printer[i]->reg->name,	printer[i]->bin.left->reg->name, printer[i]->bin.right->reg->name);
					}
					break;
				} 
				case NT_Sub:{
					printf("%ld sub %s, %s, %s\n",printer[i]->post, printer[i]->reg->name,	printer[i]->bin.left->reg->name, printer[i]->bin.right->reg->name);
					break;
				}
				case NT_Div:
					break;
				case NT_Mult:{
					printf( "%ld mult %s, %s\n",printer[i]->post, printer[i]->bin.left->reg->name, printer[i]->bin.right->reg->name);
					printf( "mflo %s\n", printer[i]->reg->name);
					break;
				}
				case NT_Mod:{
					break;
				}
				case NT_String:
					break;
				case NT_Not: //Not yes seen a valid implementation
					break;
				case NT_If:{
					break;
				}
				case NT_Jump:
						printf("%ld j    %s\n",printer[i]->post, printer[i]->jump_label);
					break;
				case NT_BranchZ:{
						printf("%ld bne  $0, %s, %s\n",printer[i]->post,printer[i]->branchz.condition->reg->name, printer[i]->branchz.label);
					break;
				}
				case NT_LessThan:{
					printf( "%ld slt  %s, %s, %s\n",printer[i]->post, printer[i]->reg->name,	printer[i]->bin.left->reg->name, printer[i]->bin.right->reg->name);
					break;
				}
				case NT_While:{
					break;
				}
				case NT_For:{
					break;
				}
        case NT_Subi:{
          printf("%ld subi %s, %s, %d\n", printer[i]->post, printer[i]->reg->name, printer[i]->bin_semi.left->reg->name, printer[i]->bin_semi.immediate);
          break;
        }


        case NT_Ori:{
          printf("%ld ori %s, %s, %d\n", printer[i]->post,  printer[i]->reg->name, printer[i]->bin_semi.left->reg->name, printer[i]->bin_semi.immediate);
          break;
        }
        case NT_LessThani:{
          printf("%ld slti %s, %s, %d\n", printer[i]->post,  printer[i]->reg->name, printer[i]->bin_semi.left->reg->name, printer[i]->bin_semi.immediate);
          break;
        }
        
        case NT_Addi:{
          printf("%ld addi %s, %s, %d\n", printer[i]->post,  printer[i]->reg->name, printer[i]->bin_semi.left->reg->name, printer[i]->bin_semi.immediate);
          break;
        }
          
        case NT_Lui: {
          printf("%ld lui %s, %d\n", printer[i]->post,  printer[i]->reg->name, printer[i]->iconst);
          break;
        }

				case NT_Nop:
					printf("add  $0, $0, $0\n");
					break;
				default:
					assert(0 && "Unhandled type in tree");	
			}
		}
}

void find_last_parent(node_t *node){
	int i;
	int max_time = 0; 
	if(node->load.is_scalar == 0){
		return;
	}
	
	for( i = 0 ; i < node->num_parents ; i++){
		if(max_time<node->parent[i]->time){
			max_time = node->parent[i]->time; 
		}
	}
	if(max_time > mem_reg[node->load.unique_id].start)
		mem_reg[node->load.unique_id].finish = max_time;
	if(mem_reg[node->load.unique_id].start == -1)
		mem_reg[node->load.unique_id].start  = node->time;
}


void update_mem_info(node_t *node){
	if(node->store.is_scalar == 1){
		if(node->time>mem_reg[node->store.unique_id].finish)
			mem_reg[node->store.unique_id].finish = node->time ;
		if((mem_reg[node->store.unique_id].start == -1) ||( mem_reg[node->store.unique_id].start > node->store.data->time))
			mem_reg[node->store.unique_id].start = node->store.data->time;
	}
}

void reg_mem_node(node_t *node){
		if(!node)
			return;
	  switch (node->type){
			case NT_LessThani:
			case NT_Subi:
			case NT_Ori:
			case NT_Addi: 
				reg_mem_node(node->bin_semi.left);
				break;
			case NT_Lui:
			case NT_Iconst: 
			case NT_Bconst:
			case NT_Rconst:
			case NT_Cconst:
				break;
			case NT_Load:
					reg_mem_node(node->load.address);
					find_last_parent(node);
				break;
			case NT_Store:
					reg_mem_node(node->store.address);
					reg_mem_node(node->store.data);
					update_mem_info(node);
				break;
			case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
			case NT_Sub:
			case NT_Div:
			case NT_LessThan:
			case NT_Mult:
			case NT_Mod:
				reg_mem_node(node->bin.left);
				reg_mem_node(node->bin.right);
				break;
			case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
				break;
			case NT_Not: //Not yes seen a valid implementation
				break;
			case NT_If:
					reg_mem_node(node->_if.branch);
					start_reg_mem(node->_if._true);
					start_reg_mem(node->_if._false);
				break;
			case NT_Jump:
				break;
			case NT_BranchZ:
				reg_mem_node(node->branchz.condition);
				break;
			case NT_While:
				reg_mem_node(node->_while.branch);
				start_reg_mem(node->_while.loop);
				break;
			case NT_For:
				reg_mem_node(node->_for.init);
				reg_mem_node(node->_for.branch);
				start_reg_mem(node->_for.loop);
				break;
			case NT_Nop:
				break;
			default:
				assert(0 && "Unhandled type in tree");
  }
  return;
}


void start_reg_mem(node_list_t *start){
	node_list_t *c;
	for(c = start; c!=NULL ; c= c->next){
		reg_mem_node(c->node);
	}
	
}

void give_reg_to_mem(){
	int i;
	long int life;
	int reg;
	long int time;
	for( i = 0 ; i < unique_id ; i++ ){
		time = mem_reg[i].start;
		life = mem_reg[i].finish - time;
		if(time!=-1 && life!=-1){
			reg = give_reg(time,life);
			mem_reg[i].reg = give_reg_id(&rf_saved,reg); 
		}
		else
			mem_reg[i].reg = NULL;
		}
}

void assign_mem_to_reg_node(node_t *node){
		if(!node)
			return;
	  switch (node->type){
			case NT_LessThani:
			case NT_Subi:
			case NT_Ori:
			case NT_Addi: 
				assign_mem_to_reg_node(node->bin_semi.left);
				break;
			case NT_Lui:
			case NT_Iconst: 
			case NT_Bconst:
			case NT_Rconst:
			case NT_Cconst:
				break;
			case NT_Load:
					if(node->load.is_scalar == 1)
						node->reg = mem_reg[node->load.unique_id].reg;
					assign_mem_to_reg_node(node->load.address);
				break;
			case NT_Store:
					assign_mem_to_reg_node(node->store.address);
					if(node->store.is_scalar == 1 && node->store.data->type != NT_Load )
						node->store.data->reg = mem_reg[node->store.unique_id].reg;
					assign_mem_to_reg_node(node->store.data);
				break;
			case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
			case NT_Sub:
			case NT_Div:
			case NT_LessThan:
			case NT_Mult:
			case NT_Mod:
				assign_mem_to_reg_node(node->bin.left);
				assign_mem_to_reg_node(node->bin.right);
				break;
			case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
				break;
			case NT_Not: //Not yes seen a valid implementation
				break;
			case NT_If:
					assign_mem_to_reg_node(node->_if.branch);
					assign_mem_to_reg(node->_if._true);
					assign_mem_to_reg(node->_if._false);
				break;
			case NT_Jump:
				break;
			case NT_BranchZ:
				assign_mem_to_reg_node(node->branchz.condition);
				break;
			case NT_While:
				assign_mem_to_reg_node(node->_while.branch);
				assign_mem_to_reg(node->_while.loop);
				break;
			case NT_For:
				assign_mem_to_reg_node(node->_for.init);
				assign_mem_to_reg_node(node->_for.branch);
				assign_mem_to_reg(node->_for.loop);
				break;
			case NT_Nop:
				break;
			default:
				assert(0 && "Unhandled type in tree");
  }
  return;
}





void assign_mem_to_reg(node_list_t *start){
	node_list_t *c;
	for(c = start; c!= NULL ; c= c->next){
		assign_mem_to_reg_node(c->node);
	}
	
}

void scan_mem(node_list_t *start){
	mem_reg = (mem_t*) malloc (sizeof(mem_t)*unique_id);
	assert(mem_reg);
	int i;
	for(i = 0; i< unique_id ; i++){
		mem_reg[i].start= -1;
		mem_reg[i].finish = -1;
	}
	start_reg_mem(start);
	give_reg_to_mem();
	assign_mem_to_reg(start);
	for(i = 0 ; i < unique_id ; i++){
		printf("mem_reg : %ld %ld\n",mem_reg[i].start,mem_reg[i].finish);
	}

	give_regs();
	
}




















