#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define get_rand(max) rand() % max
#define DEBUG 0
#if DEBUG == 1
	int step = 0;
#endif

typedef struct Element {
	Data data;
	Key key;
	struct Element* next;
	struct Element* prev;
} Element_t;
typedef Element_t* Element_p;
static Element_p element_allocate(const Data data = NULL, const Key key = NULL,
								  const Element_p next = NULL, const Element_p prev = NULL) {
#if DEBUG == 2
    printf("%*sDEBUG: Allocating element with data = '%s' and key = '%s'\n", step++, "", (const char*)data, (const char*)key);
#endif
	Element_p element = (Element_p)malloc(sizeof(Element_t));
	if(!element) {
#if DEBUG == 2
    printf("%*serror\n", step--, "");
#endif
		perror("Error in element_allocate while allocating memory for element");
		return(NULL);
	}
	element->data = data;
	element->key = key;
	element->next = next;
	element->prev = prev;
#if DEBUG == 2
    printf("%*sallocated\n", step--, "");
#endif
	return(element);
}
//TODO debug and error check
static void element_free(Element_p element) {
	if(!element)
		return;

	if(element->next)
		element->next->prev = element->prev;

	element->prev->next = element->next;

	free(element);
}
static void list_free(Element_p element) {
	Element_p curr = element;
	Element_p next = NULL;
	while(curr) {
#if DEBUG == 2
    printf("%*sDEBUG: Freeing element with key = '%s'\n", step++, "", (const char*)(curr->key));
#endif
		next = curr->next;
		free(curr);
#if DEBUG == 2
    printf("%*sfreed\n", step--, "");
#endif
		curr = next;
	}
}

typedef unsigned short int Hash_t;
static unsigned char Rand8[256];

typedef struct {
	Element_p* elements;
	Hash_t size;
} Map_t;
typedef Map_t* Map_p;
static Hash_t get_hash(const char *str, const Hash_t size) {
#if DEBUG == 1
    printf("%*sDEBUG: Calculating hash for '%s'\n", step++, "", str);
#endif
    if(!str) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in get_hash: str is NULL\n");
    	return(0);
    }

    Hash_t h;
    unsigned char h1, h2;

    if (*str == 0) {
#if DEBUG == 1
    printf("%*sthe hash is 0\n", step--, "");
#endif
    	return(0);
    }

    h1 = *str;
    h2 = *str + 1;
    str++;
    while (*str) {
        h1 = Rand8[h1 ^ *str];
        h2 = Rand8[h2 ^ *str];
        str++;
    }

    /* h is in range 0..65535 */
    h = ((Hash_t)h1 << 8)|(Hash_t)h2;
    /* use division method to scale */
#if DEBUG == 1
    printf("%*sthe hash is %d\n", step--, "", h % size);
#endif
    return(h % size);
}

Map map_allocate(const unsigned short size) {
#if DEBUG == 1
    printf("%*sDEBUG: Allocating memory for map sized %d\n", step++, "", size);
#endif
	Element_p* elements = (Element_p*)calloc(size, sizeof(Element_p));
	if(!elements) {
#if DEBUG == 1
    printf("%*serror\n", step--, "");

#endif
		perror("Error in map_allocate while allocating memory for elements");
		return(NULL);
	}
	for(int i = 0; i < size; i++) {
		elements[i] = element_allocate();
		if(!elements[i]) {
#if DEBUG == 1
	printf("%*serror\n", step--, "");
#endif
				perror("Error in map_allocate while allocating memory for elements[i]");
				for(int j = 0; j < i; j++)
					free(elements[j]);
				free(elements);
				return(NULL);
			}
	}
	Map_p map = (Map_p)malloc(sizeof(Map_t));
	if(!map) {
#if DEBUG == 1
    printf("%*serror\n", step--, "");

#endif
		perror("Error in map_allocate while allocating memory for map");
		free(elements);
		return(NULL);
	}
	map->elements = elements;
	map->size = size;
	srand(0);
	for(int i = 0; i < 256; i++)
		Rand8[i] = get_rand(256);
#if DEBUG == 1
    printf("%*sallocated and created Rand8 array\n", step--, "");

#endif
	return((Map)map);
}
int map_free(Map _map) {
#if DEBUG == 1
    printf("%*sDEBUG: Freeing map\n", step++, "");
#endif
	Map_p map = (Map_p)_map;
	if(!map) {
#if DEBUG == 1
    printf("%*serror\n", step--, "");

#endif
		fprintf(stderr,"Error in map_free trying to free NULL\n");
		return(0);
	}
	for(int i = 0; i < map->size; i++)
		list_free(map->elements[i]);
	free(map);
#if DEBUG == 1
    printf("%*sfreed\n", step--, "");

#endif
	return(1);
}
int map_clear(const Map _map) {
#if DEBUG == 1
    printf("%*sDEBUG: Clearing map\n", step++, "");
#endif
	Map_p map = (Map_p)_map;
	if(!map) {
#if DEBUG == 1
    printf("%*serror\n", step--, "");
#endif
		fprintf(stderr,"Error in map_clear trying to clear NULL\n");
		return(0);
	}
	for(int i = 0; i < map->size; i++) {
		list_free(map->elements[i]->next);
		map->elements[i]->next = NULL;
	}
#if DEBUG == 1
    printf("%*scleared\n", step--, "");
#endif
	return(1);
}

static Element_p map_find_element(Map_p map, const Key key) {
#if DEBUG == 1
    printf("%*sDEBUG: Looking for element with key = '%s'\n", step++, "", (const char*)key);
#endif
    if(!map) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_find_element: map is NULL\n");
    	return(NULL);
    }
    if(!key) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_find_element: key is NULL\n");
    	return(NULL);
    }
	Hash_t hash = get_hash((char*)key, map->size);
	if(!(map->elements[hash])) {
#if DEBUG == 1
    printf("%*snot found\n", step--, "");

#endif
		return(NULL);
	}
	Element_p curr = map->elements[hash]->next;
	while(curr) {
		if(strcmp((char*)(curr->key), (char*)key) == 0) {
#if DEBUG == 1
    printf("%*sfound\n", step--, "");
#endif
			return(curr);
		}
		curr = curr->next;
	}
#if DEBUG == 1
    printf("%*snot found\n", step--, "");
#endif
	return(NULL);
}
Data map_find(const Map _map, const Key key) {
#if DEBUG == 1
    printf("%*sDEBUG: Looking for data with key = '%s'\n", step, "", (const char*)key);
#endif
    Element_p element = map_find_element((Map_p)_map, key);
    if(!element)
    	return(NULL);
	return(element->data);
}

int map_insert(const Map _map, const Key key, const Data data) {
#if DEBUG == 1
	printf("%*sDEBUG: Inserting %s with key %s\n", step++, "", (const char*)data, (const char*)key);
#endif
    if(!_map) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_insert: map is NULL\n");
    	return(0);
    }
    if(!key) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_insert: key is NULL\n");
    	return(0);
    }
    if(!data) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_insert: data is NULL\n");
    	return(0);
    }
	Map_p map = (Map_p)_map;
	Hash_t hash = get_hash((const char*)key, map->size);
	Element_p element = NULL;

	if((element = map_find_element(map, key))) {
		element->data = data;
#if DEBUG == 1
    printf("%*sinserted\n", step--, "");
#endif
		return(1);
	}

	element = element_allocate(data, key);
	Element_p curr = map->elements[hash]->next;
	if(!curr) {
		map->elements[hash]->next = element;
		element->prev = map->elements[hash];
#if DEBUG == 1
    printf("%*sinserted\n", step--, "");
#endif
		return(1);
	} else {
		while(curr->next)
			curr = curr->next;
		curr->next = element;
		element->prev = curr;
#if DEBUG == 1
    printf("%*sinserted\n", step--, "");
#endif
		return(1);
	}
}
int map_remove(const Map _map, const Key key) {
#if DEBUG == 1
	printf("%*sDEBUG: Removing element with key %s\n", step++, "", (const char*)key);
#endif
    if(!_map) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_remove: map is NULL\n");
    	return(0);
    }
    if(!key) {
#if DEBUG == 1
    	printf("%*serror\n", step--, "");
#endif
    	fprintf(stderr, "Error in map_remove: key is NULL\n");
    	return(0);
    }
	Map_p map = (Map_p)_map;
	Element_p element = map_find_element(map, key);
	if(!element) {
#if DEBUG == 1
    	printf("%*sElement not found\n", step--, "");
#endif
    	return(0);
	}
	element_free(element);
#if DEBUG == 1
    	printf("%*sElement removed\n", step--, "");
#endif
	return(1);
}
