#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "number.h"

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
    Digits *head;
    Digits *tail;
    Number *numbers = malloc(sizeof(Number));

    head = (Digits*)malloc(sizeof(Digits));
    tail = (Digits*)malloc(sizeof(Digits));
    head->prev = head;
    head->next = tail;
    tail->prev = head;
    tail->next = tail;

    numbers->head = head;
    numbers->tail = tail;
    numbers->isPositive = true;
    numbers->deciaml_point = 0;

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
void release_numbers(Number **number) {
    Digits* temp = (*number)->head;
	Digits* deleteNode;
	while (temp != (*number)->tail) {
		deleteNode = temp;
		temp = temp->next;
		free(deleteNode);
	}
    free((*number)->tail);
    free(*number);
}

/*
모든 표현식을 메모리에서 해제하는 함수

ExpressHeadTail *exp: 해제 할 숫자 표현
*/
void release_all(ExpressHeadTail **exp) {
    Expression* temp = (*exp)->head;
	Expression* deleteNode;
    while (temp != (*exp)->tail) {
        deleteNode = temp;
        temp = temp->next;
        if (deleteNode->type == TYPE_DIGIT) {
            release_numbers(&(deleteNode->data));
            free(deleteNode);
        } else {
            free(deleteNode);
        }
    }
    free((*exp)->tail);
    free(*exp);
}

/*
소수점 윗 자리수에 쓸데없는 0을 제거하는 함수. 0004433.43 -> 4433.43

Digits* head: 0을 제거하려는 숫자의 시작부분
Digits* tailㅣ 0을 제거하려는 숫자의 끝부분
*/
int deletee_zero_up_deciaml(Digits* head, Digits* tail) {
    Digits *now = head->next;
    Digits *next;
    int count = 0;
    while (now != tail && now->data == '0') {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;
        free(now);
        now = head->next;
        count++;
    }
    return count;
}

/*
소수점 아래 자리수에 쓸데없는 0을 제거하는 함수. 4433.43000 -> 4433.43

Digits* head: 0을 제거하려는 숫자의 시작부분
Digits* tailㅣ 0을 제거하려는 숫자의 끝부분
*/
int deletee_zero_down_deciaml(Digits* head, Digits* tail) {
    Digits *now = tail->prev;
    Digits *prev;
    int count = 0;
    while (now->prev != head && now->data == '0') {
        now->prev->next = now->next;
        now->next->prev = now->prev;
        now->next = NULL;
        now->prev = NULL;
        free(now);
        now = tail->prev;
        count++;
    }
    return count;
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

void print_numbers(Number *numbers) {
    if (!numbers->isPositive) {
        putchar('-');
    }
    int i = 0;
    int total_count = get_count_digits(numbers->head, numbers->tail);
    if (numbers->deciaml_point >= total_count) {
        unsigned int diff = numbers->deciaml_point - total_count + 1;
        putchar('0');
        putchar('.');
        for (int i = 0;i < diff - 1;i++) {
            putchar('0');
        }
        Digits *noow = numbers->head->next;
        while (noow != numbers->tail) {
            putchar(noow->data);
            noow = noow->next;
        }
    } else {
        int i = 0;
        Digits *noow = numbers->head->next;
        while (noow != numbers->tail) {
            if (i == total_count - numbers->deciaml_point) putchar('.');
            putchar(noow->data);
            noow = noow->next;
            i++;
        }
    }
}

/*
ExpressHeadTail 안에 있는 모든 표현식을 출력하는 함수

ExpressHeadTail *expHT: ExpressHeadTail 구조체
*/
void print_all(ExpressHeadTail **expHT) {
    Expression *now = (*expHT)->head->next;
    while (now != (*expHT)->tail) {
        if (now->type == TYPE_DIGIT) {
            print_numbers(now->data);
        } else {
            printf(" %c", now->opr);
        }
        now = now->next;
    }
    putchar('\n');
}