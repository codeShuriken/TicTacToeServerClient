#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define MAX_SIZE 3
#define KEY 13

char winner;
void declareWinner(char c){
    if (c == 'X'){
        winner = 'X';
    }
    else if (c == 'O'){
        winner = 'O';
    }
    else{
        winner = 'D';
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
            declareWinner(board[i][0]);
            return TRUE;
        }
    }

    //Check Columns
    for (i = 0; i < MAX_SIZE; i++){
        if ((board[0][i] != ' ') && (board[0][i] == board[1][i]) && (board[0][i] == board[2][i])){
            declareWinner(board[0][i]);
            return TRUE;
        }
    }

    //Check Diagonals
    if ((board[0][0] != ' ') && (board[0][0] == board[1][1]) && (board[0][0] == board[2][2])){
        declareWinner(board[0][0]);
        return TRUE;
    }

    if ((board[0][2] != ' ') && (board[0][2] == board[1][1]) && (board[0][2] == board[2][0])){
        declareWinner(board[0][2]);
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
 * This funciton decodes the message sent from server using Substitution Cypher
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
 * This funciton decodes the message sent from server using Substitution Cypher
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

void displayInstructions(){
    printf("Choose a number from 1 to 9 as below to place your move(Can't choose a number where the place is already taken!).\n\n");
        printf("  1 | 2  | 3  \n--------------\n");
        printf("  4 | 5  | 6  \n--------------\n");
        printf("  7 | 8  | 9  \n\n");

        printf("Data is encoded using ROT13 Substitution Cipher:\n");
        //Print all the flags information
        printf("Options\n");
        printf("-s \t\t\t The flag for including server name\n");
        printf("-p \t\t\t The flag for including port number\n");
}

/*
*   This function places a move on an empty cell in the board
*/
void makeMove(char board[][MAX_SIZE], char input[]){
    int input1 = atoi(input);
    switch(input1){
        case 1:
        case 2:
        case 3:
            if (board[0][input1 - 1] == ' '){
                board[0][input1 - 1] = 'X';
            }
            else{
                printf("Already filled, enter in an unfilled cell!\n");
            }
            break;
        case 4:
        case 5:
            if (board[1][(input1%3) - 1] == ' '){
                board[1][(input1%3) - 1] = 'X';
            }
            else{
                printf("Already filled, enter in an unfilled cell!\n");
            }
            break;
        case 6:
            if (board[1][2] == ' '){
                board[1][2] = 'X';
            }
            else{
                printf("Already filled, enter in an unfilled cell!\n");
            }
            break;
        case 7:
        case 8:
            if (board[2][(input1%3) - 1] == ' '){
                board[2][(input1%3) - 1] = 'X';
            }
            else{
                printf("Already filled, enter in an unfilled cell!\n");
            }
            break;
        case 9:
            if (board[2][2] == ' '){
                board[2][2] = 'X';
            }
            else{
                printf("Already filled, enter in an unfilled cell!\n");
            }
            break;
        default:
            printf("Invalid input!(Enter number b/w 1-9)!\n");
        }
}

int checkInput(char input[], char board[][MAX_SIZE]){
    char c = input[0];
    if (c < '1' || c > '9'){
        return TRUE;
    }
    int i = atoi(input);
    switch(i){
        case 1:
        case 2:
        case 3:
            if (board[0][i - 1] == ' '){
                return FALSE;
            }
            else{
                return TRUE;
            }
            break;
        case 4:
        case 5:
            if (board[1][(i%3) - 1] == ' '){
                return FALSE;
            }
            else{
                return TRUE;
            }
            break;
        case 6:
            if (board[1][2] == ' '){
                return FALSE;
            }
            else{
                return TRUE;
            }
            break;
        case 7:
        case 8:
            if (board[2][(i%3) - 1] == ' '){
               return FALSE;
            }
            else{
                return TRUE;
            }
            break;
        case 9:
            if (board[2][2] == ' '){
                return FALSE;
            }
            else{
                return TRUE;
            }
            break;
        default:
            return TRUE;
        }
}

int main(int argc, char **argv){

    int clientSocket;
    struct sockaddr_in serverAddress;
    struct hostent* serverInfo;
    char ipAddress[128];
    int portNumber;

    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

     //Check if user enters the input in the right way
    if (argc != 5){
        fprintf(stderr, "Should enter input in the format:  ./p3client –s SERVER_NAME -p PORT_NUMBER\n");
        exit(0);
    }

    //Get IP Address of the server
    serverInfo = gethostbyname(argv[2]);

    if (serverInfo == NULL) {
        fprintf(stderr, "host information for %s not found\n", argv[1]);
        exit (1);
    }
    strcpy(ipAddress, inet_ntoa(*((struct in_addr*)serverInfo->h_addr_list[0])));
    printf("%s\n", ipAddress);

    //Read the port number from command line
    portNumber = atoi(argv[4]);

    //Create TCP welcoming socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Socket Failed!\n");
        exit(2);
    }

    //Define the address for the server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress);

    //serverAddress.sin_addr.s_addr =  inet_addr(ipAddress);

    if((connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) < 0){
        fprintf(stderr, "Connection Failed!\n");
        exit(3);
    }

    //Start game
    displayBoard(board);
    char input[3];

    printf("START GAME!\n");
    printf("You're X. Go First!(Type -h for help)\n");

    int gameOver = FALSE;
    while(!gameOver){
        //Wait till user makes a move
        printf("Your move: ");
        scanf("%s", input);

        while(checkInput(input, board)){
            if (strcmp(input, "-h") == 0){
                displayInstructions();
            }
            else{
                printf("Please enter a valid input!\n");
            }

            printf("Your move: ");
            scanf("%s", input);
        }

        makeMove(board, input);
        displayBoard(board);
        encode(board);

       if ((send(clientSocket, board, sizeof(board), 0)) < 0){
            fprintf(stderr, "Couldn't send!\n");
            exit(4);
        }

        if((recv(clientSocket, board, sizeof(board), 0)) < 0){
            fprintf(stderr, "Couldn't Receive!\n");
            exit(5);
        }
        decode(board);
        displayBoard(board);
        //Check if there is a winner
        if (isWinningBoard(board) || isGameOver(board)){
            gameOver = TRUE;
        }
    }

    close(clientSocket);

    //Print the game results.
    if (winner == 'X'){
        printf("You Win!\n");
    }
    else if (winner == 'O'){
        printf("Computer Wins!\n");
    }
    else{
        printf("Draw Match!\n");
    }

    return 6;
}