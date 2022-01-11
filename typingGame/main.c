#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>

void drawTopper();
void drawPlane();
void dropBomb(int bombCordinateY, int bombCordinateX);
void drawCity();
void selectCodewords();
void gamePlay();


int planeCordinate = 1; // variable for plane's horizontal coordinates
int a=0, b=3; // a is for deciding whether the plane will move forwards or backwards. I describe the b down below where I used it.

int bombCordinateY = 1; // variable for bomb's vertical coordinates
int bombCordinateX; // variable for bomb's horizontal coordinates
int *bombCordPtr = &bombCordinateY;

int bombCount = 0; // to know if there is a bomb on the screen (There needs to be max 1 bomb on the screen)

int level = 1; // level to change game's difficulty
int gameSpeed = 500; // game speed in microseconds

int digitNum; // deciding how many digit codeword will have

// Variables for storing the codewords
char a_word[64];
int *wordsPtr;
//

int wordKnown = 0; // This is to count how many words known.

char temp[12]; // this is for replacing the codeword. So I wont have to damage the original

// Variables for score
int plusPoints = 0;
int negativePoints = 0;
int score = 0;
int placeTracker = 0;
//

int gameDecision = 0; // Variable for making menu decisions

// Save struct
struct SaveDatas{
        int planeCordinate;
        int a;
        int bombCordinateY;
        int bombCount;
        int level;
        int plusPoints;
        int negativePoints;
        int score;
        int placeTracker;
        int gameSpeed;
        int wordKnown;
        int digitNum;
        int bombCordinateX;
    };
//

int main()
{
    // STORING THE CODEWORDS IN AN ARRAY
    FILE *codewordsPtr;
    if((codewordsPtr = fopen("codewords.txt","rb"))==NULL){
        printf("file could not be openedaaaa");
    }
    wordsPtr = (char*)calloc(39, sizeof(char[12]));
    for(int i=0; i<39; i++){
        fscanf(codewordsPtr, "%63s", a_word);
        wordsPtr[i] = strdup(a_word);
    }
    fclose(codewordsPtr);
    //


    while(1){

    while(gameDecision==0){ // Since this is the opening window I did not include Save and Resume.
        printf("    Welcome to Typing of The BOMBS!\n");
        printf("    1- New Game\n    2- Load Game\n    3- Exit\n    ");
        scanf("%d", &gameDecision);
    }

    while(gameDecision==6){ // when ESC is pressed this menu will show up
        printf("\n\n");
        printf("    1- New Game\n    2- Load Game\n    3- Exit\n    4- Return to Game\n    5- Save Current Game    ");
        scanf("%d", &gameDecision);
    }



    if(gameDecision==1){ // When 1 is selected. It will be a new game so variables are reseted here.
        planeCordinate = 1;
        a=0, b=3;
        *bombCordPtr = 1;
        bombCount = 0;
        level = 1;
        plusPoints = 0;
        negativePoints = 0;
        score = 0;
        placeTracker = 0;
        gameSpeed = 500;
        wordKnown = 0;
    }

    if(gameDecision==5){ // Hocam bir tane save slotu ekledim. Kullanicinin save dosyasi ismini girdigi sekilde yapmadim. tek save slotu hakki ve oradan load var.
        FILE *savePtr;
        struct SaveDatas mysave = {planeCordinate, a, bombCordinateY, bombCount, level, plusPoints, negativePoints, score, placeTracker, gameSpeed, wordKnown, digitNum, bombCordinateX};
        if((savePtr = fopen("mysave.txt","wb"))==NULL){printf("file could not be opened");}
        fwrite(&mysave, sizeof(struct SaveDatas), 1, savePtr);

        fclose(savePtr);
        printf("\nGame successfully saved");
        gameDecision = 6;
    }

    if(gameDecision==2){
        FILE *loadPtr;

        if((loadPtr = fopen("mysave.txt","rb"))==NULL){printf("Save file cannot found");}
        else{
        struct SaveDatas blank = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        fread(&blank, sizeof(struct SaveDatas), 1, loadPtr);
        planeCordinate = blank.planeCordinate;
        a = blank.a;
        bombCordinateY = blank.bombCordinateY;
        bombCount = blank.bombCount;
        level = blank.level;
        plusPoints = blank.plusPoints;
        negativePoints = blank.negativePoints;
        score = blank.score;
        placeTracker = blank.placeTracker;
        gameSpeed = blank.gameSpeed;
        wordKnown = blank.wordKnown;
        digitNum = blank.digitNum;
        bombCordinateX = blank.bombCordinateX;



        fclose(loadPtr);

        printf("%10s","Loading the game...");
        sleep(3);

        gameDecision = 4;
        }
    }

    if(gameDecision == 3){ // EXIT
        printf("    !!!THANKS FOR PLAYING!!!");
        exit(0);
    }



    while(gameDecision == 1 || gameDecision == 4){// MAIN LOOP

        // TOPPER
        drawTopper(); // Draws score and level
        //

        // MOVING PLANE PART
        drawPlane();
        //

        // BOMB PART
        if(bombCount==0){
            bombCordinateX = planeCordinate;
            bombCount = 1;
            //selecting codewords
            selectCodewords();
            //
        }


        dropBomb(bombCordinateY, bombCordinateX);
        //

        // GAMEPLAY
        gamePlay();
        //

        // UPDATING THE SCORE
        updateScore();
        //

        // CITY PART
        drawCity();
        //





        Sleep(gameSpeed);

        system("cls");// to clear the screen in order to update the drawing.
    }

    }

    return 0;
}

void drawTopper(){
    printf("\n%50s\n","TYPING OF THE BOMBS || PROTECT THE CITY!");
    printf("\n%32s","SCORE: ");
    printf("%d\n\n",score);
    printf("LEVEL = %d\n\n", level);
}


void drawPlane(){
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    FILE *planePtr;
    if((planePtr = fopen("plane.txt", "r")) == NULL){
        printf("file could not be opened");
    }

        // MOVING PLANE PART
        while(fgets(buffer, MAX_LENGTH, planePtr)){ //printing the plane
            if(a==0){
                for(int i=0; i<=planeCordinate; i++){
                    printf(" ");
                }
                b++;
                if(b%3==0){planeCordinate++; b=3;}// so reason why I included b%3 is that plane is made of 3 lines. I dont want plane cordinate to change every line so thats why i used b%3==0
                if(planeCordinate==50){
                    a=1;
                }

        } else if(a==1){
                for(int i=0; i<=planeCordinate; i++){
                    printf(" ");
                }
                b++;
                if(b%3==0){planeCordinate--; b=3;}
                if(planeCordinate==0){
                    a=0;
                }
        }
            printf("%s", buffer);

        }
        rewind(planePtr);// if we don't rewind the cursor will stay at the end of the text file and dont print anymore.
        fclose(planePtr);
        //
}


void dropBomb(int bombCordinateY, int bombCordinateX){

    FILE *bombPtr;
    if((bombPtr = fopen("bomb.txt","r"))==NULL){printf("file could not be opened");}
    //if((codewordsPtr = fopen("codewords.txt","r"))==NULL){printf("file could not be opened");}

    //Variables for printing the bomb
    const unsigned MAX_LENGTH = 64;
    char buffer[MAX_LENGTH];
    //
    for(int i=0; i<=bombCordinateY; i++){// bomb's length is 3 lines. So I will assign 23 lines between plane and city. Therefore, after 20 lines bomb will drop to the city.
            printf("\n");
        }

        //Printing the bomb
        for(int j=0; j<3; j++){
            fscanf(bombPtr, "%63s", buffer);
            // Printing the left space for the bomb
            if(j == 0){
                for(int i=0; i<=bombCordinateX-1; i++){
                    printf(" ");
                }
            } else{
                for(int i=0; i<=bombCordinateX; i++){
                    printf(" ");
                }
            }
            //

            printf("%s", buffer);// prints the bomb

            // CODEWORDS PART
            if(j == 1){
                switch(placeTracker){
                    case 0:
                        printf("%s", wordsPtr[digitNum]);
                        break;
                    case 1:
                        printf("%.*s",63 ,wordsPtr[digitNum]+1);
                        break;
                    case 2:
                        printf("%.*s",63 ,wordsPtr[digitNum]+2);
                        break;
                    case 3:
                        printf("%.*s",63 ,wordsPtr[digitNum]+3);
                        break;
                    case 4:
                        printf("%.*s",63 ,wordsPtr[digitNum]+4);
                        break;
                    case 5:
                        printf("%.*s",63 ,wordsPtr[digitNum]+5);
                        break;
                    case 6:
                        printf("%.*s",63 ,wordsPtr[digitNum]+6);
                        break;
                    case 7:
                        printf("%.*s",63 ,wordsPtr[digitNum]+7);
                        break;
                    case 8:
                        printf("%.*s",63 ,wordsPtr[digitNum]+8);
                        break;
                    case 9:
                        printf("%.*s",63 ,wordsPtr[digitNum]+9);
                        break;

                }
            }
            printf("\n");
            //
        }
        //

        *bombCordPtr+=1;

        // SPACE BETWEEN BOMB AND THE CITY
        for(int i=0; i<=42-bombCordinateY; i++){
            printf("\n");
        }
        //
        rewind(bombPtr);
        fclose(bombPtr);
}

void drawCity(){
    FILE *cityPtr;
    if((cityPtr = fopen("city.txt", "r")) == NULL){
        printf("file could not be opened");
        return 1;
    }
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    while(fgets(buffer, MAX_LENGTH, cityPtr)){ //printing the city.
        printf("%s", buffer);
        }

    if(bombCordinateY==42){ // So this was not originally here but since drawCity is the last function for the main, I put this here. Otherwise, it prints the city after gameover.
            system("cls");
            printf("\n    GAME OVER\n    Your Score Was: %d",score);
            sleep(3);
            gameDecision = 0;
        }
        rewind(cityPtr);
        fclose(cityPtr);
}


void selectCodewords(){
    srand(time(NULL));
    // Selecting codewords
    if(level == 1){digitNum = rand()%4;}
    if(level == 2){digitNum = 4 + rand()%5;}
    if(level == 3){digitNum = 9 + rand()%5;}
    if(level == 4){digitNum = 14 + rand()%4;}
    if(level == 5){digitNum = 18 + rand()%6;}
    if(level == 6){digitNum = 24 + rand()%6;}
    if(level == 7){digitNum = 30 + rand()%4;}
    if(level >= 8){digitNum = 35 + rand()%4;}
    //
}

void gamePlay(){
    strcpy(temp, wordsPtr[digitNum]); // Since wordsPtr is a pointer I used temp for comparison.
    memmove(temp, temp + placeTracker, strlen(temp));// This is for deleting the first char if it is typed correctly
    while (_kbhit()) {
        char c = _getch();
        if(c == temp[0]){ // Since first chars are being when typed correct, the comparison always will be between c and the first char(temp[0])
            plusPoints++; // Tracks the number of times correct button hit
            placeTracker++; // To decide which letter are we at
        } else if(c == 27){
            gameDecision = 6;
        }
        else{
            negativePoints++; // Tracks the number of times wrong button hit.
        }
    }
}

void updateScore(){
    if(placeTracker == strlen(wordsPtr[digitNum])){// When place tracker reaches the word's length, it means that user typed all of it correctly.
        score += plusPoints - negativePoints;
        // These values need to be reseted because for every bomb the process will start again
        plusPoints = 0;
        negativePoints = 0;
        bombCount = 0;
        *bombCordPtr = 1;
        placeTracker = 0;
        //
        wordKnown += 1; // To track words known to level up every 5 correct answer
        if(wordKnown%5 == 0 && wordKnown!= 0){
        level+=1;
        if(gameSpeed>0){gameSpeed -= 100;} // So I stopped decreasing the gamespeed because it is hard enough and one cannot keep going for so long.(I guess :D)
    }
    }



}

