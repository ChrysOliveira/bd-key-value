// https://codereview.stackexchange.com/questions/63427/simple-key-value-store-in-c
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>

#include "command.h"
#include "kvs.h"
#include "main.h"

/* function declaration */
void add (lua_State* L, KVSstore *store, const char *key, char *value);
void get (lua_State* L, KVSstore *store, const void *key);
void error (lua_State *L, const char *fmt, ...);

int main(int argc, char *argv[]) {
  lua_State* L = luaL_newstate();
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
      kvs_put(banco, command.key, command.value);
    } else if (!strcmp(command.command, "GET")) {
      char *value = kvs_get(banco, command.key);
      printf("%s", value);
      printf("\n");
    } else {
      printf("Comando não reconhecido!\n");
    }
  }

  kvs_destroy(banco);
  lua_close(L);
  return 0;
}

void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(EXIT_FAILURE);
}