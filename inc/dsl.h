#ifndef DSL_H
#define DSL_H

#include "differentiator_lib.h"
// Дифференцирование левого узла
#define dL diff(node->left)
// Дифференцирование правого узла
#define dR diff(node->right)

// Копирование левого узла
#define cL copy_node(node->right)
// Копирование левого узла
#define cR copy_node(node->left)

// Создание узла с const
#define _NUM(value)         create_num_node(value,    nullptr, nullptr)
// Создание узла с переменной
#define _VAR(variable)      create_var_node(variable, nullptr, nullptr)

// Создание узлов операций
#define _ADD(left, right)   create_op_node(add, left, right)
#define _SUB(left, right)   create_op_node(sub, left, right)
#define _MUL(left, right)   create_op_node(mul, left, right)
#define _DIV(left, right)   create_op_node(div, left, right)
// Сложные функции
#define _SIN(left)          create_op_node(sin, left, nullptr)
#define _COS(left)          create_op_node(cos, left, nullptr)     

#endif // DSL_H