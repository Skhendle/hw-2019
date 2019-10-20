// allocator.c
#define _DEFAULT_SOURCE
#include "allocator.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

struct block* test[MAX_EXP];
/** Allocate a block of at least the requested size **/
void* custom_malloc(size_t size) {

	if(size == 0){
		return NULL;
	}else{


		head=(struct block*)sbrk(MAX_SIZE);
		head->free=true;
		head->size=MAX_SIZE;
		head->data=(void *)((void *)head+sizeof(struct block));
		struct block* t=head;

		int i = 0;
		while(t->size > findSize(size+sizeof(struct block))){

			t->size=t->size/2;

			struct block* budy=(struct block*)((void *)t+t->size);
			budy->free=true;
			budy->size=t->size;

			test[i] = budy;
			struct block* next=(struct block*)((void *)t+t->size);
			next->data=(void *)((void *)t+sizeof(struct block));
			next->free=true;
			next->size=t->size;
			next->buddy = budy;

			for(int j = 0; j < i ; j++){
				next->merge_buddy[j] = test[j];
			}
			test[i+1] = next;
			next->next=t->next;

			t->next=next;

			i++;
		}
		for(int j = 0; j < i ; j++){
			t->merge_buddy[j] = test[j];
		}
		t->free = false;
		return (void *)t->data;
	}

}

/** Mark a data block as free and merge free buddy blocks **/
void custom_free(void* ptr) {
    // TODO
}

/** Change the memory allocation of the data to have at least the requested size **/
void* custom_realloc(void* ptr, size_t size) {
    // TODO
    return NULL;
}
int findSize(size_t size){

	size_t num = MAX_SIZE;
	bool a = true;
	while(a){
		if(size > num && size ){
			a = false;
			break;
		}else{
			num = num /2;
		}
	}
	size_t b = 256;

	if(num < b){
		num = b;
	}

	return num;
}
/*------------------------------------*\
|            DEBUG FUNCTIONS           |
\*------------------------------------*/

/** Prints the metadata of a block **/
void print_block(struct block* b) {
    if(!b) {
        printf("NULL block\n");
    }
    else {
        int i = 0;
        printf("Strt = %p\n",b);
        printf("Size = %lu\n",b->size);
        printf("Free = %s\n",(b->free)?"true":"false");
        printf("Data = %p\n",b->data);
        printf("Next = %p\n",b->next);
        printf("Buddy = %p\n",b->buddy);
        printf("Merge Buddies = ");
        while(b->merge_buddy[i] && i < MAX_EXP) {
            printf("%p, ",b->merge_buddy[i]);
            i++;
        }
        printf("\n\n");
    }
}

/** Prints the metadata of all blocks **/
void print_list() {
    struct block* curr = head;
    printf("--HEAP--\n");
    if(!head) printf("EMPTY\n");
    while(curr) {
        print_block(curr);
        curr = curr->next;
    }
    printf("--END--\n");
}
