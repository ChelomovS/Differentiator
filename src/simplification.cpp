#include <math.h>
#include <string.h>
#include "simplification.h"
#include "differentiator_lib.h"
#include "../shared/debug.h"

Node* simplificate(Node* node)
{
    ASSERT(node != nullptr);

    bool change_flag = false;
    do 
    {
        simplificate_num(node, &change_flag);
        simplificate_op (node, &change_flag);
    }
    while (change_flag == true);
    return node;
}

void simplificate_num(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->arg_number == 1 && node->left->type == type_num)
    {
        switch(node->operation)
        {
            case SIN:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = sin(node->left->value);
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }


            case COS:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = cos(node->left->value);
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }

            case LN:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = log(node->left->value);
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }
        }
    }

    else if (node->arg_number == 2 && node->left->type == type_num && 
                                                        node->right->type == type_num)
    {
        switch (node->operation)
        {
            case ADD:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = node->left->value + node->right->value;
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }

            case SUB:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = node->left->value - node->right->value;
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }

            case MUL:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = node->left->value - node->right->value;
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }

            case DIV:
            {
                node->type = type_num;
                node->operation = NOT_OPERATION;
                node->value = node->left->value / node->right->value;
                node->arg_number = 0;
                node_delete(node->left);
                node_delete(node->right);
                *change_flag = true;
                return;
            }
        }
    } 
    simplificate_num(node->left,  change_flag);
    simplificate_num(node->right, change_flag);
}

void simplificate_op(Node* node, bool* change_flag)
{
    if (node->type == type_operation)
    {
        switch (node->operation)
        {
            case ADD: simplificate_add(node, change_flag);
            case SUB: simplificate_sub(node, change_flag);
            case MUL: simplificate_mul(node, change_flag);
            case DIV: simplificate_div(node, change_flag);
        }
    }
}

void simplificate_add(Node* node, bool* change_flag)
{
    if (node->left->value == 0)
    {
        node->operation = node->right->operation;
        node->value = node->right->value;
        strcpy(node->var, node->right->var);
        node->arg_number = node->right->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
    else if (node->right->value == 0)
    {
        node->operation = node->left->operation;
        node->value = node->left->value;
        strcpy(node->var, node->left->var);
        node->arg_number = node->left->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
}

void simplificate_sub(Node* node, bool* change_flag)
{
    if (node->left->value == 0)
    {
        node->type = node->right->type;
        node->operation = node->right->operation;
        node->value = node->right->value;
        strcpy(node->var, node->right->var);
        node->arg_number = node->right->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
    else if (node->right->value == 0)
    {
        node->type = node->left->type;
        node->operation = node->left->operation;
        node->value = node->left->value;
        strcpy(node->var, node->left->var);
        node->arg_number = node->left->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
}

void simplificate_mul(Node* node, bool* change_flag)
{
    if (node->right->value == 0 || node->right->value == 0)
    {
        node->type = type_num;
        node->operation = NOT_OPERATION;
        node->value = 0;
        node->arg_number = 0;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
    else if (node->left->value == 1)
    {
        node->type = node->right->type; 
        node->operation = node->right->operation;
        node->value = node->right->value;
        node->arg_number = node->right->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
}

void simplificate_div(Node* node, bool* change_flag)
{
    ASSERT(node != nullptr);
    ASSERT(change_flag != nullptr);

    if (node->right->value == 1)
    {
        node->operation = node->left->operation;
        node->value = node->left->value;
        node->arg_number = node->left->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
    else if (node->left->value == 0)
    {
        node->operation = node->left->operation;
        node->value = node->left->value;
        node->arg_number = node->left->arg_number;
        node_delete(node->left);
        node_delete(node->right);
        *change_flag = true;
    }
}
