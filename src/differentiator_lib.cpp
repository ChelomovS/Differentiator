#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/differentiator_lib.h"
#include "../inc/simplification.h"
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
    ASSERT(file_name != nullptr);

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
    differentiator->buffer = nullptr;

    tree_dtor(differentiator->ptr_node);
    differentiator->ptr_node = nullptr;
}

void tree_dtor(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    if (ptr_node->left != nullptr)
        tree_dtor(ptr_node->left);

    if (ptr_node->right != nullptr)
        tree_dtor(ptr_node->right);

    free(ptr_node);
}

Node* diff(Node* ptr_node) // обработка ошибок при вызове diff
{
    ASSERT(ptr_node != nullptr);

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    if (new_node == nullptr)
        return nullptr;

    switch(ptr_node->type)
    {
        case type_num: return _NUM(0, ptr_node->parent);
        case type_var: return _NUM(1, ptr_node->parent);

        case type_operation:
        {
            switch (ptr_node->operation)
            {
                case ADD:
                {
                    new_node = _ADD(dL, dR, ptr_node->parent);
                    return new_node;
                }

                case SUB:
                {
                    new_node = _DIV(dL, dR, ptr_node->parent);
                    return new_node;
                }

                case MUL:
                {
                    new_node = _MUL(_ADD(dR, cL, new_node), _ADD(cR, dL, new_node), 
                                                            ptr_node->parent);
                    return new_node;
                }

                case DIV: 
                {
                    new_node = _DIV(_SUB(_MUL(dL, cR, new_node->left), 
                                        _MUL(cL, dR, new_node->right), new_node), 
                                        _MUL(cR, cR, new_node), ptr_node->parent);
                    return new_node;
                }

                case SIN:
                {
                    new_node = _MUL(dL, _COS(cL, new_node), ptr_node->parent);
                    return new_node;
                }

                case COS:
                {
                    new_node = _MUL(_NUM(-1, new_node), _MUL(dL, _SIN(cL, new_node),
                                                ptr_node->parent), ptr_node->parent);
                    return new_node;
                }

                case LN:
                {
                    new_node = _MUL(dL, _DIV(_NUM(1,0), cL, new_node), ptr_node->parent);
                    return new_node;
                }
                
                case POW:
                {
    
                }
                case NOT_OPERATION:
                {
                    fprintf(stderr, "shit happened\n");
                    ASSERT(0 && ":(");
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

Node* differentiate(Node* ptr_node, differentiator_error error)
{
    ASSERT(ptr_node != nullptr);

    bool change_flag = 0;

    Node* diff_tree = diff(ptr_node);
    if (diff_tree == nullptr)
        return nullptr;

    diff_tree = simplificate(diff_tree);
}

Node* copy_node(Node* ptr_node)
{
    ASSERT(ptr_node != nullptr);

    Node* copied_node = (Node*)calloc(1, sizeof(Node));
    if (copied_node == nullptr)
        return nullptr;

    // копирование данных из старого узла
    copied_node->type      = ptr_node->type;
    copied_node->operation = ptr_node->operation;
    copied_node->value     = ptr_node->value;
    strcpy(copied_node->var, ptr_node->var);
    copied_node->parent    = ptr_node->parent;

    // рекурсивный вызов в левое поддерево
    if (ptr_node->left)
        copied_node->left = copy_node(ptr_node->left);
    
    // рекурсивный вызов в правое поддерево
    if (ptr_node->right)
        copied_node->left = copy_node(ptr_node->right);

    return copied_node;
}

Node* create_op_node(operation operation, Node* left, Node* right, Node* parent, size_t arg_number)
{
    ASSERT(parent != nullptr);
    ASSERT(left   != nullptr);

    Node* op_node = (Node*)calloc(1, sizeof(Node));
    if (op_node == nullptr)
        return nullptr;
        
    op_node->type        = type_operation;
    op_node->value       = 0;
    op_node->operation   = operation;
    op_node->parent      = parent;
    op_node->left        = left;
    op_node->right       = right;
    op_node->arg_number  = arg_number;

    return op_node;
}

Node* create_num_node(double value, Node* left, Node* right, Node* parent, size_t arg_number)
{
    ASSERT(parent != nullptr);

    Node* num_node = (Node*)calloc(1, sizeof(Node));
    if (num_node == nullptr)
        return nullptr;

    num_node->type       = type_num;
    num_node->value      = value;
    num_node->operation  = NOT_OPERATION;
    num_node->left       = left;
    num_node->right      = right;
    num_node->parent     = parent;
    num_node->arg_number = arg_number;

    return num_node;
}

Node* create_var_node(char* variable, Node* left, Node* right, Node* parent, size_t arg_number)
{
    ASSERT(parent   != nullptr);
    ASSERT(variable != nullptr);

    Node* var_node = (Node*)calloc(1, sizeof(Node));
    if (var_node == nullptr)
        return nullptr;

    var_node->type       = type_var;
    var_node->value      = 0;
    strcpy(var_node->var, variable);
    var_node->operation  = NOT_OPERATION;
    var_node->left       = left;
    var_node->right      = right;
    var_node->parent     = parent;
    var_node->arg_number = arg_number;

    return var_node;
}

void node_delete(Node* node)
{
    free(node);
    node = nullptr;
}

void error_processing(differentiator_error error)
{
    switch (error)
    {
        case differentiator_ok:
            return ;

        case differentiator_bad_alloc:
            fprintf(stderr, "BAD ALLOCATION\n");
            break;

        case differentiator_bad_open_file:
            fprintf(stderr, "NO SUCH FILE OR DIRECTORY");
            break;

        case differentiator_too_few_files:
            fprintf(stderr, "TOO FEW FILE\n");
            break;

        default:
            fprintf(stderr, "Shit happend\n");
            ASSERT(0 && ":(");
    }

    useage();
}

void useage()
{
    fprintf(stderr, "Эта программа - дифференциатор, чтобы воспользоваться ей, нужно:\n");
    fprintf(stderr, "Ввести как аргумент командой строки название файла или директории\n");
    fprintf(stderr, "В этом файле должно быть арифметическое выражение\n");
}