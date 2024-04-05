#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

Node* simplificate    (Node* node);
void  simplificate_num(Node* node, bool* change_flag);
void  simplificate_op (Node* node, bool* change_flag);
void  simplificate_add(Node* node, bool* change_flag);
void  simplificate_sub(Node* node, bool* change_flag);
void  simplificate_mul(Node* node, bool* change_flag);
void  simplificate_div(Node* node, bool* change_flag);
//Node* simplificate_sin(Node* node, bool* change_flag);
//Node* simplificate_cos(Node* node, bool* change_flag);
//Node* simplificate_ln (Node* node, bool* change_flag);
//Node* simplificate_pow(Node* node, bool* change_flag);


#endif // SIMPLIFICATION_H