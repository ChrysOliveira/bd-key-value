Este banco é um chave -> valor simples feito em C com extensões para CPF e Data feitos em Lua. 

Para buildar o programa localmente, é necessário ter os pacotes de lua developer instalado em sua máquina. Intalação Ubuntu: sudo apt install lua5.4 liblua5.4-dev

Comando para buildar localmente usando gcc: gcc -fdiagnostics-color=always -g *.c -o main -I/usr/include/lua5.4 -llua5.4

O banco possui apenas os comandos ADD e GET para adicionar registros ao banco. 

As extensoes validam se a chave do registro começa com "cpf_" ou "data_" para adicionar validações antes de persistir e para adicionar mascaras ao realizar o GET. 
