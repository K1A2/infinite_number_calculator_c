#include "number.h"
#include <stdio.h>

Number* init_number() {
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

    return &numbers;
}

void release_all(Number *number) {
    Number num = *number;
    Digits* temp = num.up_decimal_point_head;
	Digits* deleteNode;
	while (temp != num.up_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(num.up_decimal_point_tail);
    temp = num.down_decimal_point_head;
	while (temp != num.down_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(num.down_decimal_point_tail);
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