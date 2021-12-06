# 무한소수 계산기

## 실행
```
gcc -o main.out main.c include/input.c include/express/number.c include/error.c

./main.out
```

### 입력 식 수정하기
input 파일에서 식을 수정하면 됩니다.

## 기능

### 입력

1. input 파일로 부터 계산 식 읽어오기
   * 이때 문자열의 길이 제한 x -> 동적할당을 위해 **연결 리스트**로 구현
2. 입력에 대한 예외처리 필수
   * 0-9, (), *, +, - 이외의 입력
   * 괄호의 갯수가 다를때
   * 소숫점의 개수가 2개 이상일 때
3. 곱하기 기호 생략의 예외 처리 (ex. 2(3+2) = 2*(3+2))

### 계산

1. 수식 표기법을 중위표기법에서 후위표기법으로 변환
   * **스택**을 이용
2. 덧셈/곱셈/뺄셈 함수 구현. 나눗셈은 필수는 아님
   * **연산의 정확도**가 평가 기준

## 아이디어

* 숫자 표현은 소숫점 자릿수, 일반 자릿수 따로 표현하기

## 구현 사항

### 입력 오류 검출

#### ERROR_INVALID_CHARACTER

0-9, +, -, *, () 이외의 입력

#### ERROR_FILE_IO

파일 입출력 오류

#### ERROR_TOO_MANY_DECIMAL_POINT

소수점이 여러개 일때

#### ERROR_TOO_MANY_OPERATOR

연산자가 연속으로 여러게 있을때. +-+, +* 등

#### ERROR_OPERATOR_WRONG_ORDER

연산자가 순서에 맞지 않게 나올때

#### ERROR_TOO_MANY_OPERATOR_BEFORE_BRACKET

괄호가 열리고 닫히기 전에 다른 연산자가 나올때

#### ERROR_RIGHT_BRACKET_WRONG_POSITION

닫히는 괄호가 식의 맨 앞이나 열리는 괄호 앞에 있을때

#### ERROR_RIGHT_BRACKET_AFTER_OPERATOR

닫히는 괄호 앞에 연산자가 있을때

#### ERROR_MUTIPLE_OPERATOR_WRONG_POSITION

곱하기 기호가 식의 맨 앞에 나올때

#### ERROR_OPERATOR_FIRST_ERROR

맨 앞에 연산자가 여러개 나올때

## 평가 기준

연산의 정확도. 3가지 항목으로 평가

1. 결과의 정확도 (오차)
2. 괄호 처리 여부, 입력의 다양성, 예외처리 등
3. 나눗셈 구현 시 추가 점수

