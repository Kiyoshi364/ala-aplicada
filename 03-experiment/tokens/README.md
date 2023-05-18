# Tokens

Esse programa lê todos os arquivos `.zig` em `zig_std`
e gera
`matrix.txt` (a matrix),
`tokens.txt` (as labels das colunas) e
`docks.txt` (as labels das linhas).

> Observe que em `matrix.txt`
os números são separados por espaços e
caso a linha não esteja completa,
complete com zeros
(isso foi uma forma de gerar a matrix em uma passada
e economizar espaço de disco).

Os arquivos estão commitados com a extênsão de `.bak`.

# Como rodar

Precisamos de um `zig` na versão `0.10.*`
(procure em [downloads](https://ziglang.org/downloads)).
Lembre-se de dar `git submodule update` antes
(talvez `--init` é necessário).

Temos 3 comandos interessantes:
```console
$ zig build lib2data # Anda pelos arquivos e gera os *.txt
$ zig build data2j   # Usa os *.txt para gerar os *.ijs
$ zig build run      # Roda os dois anteriores
```
