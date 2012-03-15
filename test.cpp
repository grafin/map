/*
 * test.cpp
 *
 *  Created on: Feb 24, 2012
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#define err(msg) fprintf(stderr, msg)

typedef struct {
	int a;
	int b;
} myStruct_t;

myStruct_t foo, bar;

int test_free(Map map) {
	if(!map_free(map)) {
		err("test_free failed map_free returned an error\n");
		return(0);
	}
	return(1);
}

int test_allocate() {
	Map map = NULL;
	for(int i = 65535; i > 0; i /= 2) {
		if(!(map = map_allocate(i))) {
			err("test_alloc failed map was not allocated\n");
			return(0);
		}
		printf("Map with size %d allocated successful\n", i);
		test_free(map);
	}
	return(1);
}
int test_instert(Map map, Key key1, Data data1,
						  Key key2, Data data2) {

	if(map_insert(map, NULL, data1)) {
		err("test_insert failed on step 1\n");
		return(0);
	}
	if(map_insert(map, key1, NULL)) {
		err("test_insert failed on step 2\n");
		return(0);
	}
	if(!map_insert(map, key1, data1)) {
		err("test_insert failed on step 3\n");
		return(0);
	}
	if(!map_insert(map, key1, data2)) {
		err("test_insert failed on step 4\n");
		return(0);
	}
	if(!map_insert(map, key1, data1)) {
		err("test_insert failed on step 5\n");
		return(0);
	}
	if(!map_insert(map, key2, data2)) {
		err("test_insert failed on step 6\n");
		return(0);
	}
	printf("test_insert successful\n");
	return(1);
}
int test_find(Map map, Key key1, myStruct_t* data1,
		  	  Key key2, myStruct_t* data2) {
	myStruct_t* ans1 = (myStruct_t*)map_find(map, key1);
	myStruct_t* ans2 = (myStruct_t*)map_find(map, key2);
	myStruct_t* ans3 = (myStruct_t*)map_find(map, "Not found");

	if(ans1 != data1) {
		err("test_find failed on step 1\n");
		return(0);
	}
	if(ans2 != data2) {
		err("test_find failed on step 2\n");
		return(0);
	}
	if(ans3) {
		err("test_find failed on step 3\n");
		return(0);
	}

	map_remove(map, key1);
	ans1 = (myStruct_t*)map_find(map, key1);
	if(ans1) {
		err("test_find failed on step 4\n");
		return(0);
	}
	map_clear(map);
	ans2 = (myStruct_t*)map_find(map, key2);
	if(ans2) {
		err("test_find failed on step 5\n");
		return(0);
	}

	printf("test_find successful\n");
	return(1);
}

int main(void) {
	if(!test_allocate())
		return(1);

	Map map = NULL;
	map = map_allocate(1024);

	foo.a = 0;
	foo.b = 1;
	bar.a = 2;
	bar.b = 3;

	if(!test_instert(map, "foo", &foo, "bar", &bar))
		return(1);
	if(!test_find(map, "foo", &foo, "bar", &bar))
		return(1);

	return(!test_free(map));
}

