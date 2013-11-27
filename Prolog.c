#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "sharedVariables.h"

#define PORTNUMMER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

char *ID; //Die modifizierte GameID die wir mit der performConnection Funktion teilen werden
char * playerNum; // Die gewünschte Spielernummer die wir optional angeben können
config_struct *conf; // Die Struktur, die die Konfigurationsparameter der Datei speichert

int main (int argc, char** argv )
{
    conf = calloc(5,sizeof(config_struct));
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

        if (argc == 3)
        {
            if (openConfig(argv[2])!= 0)  //Falls Custom-config angegeben wurde
            {
                return EXIT_FAILURE;
            }
        }
        else
        {
            if  (openConfig("client.conf") != 0)  //Sonst Standard-config
            {
                return EXIT_FAILURE;
            }
        }
        strcpy(gameID,argv[1]);
        printf("Deine Game ID: %s\n",gameID);
        strcat(ID,gameID);
    }

    // Initialisiert den für die Verbindung benötigten Socket //
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in host;
    struct hostent* ip;
    ip = (gethostbyname(conf->hostname)); //Übersetze HOSTNAME in IP Adresse
    memcpy(&(host.sin_addr),ip ->h_addr,ip ->h_length);
    host.sin_family = AF_INET;
    host.sin_port = htons(conf->portnumber);

    if (connect(sock,(struct sockaddr*)&host, sizeof(host)) == 0) //Verbinde mit Server
    {
        printf("\nVerbindung mit %s hergestellt!\n",conf->hostname);
    }
    else
    {
        perror("\n Fehler beim Verbindungsaufbau");
        return EXIT_FAILURE;
    }

    performConnection(sock);//Führe Prolog Protokoll aus
    free(playerNum);
    free(gameID);
    free(ID);
    free(conf);
    return EXIT_SUCCESS;
}
