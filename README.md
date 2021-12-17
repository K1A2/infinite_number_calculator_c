# 무한소수 계산기

## 실행

### 컴파일

첫 번째 방법 - makefile 실행

```shell
make
```
두 번째 방법 - make가 오류가 날 때
```shell
gcc -o main.out main.c include/input.c include/express/number.c include/error.c include/express/stack.c include/calculator.c
```
### 실행
```shell
./main.out
```

### 입력 식 수정하기
input 파일에서 식을 수정하면 됩니다.

## 요구 사항

### 프로그램 정의

무한소수 계산기는 
