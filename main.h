#ifndef __MAIN_H__
#define __MAIN_H__

#include "kvs.h"

void add(lua_State *L, KVSstore *store, const char *key, char *value);
char* get(lua_State *L, KVSstore *store, const void *key);
void error(lua_State *L, const char *fmt, ...);

#endif /* #define __MAIN_H__ */