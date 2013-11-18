#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

//test

#define PORTNUMMER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUFFR 256
#define GAMEKINDNAME "Quarto\n"
#define VERSION "VERSION 1.0\n"

extern int performConnection(int socket, char* gameID);

int main (int argc, char** argv )
{
    char *gameID;
    char * ID;
    gameID = malloc(sizeof(char)*17);
    ID = malloc(sizeof(char)*30);
    strcpy(ID,"ID  ");

    // char* buffer = (char*) malloc(sizeof(char)*BUFFR); //Buffer für den Stream

    //Überprüfe ob die Game-ID überhaupt die richtige Länge hat
    if ( argc == 1 || (strlen (argv[1])) != 11)
    {

        printf("Fehler: Der übergebene Parameter hat nicht die korrekte Länge");
        return EXIT_FAILURE;
    }
    else
    {
        strcpy(gameID,argv[1]);
        gameID =strcat(ID,gameID);
        gameID =strcat(ID, "\n");
        printf("Game ID: %s\n",ID);
    }

    // Initialisiert den für die Verbindung benötigten Socket //
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in host;
    struct hostent* ip;
    ip = (gethostbyname(HOSTNAME)); //Übersetze HOSTNAME in IP Adresse
    memcpy(&(host.sin_addr),ip ->h_addr,ip ->h_length);
    host.sin_family = AF_INET;
    host.sin_port = htons(PORTNUMMER);
    // FILE * stream = fdopen(sock, "r+b");

    if (connect(sock,(struct sockaddr*)&host, sizeof(host)) == 0) //Verbinde mit Server
    {
        printf("Verbindung mit %s hergestellt!",HOSTNAME);

    }
    else
    {
        printf("Fehler beim Verbindungsaufbau mit %s!", HOSTNAME);
        return EXIT_FAILURE;
    }

    performConnection(sock, ID);  //Führe Prolog Protokoll aus

    close(sock);
    return EXIT_SUCCESS;
}
