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
#include <stdlib.h>
#include <signal.h>

/* Definició de constants, p.e.,                                          */

#define FILE_PORT_TIPIC "p2-serUEB.cfg"

/* Definició de variables globals, p.e.,                                  */
int sesc;
int scon;

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
void aturadaS(int signal);
int readPortTipic(char* file_cfg, char* arrel_lloc_ueb);
int exitError(char* textRes);
void Tanca(int Sck);

int main(int argc,char *argv[]) {
    /* Senyals.                                                           */
    signal(SIGINT, aturadaS);

    /* Declaració de variables                                            */
    int port_tipic;
    char arrel_lloc_ueb;
    char textRes;
    char iprem[16], iploc[16];
    int portrem, portloc;
    char buffer[1000];
    int bytes_llegits, bytes_escrits;

    /* Situació pre-inicial                                               */

    port_tipic = readPortTipic(FILE_PORT_TIPIC, &arrel_lloc_ueb); // llegeix el port típic

    if (UEBs_IniciaServ(&sesc, port_tipic, &textRes) == -1) { // crea el seu socket d'escolta amb aquest port típic
        exit(exitError(&textRes));
    }

    if (UEBs_TrobaAdrSckConnexio(sesc, iploc, &portloc, iprem, &portrem, &textRes) == -1) {
        Tanca(sesc);
        exit(exitError(&textRes));
    }

    printf("\nS iniciat amb @IP=%s;#Port=%d:\n", iploc, portloc);

    /* Situació inicial                                                   */

    for (;;) {
        if ((scon = UEBs_AcceptaConnexio(sesc, &textRes)) == -1) {
            Tanca(sesc);
            exit(exitError(&textRes));
        }

        if (UEBs_TrobaAdrSckConnexio(scon, iploc, &portloc, iprem, &portrem, &textRes) == -1) {
            Tanca(sesc);
            Tanca(scon);
            exit(exitError(&textRes));
        }

        printf("C @ip=%s;#port=%d connectat\n", iprem, portrem);

        bytes_llegits = 1;
        while (bytes_llegits > 0) {
            if ((bytes_llegits = read(scon, buffer, 1000)) == -1) { // S rep del C una peticio UEB i la respon
                Tanca(scon);
                Tanca(sesc);
                exit(exitError("Error in read message of the C.\n"));
            }

            if (bytes_llegits > 0) {

                //UEBs_ServeixPeticio(scon, tipuspet, fitxer, &textRes);

                // al S, la carpeta / o arrel del lloc UEB, correspon a la carpeta on s'executa el S
                // es mostra per pantalla la peticio: "obtenir", nom_fitxer, @socket(@IP:#Port) de C i S
                // i si el fitxer s'ha servit bé o no existia (o altres errors)
                
            } else {
                printf("C desconnectat\n");

                Tanca(scon);
            }
        }
    }

    /* Situació final                                                     */

    Tanca(sesc);

    exit(0);
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

void aturadaS(int signal) {
    printf("\nS'ha detectat el senyal Control+c. Espera un moment...\n"); // informa a l'usuari per pantalla

    if (sesc >= 0) {
        Tanca(sesc);

        printf("socket d'escolta tancat correctament...\n"); // informa a l'usuari per pantalla
    }

    if (scon >= 0) {
        Tanca(scon);

        printf("socket de connexió tancat correctament...\n"); // informa a l'usuari per pantalla
    }

    printf("\nFI del programa.\n"); // informa a l'usuari per pantalla de què s'ha aturat l'execució del S

    exit(0);
}

/* Llegeix el port tipic UEB del fitxer entrat per paramètre              */
/* i el retorna.                                                          */
/*                                                                        */
/* Paràmetre file_cfg, correspon a un string que conté el nom del fitxer  */
/* p2-serUEB.cfg, o un altre nom amb extensió .cfg                        */
/*                                                                        */
/* Retorna:                                                               */
/*  el port tipic llegit si tot va bé;                                    */
/* -1 si hi ha error.                                                     */
int readPortTipic(char* file_cfg, char* arrel_lloc_ueb) {
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
        } else if (strncmp(line, "Arrel", 5) == 0) {
            sscanf(line+6, "%s", arrel_lloc_ueb);
            printf("%s", arrel_lloc_ueb);
        }
    }

    return port;
}

/* Executa el codi per parar l'execució del programa en cas d'error.      */
/*                                                                        */
/* Només té un paràmetre, textRes com una cadena de caràcters que conté   */
/* el text retornat per la crida a la llibreria UEB.                      */
/*                                                                        */
/* Retorna:                                                               */
/*  -1 sempre                                                             */
int exitError(char* textRes) {
    perror(textRes);
    return -1;
}

/* Tanca un socket entrat per paràmetre (Sck, de tipus enter).                 */
/*                                                                             */
/* Retorna:                                                                    */
/*  void                                                                       */
void Tanca(int Sck) {
    char textRes;

    if (UEBs_TancaConnexio(Sck, &textRes) == -1) {
        exit(exitError(&textRes));
    }
}
