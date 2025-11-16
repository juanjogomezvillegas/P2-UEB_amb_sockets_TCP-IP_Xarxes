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

#define FILE_CONFIG "p2-serUEB.cfg"

/* Definició de variables globals, p.e.,                                  */
int sesc;
int scon;

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
void aturadaS(int signal);
int exitError(char* textRes);
void Tanca(int Sck);
int ReadConf(char* file_cfg, char* arrel_lloc_ueb, char *TextRes);

int main(int argc,char *argv[]) {
    /* Senyals.                                                           */
    signal(SIGINT, aturadaS);

    /* Declaració de variables                                            */
    int port_tipic;
    char arrel_lloc_ueb;
    char textRes;
    char iprem[16], iploc[16];
    int portrem, portloc;
    int retornPeticio;
    char tipus[4];
    char nomFitxer[10000];

    /* Situació pre-inicial                                               */

    // llegeix el port típic i l'arrel del lloc UEB
    if ((port_tipic = ReadConf(FILE_CONFIG, &arrel_lloc_ueb, &textRes)) == -1) {
        port_tipic = 8000;
    }

    if (UEBs_IniciaServ(&sesc, port_tipic, &textRes) == -1) { // crea el seu socket d'escolta amb aquest port típic
        exit(exitError(&textRes));
    }

    printf("\nServidor UEB iniciat al #Port=%d.\n", port_tipic);

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

        printf("\nSocket local @IP=%s;#port=%d.\nS'ha connectat un C amb @IP=%s;#port=%d.\n", iploc, portloc, iprem, portrem);

        retornPeticio = 0;
        while (retornPeticio != -3 && retornPeticio != -2) {
            // rep i serveix la petició
            if ((retornPeticio = UEBs_ServeixPeticio(scon, tipus, nomFitxer, &textRes)) == -1) {
                Tanca(sesc);
                Tanca(scon);
                exit(exitError(&textRes));
            }

            if (retornPeticio == -2) {
                printf("\n%s\n", &textRes);
            } else {
                if (retornPeticio != -3) {
                    if (retornPeticio == 0) {
                        // es mostra per pantalla la petició: “obtenir”, nom_fitxer, @socket (@IP:#portTCP) de C i S
                        printf("\nobtenir, %s, @socket del C %s:%d, @socket del S %s:%d.\n", nomFitxer, iploc, portloc, iprem, portrem);
                    } else if (retornPeticio == 1) {
                        // es mostra per pantalla la petició: “obtenir”, nom_fitxer, @socket (@IP:#portTCP) de C i S
                        printf("\nerror, fitxer %s no trobat, @socket del C %s:%d, @socket del S %s:%d.\n", nomFitxer, iploc, portloc, iprem, portrem);
                    } else {
                        printf("\n%s\n", &textRes);
                    }
                } else {
                    printf("\nC desconnectat\n");
                }
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

/* Tanca els sockets oberts abans d'aturar l'execució del S suaument.     */
/*                                                                        */
/* Retorna:                                                               */
/*   0 sempre                                                             */
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

/* Llegeix el port tipic UEB del fitxer entrat per paramètre              */
/* i el retorna.                                                          */
/*                                                                        */
/* Paràmetre file_cfg, correspon a un string que conté el nom del fitxer  */
/* p2-serUEB.cfg, o un altre nom amb extensió .cfg                        */
/*                                                                        */
/* Retorna:                                                               */
/*  el port tipic llegit si tot va bé;                                    */
/* -1 si hi ha error.                                                     */
int ReadConf(char* file_cfg, char* arrel_lloc_ueb, char *TextRes) {
    // declaració de variables
    FILE *file;
    char line[256];
    int port = -1;

    // obre el fitxer cfg
    file = fopen(file_cfg, "r");
    if (file == NULL) { // retorna -1 si hi ha error
        sprintf(TextRes, "Error en obrir el fitxer!");
        return -1;
    }
    
    // si tot va bé, cerca la línia "numportTCP P", i finalment només retornarà "P"
    bool fi = false;
    while (fgets(line, sizeof(line), file) && !fi) {
        if (strncmp(line, "numportTCP", 10) == 0) {
            sscanf(line+11, "%d", &port);
        } else if (strncmp(line, "Arrel", 5) == 0) {
            sscanf(line+6, "%s", arrel_lloc_ueb);
        }
    }

    return port;
}
