#include <stdio.h>
#include "map.h"

int main(void) {
	Map map = map_allocate(2);
	map_insert(map, "0", "world");
	map_insert(map, "1", "world1");
	map_insert(map, "2", "world2");
	printf("%s\n", (char*)map_find(map, "0"));
	printf("%s\n", (char*)map_find(map, "1"));
	printf("%s\n", (char*)map_find(map, "2"));
	map_free(map);
	printf("Normal exit\n");
	return(0);
}
