/*
 * map.h
 *
 * Created on: Feb 9, 2012
 * Author: Boris Stepanenko
 */

#ifndef MAP_H_
#define MAP_H_

typedef void* Map;
typedef const void* Data;
typedef const void* Key;

Map map_allocate(const unsigned short); //Returns Map_p on success or NULL on error
int map_insert(const Map, Key, Data); //Returns 1 on success and 0 on error
int map_remove(const Map, Key); //Returns 1 on success and 0 on error
int map_clear(const Map); //Returns 1 on success and 0 on error
int map_free(Map); //Returns 1 on success and 0 on error
Data map_find(const Map, Key); //Returns Data_p on success and NULL on error

#endif /* MAP_H_ */
