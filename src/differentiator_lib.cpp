#include <stdio.h>

#include "../inc/differentiator_lib.h"
#include "../inc/operation_lib.h"
#include "../shared/debug.h"
#include "../shared/file_lib.h"

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

    // ЧИТАЛКА)))
    return differentiator->buffer;
}

differentiator_error make_node(Node* node, int type, operation operation, double value, Node* left, Node* right)
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