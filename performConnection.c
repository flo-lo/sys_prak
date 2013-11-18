#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#define BUFFR 256
#define GAMEKINDNAME "Quarto\n"
#define VERSION "VERSION 1.0\n"
//sscanf

//Prüfe ob der Gegenspieler im Spiel und bereit ist
int checkPlayerReady(char* response)
{

    if(strstr(response,"0") !=NULL)
    {
        printf("\nDein Gegner ist noch nicht bereit, bitte warte ein wenig.\n");
        return 1;
    }
    else
    {
        printf("\nDein Gegenspieler ist bereit");
        return 0;

    }

}
void testBuffer(char* buffer, int size) {
int i;
for (i=0;i<size;i++){

    printf("%c\n",buffer[i]);

}


}


int performConnection(int sock, char* gameID)
{
   // char *gameID;
//FILE * stream = fdopen(sock, "r+b");

  //  gameID = malloc(sizeof(char)*20);
    char* buffer = (char*) malloc(sizeof(char)*BUFFR);
  //  strcpy(gameID,"ID  fm1y4PiVKfU");
  //strcpy(gameID,"ID  528902d1b0074");

    int size; //Return-Wert von size für Fehlerbehandlung

    size = recv(sock, buffer, BUFFR-1, 0); //Empfange Server Nachrichtung und speichere sie in Buffer
    //if (size > 0) buffer[size]='\0'; //Markiere Ende der Übertragung mit \0
    printf("\n%s",buffer); //Ausgabe

    send(sock,VERSION,strlen(VERSION),0); //Schicke Version des Spieles an Server

    recv(sock, buffer, BUFFR-1, 0);
    // if (size > 0) buffer[size]='\0';
    printf("\n%s",buffer);
    //gameID =strcat(gameID, "\n");


    size =    send(sock,gameID,strlen(gameID),0); //Schicke Spiel-ID an Server


    size = recv(sock, buffer, BUFFR-1, 0);
    if (size > 0) buffer[size]='\0';
    printf("\n%s",buffer);

    if (strcmp(buffer,"+ PLAYING Quarto\n") !=0)   //Teste ob auch das richtige Spiel vom Server gespielt wird, falls nicht beende Verbindung
    {
        printf("Das Spiel ist nicht Quarto! Beende Verbindung.");
        //size = send(sock,buffer,strlen(buffer),0);
       // printf("\n%s",buffer);

        close(sock);
        return EXIT_FAILURE;
    }
    else
    {
//Schicke Spieler Konfiguration und lese Nachrichten vom Server
        size = recv(sock, buffer, BUFFR-1, 0);
        if (size > 0) buffer[size]='\0';
        printf("\nSpiel: %s",&buffer[2]); //Zeige Spielnamen an, schneide das "+" ab

        size =  send(sock,"PLAYER\n",strlen("PLAYER\n"),0);

        size = recv(sock, buffer, BUFFR-1, 0);
        if (size > 0) buffer[size]='\0';

        if (buffer[0] == '-')
        {
            printf("\nAlle Plaetze sind bereits belegt, versuchen sie es spaeter noch einmal!\n");
            close(sock);
            return EXIT_FAILURE;
        }
        else
        {


            printf("\n%s",buffer);
        }

        size = recv(sock, buffer, BUFFR-1, 0);
        if (size > 0) buffer[size]='\0';
        checkPlayerReady(buffer);
        printf("\n%s",buffer);
        do
        {
            send(sock,"OKWAIT\n",strlen("OKWAIT\n"),0);
            size = recv(sock, buffer, BUFFR-1, 0);
            if (size > 0) buffer[size]='\0';
            printf("\n%s",buffer);

        }
        while (strcmp(buffer,"+ WAIT\n") == 0);
    }
    free(buffer);
    if (size < 0) perror("Fehler bei der Kommunikation:");
    return EXIT_SUCCESS;

}




