/*******************************************************************************/
/*                                                                             */
/* L'aplicació UEB amb sockets TCP/IP                                          */
/* Fitxer cliUEB.c que implementa la interfície aplicació-usuari               */
/* d'un client de l'aplicació UEB, sobre la capa d'aplicació de                */
/* (la part client de) UEB (fent crides a la interfície de la part             */
/* client de la capa UEB).                                                     */
/*                                                                             */
/* Autors: Juan José Gómez Villegas, u1987338, u1987338@campus.udg.edu, GEINF  */
/*         Martí Valverde Rodríguez, u1994928, u1994928@campus.udg.edu, GEINF  */ 
/* Data: 05 de novembre de 2025                                                */
/*                                                                             */
/*******************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "p2-aUEBc.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

/* Definició de constants, p.e.,                                          */

#define FILE_PORT_TIPIC "p2-serUEB.cfg"
int SckCon;                             // socket de connexió global per gestionar el tancament en cas de senyal
int PORT_TIPIC;                         // port UEB típic

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
void aturadaC(int signal);
int exitError(char* textRes);
void Tanca(int Sck);
int readPortTipic(char* file_cfg);

int main(int argc,char *argv[]) {
    /* Senyals.                                                           */
    signal(SIGINT, aturadaC);

    PORT_TIPIC = readPortTipic(FILE_PORT_TIPIC);

    /* Declaració de variables                                            */
    struct timeval time_ini_resposta, time_fi_resposta, time_ini_envia, time_fi_envia;
    int option;
    char textRes;
    char ipSer[16], vellaIpSer[16];
    int portSer, vellPortSer = PORT_TIPIC;
    char IPloc[16], IPrem[16];
    int portloc, portrem;
    // variables per obtenir el fitxer
    char nomFitxer[100];
    char Fitxer[10000];   // buffer per rebre el fitxer
    int longFitxer;

    bool sortir, connexioIgual = false;
    while (sortir == false) {
        memcpy(vellaIpSer, ipSer, sizeof(ipSer));
        vellPortSer = portSer;

        /* Situació inicial                                                   */
        printf("Què vols fer [obtenir -> 1|sortir -> 0]? ");
        scanf("%d", &option);

        if (option == 1) { // l'usuari vol obtenir un fitxer

            gettimeofday(&time_ini_resposta, NULL);

            /* El C crea el seu socket en un #port TCP qualsevol i, demana al S la connexió TCP */
            connexioIgual = false;
            do {
                // demana la IP del servidor
                printf("Entra l'@IP del servidor UEB: \n");
                scanf("%63s", ipSer);
                // demana la port del servidor
                printf("Entra el #Port del servidor UEB [0 pel port típic]: \n");
                scanf("%d", &portSer);
                if (portSer == 0) { // si entra 0 assigna el port típic
                    portSer = PORT_TIPIC;
                }
                if (SckCon > 0 && memcmp(ipSer, vellaIpSer, sizeof(ipSer)) == 0 && portSer == vellPortSer) {
                    connexioIgual = true;
                } else {
                    if (SckCon > 0) {
                        Tanca(SckCon);
                    }
                    if ((SckCon = UEBc_DemanaConnexio(ipSer, portSer, &textRes)) == -1) {
                        printf("%s\n", &textRes);
                    }
                }
            } while (SckCon == -1);

            /* dins la connexió TCP establerta, el C envia al S la petició UEB                    */

            do {
                // demana el nom del fitxer que vol obtenir del servidor
                printf("Entra el nom d'un fitxer [ha de començar per \"/\"]: \n");
                scanf("%255s", nomFitxer);
                if (nomFitxer[0] != '/') {
                    perror("El nom d'un fitxer ha de començar per \"/\".\n");
                }
            } while (nomFitxer[0] != '/');

            if (UEBc_TrobaAdrSckConnexio(SckCon, IPloc, &portloc, IPrem, &portrem, &textRes) == -1) {
                printf("Error en obtenir les adreçes del socket de connexio: %s\n", &textRes);
                Tanca(SckCon);
                exit(exitError(&textRes));
            }

            gettimeofday(&time_ini_envia, NULL);

            /* Crida a la funció per sol·licitar el fitxer i rebre'l                                */
            if (UEBc_ObteFitxer(SckCon, nomFitxer, Fitxer, &longFitxer, &textRes) == -1) {
                printf("Error en obtenir el fitxer: %s\n", &textRes);
                Tanca(SckCon);
                exit(exitError(&textRes));
            }

            // es mostra per pantalla la petició: “obtenir”, nom_fitxer, @socket (@IP:#portTCP) de C i S
            printf("\nobtenir, %s, @socket del C %s:%d, @socket del S %s:%d.\n", nomFitxer, IPloc, portloc, IPrem, portrem);

            printf("\n%s\n", Fitxer);

            printf("\nFitxer rebut correctament. Longitud: %d bytes\n", longFitxer);

            gettimeofday(&time_fi_envia, NULL);

            gettimeofday(&time_fi_resposta, NULL);

            double temps_envia = (time_fi_envia.tv_sec - time_ini_envia.tv_sec) + (time_fi_envia.tv_usec - time_ini_envia.tv_usec) / 1000000.0;
            double temps_resposta = (time_fi_resposta.tv_sec - time_ini_resposta.tv_sec) + (time_fi_resposta.tv_usec - time_ini_resposta.tv_usec) / 1000000.0;

            printf("\nTemps d'enviament: %.6f segons\n", temps_envia);
            printf("Temps de resposta: %.6f segons\n", temps_resposta);
            printf("Velocitat efectiva: %.6f bits/segon\n", ((longFitxer*8)/temps_envia));

            /*char nomLocal[128];
            sprintf(nomLocal, ".%s", nomFitxer); // guarda amb el mateix nom (sense '/')
            FILE *f = fopen(nomLocal, "wb");
            if (f == NULL) {
                perror("No s'ha pogut crear el fitxer local");
                Tanca(SckCon);
                exit(exitError(textRes));
            }

            // Escriure el contingut rebut al fitxer local
            fwrite(Fitxer, 1, longFitxer, f);
            fclose(f);
            printf("Fitxer desat com a %s\n", nomLocal);

            // Obrir el fitxer rebut amb el navegador per defecte del sistema
            char cmd[200];
            sprintf(cmd, "xdg-open %s", nomLocal);
            system(cmd);*/
        } else if (option == 0) { // l'usuari vol sortir
            sortir = true;
        }
    }

    // Tancar la connexió
    if(UEBc_TancaConnexio(SckCon, &textRes) == -1) {
        printf("Error en tancar la connexió: %s", &textRes);
        exit(exitError(&textRes));
    }

    return 0;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
    
void aturadaC(int signal) {
    printf("\nS'ha detectat el senyal Control+c. Espera un moment...\n"); // informa a l'usuari per pantalla
    if(SckCon>0){
        Tanca(SckCon);
        printf("socket de connexió tancat correctament...\n"); // informa a l'usuari per pantalla
    }
    printf("\nFI del programa.\n"); // informa a l'usuari per pantalla de què s'ha aturat l'execució del C
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
    char* TextRes;
    if (UEBc_TancaConnexio(Sck, TextRes) == -1) {
        exit(exitError(TextRes));
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
        }
    }

    return port;
}
