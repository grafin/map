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

int test_insert()

typedef struct {
	int a;
	int b;
} myStruct_t;

int main(void) {
	myStruct_t foo;
	foo.a = 0;
	foo.b = 1;

	if( !test_allocate())
		return(-1);

	Map map = map_allocate(1024);
	if( !test_free(map))
		return(-1);



	return(test_free(map));
}

