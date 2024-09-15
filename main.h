#ifndef __MAIN_H__
#define __MAIN_H__

#include "kvs.h"

void add(lua_State *L, KVSstore *store, const char *key, char *value);
void add_cpf(lua_State *L, KVSstore *store, const char *key, char *value);
void add_data(lua_State *L, KVSstore *store, const char *key, char *value);
char* get(lua_State *L, KVSstore *store, const void *key);
char* get_cpf(lua_State *L, KVSstore *store, const void *key);
void error(lua_State *L, const char *fmt, ...);
int starts_with(const char *a, const char *b);

#endif /* #define __MAIN_H__ */