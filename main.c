/* ---------------------------------------------
Program 3: Best Wordle Game
Course: CS 211, Fall 2022
System: Mac using Replit
Author 1: Muhammad Muzzammil
---------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>
// For all 2d arrays we will use it like: Array[MAX_LINES][MAX_LEN] 
#define MAX_LINES 50000
#define MAX_LEN 10


struct wordle
{
   int answerRowCount; // # of words in answer file
   int guessRowCount; // # of words in guess file
   int totalWordCount; // # of words when answer & guess files combined
   int topWordsCount; // total # of first best words (useful when there is a tie)
   int secondWordsCount; // total # of second best words (useful when there is a tie)
   int firstBestScore[MAX_LINES]; // Just to hold the highest # found in first best words
   int secondBestScore[MAX_LINES]; // Just to hold the highest # found in second best words
   int score[MAX_LINES]; // Holds all scores for all words compared (used later for comparison)
   int score2[MAX_LINES]; // Holds all scores for all words compared (used later for comparison)
   char answerWord[MAX_LINES][MAX_LEN]; // all the words in answer file
   char guessWord[MAX_LINES][MAX_LEN]; // all the words in guess file
   char combineBothFileWords[MAX_LINES][MAX_LEN]; // all words in guesses + answers file
   char topWords[MAX_LINES][MAX_LEN]; // all the first best words (including ties)
   char secondTopWords[MAX_LINES][MAX_LEN]; // all the second best words (including ties)
};


// The purpose of this function is to:
// 1. Read Answer file and store each word into the struct.
// 2. Store the total number of words read in the Answer file into the struct
// 3. Print error if file not found
// Recieves the "file name" and the "struct" as parameters
void readAnswerFile(char fileName[81], struct wordle *word){
    FILE *inFilePtr  = fopen(fileName, "r");
    int row = 0;
    char inputString[ 81];

    if( inFilePtr == NULL ) {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);
    }

    // reading the file
    while( !feof(inFilePtr) && !ferror(inFilePtr)) {
        if(fgets(word->answerWord[row], MAX_LEN, inFilePtr) != NULL){ // store words
          if(word->answerWord[row][strlen(word->answerWord[row]) - 1] == '\n'){ // Delete extra newline
            word->answerWord[row][strlen(word->answerWord[row]) - 1] = '\0';
          } 
          row++;
        }
      }

    word->answerRowCount = row; // store total words found in the file

    // Close the file
    fclose( inFilePtr);
}


// The purpose of this function is to:
// 1. Read Guesses file and store each word into the struct.
// 2. Store the total number of words read in the Guesses file into the struct
// 3. Print error if file not found
// Recieves the "file name" and the "struct" as parameters
void readGuessesFile(char fileName[81], struct wordle *word){
    FILE *inFilePtr  = fopen(fileName, "r");
    int row = 0;
    char inputString[ 81];

    if( inFilePtr == NULL ) {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);
    }

    // reading the file
    while( !feof(inFilePtr) && !ferror(inFilePtr)) {
        if(fgets(word->guessWord[row], MAX_LEN, inFilePtr) != NULL){ // store words
          if(word->guessWord[row][strlen(word->guessWord[row]) - 1] == '\n'){ // Delete extra newline
            word->guessWord[row][strlen(word->guessWord[row]) - 1] = '\0';
          } 
          row++;
        }
      }

    word->guessRowCount = row; // store total words found in the flle

    // Close the file
    fclose( inFilePtr);
}


// The purpose of this function is to:
// 1. combine words in both Answer and Guess files
// 2. Store those words into the struct
// 3. Store the count of total number of words after combining
// 4. These words will later be used for comparisons.
// Recieves the "struct" as its parameter
void combineAnswerGuessWords(struct wordle *word){

  int total = word->answerRowCount + word->guessRowCount;

  // savng guess words in the array
  for(int i = 0; i < word->guessRowCount; i++){
    for(int j = 0; j < MAX_LEN; j++){
      word->combineBothFileWords[i][j] = word->guessWord[i][j]; // saved into struct
    }
  }

  // saving answer words with guess words in the same array
  int t = 0;
  for(int k = word->guessRowCount; k < total; k++){
    for(int l = 0; l < MAX_LEN; l++){
      word->combineBothFileWords[k][l] = word->answerWord[t][l]; // saved into struct
    }
    t++;
  }

  word->totalWordCount = total;
}


// The purpose of this function is to:
// 1. find the highest score out of all the scores given in second best words
// 2. check if there are any ties with the highest score
// 3. Store the highest score number for second best words into the struct
// 4. Find all the words that hold the highest score
// 5. Store those words into the struct
// Recieves the "struct" as its parameter
void secondBestWordScore(struct wordle *word){

  int highestScore = -1;
    int saveIndex;
    int totalBestWords = 1;

  for(int i = 0; i < word->totalWordCount; i++){
      if(word->score2[i] > highestScore){
        highestScore = word->score2[i];
        saveIndex = i;
      }
    }

    // saving the highest count word in the struct
    for(int i = 0; i < 1; i++){
      for(int k = 0; k < 5; k++){
        word->secondTopWords[i][k] = word->combineBothFileWords[saveIndex][k];
      }
    }

    // This loop will check if there is any other words with same score
    for(int i = 0; i < word->totalWordCount; i++){
      if(i != saveIndex && word->score2[i] == highestScore){
        int k = 1; // since at index 0 we have highest word saved already
        for(int j = 0; j < 5; j++){
          word->secondTopWords[k][j] = word->combineBothFileWords[i][j];
        }
        k++;
        totalBestWords++;
      }
    }
    word->secondWordsCount = totalBestWords;
  
    // this will hold the highest score for second best words
    // No matter what this number will always be same.
    word->secondBestScore[0] = word->score2[saveIndex];
}


// The purpose of this function is to:
// 1. Print the menu option 2 for second best words
// Recieves the "struct" and an "integer i" as its parameters
// - Integer i represents the index of the first best words
// - This function is being called inside of another function's loop
// - So it can print out EACH best first word's --> second best words accordingly
void printMenu2(struct wordle *word, int i){

  printf("%s %d\n", word->topWords[i], word->firstBestScore[0]);
  for(int r = 0; r < word->secondWordsCount; r++){
    printf("\t%s %d\t", word->secondTopWords[r], word->secondBestScore[0]);
  }
  printf("\n");
}


// This is a Helper Function
// Purpose of this function is to:
// 1. Be used by findSecondBestWord()
// 2. Make copies and make comparisons
// 3. Save the words after blanking them into arrays
// Recieves the following parameters: 
// - Arrays: "copyTopWord", "copyOfBlankWord", "answersCopy"
//            These arrays are passed by a function to save blank words for later use
// - Integer: "i"
//            It is sending the index because this function is being called in a loop
// - Struct: "word"
void findFirstTopForSecondBest(struct wordle *word, char copyTopWord[MAX_LINES][MAX_LEN],
                               char copyOfBlankWord[MAX_LINES][MAX_LEN], 
                               char answersCopy[MAX_LINES][MAX_LEN], int i){

  // make copy of the actual top word being compared
    for(int j = 0; j < MAX_LEN; j++){
      copyTopWord[i][j] = word->topWords[i][j];
    }

    // making copy of all the answer.txt file words and reset words back to normal
    for(int j = 0; j < word->answerRowCount; j++){
      for(int k = 0; k < MAX_LEN; k++){
        answersCopy[j][k] = word->answerWord[j][k];
      }
    }

    for(int j = 0; j < word->answerRowCount; j++){
      for(int k = 0; k < 5; k++){
        if(word->topWords[i][k] == answersCopy[j][k]){
          answersCopy[j][k] = ' ';
          copyTopWord[i][k] = ' ';
        }
      }
      // make another copy of the word below for second pass:
      for(int l = 0; l < 5; l++){
        copyOfBlankWord[j][l] = copyTopWord[i][l];
        copyTopWord[i][l] = word->topWords[i][l];
      }
    }

    // second pass to see if letters match in any other order
    for(int j = 0; j < word->answerRowCount; j++){
      for(int k = 0; k < 5; k++){
        for(int l = 0; l < 5; l++){
          if(copyOfBlankWord[j][k] == answersCopy[j][l]
            && copyOfBlankWord[j][k] != ' ' && answersCopy[j][l] != ' '){
            
            copyOfBlankWord[j][k] = ' ';
            answersCopy[j][l] = ' '; 
          }
        }
      }
    }
}


// This is a Helper Function
// Purpose of this function is to:
// 1. Be used by findSecondBestWord(), & findFirstBestWord()
// 2. Make copies and make comparisons
// 3. Save the best words into arrays accordingly
// Recieves the following parameters:
// - Arrays: "allWordsCopy", "answersCopy", "answersCopy2", "copyOfBlankWord"
//           These arrays are used to save copies and blankwords
// - Integer: "p"
//            This function is being called in a loop so p is an index used in arrays
// - struct
// This function returns "score" of type integer for calculating score
int findTopWords(struct wordle *word, char allWordsCopy[MAX_LINES][MAX_LEN],
                  char answersCopy[MAX_LINES][MAX_LEN], 
                  char answersCopy2[MAX_LINES][MAX_LEN], char copyOfBlankWord[MAX_LINES][MAX_LEN],
                  int p, int score){

      score = 0; // resetting score for new word
  
      // making copy of all the words and reset words back to normal for next word
      for(int j = 0; j < word->totalWordCount; j++){
        for(int k = 0; k < MAX_LEN; k++){
          allWordsCopy[j][k] = word->combineBothFileWords[j][k];
        }
      }
  
      // making copy of all the answer.txt file words and reset words back to normal
      for(int j = 0; j < word->answerRowCount; j++){
        for(int k = 0; k < MAX_LEN; k++){
          answersCopy2[j][k] = answersCopy[j][k];
        }
      }
  
      //first pass to see if all letter match position
      for(int j = 0; j < word->answerRowCount; j++){
        for(int k = 0; k < 5; k++){
          if(word->combineBothFileWords[p][k] == answersCopy2[j][k]){
            allWordsCopy[p][k] = ' '; 
            answersCopy2[j][k] = ' ';
            score+=3;
          }
        }
        // make another copy of the word below for second pass:
        for(int l = 0; l < 5; l++){
          copyOfBlankWord[j][l] = allWordsCopy[p][l]; // might be issue
          allWordsCopy[p][l] = word->combineBothFileWords[p][l];
        }
      }
      
      // second pass to see if letters match in any other order
      for(int j = 0; j < word->answerRowCount; j++){
        for(int k = 0; k < 5; k++){
          for(int l = 0; l < 5; l++){
            if(copyOfBlankWord[j][k] == answersCopy2[j][l]
              && copyOfBlankWord[j][k] != ' ' && answersCopy2[j][l] != ' '){
              
              copyOfBlankWord[j][k] = ' ';
              answersCopy2[j][l] = ' ';
              score++;
              
            }
          }
        }
      }
    return score;
  
}


// Purpose of this function is to:
// 1. Find best first words
// 1. Compare all words in both files to first best words (blank answer words)
// 2. Calculate scores for each comparison and save it into an array (Struct)
// 3. Call a function to compare scores and find the highest score word
// Recieves "struct", "Both file names" as its parameters
void findSecondBestWord(struct wordle *word, char fileName1[81],
                     char fileName2[81]){

  printf("%s has %d words\n", fileName1, word->answerRowCount);
  printf("%s has %d words\n", fileName2, word->guessRowCount);  
  printf("\nWords and scores for top first words and second words:\n");

  char copyTopWord[MAX_LINES][MAX_LEN];
  char copyOfBlankWord[MAX_LINES][MAX_LEN];
  char answersCopy[MAX_LINES][MAX_LEN];
  char answersCopy2[MAX_LINES][MAX_LEN];
  char allWordsCopy[MAX_LINES][MAX_LEN];
  int score;

  // Loop through all the words and find the first top and blank them out for later use
  // in finding the second best word
  for(int i = 0; i < word->topWordsCount; i++){

    findFirstTopForSecondBest(word, copyTopWord, copyOfBlankWord, answersCopy, i);


    // Loop through the all answer words again and find the second best words using
    // the first best found above
    for(int p = 0; p < word->totalWordCount; p++){ // loop through all words

      score = findTopWords(word, allWordsCopy, answersCopy, answersCopy2, copyOfBlankWord, p, score);

      word->score2[p] = score;
    }
    
    // Below we will find the highest score for second best words

    secondBestWordScore(word);

    printMenu2(word, i);
  
  }
  printf("Done\n");

}


// The purpose of this function is to:
// 1. Print the menu option 1 for first best words
// Recieves the "struct" and and "both file names" as its parameters
void printMenu1(struct wordle *word, char fileName1[81], 
                char fileName2[81]){
  
  printf("%s has %d words\n", fileName1, word->answerRowCount);
  printf("%s has %d words\n", fileName2, word->guessRowCount);
  
  printf("\nWords and scores for top first words:\n");
  
  for(int i = 0; i < word->topWordsCount; i++){
    printf("%s\t%d\n", word->topWords[i], word->firstBestScore[0]);
  }
   
  printf("Done\n");
  
}


// The purpose of this function is to:
// 1. find the highest score out of all the scores given in first best words
// 2. check if there are any ties with the highest score found
// 3. Store the highest score number for first best words into the struct
// 4. Find all the words that hold that highest score
// 5. Store those words into the struct
// 6. It also controls the MENU OPTION 1 and MENU OPTION 2
// Recieves the "struct", "menu option", and "both file names" as its parameter
void findHighestScoringWords(struct wordle *word, int menuOption,
                             char fileName1[81], char fileName2[81]){

  int highestScore = -1;
  int saveIndex;
  int totalBestWords = 1;

  // this will find the highest scores for some word at the saveIndex
  for(int i = 0; i < word->totalWordCount; i++){
    if(word->score[i] > highestScore){
      highestScore = word->score[i];
      saveIndex = i;
    }
  }

  // saving the highest count word in the struct
  for(int i = 0; i < 1; i++){
    for(int k = 0; k < 5; k++){
      word->topWords[i][k] = word->combineBothFileWords[saveIndex][k];
      }
  }

  // This loop will check if there is any other words same score
  for(int i = 0; i < word->totalWordCount; i++){
    if(i != saveIndex && word->score[i] == highestScore){
      int k = 1; // since at index 0 we have highest word saved already
      for(int j = 0; j < 5; j++){
        word->topWords[k][j] = word->combineBothFileWords[i][j];
      }
      k++;
      totalBestWords++;
    }
  }
  word->topWordsCount = totalBestWords;
  
  // this will hold the highest score for first
  // No matter what this number will always be same
  word->firstBestScore[0] = word->score[saveIndex];

  // If user picks menu option 1 we can call the print menu option 1 function
  // Then end the function there since we have all the information needed for #1
  if(menuOption == 1){ 
    printMenu1(word, fileName1, fileName2);
  }
  // If user picks menu option 2 we can then go furthermore into the function
  // And find the second best words
  else if(menuOption == 2){
    findSecondBestWord(word, fileName1, fileName2);
  }
}


// Purpose of this function is to:
// 1. Compare all words in both files to answer file
// 2. Calculate scores for each comparison and save it into an array (Struct)
// 3. Call a function to compare scores and find the highest score word
// Recieves "menu option #", "Both file names" as its parameters
// - menu # is only passed so it can be used in later functions.
void findFirstBestWord(int menuOption, char fileName1[81],
                       char fileName2[81]){

  struct wordle word;
  readAnswerFile(fileName1, &word);
  readGuessesFile(fileName2, &word);
  combineAnswerGuessWords(&word);

  char answersCopy[word.answerRowCount][MAX_LEN];
  char answersCopy2[word.answerRowCount][MAX_LEN];
  char allWordsCopy[word.totalWordCount][MAX_LEN];
  char copyOfBlankWord[word.totalWordCount][MAX_LEN];
  int score;

  // making copy of all the answer.txt file words and reset words back to normal
    for(int j = 0; j < word.answerRowCount; j++){
      for(int k = 0; k < MAX_LEN; k++){
        answersCopy[j][k] = word.answerWord[j][k];
      }
    }


  for(int i = 0; i < word.totalWordCount; i++){ // loop through all words

    score = findTopWords(&word, allWordsCopy, answersCopy, answersCopy2, copyOfBlankWord, i, score);
    word.score[i] = score;

  } // end of looping through all words

  if(menuOption != 3){ // 3 is if user wants to change files
    findHighestScoringWords(&word, menuOption, fileName1, fileName2);
  }

}


int main(void) {

  // Initializing Answer filename to default: "answersTiny.txt"
  // Initializing Guesses filename to default: "GuessesTiny.txt"
  char answersFileName[81] = {'a', 'n', 's', 'w', 'e', 'r', 's',
                              'T', 'i', 'n', 'y', '.', 't', 'x', 't'};
  char guessesFileName[81] = {'g', 'u', 'e', 's', 's', 'e', 's',
                              'T', 'i', 'n', 'y', '.', 't', 'x', 't'};

  // First thing to print when the program starts:
  printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

  // Controlling all the menu options below
  int menuOption = 0;
  do {
      printf("\n");
      printf("Menu Options:\n");
      printf("  1. Display best first words only\n");
      printf("  2. Display best first and best second words\n");
      printf("  3. Change answers and guesses filenames\n");
      printf("  4. Exit\n");
      printf("Your choice: ");
      scanf("%d", &menuOption);

      // Exit the program when user enters 4
      if (menuOption == 4) {
          exit(1); // Exit the program
      } 
      // Find the best first words if user enters 1
      else if (menuOption == 1) {
        findFirstBestWord(menuOption, answersFileName, guessesFileName);
      }
      // Find the second best words along with their best first word  if user enters 2
      else if (menuOption == 2) {
        findFirstBestWord(menuOption, answersFileName, guessesFileName);
      }
      // Change file names.  Menu will then be redisplayed  if user enters 3
      else if (menuOption == 3) {
          printf("Enter new answers and guesses filenames: ");
          scanf("%s %s", answersFileName, guessesFileName);
      }
  } while( menuOption == 3); // Program should repeat if user decides to change files

  return 0;
}