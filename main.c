// https://codereview.stackexchange.com/questions/63427/simple-key-value-store-in-c
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "command.h"

int main(int argc, char *argv[]) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, "script.lua");

  char input[100];
  KVSstore *banco;

  // Inicializando KVS
  banco = kvs_create();

  printf("Execute algum comando. Exemplos:\n");
  printf("- ADD <chave> <valor> --> Adiciona o valor <valor> na chave <chave>\n");
  printf("- GET <chave>         --> Retorna o valor da chave <chave> se existir\n");
  printf("- EXIT                --> Sair do programa\n");

  for (;;) {
    // Imprime e mantém na mesma linha
    printf("> ");
    fflush(stdout);
    gets(input);

    Command command = get_command(input);

    // se igual, strcmp == 0
    if (!strcmp(command.command, "EXIT")) {
      printf("Inté!\n");
      break;
    } else if (!strcmp(command.command, "ADD")) {
      add(L, banco, command.key, command.value);
    } else if (!strcmp(command.command, "GET")) {
      char *value = get(L, banco, command.key);
      printf("%s\n", value);
    } else {
      printf("Comando não reconhecido!\n");
    }
  }

  kvs_destroy(banco);
  lua_close(L);
  return 0;
}

void error(lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  // lua_close(L);
  // exit(EXIT_FAILURE);
}

void add(lua_State *L, KVSstore *store, const char *key, char *value) {
  if (!starts_with(key, "cpf_") && !starts_with(key, "data_")){
    kvs_put(store, key, value);
  }else if (starts_with(key, "cpf_")){
    add_cpf(L, store, key, value);
  } else if(starts_with(key, "data_")){
    add_data(L, store, key, value);
  }
}

char* get(lua_State *L, KVSstore *store, const void *key){
  if (!starts_with(key, "cpf_") && !starts_with(key, "data_")){
    return kvs_get(store, key);
  }else if (starts_with(key, "cpf_")){
    return get_cpf(L, store, key);
  }
}

int starts_with(const char *a, const char *b){
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

void add_cpf(lua_State *L, KVSstore *store, const char *key, char *value){
  lua_getglobal(L, "ValidaCpf");
  lua_pushstring(L, value);

  if (lua_pcall(L, 1, 1, 0) != 0){
    error(L, "error running function `ValidaCpf' in %s\n", lua_tostring(L, -1));
    return;
  }

  if (!lua_isboolean(L, -1)) error(L, "function `ValidaCpf' must return a boolean\n");

  int result = lua_toboolean(L, -1);
  lua_pop(L, 1);

  if (result) {
    kvs_put(store, key, value);
  } else {
    printf("O CPF informado nao e valido\n");
  }
}

void add_data(lua_State *L, KVSstore *store, const char *key, char *value){
  lua_getglobal(L, "ValidaData");
  lua_pushstring(L, value);

  if (lua_pcall(L, 1, 1, 0) != 0){
    error(L, "error running function `ValidaData' in %s\n", lua_tostring(L, -1));
    return;
  }

  if (!lua_isboolean(L, -1)) error(L, "function `ValidaData' must return a boolean\n");

  int result = lua_toboolean(L, -1);
  lua_pop(L, 1);

  if (result) {
    kvs_put(store, key, value);
  } else {
    printf("A Data informada nao e valida\n");
  }
}

char* get_cpf(lua_State *L, KVSstore *store, const void *key){
  lua_getglobal(L, "MascaraCPF");
  char* valor_original = kvs_get(store, key);
  lua_pushstring(L, valor_original);

  if (lua_pcall(L, 1, 1, 0) != 0){
    error(L, "error running function `MascaraCPF' in %s\n", lua_tostring(L, -1));
    return;
  }

  if (!lua_isstring(L, -1)) error(L, "function 'MarcaraCPF' must return a string\n");

  char* result = lua_tostring(L, -1);
  lua_pop(L, 1);

  return result;
}