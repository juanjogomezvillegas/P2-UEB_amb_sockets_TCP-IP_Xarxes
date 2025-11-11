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

/* Definició de constants, p.e.,                                          */

#define PORT_TIPIC 8000

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
void aturadaS(int signal, int SckCon);
int exitError(char* textRes);
void Tanca(int Sck);
/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
    signal(SIGINT, aturadaS);

    //Declaració de les variables necessaries per fer la connexió
    char *IPser;
    int portTCPser;
    char TextRes[256];
    int n = -1;

    //Demanem al usuari la IP i el port del servidor
    printf("Direccio IP del servidor UEB: ");
    scanf("%s", &IPser);
    printf("Port TCP del servidor UEB (per defecte %d): ", PORT_TIPIC);
    scanf("%d", &portTCPser);

    //intentem connectarnos, si no ho aconseguim tornem a demanar les dades
    while(n == -1){
        printf("Direccio IP del servidor UEB: ");
        scanf("%s", &IPser);
        printf("Port TCP del servidor UEB (per defecte %d): ", PORT_TIPIC);
        scanf("%d", &portTCPser);
        n = UEBc_DemanaConnexio(IPser, portTCPser, TextRes);
        if(n == -1){
            printf("Error en demanar la connexió: %s", TextRes);
        }
    }
    printf("%s\n", TextRes);

    //Declarem les variables per obtenir les adreces del socket de connexió
    int SckCon;
    int portTCPloc;
    char *IPloc;

    if(n = UEBc_TrobaAdrSckConnexio(SckCon, IPloc, portTCPloc, IPser, portTCPser, TextRes) == -1){
        printf("Error en obtenir les adreces del socket de connexió: %s", TextRes);
        Tanca(SckCon);
        exit(exitError(&TextRes));
    }
    //Ara que tenim el socket i les adreçes locals ja podem demanar el fitxer

    //Declarem les variables per obtenir el fitxer
    char *Fitx;
    int LongFitx;

    printf("Introduir nom del fitxer a obtenir(format: \"/nom_fitxer.ext\"): ");
    char nomFitxer[100];
    scanf("%s", nomFitxer);

    /* Crida a la funció per sol·licitar el fitxer i rebre'l (Dintre de la funcio és construeix el 
    missatge d'anada i després es deconstrueix el de tornada)*/
    if(n = UEBc_ObteFitxer(SckCon, nomFitxer, Fitx, LongFitx, TextRes) == -1){
        printf("Error en obtenir el fitxer: %s", TextRes);
        Tanca(SckCon);
        exit(exitError(&TextRes));
    }
    printf("Fitxer rebut correctament. Longitud: %d bytes\n", LongFitx);

    // Tancar la connexió
    if(n = UEBc_TancaConnexio(SckCon, TextRes) == -1){
        printf("Error en tancar la connexió: %s", TextRes);
        Tanca(SckCon);
        exit(exitError(&TextRes));
    }
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */
    
void aturadaS(int signal, int SckCon) {
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
    char textRes;
    if (UEBc_TancaConnexio(Sck, &textRes) == -1) {
        exit(exitError(&textRes));
    }
}

