#ifndef DIFFERENTIATOR_LIB
#define DIFFERENTIATOR_LIB

#include <stdio.h>
#include "operation_lib.h"

const size_t MAX_SIZE_OF_BUFFER = 1000;
const size_t MAX_SIZE_OF_VAR    = 5;

struct Node
{ 
    type type;
    operation operation;
    double value;
    char var[MAX_SIZE_OF_VAR]; 
    
    Node* left;
    Node* right;
    Node* parent;
};

enum type
{
    type_num       = 1, // число
    type_var       = 2, // переменная 
    type_operation = 3  // операция 
};

struct Differentiator
{
    Node* ptr_node;
    char* buffer;
};

enum differentiator_error{
    differentiator_ok                 = 0,
    differentiator_bad_alloc          = 1,
    differentiator_bad_open_file      = 2,
    differentiator_too_few_files      = 3
};


differentiator_error differentiator_ctr(Differentiator* differntiator);
differentiator_error load_data(Differentiator* differentiator, const char* file_name);
char* make_tree(Differentiator* differntiator);
differentiator_error make_node(Node* node, int type, operation operation, double value, Node* left, Node* right);
void differentiator_dtr(Differentiator* differentiator);
void tree_dtor(Node* ptr_node);
void error_processing(differentiator_error error);
void useage();

#endif // DIFFERENTIATOR_LIB