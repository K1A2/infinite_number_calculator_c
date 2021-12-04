#include <stdlib.h>
#include <stdio.h>

#include "number.h"

ExpressHeadTail init_expression() {
    Expression *head;
    Expression *tail;

    head = (Expression*)malloc(sizeof(Expression));
    tail = (Expression*)malloc(sizeof(Expression));
    head->prev = head;
    head->next = tail;
    tail->prev = head;
    tail->next = tail;

    head->type = -1;
    tail->type = -1;

    ExpressHeadTail eht;
    eht.head = head;
    eht.tail = tail;

    return eht;
}

Number init_number() {
    Digits *head_up;
    Digits *tail_up;
    int size_up = 0;
    Digits *head_down;
    Digits *tail_down;
    int size_down = 0;
    Number numbers;

    head_up = (Digits*)malloc(sizeof(Digits));
    tail_up = (Digits*)malloc(sizeof(Digits));
    head_up->prev = head_up;
    head_up->next = tail_up;
    tail_up->prev = head_up;
    tail_up->next = tail_up;
    size_up = 0;
    head_down = (Digits*)malloc(sizeof(Digits));
    tail_down = (Digits*)malloc(sizeof(Digits));
    head_down->prev = head_down;
    head_down->next = tail_down;
    tail_down->prev = head_down;
    tail_down->next = tail_down;
    size_down = 0;

    numbers.up_decimal_point_head = head_up;
    numbers.down_decimal_point_head = head_down;
    numbers.up_decimal_point_tail = tail_up;
    numbers.down_decimal_point_tail = tail_down;

    return numbers;
}

void expression_insert_tail(EXPRESSION_TYPE type, Number data, char opr, Expression* node) {
    
	Expression *newNode = (Expression*)malloc(sizeof(Expression));	
	newNode->data = data;
    newNode->type = type;
    newNode->opr = opr;
	node->prev->next = newNode;
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev = newNode;
}

void release_numbers(Number number) {
    Digits* temp = number.up_decimal_point_head;
	Digits* deleteNode;
	while (temp != number.up_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(number.up_decimal_point_tail);
    temp = number.down_decimal_point_head;
	while (temp != number.down_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(number.down_decimal_point_tail);
}

void release_all(ExpressHeadTail exp) {
    Expression* temp = exp.head;
	Expression* deleteNode;
    while (temp != exp.tail) {
        deleteNode = temp;
        temp = temp->next;
        if (deleteNode->type == TYPE_DIGIT) {
            release_numbers(deleteNode->data);
            free(deleteNode);
        } else {
            free(deleteNode);
        }
    }
    free(exp.tail);
}

void insert_tail(char value, Digits* node) {
	Digits *newNode = (Digits*)malloc(sizeof(Digits));	
	newNode->data = value;				
	node->prev->next = newNode;			
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev = newNode;
}

void insert_head(char value, Digits* node) {
	Digits* newNode = (Digits*)malloc(sizeof(Digits));
	newNode->data = value;
	node->next->prev = newNode;
	newNode->next = node->next;
	newNode->prev = node;
	node->next = newNode;
}

void print_nodes_from_head(Digits* node, Digits* to_node) {
    Digits *tmp = node->next;
    while (tmp != to_node) {
        printf("%c\n", tmp->data);
        tmp = tmp->next;
    }
}

void print_all(ExpressHeadTail expHT) {
    
}