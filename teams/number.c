#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "number.h"


char num(Digits* tail){
        Digits *now = tail->prev;
        Digits *next;
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;
        char a_d = now->data;
        free(now);
        return a_d;
}
/*
ExpressHeadTail 구조체 초기화 함수
*/
ExpressHeadTail *init_expression() {
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

    ExpressHeadTail *eht = malloc(sizeof(ExpressHeadTail));
    eht->head = head;
    eht->tail = tail;

    return eht;
}

/*
Number 구조체 초기화 함수
*/
Number *init_number() {
    Digits *head_up;
    Digits *tail_up;
    int size_up = 0;
    Digits *head_down;
    Digits *tail_down;
    int size_down = 0;
    Number *numbers = malloc(sizeof(Number));

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

    numbers->up_decimal_point_head = head_up;
    numbers->down_decimal_point_head = head_down;
    numbers->up_decimal_point_tail = tail_up;
    numbers->down_decimal_point_tail = tail_down;

    return numbers;
}

/*
숫자형 Expression 노드를 한개만 만들어 리턴하는 함수
*/
Expression *init_expression_empty_noode_number() {
    Expression *exp = (Expression*)malloc(sizeof(Expression));
    exp->next = NULL;
    exp->prev = NULL;
    exp->type = TYPE_DIGIT;
    exp->opr = ' ';
    exp->data = init_number();
    return exp;
}

void number_concatenate(Expression *exp) {
    if (get_count_digits(exp->data->down_decimal_point_head, exp->data->down_decimal_point_tail) > 0) {
        exp->data->up_decimal_point_tail->prev->next = exp->data->down_decimal_point_head->next;
        exp->data->down_decimal_point_head->next->prev = exp->data->up_decimal_point_tail->prev;
    }
}

/*
기존에 존재하던 Expression 구조체를 Expression 링크드 리스트의 맨 뒤에 추가하는 함수

Expression *newNode: Expression 링크드 리스트에 추가할 Expression 구조체
Expression* node: Expression 링크드 리스트의 tail
*/
void expression_insert_tail(Expression *newNode, Expression* node) {
	node->prev->next = newNode;
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev = newNode;
}

/*
Expression 구조체를 Expression 링크드 리스트의 맨 뒤에 추가하는 함수

EXPRESSION_TYPE type: 숫자인지 연산자인지 여부
Number *data: 숫자일 때 Number 구조체
char opr: 연사자일때 연산자 종류
Expression* node: Expression 링크드 리스트의 tail
*/
void expression_insert_tail_new_node(EXPRESSION_TYPE type, Number *data, char opr, Expression* node) {
    Expression *newNode = (Expression*)malloc(sizeof(Expression));	
	newNode->data = data;
    newNode->type = type;
    newNode->opr = opr;
    expression_insert_tail(newNode, node);
}

/*
숫자를 표현하던 Digits 링크드 리스트와 Number 구조체를 메모리에서 모두 해제하는 함수

Number *number: 해제 할 구조체
*/
void release_numbers(Number *number) {
    Digits* temp = number->up_decimal_point_head;
	Digits* deleteNode;
	while (temp != number->up_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(number->up_decimal_point_tail);
    temp = number->down_decimal_point_head;
	while (temp != number->down_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(number->down_decimal_point_tail);
    free(number);
}

void release_numbers_concatenated(Number *number) {
    Digits* temp = number->up_decimal_point_head;
	Digits* deleteNode;
	while (temp != number->down_decimal_point_tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free(number->up_decimal_point_tail);
    free(number->down_decimal_point_head);
    free(number->down_decimal_point_tail);
    free(number);
}

/*
모든 표현식을 메모리에서 해제하는 함수

ExpressHeadTail *exp: 해제 할 숫자 표현
*/
void release_all(ExpressHeadTail *exp) {
    Expression* temp = exp->head;
	Expression* deleteNode;
    while (temp != exp->tail) {
        deleteNode = temp;
        temp = temp->next;
        if (deleteNode->type == TYPE_DIGIT) {
            release_numbers(deleteNode->data);
            free(deleteNode);
        } else {
            free(deleteNode);
        }
    }
    free(exp->tail);
    free(exp);
}

/*
소수점 윗 자리수에 쓸데없는 0을 제거하는 함수. 0004433.43 -> 4433.43

Digits* head: 0을 제거하려는 숫자의 시작부분
Digits* tailㅣ 0을 제거하려는 숫자의 끝부분
*/
void deletee_zero_up_deciaml(Digits* head, Digits* tail) {
    Digits *now = head->next;
    Digits *next;
    while (now->next != tail && now->data == '0') {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;
        free(now);
        now = head->next;
    }
}

/*
소수점 아래 자리수에 쓸데없는 0을 제거하는 함수. 4433.43000 -> 4433.43

Digits* head: 0을 제거하려는 숫자의 시작부분
Digits* tailㅣ 0을 제거하려는 숫자의 끝부분
*/
void deletee_zero_down_deciaml(Digits* head, Digits* tail) {
    Digits *now = tail->prev;
    Digits *prev;
    while (now->prev != head && now->data == '0') {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;
        free(now);
        now = tail->prev;
    }
}

/*
Digit 링크드 리스트의 맨 마지막 부분에 숫자를 추가하는 함수

char value: 추가 할 숫자
Digits* node: Digits 링크드 리스트의 tail
*/
void digit_insert_tail(char value, Digits* node) {
	Digits *newNode = (Digits*)malloc(sizeof(Digits));	
	newNode->data = value;				
	node->prev->next = newNode;			
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev = newNode;
}

/*
Digit 링크드 리스트의 맨 앞 부분에 숫자를 추가하는 함수

char value: 추가 할 숫자
Digits* node: Digits 링크드 리스트의 head
*/
void digit_insert_head(char value, Digits* node) {
	Digits* newNode = (Digits*)malloc(sizeof(Digits));
	newNode->data = value;
	node->next->prev = newNode;
	newNode->next = node->next;
	newNode->prev = node;
	node->next = newNode;
}

/*
숫자의 자리 수 개수를 구하는 함수

Digits* node: 자리수 개수를 구할 Digits 링크드 리스트
Digits* tail: Digits 링크드 리스트의 tail
*/
int get_count_digits(Digits* node, Digits* tail) {
    Digits *now = node->next;
    int count = 0;
    while (now != tail) {
        count += 1;
        now = now->next;
    }
    return count;
}

/*
Digits 링크드 리스트를 앞에서부터 순서대로 출력하는 함수

Digits* node: 자리수 개수를 구할 Digits 링크드 리스트
Digits* to_node: Digits 링크드 리스트의 tail
*/
void print_nodes_from_head(Digits* node, Digits* to_node) {
    Digits *tmp = node->next;
    while (tmp != to_node) {
        printf("%c", tmp->data);
        tmp = tmp->next;
    }
}

/*
ExpressHeadTail 안에 있는 모든 표현식을 출력하는 함수

ExpressHeadTail *expHT: ExpressHeadTail 구조체
*/
void print_all(ExpressHeadTail *expHT) {
    Expression *now = expHT->head->next;
    while (now != expHT->tail) {
        if (now->type == TYPE_DIGIT) {
            Number *numbers = now->data;
            putchar(' ');
            if (!numbers->isPositive) {
                putchar('-');
            }
            print_nodes_from_head(numbers->up_decimal_point_head, numbers->up_decimal_point_tail);
            putchar('.');
            print_nodes_from_head(numbers->down_decimal_point_head, numbers->down_decimal_point_tail);
            putchar(' ');
        } else {
            printf("%c", now->opr);
        }
        now = now->next;
    }
    putchar('\n');
}


