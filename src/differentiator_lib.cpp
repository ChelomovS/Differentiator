#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/differentiator_lib.h"
#include "../inc/operation_lib.h"
#include "../shared/debug.h"
#include "../shared/file_lib.h"
#include "../inc/dsl.h"

//------------------------------------------------------------------------------------
static differentiator_error differentiator_ctr(Differentiator* differntiator);
static differentiator_error load_data(Differentiator* differentiator, const char* file_name);
static char* make_tree         (Differentiator* differntiator);
static void  differentiator_dtr(Differentiator* differentiator);
static void  tree_dtor         (Node* ptr_node);
static Node* diff              (const Node* node);
static Node* create_op_node    (operation operation, Node* left, Node* right, Node* parent);
static Node* create_num_node   (double value, Node* left, Node* right, Node* parent);
static Node* create_var_node   (char* variable, Node* left, Node* right, Node* parent);
static void  error_processing  (differentiator_error error);
static void  useage();
//------------------------------------------------------------------------------------

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
        ///////////////////////READDDDDDDDDDDDDDDDDDDDDDDD
    }
    return differentiator->buffer;
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

Node* diff(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    Node* new_node = (Node*)calloc(1, sizeof(Node));

    switch(ptr_node->type)
    {
        case type_num:
        {
            return _NUM(0, ptr_node->parent);
        }

        case type_var:
        {
            return _NUM(1, ptr_node->parent);
        }

        case type_operation:
        {
            switch(ptr_node->operation)
            {
                case add:
                {
                    new_node = _ADD(dL, dR, ptr_node->parent);
                    return new_node;
                }

                case sub:
                {
                    new_node = _DIV(dL, dR, ptr_node->parent);
                    return new_node;
                }

                case mul:
                {
                    new_node = _MUL(_ADD(dR, cL, new_node), _ADD(cR, dL, new_node), 
                                                                 ptr_node->parent);
                    return new_node;
                }

                case div: 
                {
                    new_node = _DIV(_SUB(_MUL(dL, cR, new_node->left), 
                                        _MUL(cL, dR, new_node->right), new_node), 
                                        _MUL(cR, cR, new_node), ptr_node->parent);
                    return new_node;
                }

                case sn:
                {
                    new_node = _MUL(dL, _COS(cL, new_node), ptr_node->parent);
                    return new_node;
                }

                case cs:
                {
                    new_node = _MUL(_NUM(-1, new_node), 
                                        _MUL(dL, _SIN(cL, new_node), ptr_node->parent), 
                                                                     ptr_node->parent);
                    return new_node;
                }

                case ln:
                {
                    new_node = _MUL(dL, _DIV(_NUM(1,0), cL, new_node), ptr_node->parent);
                    return new_node;
                }

                default:
                {
                    fprintf(stderr, "bye bye \n");
                    ASSERT(0 && ":(");
                }
            } 
        }
    }
}

Node* copy_node(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    Node* copied_node = (Node*)calloc(1, sizeof(Node));
    ASSERT(copied_node != nullptr);

    // копирование данных из старого узла
    copied_node->type      = ptr_node->type;
    copied_node->operation = ptr_node->operation;
    copied_node->value     = ptr_node->value;
    strcpy(copied_node->var, ptr_node->var);
    copied_node->parent    = ptr_node->parent;

    // рекурсивный вызов в левое поддерево
    if (ptr_node->left)
    {
        copied_node->left = copy_node(ptr_node->left);
    }
    
    // рекурсивный вызов в правое поддерево
    if (ptr_node->right)
    {
        copied_node->left = copy_node(ptr_node->right);
    }

    return copied_node;
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

    return op_node;
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