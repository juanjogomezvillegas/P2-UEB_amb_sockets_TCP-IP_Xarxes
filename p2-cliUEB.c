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

/* Definició de constants, p.e.,                                          */

#define PORT_TIPIC 8000

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
    char* IPser;
    int portTCPser;
    char TextRes[256];
    int n;

    printf("Direccio IP del servidor UEB: ");
    scanf("%s", &IPser);
    printf("Port TCP del servidor UEB (per defecte %d): ", PORT_TIPIC);
    scanf("%d", &portTCPser);

    if(n = UEBc_DemanaConnexio(*IPser, portTCPser, TextRes) == -1){
        printf("Error en la connexió: %s", TextRes);
        return -1;
    }
    printf("%s\n", TextRes);

    int SckCon;
    int portTCPloc;
    char *IPloc;

    if(n = UEBc_TrobaAdrSckConnexio(SckCon, IPloc, portTCPloc, IPser, portTCPser, TextRes) == -1){
        printf("Error en trobar l'adreça del socket de connexió: %s", TextRes);
        return -1;

    }

    printf("Introduir nom del fitxer a obtenir(format: /nom_fitxer.ext): ");
    char nomFitxer[100];
    scanf("%s", nomFitxer);

    


}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
