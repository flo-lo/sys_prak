#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
//#include <errno.h> fuer Fehlerbehandlung (Harun)
#define BUFFR 256
#define GAMEKINDNAME "Quarto"
#define VERSION "VERSION 1.0"
//Importiere Variablen
extern char* ID;
extern char* playerNum;



//Prüfe ob der Gegenspieler im Spiel und bereit ist
/*
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
void testBuffer(char* buffer, int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        if (buffer[i] == '\n') printf(" \\n ");
        else
            printf(" %c ",buffer[i]);

    }


}
*/
//Bringe die zu sendende Antwort in ein für den Server angepasstes Format.
int sendReplyFormatted(int sock, char* reply)
{ char * container;
   container = malloc(sizeof(char)*strlen(reply)*2); // Container-String der die Antwort übernimmt und eine Newline angehängt bekommt.
    int err;
    strcpy(container,reply);
    strcat(container, "\n");
    err = send(sock,container,strlen(container),0); //Sende den Container an den Server, err für Fehlerbehandlung.
free(container);                                    //hier ebenfalss errno benutzen (Harun)
    return err;                                     //printf("ERROR: %s\n", strerror(errno));
}
/*
"Auch wenn kein Fehler auftritt, ist es dennoch sehr wichtig, den Rückgabewert zu überprüfen. 
Denn bei der Netzwerkprogrammierung sind auch gewisse Grenzen (Bandbreite) vorhanden – sprich, 
Sie können nicht unendlich viele Daten auf einmal versenden. Mit der Auswertung des Rückgabewerts 
können bzw. müssen Sie sich selbst darum kümmern, dass der eventuelle Rest, der nicht gesendet werden konnte, 
ebenfalls noch verschickt wird. Dies erledigen Sie, indem Sie data_len mit dem Rückgabewert von send() vergleichen. 
Durch diese Differenz (data_len – Rückgabewert) erhalten Sie die noch nicht gesendeten Daten." - Sollte implementiert werden (Harun)
*/


int performConnection(int sock)
{

   // gameID = malloc(sizeof(char)*20);
    char* buffer = (char*) malloc(sizeof(char)*BUFFR); //Initialisiere den Buffer für die Kommunikation
    //strcpy(gameID,"ID  fm1y4PiVKfU");
    //strcpy(gameID,"ID  528902d1b0074");

//Initialisiere Hilfsvariablen für die formatierte Ausgabe einer Serverantwort
    char* reader;
    char* readInt;
    reader = malloc(sizeof(char)*20);
    readInt     = malloc(sizeof(char));
    int var1;


    int err; //Return-Wert von err für Fehlerbehandlung

    err = recv(sock, buffer, BUFFR-1, 0); //Empfange Server Nachrichtung und speichere sie in Buffer
    sscanf(buffer, "%*s%*s%*s%s", reader);
    printf("\nDie Version des Servers ist: %s\n", reader);
    if (err > 0) buffer[err]='\0'; //Markiere Ende der Übertragung mit \0
    //printf("\n%s",buffer); //Ausgabe

    err = sendReplyFormatted(sock,VERSION);
    //send(sock,VERSION,strlen(VERSION),0); //Schicke Version des Spieles an Server

    recv(sock, buffer, BUFFR-1, 0);
    if (err > 0) buffer[err]='\0';
    //printf("\n%s",buffer);


    if (buffer[0] == '-')
    {

        printf("\nDer Server akzeptiert die Version dieses Clients nicht!\n");
        close(sock);
        return EXIT_FAILURE;
    }
    else
    {

        printf("\nDie Client-Version wurde akzeptiert, übertrage Spiel-ID...\n");



    }

    err = sendReplyFormatted(sock, ID); //Schicke Spiel-ID an Server
    //err =  send(sock,gameID,strlen(gameID),0);

    err = recv(sock, buffer, BUFFR-1, 0);

    if (err > 0) buffer[err]='\0';
    //printf("\n%s",buffer);



    sscanf(buffer, "%*s%*s%s", reader); //Scanne Antwort nach relevanten Variablen.

    if (strcmp(reader,"Quarto") !=0)   //Teste ob auch das richtige Spiel vom Server gespielt wird, falls nicht beende Verbindung
    {
        printf("Das Spiel, das der Server spielt, ist nicht Quarto! Beende Verbindung.");
        // printf("\n%s",buffer);

        close(sock);
        return EXIT_FAILURE;
    }
    else
    {
        printf("\nDer Server möchte %s spielen. Und wir auch!\n", reader);

    }

//Schicke Spieler Konfiguration und lese Nachrichten vom Server
    err = recv(sock, buffer, BUFFR-1, 0);
    if (err > 0) buffer[err]='\0';
    printf("\nSpiel: %s",&buffer[2]); //Zeige Spielnamen an, schneide das "+" ab

err = sendReplyFormatted(sock,playerNum);
   // err =  send(sock,"PLAYER\n",strlen("PLAYER\n"),0);

    err = recv(sock, buffer, BUFFR-1, 0);
    if (err > 0) buffer[err]='\0';

//Teste ob überhaupt noch was frei ist, falls ja gebe Name des Spielers und Nummer aus, falls nicht beende Übertragung
    if (buffer[0] == '-')
    {

        printf("\nAlle Plaetze sind bereits belegt, versuchen sie es spaeter noch einmal!\n");
        close(sock);
        return EXIT_FAILURE;
    }
    else
    {

        sscanf(buffer, "%*s %*s %c %s",readInt, reader);
        printf("\nDu spielst mit dem Namen %s, deine Nummer ist %s\n", reader,readInt);

    }

    err = recv(sock, buffer, BUFFR-1, 0);
    if (err > 0) buffer[err]='\0';


    sscanf(buffer, "%*s %*s %i", &var1); //Scanne Anzahl der Spieler, normalerweise immer 2

    printf("\nEs spielen %i Spieler.", var1);
    sscanf(buffer, "%*[^\n]%*s %s %s %i", readInt,reader,&var1); //Scanne Namen des Spielers und überprüfe ob dieser Bereit ist.
    if (var1 == 0)
    {
        printf("\nSpieler %s mit der Nummer %s ist noch nicht bereit.\n",reader,readInt );
    }
    else
    {
        printf("\nSpieler %s mit der Nummer %s ist bereit!\n",reader,readInt );

    }


//WAIT <->OKWAIT Schleife, später zu implementieren.
      do
     {
         send(sock,"OKWAIT\n",strlen("OKWAIT\n"),0);
         err = recv(sock, buffer, BUFFR-1, 0);
         if (err > 0) buffer[err]='\0';
         printf("\nServer bittet zu warten.\n");

     }
     while (strcmp(buffer,"+ WAIT\n") == 0);


    free(buffer);
    free(reader);
    free(readInt);
    if (err < 0) perror("Fehler bei der Kommunikation:");
    return EXIT_SUCCESS;

}




