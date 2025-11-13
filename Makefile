# compilacio del programa
compile:
	gcc -o ser p2-serUEB.c p2-aUEBs.* p2-tTCP.*
	gcc -o cli p2-cliUEB.c p2-aUEBc.* p2-tTCP.*

# esborrar fitxers d'execució
clean:
	rm cli ser

server:
	./ser

client:
	./cli
