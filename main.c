#include "library.h"


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

        //ab+ opens a binary file in append mode, results will be continually added to same file
        printf("Could not open file.\n");

    }

    else{

        if (winCheck == 0) { //Function is called at start of program with winCheck 0 so date and time of start of game is written to file

            //Writing date and time at start of game to file using time.h library functions
            char dateTime[30]; //Holds date and time
            struct tm Time;
            time_t currentTime;
            currentTime = time(NULL);
            Time = *(localtime(&currentTime));
            strftime(dateTime, 30, "%b %d, %Y; %H:%M\n", &Time); //strftime formats time as string
            fprintf(cfPtr, "%s", dateTime); //Printing date & time to file
        }

        if (winCheck == 1){ //winCheck of 1 means black has won

            //Writing game result to file
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player1, score0, player2, score1);
            fprintf(cfPtr, "%s won the game.\n\n", player1);
        }

        else if(winCheck == 2){ //winCheck of 2 means white has won

            //Writing game result to file
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player2, score1, player1, score0);
            fprintf(cfPtr, "%s won the game.\n\n", player2);
        }

        else if (winCheck == 3){ //winCheck of 3 means draw

            //Writing game result to file
            fprintf(cfPtr,  "|Final score: %s - %d : %s - %d|\n", player1, score0, player2, score1);
            fprintf(cfPtr, "The game ended as a draw.\n\n");
        }

        fclose(cfPtr); //Closing file

    }
}