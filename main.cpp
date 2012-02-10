#include <stdio.h>
#include "map.h"

typedef struct {
	int a;
	int b;
} myStruct_t;

int main(void) {
	myStruct_t foo;
	foo.a = 0;
	foo.b = 1;
	myStruct_t bar;
	bar.a = 2;
	bar.b = 3;

	Map map = map_allocate(1024);
	map_insert(map, "foo", &foo);
	map_insert(map, "bar", &bar);
	myStruct_t* foo_ = (myStruct_t*)map_find(map, "foo");
	myStruct_t* bar_ = (myStruct_t*)map_find(map, "bar");
	printf("%d %d\n", foo_->a, foo_->b);
	printf("%d %d\n", bar_->a, bar_->b);
	map_free(map);
	printf("Normal exit\n");
	return(0);
}
