# compilacio del programa
## compilació del servidor
ser_comp: p3-serUEB.c p3-aUEBs.* p3-tTCP.*
	gcc -o ser p3-serUEB.c p3-aUEBs.* p3-tTCP.*

## compilació del client
cli_comp: p3-cliUEB.c p3-aUEBc.* p3-tTCP.* p3-aDNSc.*
	gcc -o cli p3-cliUEB.c p3-aUEBc.* p3-tTCP.* p3-aDNSc.*

# esborrar fitxers d'execució
clean:
	rm cli ser

# regles d'execució
## execució del servidor
ser_exec:
	./ser

## execució del client
cli_exec:
	./cli
