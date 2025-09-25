#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *items;
    int size;
}vector_t;

void vector_print(vector_t *v)
{
 printf("[");
 for(int i = 0; i < v->size; i++)
 {
  
    printf("%d, ", v->items[i]);
    
 }
 printf("%d] \n", v->size);
}

int venctor_ctor(vector_t *v, unsigned int size)
{
v->items = malloc(size *sizeof(int));
v->size = size;
return 1;
}

void vecotr_init(vector_t *v)
{
    for(int i = 0; i < v->size; i++)
    {
        v->items[i] = i;
    }
}

void vector_dtor(vector_t *v)
{
    if(v->items != NULL)
    {
        v->items = NULL;
    }
    v->size = 0;
}
 
void vector_scalar(vector_t *v, int multiplier)
{
    for(int i = 0; i < v->size; i++)
    {
        v->items[i] *= multiplier;
    }
}

int vector_sum(vector_t *v, vector_t *u)
{
    if(v->size != u->size)
    { 
        return 0;
    }
    else
    {
    for(int i = 0; i < v->size; i++)
    {
        v->items[i] += u->items[i];
    }
    }
}
int main()
{
 vector_t v1, v2;
 printf("Vector 1: ");
 venctor_ctor(&v1, 5);
 vecotr_init(&v1);
 vector_print(&v1); 

 printf("Vector 2: ");
 venctor_ctor(&v2, 5);
 vecotr_init(&v2);
 vector_print(&v1);

 printf("Vector 1 multiplied by 2: ");
 vector_scalar(&v1, 2);
 vector_print(&v1);
 
 printf("Vector 1 + Vector 2");
 vector_sum(&v1, &v2);
 vector_print(&v1);

 vector_dtor(&v1);
 vector_dtor(&v2);
}