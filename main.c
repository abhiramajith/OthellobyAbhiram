#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

//Struct to hold Player information
typedef struct players{
    char playerName[20];
    char playerColour;
}player;

player Player[20]; //defining as array to store for 2 diff players

//Struct to hold game board
typedef struct game_board
{
    unsigned char board[9][9];
    int row;
    int col;
    char symbol;
}Game_Board;

Game_Board board; //Declaring synonym as global variable so board can be referenced and altered in any function

//function prototypes
int initializeBoard();
void printBoard(Game_Board Board);
int assignPlayers(void);
int playerMove(unsigned char colour);
int count(char colour, Game_Board Board);
int validMove (int a, int b, unsigned char colour, int check);
int legalMove(int a, int b, unsigned char colour, int check);
int isSandwich(int a, int b, int c, int d, unsigned char colour, int check);
int anyMove(unsigned char colour);
void gameOver(void);
void outFile(int winCheck, int score0, int score1);

//global variables
int row = 0, col = 0;
int gameWon = 0; //signifies if the game has been won
char player1[20], player2[20]; //holds name of player1 and 2
bool bValidmove = true; //holds whether there exists valid move for black
bool wValidmove = true; //holds whether there exists valid move for white
char colours[2] = {'B', 'W'}; //Holds game pieces/player colours

int main() {
    int wincheck = 0;
    board.symbol = '.'; //symbol to show empty spaces on board
    board.row = 8;
    board.col = 8;

    initializeBoard(); //calling function to set up board

    assignPlayers(); //Assigning players to colours

    printBoard(board); //Print initial board to users

    outFile(wincheck, count(colours[0], board), count(colours[1], board)); //Print current date and time of game start to outfile

    while (gameWon != 1){ //While game not won

        if (count(colours[0], board) + count(colours[1], board) == 64){ //if player scores add up to 64 game is over
            gameOver(); //call to end game and display winners
        }

        printf("%s's (B) turn\n", Player[0].playerName);
        playerMove(Player[0].playerColour); //calling playerMove with player's colour

        printf("%s's (W) turn\n", Player[1].playerName);
        playerMove(Player[1].playerColour);


    }
}

//When this function is called players assigned to respective colours
int assignPlayers(void){


    printf("Enter player 1 name (Black): \n");
    scanf("%s", Player[0].playerName); //Storing name in player struct

    printf("Enter player 2 name (White): \n");
    scanf("%s", Player[1].playerName);


    Player[0].playerColour = colours[0];

    Player[1].playerColour = colours[1];

    printf("%s is black \n", Player[0].playerName);
    printf("%s is white \n", Player[1].playerName);

    strcpy(player2, Player[1].playerName);
    strcpy(player1, Player[0].playerName);


    printf("%s plays first as they control black.\n", Player[0].playerName);
    return 0;
}

//This function sets up the board before the game starts
int initializeBoard()
{

    for (row = 0; row < board.row; row++) //For loop for board rows
         {
        for (col = 0; col < board.col; col++)// For loop for board columns
        {
            board.board[row][col] = board.symbol; //Filling all empty spaces with board symbol '.'

            //Setting up initial othello square in middle of board
            board.board[3][3] = colours[1];
            board.board[3][4] = colours[0];
            board.board[4][3] = colours[0];
            board.board[4][4] = colours[1];
        }


    }

    return 0;
}

//This function allows users to enter a move and then executes it accordingly
int playerMove(unsigned char colour){

    bool moveExists = true; //Holds truth value for if a move exists

    char xL; //Holds letter values for x axis of board
    int y; //Holds numbers of y axis of board

    //Label for when a new move must be chosen
    newMove:

    if (anyMove(colour) == 0){ //Call to anyMove to see if there is a move available for players colour (returns 0 if no move)

        printf("\nYou have no valid moves! Enter p to pass\n");
        moveExists = false; //Set to false as there is no move to make

    }

    printf("\nEnter the coordinates of the move you would like to make (e.g a7)\n"
           "Enter 'p' to pass (can only pass if you have no valid moves):\n " );

    scanf(" %c", &xL); //Scanning in xL (column of move)

    if (xL == 'p') { //If the user enters p for pass

        if (moveExists) { //if there is a move they can make
            printf("\nThere is still a move you can make you cannot pass\n");
            goto newMove; //Choose a newmove
        }

        else{
            printf("Passing your turn to the opponent\n");
            return 0;
        }

    }

    else if (xL < 'a' || xL > 'h' || isdigit(xL)){ //If xL is not a letter between a and h or it is a number
        printf("\nInvalid move please enter a letter between a and h followed by a number between 1 and 8\n");
        getchar(); //Removes space from previous scanf
        goto newMove; //Choose a newmove
    }


    scanf("%d", &y); //Scan in y (row of move)

    if (y < 1 || y > 8 || isalpha(y)){ //if y is less than 1 or greater than 8 or if it is a letter choose new move
        printf("\nInvalid move please enter a letter between a and f followed by a number between 1 and 8\n");
        goto newMove;
    }

    //assigning y value to x and converting x value to integer and assigning to y
    //I did this because at first I had the code set up where the user entered their
    //move as row-column instead of column-row
    //1 is minused from y as the index notation goes from 0 to 7
    //This is accounted for when 'a' is taken from xL

    int x = y - 1; //Row position of move
    y = xL - 'a'; //Column position of move

    if (validMove(x ,y, colour, 0) == 1){ //Calling validMove to see if entered move is valid to games rules, returns 1 if invalid

        printf("\nThe move entered was either invalid or illegal please try again\n");
        goto newMove;

    }

    board.board[x][y] = colour; //Placing player's colour marker in move location they gave

    printBoard(board); //Print the board to display the move has been made

    return 0;
}

//This function is for printing the board to the user
void printBoard(Game_Board Board) {

    printf("\n|Score: %s (B) - %d | %s (W) - %d|\n",player1, count(colours[0], board), player2, count(colours[1], board)); //Print the score at the top

    printf("  a b c d e f g h \n"); //Top Label for columns of board

    for (row = 0; row < Board.row; row++) {//for loop for rows of board
        printf("%d ", row + 1); //Printing row number at start of row

        for (col = 0; col < Board.col; col++) {

            printf("%c ", Board.board[row][col]); //Printing out the symbol at each location of the board
        }
        printf("%d ", row + 1); //Printing the row number at the end of the row

        printf("\n");

    }
    printf("  a b c d e f g h \n"); //Bottom Label for columns of board
}

//Function for counting number of pieces of a colour currently on the board
//Returns count
int count(char colour, Game_Board Board){

    int count = 0;

    //Going through each position on the board
    for (row = 0; row < Board.row; row++) {
        for (col = 0; col < Board.col; col++) {

            if (Board.board[row][col] == colour) { //If a piece of the given colour is found increment the count
                count++;
            }

        }
    }

    return count;
}

//This function checks if a move entered is valid on the board
//Returns 1 if invalid, 0 otherwise
int validMove (int a, int b, unsigned char colour, int check){

    if ( (a >= 0) && (a <= 7) && (b >= 0) && (b <= 7)){ //If the move is between 0 and 7 on both axes

        if ((board.board[a][b] == colours[0] || board.board[a][b] == colours[1]) || legalMove(a, b, colour, check) == 1 ) { //If the move has a piece of either colour already on the board or if legalMove returns 1

            return 1; //Return 1 as move is not valid
        }

    }

    return 0;

}

//This function checks if the move entered is legal
//Returns 1 if not legal and 0 otherwise
//Takes in an integer called check that is 1 when being called by anyMove (ensures no changes are made to the board)
// Check is 0 when being called after a move is entered (for when board must be altered)
int legalMove(int a, int b, unsigned char colour, int check){

    bool oppAdj = false; //Truth value (Tv) for if there is an opponent piece adjacent to the current move
    bool isBet = false; //Tv for if the current move is between another piece of the same colour

    //Truth values for whether the move made is on the top, left, bottom or right perimeter of the board respectively
    //This must be taken into account as for move on the perimeter you ar not able to check all positions around current move depending on which perimeter it is on
    bool topPerim = false;
    bool leftPerim = false;
    bool botPerim = false;
    bool rightPerim = false;

    unsigned char playerColour;
    unsigned char oppColour;

    //These variables represent areas around the current move

    int upx = a-1; //upx refers to the row above the current move
    int botx = a+1; //botx refers to row below
    int leftY = b-1; // column to the left
    int rightY = b+1; //column to the right

    if ( a == 0 ){ //If a is 0 the move is on the top perimeter
        topPerim = true;
    }
    if ( b == 0 ){ //left perimeter
        leftPerim = true;
    }
    if ( a == 7 ){ //bottom perimeter
        botPerim = true;
    }
    if (b == 7){ //right perimeter
        rightPerim = true;
    }


    if (colour == colours[0]) { //If passed in colour = B
        playerColour = colours[0];
        oppColour = colours[1]; // = W
    }

    else{
        playerColour = colours[1];
        oppColour = colours[0];
    }

    if (board.board[upx][b] == oppColour && !topPerim){ //if piece above is of opposite colour and current move is not on top perimeter

        /*isSandwich takes in the current move (a, b) and the co-ordinate of an opponent piece and checks if there is a piece of the player's
         * colour anywhere in the same line*/

        if (isSandwich(a, b, upx, b, playerColour,check) == -1){ //If call to isSandwich returns -1
            isBet = false; //not between any piece above
        }

        else{
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[upx][leftY] == oppColour && !leftPerim && !topPerim){ //if piece on top left diagonal is of opposite colour and current move is not on left perimeter or top perimeter

        if (isSandwich(a, b, upx, leftY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of player's colour sandwiching opponents pieces to top left diagonal of current move
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[upx][rightY] == oppColour && !topPerim && !rightPerim){ //if piece on top right diagonal is of the opposite colour and current move is not on top perimeter or right perimeter

        if (isSandwich(a, b, upx, rightY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to the top right diagonal of the current move
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[botx][b] == oppColour && !botPerim){ //if piece on bottom is of the opposite colour and current move is not on bottom perimeter

        if (isSandwich(a, b, botx, b, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to bottom of current move
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[botx][leftY] == oppColour && !leftPerim && !botPerim){ //if piece on bottom left diagonal is of opposite colour and current move is not on left perimeter or bottom perimeter

        if (isSandwich(a, b, botx,leftY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to bottom left diagonal of current move
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[botx][rightY] == oppColour && !rightPerim && !botPerim){ //if piece on bottom right diagonal is of opposite colour and current move is not on right perimeter or bottom perimeter

        if (isSandwich(a, b, botx, rightY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to bottom right diagonal of current move
            oppAdj = true;
            isBet = true;
        }

    }


    if (board.board[a][leftY] == oppColour && !leftPerim){ //if piece on left is of opposite colour and current move is not on left perimeter

        if (isSandwich(a, b, a, leftY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to left of current move
            oppAdj = true;
            isBet = true;
        }

    }

    if (board.board[a][rightY] == oppColour && !rightPerim){ //if piece on right is of opposite colour and current move is not on right perimeter

        if ( isSandwich(a, b, a, rightY, playerColour,check) == -1){
            isBet = false;
        }

        else{ //there is another piece of the player's colour sandwiching the opponents pieces to right of current move
            oppAdj = true;
            isBet = true;
        }

    }

    if(!oppAdj && !isBet){ //if no opponent piece adjacent to current move and no player piece sandwiches opponent pieces

        return 1; //Signifies illegal move

    }


    return 0; //Signifies legal move

}

/*This function checks locations around the current move where there are adjacent opponent pieces for pieces of the player's colour that
 * sandwich the opponent pieces. If a suitable piece is found all opponent pieces are turned into pieces of the player's colour.
 * The function takes in the current move (a,b) and the location of an adjacent opponent piece (c,d).
 * Using these two points you can calculate which direction to check for a sandwiching player piece by getting the difference between the
 * two points.
 * The players colour is also passed in to differentiate player and opp colours.
 * a check number is passed in, this is for differentiating between calls to the function where the board is
 * changed and calls where the board is not altered.*/

int isSandwich(int a, int b, int c, int d, unsigned char colour, int check){

    bool isSandwich = false; //Truth value for whether there is a player piece sandwiching opponent piece

    unsigned char playerColour; //Holds players colour

    //Using the difference between the two points you can find which direction to search in
    int e = a - c; // e holds the difference of a and c
    int f = b - d; // f holds the difference of a and c

    //assigning correct player colour to playerColour
    if (colour == colours[0]) {
        playerColour = colours[0];
    }
    else{
        playerColour = colours[1];
    }

    //An e of 1 and f of 0 means you should search in a straight line above the current move
    if ( e == 1 && f == 0){

        flip: //label that is called when opponent pieces need to be flipped to player colour

        for (int i = c; i >= 0; i--){ //for loop starting at row of opponent piece and moves up the board with each iteration

            if (isSandwich && check == 0){ //this if statement only runs if isSandwich is true and check is 0, this indicates changes should be made to the board

                if (board.board[i][d] == playerColour){ //If the position [i][d] on the board is a piece of the players colour (moving up on rows but column stays the same)

                    return 1; //Signifies that a players piece has been found so stop searching
                }

                board.board[i][d] = playerColour; //Turning opponent pieces to player pieces
            }

            if(isSandwich && check == 1){ //if isSandwich is true and check == 1 this means the function was called by anyMove to observe if there is a valid move to be
                                          //made without making changes to the board

                if (board.board[i][d] == playerColour){

                    return 1; //Stop searching
                }

            }

            if (board.board[i][d] == board.symbol){ //If an empty space is found

                return -1; //returning -1 means the current moves is not part of a sandwich
            }


            if (board.board[i][d] == playerColour && !isSandwich){ //If a player piece is found and isSandwich is false

                //isSandwich is set to true as there is a player piece sandwiching the opponent pieces with the current move
                isSandwich = true;

                goto flip; /* goto line 460 as now isSandwich is true so opponent pieces can be flipped to players colour */

            }

        }
        return -1; //No sandwich is found

    }

    //An e of -1 and f of 0 means you should search in a straight line below the current move
    else if (e == -1 && f == 0){

        flip2:

        for (int i = c; i <= 7; i++){//for loop starting at row of opponent piece and moves down the board with each iteration

            if (isSandwich && check == 0){

                if (board.board[i][d] == playerColour){

                    return 1; //sandwich found
                }

                board.board[i][d] = playerColour; //flip to playerColour
            }

            if(isSandwich && check == 1){

                if (board.board[i][d] == playerColour){

                    return 1; //sandwich found
            }

            if (board.board[i][d] == board.symbol){

            }
                return -1; //No sandwich is found

            }


            if (board.board[i][d] == playerColour && !isSandwich){

                isSandwich = true;

                goto flip2; //goto line 507 so pieces can be flipped to player's colours

            }
        }

        return -1; //No sandwich is found

    }

    //An e of 0 and f of 1 means you should search in a straight line left of the current move
    else if (e == 0 && f == 1){

        flip3:

        for (int j = d; j >= 0; j--){ //for loop starting at column of opponent piece and moves left of the board with each iteration

            if (isSandwich && check == 0){
                if (board.board[c][j] == playerColour){

                    return 1; //sandwich found
                }

                board.board[c][j] = playerColour;
            }

            if(isSandwich && check == 1){
                if (board.board[c][j] == playerColour){

                    return 1; //sandwich found
                }
            }

            if (board.board[c][j] == board.symbol){

                return -1; //No sandwich is found
            }

            if (board.board[c][j] == playerColour && !isSandwich){

                isSandwich = true;

                goto flip3; //goto line 552
            }
        }
        return -1; //No sandwich is found

    }

    //An e of 0 and f of -1 means you should search in a straight line right of the current move
    else if (e == 0 && f == -1){

        flip4:

        for (int i = d; i <= 7; i++){ //for loop starting at column of opponent piece and moves right of the board with each iteration

            if (isSandwich && check == 0){

                if (board.board[c][i] == playerColour){

                    return 1; //sandwich found
                }
                board.board[c][i] = playerColour;
            }

            if(isSandwich && check == 1){
                if (board.board[c][i] == playerColour){

                    return 1; //sandwich found
                }
            }

            if (board.board[c][i] == board.symbol){

                return -1; //No sandwich is found
            }


            if (board.board[c][i] == playerColour && !isSandwich){

                isSandwich = true;
                goto flip4; //goto line 591
            }
        }

        return -1; //No sandwich is found
    }

    //An e of 1 and f of 1 means you should search in a straight line to the top left diagonal of the current move
    else if (e == 1 && f == 1){

        //When searching diagonally both row and column positions change as you move so both must be iterated
        int i, j;

        flip5:
        i = c;
        j = d;

        while (i >= 0 && j >= 0){ //As your moving on top left diagonal you get closer to 0 each time

            if (isSandwich && check == 0){
                if (board.board[i][j] == playerColour){

                    return 1; //sandwich found
                }

                board.board[i][j] = playerColour;
            }

            if(isSandwich && check == 1){

                if (board.board[i][j] == playerColour){

                    return 1; //sandwich found
                }
            }

            //Decrementing i and j to move to next position on diagonal
            --i;
            --j;

            if (board.board[i][j] == board.symbol){

                return -1; //No sandwich is found
            }

            if (board.board[i][j] == playerColour && !isSandwich){

                isSandwich = true;
                goto flip5; //goto line 633
            }
        }

        return -1; //No sandwich is found
    }

    //An e of 1 and f of -1 means you should search in a straight line to the top right diagonal of the current move
    else if (e == 1 && f == -1){

        int i, j;

        flip6:
        i = c;
        j = d;
        while (i >= 0 && j < 8){ //As your moving on top right diagonal you get closer to 0 each time on rows and to 7 on columns

            if (isSandwich && check == 0){
                if (board.board[i][j] == playerColour){

                    return 1;
                }

                board.board[i][j] = playerColour;
            }

            if(isSandwich && check == 1){
                if (board.board[i][j] == playerColour){

                    return 1;
                }
            }

            //Decrementing i and incrementing j allows you to move along the diagonal to the top right
            --i;
            ++j;

            if (board.board[i][j] == board.symbol){

                return -1;
            }


            if (board.board[i][j] == playerColour && !isSandwich){

                isSandwich = true;
                goto flip6; //goto line 680
            }
        }

        return -1;

    }

    //An e of -1 and f of 1 means you should search in a straight line to the bottom left diagonal of the current move
    else if (e == -1 && f == 1){

        int i, j;

        flip7:
        i = c;
        j = d;

        while (i < 8 && j > 0){ //As your moving on bottom left diagonal you get closer to 7 each time on rows and to 0 on columns

            if (isSandwich && check == 0){
                if (board.board[i][j] == playerColour){

                    return 1;
                }

                board.board[i][j] = playerColour;
            }

            if(isSandwich && check == 1){
                if (board.board[i][j] == playerColour){

                    return 1;
                }
            }

            //Incrementing i and decrementing j allows you to move along the diagonal to the bottom left
            ++i;
            --j;

            if (board.board[i][j] == board.symbol){

                return -1;
            }

            if (board.board[i][j] == playerColour && !isSandwich){

                isSandwich = true;
                goto flip7; //goto line 727
            }
        }

        return -1;

    }

    //An e of -1 and f of -1 means you should search in a straight line to the bottom right diagonal of the current move
    else if (e == -1 && f == -1){

        int i, j;

        flip8:
        i = c;
        j = d;

        while (i < 8 && j < 8){ //As your moving on bottom right diagonal you get closer to 7 each time

            if (isSandwich && check == 0){
                if (board.board[i][j] == playerColour){

                    return 1;
                }

                board.board[i][j] = playerColour;
            }

            if(isSandwich && check == 1){

                if (board.board[i][j] == playerColour){

                    return 1;
                }
            }

            //Incrementing i and j allows you to move along the diagonal to the bottom right
            ++i;
            ++j;

            if (board.board[i][j] == board.symbol){

                return -1;
            }


            if (board.board[i][j] == playerColour && !isSandwich){

                isSandwich = true;
                goto flip8;
            }
        }

        return -1;

    }

    return 0;

}

/* This function takes in a colour on the board and checks the every board position for valid moves for that colour
 * returns 1 if there is a valid move on the board
 * returns 0 if there is not */

int anyMove(unsigned char colour){

    //x represents column position
    int x = 0;

    //This while and for loop combination allows every position on the board to be checked by checking every row for
    //a particular column before it is iterated

    while (x <= 7){

        for (int i = 0; i <= 7; ++i) { //Every row position is checked with the current column

            if(validMove(i ,x, colour, 1) == 0){ //If validMove returns 0 there is a valid move on the board for the colour specified

                if(colour == colours[0]){ //If colour is B set global variable bValidmove to true

                    bValidmove = true;
                }

                else if(colour == colours[1]) { //If colour is W set global variable bwValidmove to true
                    wValidmove = true;
                }

                return 1; //Signifies that there is a valid move
            }

        }

        x++; //increment to next column

    }

    //if no valid move is found set appropriate variable to false

    if(colour == colours[0]){
        bValidmove = false;
    }

    else if (colour == colours[1]){
        wValidmove = false;
    }

    if (!bValidmove && !wValidmove) { //If both boolean variables are false the game should end as there are no moves left

        gameWon = 1; //causes while loop in main to stop
        gameOver(); //call to gameOver to display results of game
    }

    return 0; //Signifies there is no valid move on the board for inputted colour
}

/* This function is called when the game has ended so the results of the game
 * can be displayed such as the winner and the final score*/

void gameOver(){

    //This variable allows for differentiation of who wins the game
    //wincheck of 1 means Black won, 2 means white won , 3 means it was a draw
    int wincheck;

    if (count(colours[0], board) > count(colours[1], board)){ //if there is more black pieces(colours[0]) than white (colours[1]) then black has won the game

        printf("\n%s wins the game! Final Score: %s (B) - %d |  %s (W) - %d \n", player1, player1, count(colours[0], board), player2, count(colours[1], board)); //Print game results
        wincheck = 1;
    }

    else if (count(colours[1], board) > count(colours[0], board)){ //if more white pieces than black

        printf("\n%s wins the game! Final Score: %s (W) - %d |  %s (B) - %d \n", player2, player2, count(colours[1], board), player1, count(colours[0], board));
        wincheck = 2;
    }

    else{ //else it is a draw

        printf("\nIts a draw! Final Score: %s (B) - %d |  %s (W) - %d \n", player1, count(colours[0], board), player2, count(colours[1], board));
        wincheck = 3;
    }

    printBoard(board); //print the final board

    outFile(wincheck, count(colours[0], board), count(colours[1], board)); //Call to outFile to write results of game to text file

    exit(0); //end program

}

/* This function writes the date and time of when the game started and the final result of the game
 * to a text file called othello-results.txt*/
void outFile(int winCheck, int score0, int score1){

    FILE *cfPtr; // othello-results.txt file pointer

    if ((cfPtr = fopen("othello-results.txt", "ab+")) == NULL){// fopen opens file; exits if can't be opened
        //ab+ opens a binary file in append mode
        printf("Could not open file.\n");

    }

    else{

        if (winCheck == 0) {
            char dateTime[30];
            struct tm tim;
            time_t now;
            now = time(NULL);
            tim = *(localtime(&now));
            strftime(dateTime, 30, "%b %d, %Y; %H:%M\n", &tim);
            fprintf(cfPtr, "%s", dateTime);
        }

        if (winCheck == 1){
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player1, score0, player2, score1);
            fprintf(cfPtr, "%s won the game.\n\n", player1);
        }

        else if(winCheck == 2){
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player2, score1, player1, score0);
            fprintf(cfPtr, "%s won the game.\n\n", player2);
        }

        else if (winCheck == 3){
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player1, score0, player2, score1);
            fprintf(cfPtr, "The game ended as a draw.\n\n");
        }

        fclose(cfPtr);

    }
}