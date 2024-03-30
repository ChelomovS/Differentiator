#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/differentiator_lib.h"
#include "../inc/operation_lib.h"
#include "../shared/debug.h"
#include "../shared/file_lib.h"
#include "../inc/dsl.h"

differentiator_error differentiator_ctr(Differentiator* differentiator)
{
    ASSERT(differentiator != nullptr);

    differentiator->buffer = (char*)calloc(MAX_SIZE_OF_BUFFER, sizeof(char));
    if (differentiator->buffer == nullptr)
        return differentiator_bad_alloc;
    
    differentiator->ptr_node = (Node*)calloc(1, sizeof(Node));
    if (differentiator->ptr_node == nullptr)
        return differentiator_bad_alloc;
    
    return differentiator_ok;
}

differentiator_error load_data(Differentiator* differentiator, const char* file_name)
{
    ASSERT(differentiator != nullptr);
    
    if (file_name == nullptr)
        return differentiator_bad_open_file;

    FILE* filein = fopen(file_name, "r");
    if (filein == nullptr)
        return differentiator_bad_alloc;
    
    size_t file_size = GetFileSize(filein);
    if (file_size > MAX_SIZE_OF_BUFFER)
    {
        differentiator->buffer = (char*)realloc(differentiator->buffer, file_size);
        if (differentiator->buffer == nullptr)
            return differentiator_bad_alloc;
    }
    fread(differentiator->buffer, sizeof(char), file_size, filein);
    fprintf(stderr, "%s", differentiator->buffer);

    return differentiator_ok;
}

char* make_tree(Differentiator* differentiator)
{
    ASSERT(differentiator != nullptr);

    if (*differentiator->buffer == '(')
    {
        differentiator->buffer++;
        make_tree(differentiator);
    }
    else if (*differentiator->buffer)
    {

    }
    return differentiator->buffer;
}

differentiator_error make_node(Node* node, type type, operation operation, double value, Node* left, Node* right)
{
    node = (Node*)calloc(1, sizeof(Node));
    if (node == nullptr)
        return differentiator_bad_alloc;

    node->type = type;
    node->operation = operation;
    node->value = value;
    node->left = left;
    node->right = right;

    return differentiator_ok;
}

void differentiator_dtr(Differentiator* differentiator)
{
    ASSERT(differentiator != nullptr);

    free(differentiator->buffer);

    tree_dtor(differentiator->ptr_node);
}

void tree_dtor(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    if (ptr_node->left != NULL)
    {
        tree_dtor(ptr_node->left);
    }

    if (ptr_node->right != NULL)
    {
        tree_dtor(ptr_node->right);
    }

    free(ptr_node);
}

Node* create_op_node(operation operation, Node* left, Node* right, Node* parent)
{
    ASSERT(parent != nullptr);
    ASSERT(left != nullptr);

    Node* op_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(op_node != nullptr);

    op_node->type       = type_operation;
    op_node->value      = 0;
    op_node->operation  = operation;
    op_node->parent     = parent;    
    op_node->left       = left;
    op_node->right      = right;
}

Node* create_num_node(double value, Node* left, Node* right, Node* parent)
{
    ASSERT(parent != nullptr);

    Node* num_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(num_node != nullptr);

    num_node->type      = type_num;
    num_node->value     = value;
    num_node->operation = not_operation;
    num_node->left      = left;
    num_node->right     = right;
    num_node->parent    = parent;
}

Node* create_var_node(char* variable, Node* left, Node* right, Node* parent)
{
    ASSERT(parent != nullptr);

    Node* var_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(var_node != nullptr);

    var_node->type      = type_var;
    var_node->left      = left;
    var_node->right     = right;
    var_node->value     = 0;
    strcpy(var_node->var, variable);
    var_node->operation = not_operation;
    var_node->parent    = parent;
}

void error_processing(differentiator_error error)
{
    if (error == differentiator_bad_alloc)
    {
        fprintf(stderr, "BAD ALLOCATION\n");
        useage();
    }
    else if (error == differentiator_bad_open_file)
    {
        fprintf(stderr, "NO SUCH FILE OR DIRECTORY\n");
        useage();
    }
    else if (error == differentiator_too_few_files)
    {
        fprintf(stderr, "TOO FEW FILE\n");
        useage();
    }
}

void useage()
{
    fprintf(stderr, "Эта программа - дифференциатор, чтобы воспользоваться ей, нужно:\n");
    fprintf(stderr, "Ввести как аргумент командой строки название файла или директории\n");
    fprintf(stderr, "В этом файле должно быть арифметическое выражение\n");
}