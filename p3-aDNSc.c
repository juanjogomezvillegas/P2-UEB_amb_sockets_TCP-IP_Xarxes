/*******************************************************************************/
/*                                                                             */
/* L'aplicació UEB amb sockets TCP/IP                                          */
/* Fitxer capçalera de aDNSc.c                                                 */
/*                                                                             */
/* Autors: Juan José Gómez Villegas, u1987338, u1987338@campus.udg.edu, GEINF  */
/*         Martí Valverde Rodríguez, u1994928, u1994928@campus.udg.edu, GEINF  */ 
/* Data: 27 de desembre de 2025                                                */
/*                                                                             */
/*******************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

int extreureAddr(const char *NomDNS, char *IP);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int DNSc_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa DNS, en la part client.                */

/* Donat el nom DNS "NomDNS" obté la corresponent @IP i l'escriu a "IP"   */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "NomDNS" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud qualsevol.                                        */
/* "IP" és un "string" de C (vector de chars imprimibles acabat en        */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error.                                                  */
int DNSc_ResolDNSaIP(const char *NomDNS, char *IP, char *TextRes)
{
    if(extreureAddr(NomDNS, IP) == -1){
        sprintf(TextRes, "Error en Extreure l'IP del NomDNS: %d", NomDNS);
        return -1;
    }
    return 0;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es        */
/* troben a l'inici d'aquest fitxer.       */

/* Aquesta funció extreu l'adreça IP corresponent al nom DNS passat com  */
/* a paràmetre.                                                          */
int extreureAddr(const char *NomDNS, char *IP) {
    /*char *addr;
    addr = resolver(NomDNS);
    if(addr == NULL){
        return -1;
    }
    strcpy(IP, addr);*/
    return 0;
}