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
#define _NUM(value, parent)         create_num_node(value,    nullptr, nullptr, parent)
// Создание узла с переменной
#define _VAR(variable, parent)      create_var_node(variable, nullptr, nullptr, parent)

// Создание узлов операций
#define _ADD(left, right, parent)   create_op_node(add, left, right, parent)
#define _SUB(left, right, parent)   create_op_node(sub, left, right, parent)
#define _MUL(left, right, parent)   create_op_node(mul, left, right, parent)
#define _DIV(left, right, parent)   create_op_node(div, left, right, parent)

// Сложные функции
#define _SIN(left, parent)          create_op_node(sin, left, nullptr, parent)
#define _COS(left, parent)          create_op_node(cos, left, nullptr, parent)     
#define _LN (left, parent)          create_op_node(ln,  left, nullptr, parent)

#endif // DSL_H