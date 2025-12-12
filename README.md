# P2-UEB_amb_sockets_TCP-IP_Xarxes

## Deployment

Machine 1:

```shell
$ make ser_comp
```

<details>
    <summary>Other orders</summary>

```shell
$ ./run.sh ser_comp
```

or

```shell
$ gcc -o ser p3-serUEB.c p3-aUEBs.* p3-tTCP.*
```
</details>

Machine 2:

```shell
$ make cli_comp
```

<details>
    <summary>Other orders</summary>

```shell
$ ./run.sh cli_comp
```

or

```shell
$ gcc -o cli p3-cliUEB.c p3-aUEBc.* p3-tTCP.* p3-aDNSc.*
```
</details>

## Run

Machine 1:

```shell
$ make ser_exec
```

<details>
    <summary>Other orders</summary>

```shell
$ ./run.sh ser_exec
```

or

```shell
$ ./ser
```
</details>

Machine 2:

```shell
$ make cli_exec
```

<details>
    <summary>Other orders</summary>

```shell
$ ./run.sh cli_exec
```

or

```shell
$ ./cli
```
</details>

## Remove files

```shell
$ make clean
```

or

```shell
$ ./run.sh clean
```

or

```shell
$ rm ser cli
```

Proves:

- pueb://84.88.155.97/primera.html
- pueb://84.88.155.97/segona.html
