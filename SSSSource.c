/*
FLOW OF THE CODE

main() > scheduler() > mainMenu()

mainMenu() > preGame() > game()
game() > preGame()
game() > mainMenu()
game() > moveDn() > gameMotion() > game()
game() > moveUP() > gameMotion() > game()
game() > obsLeft() > gameMotion() > game()

mainMenu() > tutorial()
tutorial() > mainMenu()

mainmenu() > readScore();

mainMenu() > exit()

readScore() > mainMenu()
*/

// Precompiler definitions
#define _CRT_SECURE_NO_WARNINGS

// Includes section
#include <conio.h> // for the game
#include <stdio.h> // for the game
#include <stdlib.h> // for the game
#include <string.h> // for the menu
#include <time.h> // for the game
#include <memory.h> // for the game
#include <windows.h> // for the game

// Declare UDFs
char game();
char gameMotion();
char preGame(); // This moves everybody to the left so that the hole goes away!!!!! So glad this finally works!
int collect(); // this is the checker that sees if the coin is collected.
int collisons(); // Use this to check if the player hit an obstacle
int footer(); // Simply prints out whatever goes below the game or menu render
int header(); // Cool little header in the top of the menus and games
// Right here is where the int main() can be found. It doesn't have to be declared, but this is the definition can be found
int mainMenu(); // Function for the main menu
int readScore(); // This function reads the high score from the save file.
int scheduler(); // simple scheduler for scheduling things
int spawn(); // spawn a new obstacle in, this UDF also moves everything left by removing the first index in the array, and adding one on the end.
int spawnCoin(); // this code adds in the coins
int writeScore(); // Writes the score to the high score file if it is bigger that the previously saved.
void mallocLn(); // This handles the creation of all of the lanes. Allocating the mem and freeing it too
void menuHeader(); // This is a header, but the score is removed since it is the menu
void moveDn(); // move the character down
void moveUp(); // move the character up
void obsLeft(); // move everything to the left
void render(); // render the content of the game
void resetGame(); // use this to reset the game variables to a new game state.
void tutorial(); // Just print a pretty thing that tells you how to play.

// Global variables
// The following pointers create the arrays that eventually become the lanes.
char* lane0[75];
char* lane1[75];
char* lane2[75];
char* lane3[75];
char* lane4[75];
int coins = 0; // This is the storage for the coins
int gameOver = 0; // This is used to store the state of the game, 0 = playing, 1 = over.
int lanes; // Collect the total number of lines here and use it for the number of lanes in the game
//int length = 15; // maybe try and use this to change the length of the track. For now, set it to a constant number.
int location = 2; // Starting lane for the character. With 5 lanes, this should be set to 2
int obsDis = 75; // How far away the obstacle is from the character
int obsLane; // What lane the obstacle is in. Used for checking if the obstacle collides with the character. Compare this to the location variable, just a few lines above.
int score = 0; // use this as the score in the game

// UDFs
char game() {	// game code here
	while (gameOver == 0) {
		// These three lines render the frame
		header();
		render();
		footer();

		// Collect input for motion
		char move = _getch();

		// Move the character
		if (move == 'w') {
			// move everything left
			obsLeft();
			obsLeft(); // Before gameMotion() was created, this ran twice. Just helps the game move along a bit faster.
			moveUp();
			gameMotion();
		}
		else if (move == 's') {
			// move everything left
			obsLeft();
			obsLeft(); // Before gameMotion() was created, this ran twice. Just helps the game move along a bit faster.
			moveDn();
			gameMotion();
		}
		else if (move == 'd') {
			obsLeft();
			obsLeft(); // Before gameMotion() was created, this ran twice. Just helps the game move along a bit faster.
			gameMotion();
		}
		else if (move == 'q') {
			printf("Are you sure you want to quit? Y/n");
			char quit = _getch();
			if (quit == 'y') { // Yes quit the game, go back to the main menu, and reset all of the scores.
				resetGame();
				mainMenu();
			}
			else if (quit == 'n') { // Don't quit, kinda just let the game loop continue
				printf("\nOk, returning to game...");
				Sleep(2000);
			}
		}
		else { // Error handling for motion
			printf("Captain, I don't understand. Please clarify your instructions.");
			Sleep(800);
		}

		// This seems to control refresh rate I think?
		Sleep(10);
		system("cls");
	}

	menuHeader();
	writeScore();


	header();
	printf("\n\nGame over! You lose! Good day sir. Press 'r' to restart, or 'm' to go to the main menu\n\n\n");
	footer();
	Sleep(3000);

	// Menu handling here
	while (1) {
		char endGame = _getch(); // this should come after the user sees the last printf. They should know what to do.
		if (endGame == 'r') { // r for restart
			resetGame();
			system("cls");
			preGame(); // This, and the next line are essential in any case, now that preGame() exists.
			game();
		}
		else if (endGame == 'm') { // m for menu
			resetGame();
			system("cls");
			mainMenu();
		}
	}
}

char gameMotion() { // The code here was taken out of the back of the game. If the user tried to quit, or pressed a random key, they would still move and get a point.
	// Use this to count when to spawn characters in
	score++;

	if (score < 100) {
		if (score % 4 == 0) { // Spawn every 4th motion
			spawn();
			spawnCoin();
		}
	}
	else if (score < 200 && score > 100) {
		if (score % 3 == 0) { // Spawn every 3rd motion
			spawn();
			spawnCoin();
		}
	}
	else if (score < 300 && score > 200) {
		if (score % 2 == 0) { // Spawn every 2nd motion
			spawn();
			spawnCoin();
		}
	}
	else if (score < 400 && score > 300) {
		if (score % 1 == 0) { // spawn every motion
			spawn();
			spawnCoin();
		}
	}
	else {
		if (score % 1 == 0) { // spawn MORE every motion
			spawn();
			spawn();
			spawnCoin();
			spawnCoin();
		}
	}

	collisons(); // this checks for collisions with objects
	collect(); // this checks for collisons with coins
}

char preGame() {	// The purpose of this UDF is to move the character over a set amount of spaces automatically, so the player doesn't have to manually push the button 40 to get to the obstacles.
	system("cls"); // this is here to clear the screen, added when the user wants to restart directly after they lose.
	printf("Getting your ship ready!\n"); // start the game
	Sleep(1000);
	printf("Entering orbit...\n");
	Sleep(1000);
	printf("All systems functional! Ready to fly!\n\n");
	Sleep(500);
	printf("Press enter to take off!\n");
	_getch();
	system("cls");
	mallocLn();

	int a;
	location = 2; // This sets the character back to the center lane automatically

	for (a = 0; a < 40; a++) { // a < 40 is the controller for how far the character automatically moves in the beginning of the game. 
		// Print the character and header out
		header();
		render();
		footer();

		// Collect input
		char move = 'd';

		// move everything left
		obsLeft();

		// Move the character
		if (move == 'd') {
			obsLeft();
		}

		// refresh
		Sleep(10); // Frames per second is controlled here. 1000 is 1 fps, 100 is 10 fps, 10 is 100 fps.
		system("cls");

		if (a % 4 == 0) { // When this is set to score, as in game(), it will spawn something every single time. Setting this variable to a makes things spawn normally.
			spawn();
		}
	}
}

int collect() {
	// These will detect if the player hits a coin
	// Lane 0
	if ((location == 0) && (lane0[0] == "O")) { // not sure why, but adding the extra parenthesis on these is the only way they work. Removing any of them breaks the collision checking
		coins++;
	}

	// Lane 1
	else if ((location == 1) && (lane1[0] == "O")) {
		coins++;
	}

	// Lane 2
	else if ((location == 2) && (lane2[0] == "O")) {
		coins++;
	}

	// Lane 3
	else if ((location == 3) && (lane3[0] == "O")) {
		coins++;
	}

	// Lane 4
	else if ((location == 4) && (lane4[0] == "O")) {
		coins++;
	}
}

int collisons() {
	// These will detect if the player hits an obstacle
	// Lane 0
	if ((location == 0) && (lane0[0] == "X")) { // not sure why, but adding the extra parenthesis on these is the only way they work. Removing any of them breaks the collision checking
		gameOver = 1;
	}

	// Lane 1
	else if ((location == 1) && (lane1[0] == "X")) {
		gameOver = 1;
	}

	// Lane 2
	else if ((location == 2) && (lane2[0] == "X")) {
		gameOver = 1;
	}

	// Lane 3
	else if ((location == 3) && (lane3[0] == "X")) {
		gameOver = 1;
	}

	// Lane 4
	else if ((location == 4) && (lane4[0] == "X")) {
		gameOver = 1;
	}
}

int footer() {
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

int header() {
	printf(" ___                       ___                       ___                 _  _           \n");
	printf("/ __> _ _  ___  ___  _ _  / __> ___  ___  ___  ___  / __> ___  _ _  ___ | || | ___  _ _ \n");
	printf(" __  | | || .  / ._>| '_>  __  | .  <_> |/ | '/ ._>  __  / | '| '_>/ .  | || |/ ._>| '_>\n");
	printf("<___/`___||  _/ ___.|_|   <___/|  _/<___| _|_. ___. <___/ _|_.|_|   ___/|_||_| ___.|_|  \n");
	printf("          |_|                  |_|\n");
	printf("\n----------------------------------------------------------------------------------------\n");
	printf("Score = %d\nCoins = %d", score, coins);
	printf("\n----------------------------------------------------------------------------------------\n\n");
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

// Here is the int main that gets everything going
int main() {
	scheduler();
}

int mainMenu() {
	system("cls"); // clear the screen
	menuHeader(); // Display the special header built for the menu (It doesn't show the score)
	printf("\nWelcome Space Pilot!\n\nPress 'p' to play!\nPress 'h' for how to play\nPress 's' to view the saved high score\nPress 'q' to quit\n\n");
	char menu = _getch();

	if (menu == 'p') {
		system("cls");
		preGame();
		game(); // if this works, get rid of the inGame UDF
	}
	else if (menu == 'h') {
		tutorial();
	}
	else if (menu == 's') {
		readScore();
	}
	else if (menu == 'q') {
		exit(0);
	}
	else {
		mainMenu();
	}
	//exitapp();
}

// Read the high score from the save file
int readScore() {
	char c[1000]; // Create a string for the file data to be pulled from
	errno_t err = 0;
	FILE* fptr; // pointer to save the location of the file
	if ((fptr = fopen("highscore.txt", "r")) == NULL) { // if the file cannot be opened, print an error message
		printf("Error! No save file exists. Press enter to create one.");
		_getch();
		fptr = fopen("highscore.txt", "a+"); // Create the new save file
		char newFile[200] = "No saved highscores yet.\n"; // text for the created file.
		fprintf(fptr, "%s", &newFile); // write the text to the file so it doesn't appear weird.
		printf("\n\nSave file created! Go play!");
		_getch();
		fclose(fptr); // THIS LINE IN THIS EXACT POSITION FIXES THE BROKEN ASPECTS OF THE READER! LETS GOOOOO.
		mainMenu();
	}

	// make the output look pretty
	system("cls");
	menuHeader();

	// Print everthing out
	fscanf(fptr, "%[^\n]", c);// reads text until newline is encountered
	printf("\nSaved high score: %s", &c); // print data
	fclose(fptr);

	// return to the menu
	printf("\n\nPress any key to return to the main menu.");
	_getch();
	mainMenu();
}

// a very useful scheduler. Kinda went a different direction with this.
int scheduler() {
	mainMenu();
	_getch();
}

int spawn() {
	int obsLane = (rand() % (4 - 0 + 1)) + 0; // this number is supposed to generate the lane the obstacle will spawn in.

	// append the obstacle to the end of the desired array. This is the spawn of the new obstacle
	// lane 0
	if (obsLane == 0) {
		lane0[74] = "X";
	}

	// lane 1
	if (obsLane == 1) {
		lane1[74] = "X";
	}

	// lane 2
	if (obsLane == 2) {
		lane2[74] = "X";
	}

	// lane 3
	if (obsLane == 3) {
		lane3[74] = "X";
	}

	// lane 4
	if (obsLane == 4) {
		lane4[74] = "X";
	}
}

int spawnCoin() {
	int coinLane = (rand() % (4 - 0 + 1)) + 0; // this number is supposed to generate the lane the coin will spawn in.

	// append the coin to the end of the desired array. This is the spawn of the new coin
	// lane 0
	if (coinLane == 0) {
		lane0[74] = "O";
	}

	// lane 1
	if (coinLane == 1) {
		lane1[74] = "O";
	}

	// lane 2
	if (coinLane == 2) {
		lane2[74] = "O";
	}

	// lane 3
	if (coinLane == 3) {
		lane3[74] = "O";
	}

	// lane 4
	if (coinLane == 4) {
		lane4[74] = "O";
	}
}

int writeScore() {
	char s[1000] = "";
	errno_t err = 0;
	FILE* fptr;
	/*if ((fptr = fopen("highscore.txt", "a+")) == NULL) {
		printf("Error! File cannot be opened. Press enter to return to the main menu.");
		_getch();
		mainMenu();
	} */

	if ((fptr = fopen("highscore.txt", "r")) == NULL) { // if the file cannot be opened, create it
		fptr = fopen("highscore.txt", "a+"); // Create the new save file
		char newFile2[200] = "No saved highscores yet.\n"; // text for the created file.
		fprintf(fptr, "%s", &newFile2); // write the text to the file so it doesn't appear weird.
		fclose(fptr); // THIS LINE IN THIS EXACT POSITION FIXES THE BROKEN ASPECTS OF THE READER! LETS GOOOOO.
	}

	// reads text until newline is encountered
	fscanf(fptr, "%[^\n]", &s);
	// prints last saved high score and current score

	fclose(fptr);

	// prompt user to overwrite their score
	while (1) {
		printf("Previously saved high score: %s\n", s);
		printf("Current score: %d\n", score);
		printf("\nDo you want to overwrite your save with your current score? Y/n ");
		char write = _getch();
		if (write == 'y') {
			// creating file pointer to work with files
			errno_t  err = 0;
			FILE* fptr;

			// opening file in writing mode
			fptr = fopen("highscore.txt", "wt"); // open in write mode

			// print if there is an error message, gets skipped if so
			if (fptr == NULL) {
				printf("Error!");
			}

			// get the user's name
			printf("\n\n\nWhat name shall you be remembered by, Captain? ");
			char saveName[10000] = ""; // Unoverflowable buffer :) unless they try really hard. 
			scanf("%s", saveName);

			// write the score to the save file
			fprintf(fptr, "%d by %s", score, saveName);
			fclose(fptr);
			printf("\nHigh score saved!\n");

			// Return to the menu
			printf("\n\nPress enter to return to the main menu... ");
			_getch();
			resetGame();
			mainMenu();
		}
		else if (write == 'n') {
			printf("\n\nOk, deleting this score...");
			printf("\n\nPress enter to return to the main menu... ");
			_getch();
			resetGame();
			mainMenu();
		}
		else {
			printf("Command not understood.");
			Sleep(1000);
			system("cls");
		}
	}
}

void mallocLn() { // Wow this UDF took a lot more responsibility than originally intended
	int n = 75; // n is for the length of the lane
	int i = 0; // use this for the loop

	lane0[i] = (char*)malloc(n * sizeof(char)); // This will be used to dynamically allocate the memory
	lane1[i] = (char*)malloc(n * sizeof(char)); // This will be used to dynamically allocate the memory
	lane2[i] = (char*)malloc(n * sizeof(char)); // This will be used to dynamically allocate the memory
	lane3[i] = (char*)malloc(n * sizeof(char)); // This will be used to dynamically allocate the memory
	lane4[i] = (char*)malloc(n * sizeof(char)); // This will be used to dynamically allocate the memory

	// The next block of code is used to generate the data in the lane arrays
	// lane 0
	for (i = 0; i < n; ++i) { // for everything less than the length of the lane
		lane0[i] = " "; // write a space in the line
	}

	// lane 1
	for (i = 0; i < n; ++i) { // for everything less than the length of the lane
		lane1[i] = " "; // write a space in the line
	}

	// lane 2
	for (i = 0; i < n; ++i) { // for everything less than the length of the lane
		lane2[i] = " "; // write a space in the line
	}

	// lane 3
	for (i = 0; i < n; ++i) { // for everything less than the length of the lane
		lane3[i] = " "; // write a space in the line
	}

	// lane 4
	for (i = 0; i < n; ++i) { // for everything less than the length of the lane
		lane4[i] = " "; // write a space in the line
	}
}

void menuHeader() {
	printf(" ___                       ___                       ___                 _  _           \n");
	printf("/ __> _ _  ___  ___  _ _  / __> ___  ___  ___  ___  / __> ___  _ _  ___ | || | ___  _ _ \n");
	printf(" __  | | || .  / ._>| '_>  __  | .  <_> |/ | '/ ._>  __  / | '| '_>/ .  | || |/ ._>| '_>\n");
	printf("<___/`___||  _/ ___.|_|   <___/|  _/<___| _|_. ___. <___/ _|_.|_|   ___/|_||_| ___.|_|  \n");
	printf("          |_|                  |_|\n");
	printf("\n----------------------------------------------------------------------------------------\n");
}

void moveDn() {
	// This math appears to keep the location variable between 0 and 4... It does this by denying motion if location is 0 or 4
	if (location == 4) {
		location += 0;
	}
	else if (location == 0) {
		location += 1;
	}
	else {
		location += 1;
	}
}

void moveUp() {
	// This math should keep the location variable between 0 and 4... It does this by denying motion if location is 0 or 4
	if (location == 4) {
		location -= 1;
	}
	else if (location == 0) {
		location -= 0;
	}
	else {
		location -= 1;
	}
}

void obsLeft() { // move this to the void section when complete
	int size = 75;

	// remove the first element of the array with these loops
	// lane 0
	for (int b = 0; b < size - 1; b++) {
		lane0[b] = lane0[b + 1];
	}

	// lane 1
	for (int b = 0; b < size - 1; b++) {
		lane1[b] = lane1[b + 1];
	}

	// lane 2
	for (int b = 0; b < size - 1; b++) {
		lane2[b] = lane2[b + 1];
	}

	// lane 3
	for (int b = 0; b < size - 1; b++) {
		lane3[b] = lane3[b + 1];
	}

	// lane 4
	for (int b = 0; b < size - 1; b++) {
		lane4[b] = lane4[b + 1];
	}

	// These next five lines add a space at the end of the array
	lane0[74] = " ";
	lane1[74] = " ";
	lane2[74] = " ";
	lane3[74] = " ";
	lane4[74] = " ";
}

void render() {
	int i, n = 75;

	// lane 0
	if (location != 0) { // use this if to print a space if the character is not in the lane. This makes the length of the lanes the same
		printf("  ");
	}
	else {
		printf("8>"); // This prints the character, in theory allowing the removal of the character() UDF.
	}
	for (i = 0; i < n; ++i) { // Print out the lane loop
		printf("%s", lane0[i]); // this does the printing
	}
	printf("\n"); // move to the next lane

	// lane 1
	if (location != 1) { // use this if to print a space if the character is not in the lane. This makes the length of the lanes the same
		printf("  ");
	}
	else {
		printf("8>"); // This prints the character, in theory allowing the removal of the character() UDF.
	}
	for (i = 0; i < n; ++i) { // Print out the lane loop
		printf("%s", lane1[i]); // this does the printing
	}
	printf("\n"); // move to the next lane

	// lane 2
	if (location != 2) { // use this if to print a space if the character is not in the lane. This makes the length of the lanes the same
		printf("  ");
	}
	else {
		printf("8>"); // This prints the character, in theory allowing the removal of the character() UDF.
	}
	for (i = 0; i < n; ++i) { // Print out the lane loop
		printf("%s", lane2[i]); // this does the printing
	}
	printf("\n"); // move to the next lane

	// lane 3
	if (location != 3) { // use this if to print a space if the character is not in the lane. This makes the length of the lanes the same
		printf("  ");
	}
	else {
		printf("8>"); // This prints the character, in theory allowing the removal of the character() UDF.
	}
	for (i = 0; i < n; ++i) { // Print out the lane loop
		printf("%s", lane3[i]); // this does the printing
	}
	printf("\n"); // move to the next lane

	// lane 4
	if (location != 4) { // use this if to print a space if the character is not in the lane. This makes the length of the lanes the same
		printf("  ");
	}
	else {
		printf("8>"); // This prints the character, in theory allowing the removal of the character() UDF.
	}
	for (i = 0; i < n; ++i) { // Print out the lane loop
		printf("%s", lane4[i]); // this does the printing
	}
	printf("\n"); // move to the next lane
}

void resetGame() {
	gameOver = 0; // these next 4 variables return the game to the beginning state, middle of the lanes, no score/coins etc.
	score = 0;
	coins = 0;
	location = 2; // This puts the character back in the middle lane on the restart.
}

void tutorial() { // This section is just a bunch of printouts that look good, and describe how to play
	system("cls");
	menuHeader();
	printf("\nWelcome to space camp! Here is the crash course :)\n\n");
	printf("\nMOVEMENT\n----------------------------------------------------------------------------------------\n");
	printf("To move your ship up, press 'w'\nTo move your ship down, press 's'\nTo move your ship forward, press 'd'\n\n");
	printf("\nMISSION OBJECTIVE\n----------------------------------------------------------------------------------------\n");
	printf("Travel through the infinite loop of space, dodging obstacles that look like X.\nRack up the highest score possible to brag to your friends I suppose.\nYour ship has a shield that can sometimes protect you, but be wary, hitting obstacles\ncan always cause you to crash!\n\n");
	printf("Press any key to return to the main menu");
	_getch();
	mainMenu();
}

/* DEVELOPMENT NOTES AND IDEAS

Alphabetize everything! DONE DONE DONE!!!!!
Automatically push d like 40 times, so there isn't the intial gap in the game? DONE DONE DONE!!!!!!!
Coins, the counterpart of obstacles? DONE DONE DONE!!!!!!!
Write high scores to a text file, plaintext, so that they can be read and have another page in the menu for saved highscores, and usernames? DONE
Have more obstacles spawn in the higher the score is? Make the game get progressively harder? DONE DONE DONE!!!!!!

UDFs removed ("Optimization")
character()
inGame()

*/