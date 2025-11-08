/*******************************************************************************/
/*                                                                             */
/* L'aplicació UEB amb sockets TCP/IP                                          */
/* Fitxer serUEB.c que implementa la interfície aplicació-administrador        */
/* d'un servidor de l'aplicació UEB, sobre la capa d'aplicació de              */
/* (la part servidora de) UEB (fent crides a la interfície de la part          */
/* servidora de la capa UEB).                                                  */
/*                                                                             */
/* Autors: Juan José Gómez Villegas, u1987338, u1987338@campus.udg.edu, GEINF  */
/*         Martí Valverde Rodríguez, u1994928, u1994928@campus.udg.edu, GEINF  */ 
/* Data: 05 de novembre de 2025                                                */
/*                                                                             */
/*******************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "p2-aUEBs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Definició de constants, p.e.,                                          */

#define FILE_PORT_TIPIC "p2-serUEB.cfg"

/* Definició de variables globals, p.e.,                                  */

int PORT_TIPIC;

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
int readPortTipic(char* file_cfg);

int main(int argc,char *argv[]) {
    PORT_TIPIC = readPortTipic(FILE_PORT_TIPIC); // llegeix el port típic

    /* Declaració de variables                                            */
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Llegeix el port tipic UEB del fitxer entrat per paramètre              */
/* i el retorna.                                                          */
/*                                                                        */
/* Paràmetre file_cfg, correspon a un string que conté el nom del fitxer  */
/* p2-serUEB.cfg, o un altre nom amb extensió .cfg                        */
/*                                                                        */
/* Retorna:                                                               */
/*  el port tipic llegit si tot va bé;                                    */
/* -1 si hi ha error.                                                     */
int readPortTipic(char* file_cfg) {
    // declaració de variables
    FILE *file;
    char line[256];
    int port = -1;

    // obre el fitxer cfg
    file = fopen(file_cfg, "r");
    if (file == NULL) { // retorna -1 si hi ha error
        perror("Error en obrir el fitxer!");
        return -1;
    }
    
    // si tot va bé, cerca la línia "numportTCP P", i finalment només retornarà "P"
    bool fi = false;
    while (fgets(line, sizeof(line), file) && !fi) {
        if (strncmp(line, "numportTCP", 10) == 0) {
            sscanf(line+11, "%d", &port);
            fi = true;
        }
    }

    return port;
}
