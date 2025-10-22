/*******************************************************************************/
/*                                                                             */
/* L'aplicació UEB amb sockets TCP/IP                                          */
/* Fitxer capçalera de tTCP.c                                                  */
/*                                                                             */
/* Autors: Juan José Gómez Villegas, u1987338, u1987338@campus.udg.edu, GEINF  */
/*         Martí Valverde Rodríguez, u1994928, u1994928@campus.udg.edu, GEINF  */ 
/* Data: 22 d'octubre de 2025                                                  */
/*                                                                             */
/*******************************************************************************/

/* Declaració de funcions EXTERNES de tTCP.c, és a dir, d'aquelles        */
/* funcions que es faran servir en un altre fitxer extern a tTCP.c,       */
/* p.e., int TCP_FuncioExterna(arg1, arg2...) { }                         */
/* El fitxer extern farà un #include del fitxer tTCP.h a l'inici, i       */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES són la "nova" interfície de la capa de transport TCP (la      */
/* "nova" interfície de sockets TCP).                                     */

int TCP_CreaSockClient(const char *IPloc, int portTCPloc);
int TCP_CreaSockServidor(const char *IPloc, int portTCPloc);
int TCP_DemanaConnexio(int Sck, const char *IPrem, int portTCPrem);
int TCP_AcceptaConnexio(int Sck, char *IPrem, int *portTCPrem);
int TCP_Envia(int Sck, const char *SeqBytes, int LongSeqBytes);
int TCP_Rep(int Sck, char *SeqBytes, int LongSeqBytes);
int TCP_TancaSock(int Sck);
int TCP_TrobaAdrSockLoc(int Sck, char *IPloc, int *portTCPloc);
int TCP_TrobaAdrSockRem(int Sck, char *IPrem, int *portTCPrem);
char* T_ObteTextRes(int *CodiRes);

/* int TCP_FuncioExterna(arg1, arg2...);                                  */
