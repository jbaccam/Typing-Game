//SE 185: Final Project Template//
/////////////////////////
/*
Team member 1 "Jeremiah Baccam" | "50%"
Team member 2 "Jared Cheney" | "50%"
*/

/*-----------------------------------------------------------------------------
-								Implementation
-----------------------------------------------------------------------------*/
/*	Main - Run with ./test wordList.txt
 to compile gcc finalproject.c -o test -lncurses

*/

////////////////////////////////////////
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses/ncurses.h>
#include <time.h>
////////////////////////////////////////

////////////////////////////////////////
#define FILENAME "wordlist.txt"
#define MAX_WORDS 1500
#define WORD_LENGTH 50
#define BOARD_ROWS 25
#define BOARD_COLS 40
#define EMPTY_SPACE ' '
////////////////////////////////////////


char BOARD[BOARD_ROWS][BOARD_COLS];

// Function prototypes
int wordCount(char *wordList[MAX_WORDS], const char *file);
char (*(initializeBoard)())[BOARD_COLS];
void drawBoard();
void drawCharacter(int x, int y, char use);
int addWordToBoard(char *wordList[MAX_WORDS],int wordamount);
void moveWordsDown();
void deleteWordFromBoard(char *word);
int checkGameEnd();
void displayGameOver(int timeElapsed);
void addword(char *wordList[MAX_WORDS], const char *file);
void trim(char *wordList[],int i);

int main() {
    char *wordList[MAX_WORDS];
    int timeElapsed = 0;
    time_t startTime, endTime;
	char (*Board)[BOARD_COLS];
	int i,j;

    printf("Hello, this is a typing game. You will need to type the words as they appear\n"); //intros game
    printf("Would you like to play or add a word?\n");
    printf("You currently have %d words that can be used.\n", wordCount(wordList, FILENAME));
    printf("Would you like to play or add a word? Type 'p' for play and 'w' to add words\n");

    char input; //stores user input
    scanf(" %c", &input);
	
	while(input == 'w'){ //while input is w meaning user is adding a word
		addword(wordList, FILENAME);
		printf("You currently have %d words that can be used.\n", wordCount(wordList, FILENAME));
		printf("Would you like to play or add a word? Type 'p' for play and 'w' to add words\n");
		scanf(" %c", &input);
	}
	
	int countOfWords = wordCount(wordList, FILENAME);
	
	
    if (input == 'p') {
		
		for(int i=0;i<countOfWords;++i){ //trims the word list to not have extra spaces
			trim(wordList,i);
		}
		
        initscr(); 
		raw();     
		//keypad(stdscr, TRUE);  //enable special keys like enter and stuff
		
		
	
        printw("Press Enter when you are ready to start...\n");
        refresh(); 
        getch();   //wait for the user to press Enter

        startTime = time(NULL);
		Board = initializeBoard(); //makes board in an array
		char newBoard[BOARD_ROWS+1][BOARD_COLS]; //allows for a temp board
		
		for(i=0;i<BOARD_ROWS;i++){ //initializes temp board
				for(j=0;j<BOARD_COLS;j++){
					newBoard[i][j]=Board[i][j];
				}
			}
		
		char userInput[WORD_LENGTH];
		int count = 0; //keeps count of number of words added (right now its every loop)
		int wordArray[1000]; //holds number assigned to words that can from wordList[]
		int rowArray[1000]; //holds row that word is on
		int colArray[1000]; //holds col that word is on
		int wordnum = 0;
		char word[50]; //holds word from wordList
		int col,len;
		int lostgame = 0;//keeps track if lost to word reaching end
		int t;
		int loopnum = 1; //makes sure it will loop 1 time to start
		int worddoop = 0;
		
		srand(time(NULL)); //sets random seed to allow for random game
		

        //game loop
        // game loop
while (strcmp(userInput, "end") != 0) { // will loop until end is typed
    clear();

    mvprintw(30, 0, "Type words here: "); // makes typing word line at row 30
    refresh();

    while (loopnum != 0) { // will loop for its number dependent on time right now set for 1 line 1s
        if (strcmp(userInput, "end") == 0) { // if end is typed program will end
            break;
        }

        loopnum--; // lowers loopnumm

        wordnum = rand() % countOfWords; // chooses a random word from the list

        while (count > 0) {
            for (i = 0; i < count; ++i) {
                if (wordnum == wordArray[i]) {
                    wordnum = rand() % countOfWords;
                }
            }
            for (i = 0; i < count; ++i) {
                if (wordnum != wordArray[i]) {
                    worddoop = 1;
                } else {
                    worddoop = 0;
                    break;
                }
            }
            if (worddoop == 1) {
                worddoop = 0;
                break;
            }
        }

        strcpy(word, wordList[wordnum]); // assigns word from wordList
        len = strlen(wordList[wordnum]); // gets word length
        col = rand() % (BOARD_COLS - len - 2) + 1; // assigns a random column to be placed in at the top
        wordArray[count] = wordnum; // array to hold wordnums
        colArray[count] = col; // array to hold col numbers
        rowArray[count] = 1; // array to hold row numbers

        for (int i = 0; i < len; i++) { // adds the word to the board and outputs col that the word is in then stored in an array
            drawCharacter(col + 1 + i, 1 + 1, word[i]);
            Board[1][col + i] = word[i];
        }

        count++; // tells how many words were added

        for (int i = 0; i < count; ++i) { // for all added words
			if (rowArray[i] != -10000) { // check if the word is still on the board
				if (strcmp(userInput, wordList[wordArray[i]]) == 0) { // if user input matches a word on the board
					for (int j = 1; j < BOARD_COLS - 1; ++j) {
						Board[rowArray[i]][j] = ' '; // update Board to be empty at the word's row and columns
						}
						rowArray[i] = -10000; // update row to be -10000, meaning the row is gone
					}
				}
			}


        for (int i = 0; i < BOARD_ROWS; ++i) { // Draws Board array
            for (int j = 0; j < BOARD_COLS; ++j) {
                drawCharacter(j + 1, i + 1, Board[i][j]);
            }
        }

       for (int i = BOARD_ROWS - 3; i >= 1; i--) { // moves everything down not including the first row and last row
			for (int j = 1; j < BOARD_COLS - 1; j++) {
				newBoard[i + 1][j] = Board[i][j];
			}
		}

		for (int i = 0; i < count; i++) { // updates rows for words moving down
			if (rowArray[i] != -10000) {
				rowArray[i] = rowArray[i] + 1;
			}
		}

		int isLetter(char c) {
			return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
		}

		for (int j = 0; j < BOARD_COLS; j++) {
			if (isLetter(Board[BOARD_ROWS - 2][j])) {
				lostgame = 1; // game over
				break;
			}
		}

		if (lostgame == 1) { // checks if lostgame
			break;
		}

		for (int i = 0; i < BOARD_ROWS; ++i) { // assigns newBoard as a temp array holding values
			for (int j = 0; j < BOARD_COLS; ++j) {
				drawCharacter(j + 1, i + 1, newBoard[i][j]);
			}
		}

		for (int i = 0; i < BOARD_ROWS; i++) { // updates Board from newBoard
			for (int j = 0; j < BOARD_COLS; j++) {
				Board[i][j] = newBoard[i][j];
			}
		}


        timeElapsed = time(NULL) - startTime; // makes time difference

        refresh();
    }

    if (lostgame == 1) { // checks if lostgame
        mvprintw(30, 0, "A word reached the bottom! Game Over."); // prints gameover
        mvprintw(35, 0, "You lasted %d seconds         ", timeElapsed); // shows time elapsed
        refresh();
        mvscanw(33, 22, "%s", userInput); // waits for the user to read
        break; // exits loop
    }

    t = time(NULL); // makes current time

    mvscanw(30, 17, "%s", userInput); // scans input after row 30 column 17

    endTime = time(NULL); // checks time after scan

    loopnum = (endTime - t); // will give the amount of times to loop based on time passed

    if ((endTime - t) < 1) { // if different less than 1 will allow looping once
        loopnum = 1;
    }
}


    }

    endwin();

    for (int i = 0; i < countOfWords; i++) {
        free(wordList[i]);
    }

    return 0;
}


char (*(initializeBoard)())[BOARD_COLS]{ 
	static char Board[BOARD_ROWS][BOARD_COLS];
	
	clear(); //clears board
	
	for(int i=0;i<BOARD_ROWS;++i){
		for(int j=0;j<BOARD_COLS;++j){
			if (i==0 || i==BOARD_ROWS-1){ //assigns top row and bottom row with -
				drawCharacter(j+1,i+1,'-');
				Board[i][j] = '-';
			}
			else if (j==0 || j==BOARD_COLS-1) { //assigns walls
				drawCharacter(j+1,i+1,'|');
				Board[i][j] = '|';
			}
			else{
				drawCharacter(j+1,i+1,' '); //assigns empty spaces for everything else
				Board[i][j] = ' ';
			}
		}
	}
	
	return Board; //returns array
				
				
}

void drawCharacter(int x, int y, char use)
{
    mvaddch(y,x,use);
    refresh();
}

int addWordToBoard(char *wordList[MAX_WORDS],int num){ //was a temp thing not in use now
	
	char word[20];
	strcpy(word,wordList[num]);
	
	int len = strlen(wordList[num]);
	
	int col = rand()%(BOARD_COLS-len)+1;
	
	for(int i = 0; i<len; i++){
		drawCharacter(col+1+i,1+1,word[i]);
	}
	return col;
}


void trim(char *wordList[],int i){
	char *end;
	
	end = wordList[i] + strlen(wordList[i]) - 1;
	while (end > wordList[i] && isspace((unsigned char)*end)) --end; //will remove spaces from wordList string
	
	end[1] = '\0';
}
	

int wordCount(char *wordList[MAX_WORDS], const char *file)
{
    int num = 0;
    char line[WORD_LENGTH];
    char *ptr;
    FILE* fp = fopen(file, "r");

    if (!fp) {
        exit(0); // file opening failed
    }
	
	while (!feof(fp)) { //goes untill end of file
		ptr = fgets(line, WORD_LENGTH, fp); //ptr looks at line
		if (ptr != NULL) //goes until end of line
		{
			wordList[num] = (char *)malloc(strlen(line) + 1);//makes memory for wordList
			strcpy(wordList[num], line);//assigns wordList with line string
			num++; //adds for each line ie word amount
		}
	}

    fclose(fp);
    return num; //returns word amount
}

void addword(char *wordList[MAX_WORDS], const char *file){
	
	int num = 0;
    char line[WORD_LENGTH];
    char *ptr;
    FILE* fp = fopen(file, "a");
	char word[30];
	
	if (!fp) {
        exit(0); // file opening failed
    }
	
	printf("Please type the word you want to add here: ");
	scanf("%s", word); //users word to add
	
	fseek(fp,0,SEEK_END);//goes to end
	
	long pos = ftell(fp); //sets position
	if (pos > 0){
		fseek(fp, pos-1, SEEK_SET); //Checks if last character is a newline
		if (fgetc(fp)!='\n'){
			fprintf(fp, "\n"); //adds new line if needed
		}
	}

	fprintf(fp,"%s",word); //adds user word

    fclose(fp);
    return;
}