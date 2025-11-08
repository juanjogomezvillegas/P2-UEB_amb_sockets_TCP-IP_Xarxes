# P2-UEB_amb_sockets_TCP-IP_Xarxes

## Deployment

Màquina 1:

```shell
$ gcc -o cli p2-cliUEB.c p2-aUEBc.* p2-tTCP.*
```

Màquina 2:

```shell
$ gcc -o ser p2-serUEB.c p2-aUEBs.* p2-tTCP.*
```

## Run

Màquina 1:

```shell
$ ./cli
```

Màquina 2:

```shell
$ ./ser
```
