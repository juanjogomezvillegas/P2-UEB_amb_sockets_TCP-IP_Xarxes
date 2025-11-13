/*******************************************************************************/
/*                                                                             */
/* L'aplicació UEB amb sockets TCP/IP                                          */
/* Fitxer aUEBc.c que implementa la capa d'aplicació de UEB, sobre la          */
/* cap de transport TCP (fent crides a la "nova" interfície de la              */
/* capa TCP o "nova" interfície de sockets TCP), en la part client.            */
/*                                                                             */
/* Autors: Juan José Gómez Villegas, u1987338, u1987338@campus.udg.edu, GEINF  */
/*         Martí Valverde Rodríguez, u1994928, u1994928@campus.udg.edu, GEINF  */ 
/* Data: 05 de novembre de 2025                                                */
/*                                                                             */
/*******************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include del propi fitxer capçalera)                              */

#include "p2-tTCP.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Definició de constants, p.e.,                                          */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1);
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int UEBc_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa UEB, en la part client.                */

/* Crea un socket TCP "client" en una @IP i #port TCP local qualsevol, a  */
/* través del qual demana una connexió a un S UEB que escolta peticions   */
/* al socket TCP "servidor" d'@IP "IPser" i #portTCP "portTCPser".        */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "IPser" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud màxima de 16 chars (incloent '\0').               */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket TCP connectat si tot va bé;                */
/* -1 si hi ha un error a la interfície de sockets.                       */
int UEBc_DemanaConnexio(const char *IPser, int portTCPser, char *TextRes)
{
    int codiRes;
    int sckC;

    if ((sckC = TCP_CreaSockClient("0.0.0.0", 0)) == -1) {
        sprintf(TextRes, "UEBc_DemanaConnexio() -> TCP_CreaSockClient(): %s\n", T_ObteTextRes(&sckC));
        return -1;
    }

	if ((codiRes = TCP_DemanaConnexio(sckC, IPser, portTCPser)) == -1) {
        sprintf(TextRes, "UEBc_DemanaConnexio() -> TCP_DemanaConnexio(): %s\n", T_ObteTextRes(&codiRes));
        return -1;
    }

    return sckC;
}

/* Obté el fitxer de nom "NomFitx" del S UEB a través de la connexió TCP  */
/* d'identificador "SckCon". Escriu els bytes del fitxer a "Fitx" i la    */
/* longitud del fitxer en bytes a "LongFitx".                             */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "NomFitx" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud <= 10000 chars (incloent '\0').                   */
/* "Fitx" és un vector de chars (bytes) qualsevol (recordeu que en C,     */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si el fitxer existeix al servidor;                                  */
/*  1 la petició és ERRònia (p.e., el fitxer no existeix);                */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio, etc.); */
/* -3 si l'altra part tanca la connexió.                                  */
int UEBc_ObteFitxer(int SckCon, const char *NomFitx, char *Fitx, int *LongFitx, char *TextRes)
{
    /* Declarem les variables necessaries */
	int codiRes;
    char tipus[4];
    char info[10000];
    int long1;

    /* Construim el missatge i l'enviem */
    if ((codiRes = ConstiEnvMis(SckCon, "OBT\0", NomFitx, sizeof(NomFitx))) < 0) {
        sprintf(TextRes, "Error en ConstiEnvMis: %d", codiRes);
        return codiRes;
    }

    /* Rebem el missatge de resposta i el desconstruim */
    if ((codiRes = RepiDesconstMis(SckCon, tipus, info, &long1)) < 0) {
        sprintf(TextRes, "Error en RepiDesconstMis: %d", codiRes);
        return codiRes;
    }

    if (strcmp(tipus, "COR") == 0) {
        memcpy(Fitx, info, long1);
        *LongFitx = long1;
        sprintf(TextRes, "Fitxer rebut correctament (%d bytes)", long1);
        return 0;
    } else if (strcmp(tipus, "ERR") == 0) {
        sprintf(TextRes, "Error del servidor: %.*s", long1, info);
        return 1;
    } else {
        sprintf(TextRes, "Tipus desconegut: %s", tipus);
        return -2;
    }
}

/* Tanca la connexió TCP d'identificador "SckCon".                        */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*   0 si tot va bé;                                                      */
/*  -1 si hi ha un error a la interfície de sockets.                      */
int UEBc_TancaConnexio(int SckCon, char *TextRes)
{
	int codiRes;

	if ((codiRes = TCP_TancaSock(SckCon)) == -1) {
        sprintf(TextRes, "UEBc_TancaConnexio() -> TCP_TancaSock(): %s\n", T_ObteTextRes(&codiRes));
        return -1;
    }

    return 0;
}

/* Donat el socket TCP “connectat” d’identificador “SckCon”, troba        */
/* l’adreça del socket local, omplint “IPloc” i “portTCPloc” amb          */
/* respectivament, la seva @IP i #port TCP, i troba l'adreça del socket   */
/* remot amb qui està connectat, omplint “IPrem” i  “portTCPrem” amb      */
/* respectivament, la seva @IP i #port TCP.                               */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "IPloc" i "IPrem" són "strings" de C (vector de chars imprimibles      */
/* acabats en '\0') d'una longitud màxima de 16 chars (incloent '\0').    */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*   0 si tot va bé;                                                      */
/*  -1 si hi ha un error a la interfície de sockets.                      */
int UEBc_TrobaAdrSckConnexio(int SckCon, char *IPloc, int *portTCPloc, char *IPrem, int *portTCPrem, char *TextRes)
{
    int codiRes;

    if ((codiRes = TCP_TrobaAdrSockLoc(SckCon, IPloc, portTCPloc)) == -1) {
        sprintf(TextRes, "UEBc_TrobaAdrSckConnexio() -> TCP_TrobaAdrSockLoc(): %s\n", T_ObteTextRes(&codiRes));
        return -1;
    }

    if ((codiRes = TCP_TrobaAdrSockRem(SckCon, IPrem, portTCPrem)) == -1) {
        sprintf(TextRes, "UEBc_TrobaAdrSckConnexio() -> TCP_TrobaAdrSockRem(): %s\n", T_ObteTextRes(&codiRes));
        return -1;
    }

    return 0;
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int UEBc_FuncioExterna(arg1, arg2...)
{
	
} */

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es        */
/* troben a l'inici d'aquest fitxer.                                      */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int FuncioInterna(arg1, arg2...)
{
	
} */

/* "Construeix" un missatge de PUEB a partir dels seus camps tipus,       */
/* long1 i info1, escrits, respectivament a "tipus", "long1" i "info1"    */
/* (que té una longitud de "long1" bytes), i l'envia a través del         */
/* socket TCP “connectat” d’identificador “SckCon”.                       */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio).       */
int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1)
{
    char buff[10008];
    char campLong[5];

    if (strlen(tipus) != 4 || long1 < 0 || long1 > 9999)
        return -2;

    sprintf(campLong, "%04d", long1);

    memcpy(buff, tipus, 4);
    memcpy(buff + 4, campLong, 5);
    if (long1 > 0)
        memcpy(buff + 8, info1, long1);

    if (TCP_Envia(SckCon, buff, long1 + 8) == -1)
        return -1;

    return 0;
}

/* Rep a través del socket TCP “connectat” d’identificador “SckCon” un    */
/* missatge de PUEB i el "desconstrueix", és a dir, obté els seus camps   */
/* tipus, long1 i info1, que escriu, respectivament a "tipus", "long1"    */
/* i "info1" (que té una longitud de "long1" bytes).                      */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio);       */
/* -3 si l'altra part tanca la connexió.                                  */
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1)
{
	char bufCap[8];
    int ret;

    ret = TCP_Rep(SckCon, bufCap, 8);
    if (ret <= 0) return (ret == 0) ? -3 : -1;

    memcpy(tipus, bufCap, 4);
    tipus[4] = '\0';

    char campLong[5];
    memcpy(campLong, bufCap + 4, 4);
    campLong[4] = '\0';
    *long1 = atoi(campLong);

    if (*long1 < 0 || *long1 > 9999) return -2;

    if (*long1 > 0) {
        ret = TCP_Rep(SckCon, info1, *long1);
        if (ret <= 0) return (ret == 0) ? -3 : -1;
    }

    return 0;
}
