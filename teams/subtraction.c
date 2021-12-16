int a, b; //큰 수 작은 수
int x, y; // 소수점 앞자리 수가 같을 때 판별
int down_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail); //a의 소수점 뒷자리 길이
int down_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail); //b의 소수점 뒷자리 길이
int up_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail); // a의 수소점 앞자리 길이
int up_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail); //b의 소수점 앞자리 길이

int a_b() {
  if (up_a >= up_b) {
    a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
    b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
  } else if(up_a < up_b) {
    a = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
    b = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
  } else if (up_a == up_b) {
    x = len(up_a);
    y = len(up_b);
    if (x > y) {
      a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
      b = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
    } else if(x < y) {
      a = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
      b = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
    }
  }
}

if(down_a <=down_b) { //a의 소수점 뒷자리가 b의 소수점 뒷자리보다 짧을때,
  while (down_a != down_b) {//0을 추가해서 길이를 같게 만들어줌 {
    digit_insert_tail('0', a->data->down_decimal_point_tail);
    down_a = get_count_digits(a->data->down_decimal_point_head, a->data->down_decimal_point_tail);
  }
}else if(down_a >=down_b) {//그 반대일 때 {
  while (down_a != down_b) {
    digit_insert_tail('0', b->data->down_decimal_point_tail);
    down_b = get_count_digits(b->data->down_decimal_point_head, b->data->down_decimal_point_tail);
  }
}

if(up_a <= up_b) {// a의 소수점 앞자리가 b의 소수점 앞자리보다 짧을때, {
  while (up_a != up_b) {//0추가해서 길이 맞추기 {
    digit_insert_head('0', a->data->up_decimal_point_head);
    up_a = get_count_digits(a->data->up_decimal_point_head, a->data->up_decimal_point_tail);
  }
}else if(up_a >= up_b) {//반대일 때 {
  while (up_a != up_b) {
    digit_insert_head('0', b->data->up_decimal_point_head);
    up_b = get_count_digits(b->data->up_decimal_point_head, b->data->up_decimal_point_tail);
  } 
}