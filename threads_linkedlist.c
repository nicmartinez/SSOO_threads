#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

int items=1;

typedef struct node{
    int data;
    struct node* next;
} node;

void print_list(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        printf("%d ", cursor->data);
        cursor = cursor->next;
    }
}

node* create(int data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
 
    return new_node;
}

node* append(node* head, int data)
{
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;
 
    return head;
}

node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    return head;
}

node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
 
    return head;
}

#define MAX 25
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* create_list(void *arg);
typedef struct __myarg_t {
     int a;
     node* b;
 } myarg_t;

int main(void) 
{
	int i;
	node* head1 = malloc(sizeof(node));
  // inserte código para agregar 100 nodos
  // primero secuencialmente (usando un ciclo)
  for (i=1;i<100;i++)
  {
    append(head1, items);
		items++;
  }

	// luego en paralelo
	//habria sido mejor un recurion de los threads pero nu lol logre, asique un for dentro del thread sera je
	myarg_t args;
	args.a = MAX;
	args.b = head1;

  pthread_t p1, p2,p3,p4;

  pthread_create(&p1, NULL, create_list, &args);
	args.a = 2*MAX;
  pthread_create(&p2, NULL, create_list, &args);
	args.a = 3*MAX;
	pthread_create(&p3, NULL, create_list, &args);
	args.a = 4*MAX;
	pthread_create(&p4, NULL, create_list, &args);



  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
	pthread_join(p3, NULL);
  pthread_join(p4, NULL);



  print_list(head1);

  

  printf("\nListo!\n");

  return 0;

}

void* create_list(void *arg){
	
	myarg_t *m = (myarg_t *) arg;

  int max_number = (intptr_t) m->a;
	node* head = (node*) m->b;

  int i;

	//crea MAX cantidad de elemtos en la lista
  for(i = max_number - MAX;i<max_number;i++){
    pthread_mutex_lock(&lock);

    if (head == NULL){

      head=create(0,NULL);

    }

    else{

      append(head,items);
			items++;

    }

    pthread_mutex_unlock(&lock);

  }
	return NULL;
}