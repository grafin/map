/*
 * map.h
 *
 * Created on: Feb 9, 2012
 * Author: Boris Stepanenko
 */

#ifndef MAP_H_
#define MAP_H_

typedef void* Map;
typedef void* Data;
typedef void* Key;

Map map_allocate(const unsigned int); //Returns Map_p on success or NULL on error
int map_insert(Map const, const Key, const Data); //Returns 1 on success and 0 on error
int map_remove(Map const, const Key); //Returns 1 on success and 0 on error
int map_clear(Map const); //Returns 1 on success and 0 on error
int map_free(Map); //Returns 1 on success and 0 on error
Data map_find(Map const, const Key); //Returns Data_p on success and NULL on error
Data map_min(const Map); //Returns Data_p on success and NULL on error
Data map_max(const Map); //Returns Data_p on success and NULL

#endif /* MAP_H_ */
