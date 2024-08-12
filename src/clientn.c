/*
A simple CLI based game on C
based on:https://github.com/samTime101/ASCII-Dice-Game
Samip Regmi -samTime101
Aug 12,2024
Game name :Gamble
Author:Samip Regmi
file:clientn.c for client ngrok
TODO: seperating and breaking into required functions
        and header files ,
        remove redundancies
        Make GUI using a C library

*/

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>


int main(int argc,char *argv[]) {

    if(argc < 2){
        printf("INVALID USAGE : %s <ngrokPort>",argv[0]);
        exit(0);
    }
    int clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in server;
    struct hostent *serverip;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));

    //NGROK
    serverip = gethostbyname("0.tcp.ap.ngrok.io");
    if (serverip == NULL) {
        printf("No such host\n");
        return -1;
    }
    memcpy(&server.sin_addr.s_addr, serverip->h_addr_list[0], serverip->h_length);

    /*
   for Local connection
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
    */

    int connectr = connect(clientfd, (struct sockaddr *)&server, sizeof(server));
    if (connectr < 0) {
        perror("Connect failed");
        close(clientfd);
        exit(1);
    }
    /*
   Game logic starts here
    */
    int your_balance = 1000;
    int server_balance = 1000;
    int servernum, serverguess, clientnum, clientguess, clientbet, serverbet;

    while (1) {
        printf("YOUR BALANCE: %d\t SERVER BALANCE: %d\n", your_balance, server_balance);
        printf("Enter a number of your choice from 1 to 6: ");
        scanf("%d", &clientnum);
        printf("Enter a guess (1 to 6) which your opponent will choose: ");
        scanf("%d", &clientguess);
        printf("Place a bet on your choice (your balance: %d): ", your_balance);
        scanf("%d", &clientbet);

        // Validate input
        if (clientguess < 1 || clientguess > 6 || clientnum < 1 || clientnum > 6) {
            printf("Invalid input. Numbers must be between 1 and 6.\n");
            continue;
        }
        if (clientbet <= 0 || clientbet > your_balance) {
            printf("Invalid input. Place your bet between 1 and your balance.\n");
            continue;
        }

        send(clientfd, &clientnum, sizeof(clientnum), 0);
        send(clientfd, &clientguess, sizeof(clientguess), 0);
        send(clientfd, &clientbet, sizeof(clientbet), 0);

        printf("WAITING FOR SERVER's TURN\n");
        recv(clientfd, &servernum, sizeof(servernum), 0);
        recv(clientfd, &serverguess, sizeof(serverguess), 0);
        recv(clientfd, &serverbet, sizeof(serverbet), 0);

        if (serverguess == clientnum) {
            printf("Server guessed it right!\n");
            server_balance += serverbet;
        } else {
            printf("Server guessed it wrong\n");
            server_balance -= serverbet;
        }

        if (clientguess == servernum) {
            printf("You guessed it right\n");
            your_balance += clientbet;
        } else {
            printf("You guessed it wrong\n");
            your_balance -= clientbet;
        }
        printf("You guessed : %d server's Num : %d\nServer guessed : %d Your num:%d\n",clientnum,servernum,serverguess,clientnum);
        recv(clientfd, &server_balance, sizeof(server_balance), 0);
        recv(clientfd, &your_balance, sizeof(your_balance), 0);

        printf("Your balance: %d, Server balance: %d\n", your_balance, server_balance);

        //game end conditions
        if (your_balance <= 0 || server_balance <= 0) {
            printf("Game over! ");
            if (your_balance <= 0)
                printf("You lost all your money.\n");
            if (server_balance <= 0)
                printf("Server lost all their money.\n");
            printf("GAMBLE \n********Made by Samip Regmi********\n");
            break;
        }
    }

    close(clientfd);
    return 0;
}
