#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
 struct _node *next;
 char a[10];
 
}Node;

Node *head = NULL;
Node *tail = NULL;


void Add(Node node)
{
	 Node *add = (Node*)malloc(sizeof(Node));
	 *add = node;
	 add->next = NULL; 
	 
	 if(head == NULL) head = add;
	 else 			   tail->next = add;
	 
	 
	 tail = add;
}


void Show()
{
	 Node *loop = head;
	 int count = 0;
	 while(loop != NULL)
	 {
	  printf("[%d]:%s\n",count, loop->a);
	  count++;
	  loop = loop->next;
	 }
}



int main()
{
	 FILE* fp = fopen("a.txt", "r");
	 Node temp;
	 char ch[2];
	 
	 while(1)
	 {
		
	  ch [0]= (char)(fgetc(fp));
	  strcpy(temp.a, ch);
	  
	 
	  if(ch [0] == EOF)
	   break;
	
	  Add(temp);
	  
	 }
	
	 Show();
	 
	 fclose(fp);
	 return 0;
}
