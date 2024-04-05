#ifndef DIFFERENTIATOR_LIB
#define DIFFERENTIATOR_LIB

#include <stdio.h>
#include "operation_lib.h"

static const size_t MAX_SIZE_OF_BUFFER = 1000;
static const size_t MAX_SIZE_OF_VAR    = 5;

enum type
{
    type_uninit    = 0, // неинициализованный тип
    type_num       = 1, // число
    type_var       = 2, // переменная 
    type_operation = 3  // операция 
};

struct Node
{ 
    type type;                 // тип хранимой информации
    operation operation;       // тип операции
    double value;              // значение const 
    char var[MAX_SIZE_OF_VAR]; // имя переменной

    Node* left;                // указатель на левый узел
    Node* right;               // указатель на правый узед
    Node* parent;              // указатель на предыдущий узел
};

struct Differentiator
{
    Node* ptr_node; // указатель на текущий узел
    char* buffer;   // буффер, куда копируется данные из файла
};

enum differentiator_error
{
    differentiator_ok                 = 0, // ошибки нет
    differentiator_bad_alloc          = 1, // ошибка выделения памяти
    differentiator_bad_open_file      = 2, // некорректный введеный файл
    differentiator_too_few_files      = 3  // недостаточно файлов
};

differentiator_error differentiator_ctr(Differentiator* differntiator);
differentiator_error load_data(Differentiator* differentiator, const char* file_name);
char* make_tree(Differentiator* differntiator);
void differentiator_dtr(Differentiator* differentiator);
void tree_dtor(Node* ptr_node);
Node* diff(Node* node);
Node* copy_node(Node* node);
Node* create_op_node(operation operation, Node* left, Node* right, Node* parent);
Node* create_num_node(double value, Node* left, Node* right, Node* parent);
Node* create_var_node(char* variable, Node* left, Node* right, Node* parent);
void error_processing(differentiator_error error);
void useage();

#endif // DIFFERENTIATOR_LIB