#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_SIZE 3
#define KEY 13
#define TRUE 1
#define FALSE 0

/*
 * This funciton decodes the message sent from client using Substitution Cypher
*/
void decode(char board[][MAX_SIZE]){
    int i, j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < MAX_SIZE; j++){
            board[i][j] -= KEY;
        }
    }
}

/*
 * This funciton encodes the message sent to the client using Substitution Cypher
*/
void encode(char board[][MAX_SIZE]){
    int i, j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < MAX_SIZE; j++){
            board[i][j] += KEY;
        }
    }
}


/*
*  This function checks whether somebody have already won the game
*  @param board: The tic-tac-toe board game
*  @return: true if there is a winner, else false
*/
int isWinningBoard(char board[][MAX_SIZE]){
    int i;
    //Check Rows
    for (i = 0; i < MAX_SIZE; i++){
        if ((board[i][0] != ' ') && (board[i][0] == board[i][1]) && (board[i][0] == board[i][2])){
            return TRUE;
        }
    }

    //Check Columns
    for (i = 0; i < MAX_SIZE; i++){
        if ((board[0][i] != ' ') && (board[0][i] == board[1][i]) && (board[0][i] == board[2][i])){
            return TRUE;
        }
    }

    //Check Diagonals
    if ((board[0][0] != ' ') && (board[0][0] == board[1][1]) && (board[0][0] == board[2][2])){
            return TRUE;
    }

    if ((board[0][2] != ' ') && (board[0][2] == board[1][1]) && (board[0][2] == board[2][0])){
            return TRUE;
    }

    return FALSE;
}

/*
*   This functionchecks whether the board is full or not
*    @return: True if the game is over, otherwise false
*/
int isGameOver(char board[][MAX_SIZE]){
    int i, j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < MAX_SIZE; j++){
            if (board[i][j] == ' '){
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*
*   This function places a move randomly on an empty cell in the board
*/
void makeMove(char board[][MAX_SIZE]){
    int i, j;
    i = rand() % MAX_SIZE;
    j = rand() % MAX_SIZE;
    int movePlaced = FALSE;
    while (!movePlaced){
        if (board[i][j] == ' '){
            board[i][j] = 'O';
            movePlaced = TRUE;
        }
        else{
            i = rand() % MAX_SIZE;
            j = rand() % MAX_SIZE;
        }
    }
}

/*
*   This function displays the board
*/
void displayBoard(char board[][MAX_SIZE]){
    printf("TIC-TAC-TOE\n\n");
    printf("  %c | %c  | %c  \n--------------\n", board[0][0],
                             board[0][1], board[0][2]);
    printf("  %c | %c  | %c  \n--------------\n", board[1][0],
                             board[1][1], board[1][2]);
    printf("  %c | %c  | %c  \n\n", board[2][0],
                             board[2][1], board[2][2]);
}

int main(int argc, char **argv){
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int length;

    //Store the message
    char board[MAX_SIZE][MAX_SIZE];

    //Check for -h flag
    if (argc == 2){
        if (strcmp(argv[1], "-h") == 0){
            //Print all the flags information
            printf("Options:\n -p \t\t\t The flag for including port number\n");
            exit(0);
        }
    }

    //Check if user enters the port number or not
    if (argc != 3){
        fprintf(stderr, "Should enter the port number in the format: ./project2 -p PORT\n");
        exit(1);
    }

    //Create TCP welcoming socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Socket Failed!\n");
        exit(0);
    }

    //Read the port number from command line
    int portNumber = atoi(argv[2]);

    //Define the address for the server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Attach the address to the socket
    if((bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress))) < 0){
        fprintf(stderr, "Bind Failed!\n");
        exit(2);
    }

    //Look for connections
    if((listen(serverSocket, 5)) < 0){
        fprintf(stderr, "Listening failed!\n");
        exit(3);
    }

    //Accept the connection request
    length = sizeof(clientAddress);

   while(1){
       if((clientSocket = accept(serverSocket,(struct sockaddr*) &clientAddress, (socklen_t *)&length)) < 0){
            fprintf(stderr, "Accepting Failed!\n");
            exit(4);
            }
        else{
            printf("Server: got connection from %s\n", inet_ntoa(clientAddress.sin_addr));
        }

        // Receive the current board
        while((recv(clientSocket, board, sizeof(board), 0)) >= sizeof(board)){
            decode(board);
            //Make move
            if (!isWinningBoard(board)){
                if(!isGameOver(board)){
                    makeMove(board);
                }
            }

            encode(board);
            //Send the board back to the client
            if((send(clientSocket, board, sizeof(board), 0)) < 0){
                fprintf(stderr, "Couln't send!\n");
                exit(6);
            }
        }
    }
     //Close the client socket
    close(clientSocket);
    return 0;
}