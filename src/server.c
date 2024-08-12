
    /*
    A simple CLI based game on C
    based on :https://github.com/samTime101/ASCII-Dice-Game
    Samip Regmi -samTime101
    Aug 12,2024
    Game name :Gamble
    Author:Samip Regmi
    file:server.c
    TODO: seperating and breaking into required functions
            and header files ,
            remove redundancies
            Make GUI using a C library
    */
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <arpa/inet.h>
    #include <stdlib.h>

    int main() {
        int serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverfd < 0) {
            perror("Socket creation failed");
            exit(1);
        }

        struct sockaddr_in server, client;
        server.sin_family = AF_INET;
        server.sin_port = htons(8080);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        int bindr = bind(serverfd, (struct sockaddr *)&server, sizeof(server));
        if (bindr < 0) {
            perror("Bind failed");
            close(serverfd);
            exit(1);
        }

        int listenr = listen(serverfd, 5);
        if (listenr < 0) {
            perror("Listen failed");
            close(serverfd);
            exit(1);
        }

        socklen_t size = sizeof(struct sockaddr);
        int clientfd = accept(serverfd, (struct sockaddr *)&client, &size);
        if (clientfd < 0) {
            perror("Accept failed");
            close(serverfd);
            exit(1);
        }
        /*
       Game logic starts here
        */
        int your_balance = 1000;
        int client_balance = 1000;
        int servernum, serverguess, clientnum, clientguess, serverbet, clientbet;

        while (1) {
            printf("WAITING FOR CLIENT's TURN\n");
            recv(clientfd, &clientnum, sizeof(clientnum), 0);
            recv(clientfd, &clientguess, sizeof(clientguess), 0);
            recv(clientfd, &clientbet, sizeof(clientbet), 0);

            printf("YOUR BALANCE: %d\t CLIENT BALANCE: %d\n", your_balance, client_balance);
            printf("Enter a number of your choice from 1 to 6: ");
            scanf("%d", &servernum);
            printf("Enter a guess (1 to 6) which your opponent will choose: ");
            scanf("%d", &serverguess);
            printf("Place a bet on your choice (your balance: %d): ", your_balance);
            scanf("%d", &serverbet);

            // validating inputs
            if (serverguess < 1 || serverguess > 6 || servernum < 1 || servernum > 6) {
                printf("Invalid input. Numbers must be between 1 and 6.\n");
                continue;
            }
            if (serverbet <= 0 || serverbet > your_balance) {
                printf("Invalid input. Place your bet between 1 and your balance.\n");
                continue;
            }

            send(clientfd, &servernum, sizeof(servernum), 0);
            send(clientfd, &serverguess, sizeof(serverguess), 0);
            send(clientfd, &serverbet, sizeof(serverbet), 0);

            if (serverguess == clientnum) {
                printf("You guessed it right!\n");
                your_balance += serverbet;
            } else {
                printf("You guessed it wrong\n");
                your_balance -= serverbet;
            }
            send(clientfd, &your_balance, sizeof(your_balance), 0);

            if (clientguess == servernum) {
                printf("Client guessed it right\n");
                client_balance += clientbet;
            } else {
                printf("Client guessed it wrong\n");
                client_balance -= clientbet;
            }
            send(clientfd, &client_balance, sizeof(client_balance), 0);
            printf("You guessed : %d clients's Num : %d\nClient guessed : %d Your num:%d\n",serverguess,clientnum,clientguess,servernum);

            printf("Your balance: %d, Client balance: %d\n", your_balance, client_balance);

            // end game condition ,
            if (your_balance <= 0 || client_balance <= 0) {
                printf("Game over! ");
                if (your_balance <= 0)
                    printf("You lost all your money.\n");
                if (client_balance <= 0)
                    printf("Client lost all their money.\n");
                printf("GAMBLE \n********Made by Samip Regmi********\n");
                break;
            }
        }

        close(clientfd);
        shutdown(serverfd, 2);
        return 0;
    }
