#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
//#include <errno.h> fuer bessere Fehlerbehandlung (Harun)

#define PORTNUMMER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"



char *ID; //Die modifizierte GameID die wir mit der performConnection Funktion teilen werden
char * playerNum; // Die gewünschte Spielernummer die wir optional angeben können
extern int performConnection(int socket);


int main (int argc, char** argv )
{
    char *gameID;
    ID = malloc(sizeof(char)*30);
    playerNum = malloc(sizeof(char)*10);
    gameID = malloc(sizeof(char)*20);
    strcpy(ID,"ID  "); // Vorbereitung der GameID für performConnection

    // char* buffer = (char*) malloc(sizeof(char)*BUFFR); //Buffer für den Stream

    //Überprüfe ob die angegebene Game-ID überhaupt die richtige Länge hat oder existiert
    if ( argc == 1 || (strlen (argv[1])) != 11)
    {

        printf("Fehler: Der uebergebene Parameter hat nicht die korrekte Laenge");
        return EXIT_FAILURE;
    }
    else

    {
        strcpy(playerNum, "PLAYER "); //Vorbereitung der Spielernummer für performConnection
        if (argc == 3 && strlen(argv[2]) == 1)
        {
            strcat(playerNum,argv[2]);

        }

        strcpy(gameID,argv[1]);
        printf("Deine Game ID: %s\n",gameID);
        strcat(ID,gameID);
    }

    // Initialisiert den für die Verbindung benötigten Socket //
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in host;
    struct hostent* ip;
    ip = (gethostbyname(HOSTNAME)); //Übersetze HOSTNAME in IP Adresse
    memcpy(&(host.sin_addr),ip ->h_addr,ip ->h_length);
    host.sin_family = AF_INET;
    host.sin_port = htons(PORTNUMMER);

    if (connect(sock,(struct sockaddr*)&host, sizeof(host)) == 0) //Verbinde mit Server
    {
        printf("\nVerbindung mit %s hergestellt!\n",HOSTNAME);

    }
    else
    {
        printf("\n Fehler beim Verbindungsaufbau mit %s!\n", HOSTNAME); //zur besseren Fehlerbehandlung sollte 
        return EXIT_FAILURE;                                            //printf("ERROR: %s\n", strerror(errno));
    }                                                                   //verwendet werden! (Harun)

    if ((performConnection(sock)) <0 )//Führe Prolog Protokoll aus
        perror(" Fehler:");

    free(gameID);
//free(ID); Warum hier Buffer Overflow?
    close(sock);
    return EXIT_SUCCESS;
}
