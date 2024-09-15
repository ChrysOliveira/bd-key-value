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
  lua_close(L);
  exit(EXIT_FAILURE);
}

void add(lua_State *L, KVSstore *store, const char *key, char *value) {
  lua_getglobal(L, "baskara");
  lua_pushnumber(L, a);
  lua_pushnumber(L, b);
  lua_pushnumber(L, c);

  if (lua_pcall(L, 3, 2, 0) != 0)
    error(L, "error running function `calculo': %s", lua_tostring(L, -1));

  if (lua_isnil(L, -1))
    error(L, "There is no square root for these values of a, b and c");

  if (!lua_isnumber(L, -1)) error(L, "function `calculo' must return a number");

  r[0] = lua_tonumber(L, -2);
  r[1] = lua_tonumber(L, -1);

  lua_pop(L, 1);
  lua_pop(L, 1);

  kvs_put(store, key, value);
}

char* get(lua_State *L, KVSstore *store, const void *key){
  return kvs_get(store, key);
}