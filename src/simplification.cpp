#include "simplification.h"
#include "differentiator_lib.h"
#include "../shared/debug.h"

Node* simplificate(Node* node, bool* change_flag)
{
    ASSERT(node);
    ASSERT(change_flag);

    switch(node->operation)
    {
        case NOT_OPERATION: // нет операции --> двигаемся в подузел
            if (node->left != nullptr)
                node = simplificate(node->left, change_flag);
            if (node->right != nullptr)
                node = simplificate(node->right, change_flag);

        case ADD:
            node = simplificate_add(node, change_flag);

        case SUB:
            node = simplificate_sub(node, change_flag);
        
        case MUL:
            node = simplificate_mul(node, change_flag);
        
        case DIV:
            node = simplificate_div(node, change_flag);
        
        case SIN: 
            node = simplificate_sin(node, change_flag);

        case COS:
            node = simplificate_cos(node, change_flag);
        
        case LN: 
            node = simplificate_ln(node, change_flag);
    }

}

Node* simplificate_num(Node* node, bool* change_flag)
{
    if (node->right->operation == NOT_OPERATION && node->left->operation == NOT_OPERATION)
    {
        
    }


}

Node* simplificate_add(Node* node, bool* change_flag)
{
    if (node->right->value == 0 || node->left->value == 0)
    {

    }
    
}

Node* simplificate_sub(Node* node, bool* change_flag)
{
    if (node->right->value == 0 || node->left->value == 0)
    {

    }

}

Node* simplificate_mul(Node* node, bool* change_flag)
{
    if (node->right->value == 0 || node->right->value == 0)
    {

    }

    if (node->right->value == 1 || node->right->value == 1)
    {

    }

}

Node* simplifacate_num(Node* node, bool* change_flag)
{
    if (node->operation)
    {
        if (node->right->type == type_num && node->left->type == type_num)
        {
            switch (node->operation)
            case 
        }
    }

} 



Node* simplificate_div(Node* node, bool* change_flag)
{

}

