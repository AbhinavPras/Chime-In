//----------------------------------
//IMPORTANT NOTE
//----------------------------------
//THIS IS PURLEY FOR TESTING PURPOSES
//FINAL GAME DOES NOT USE ARDUINO



//REMOVE
#include <Arduino.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//----------------------------------------------------------------
//Credits
//----------------------------------------------------------------

//1. Random number generator taken from: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
//2. Delay function taken from: https://c-for-dummies.com/blog/?p=69

//----------------------------------------------------------------
//Defines for all functions
//----------------------------------------------------------------

#define CHECK_BUTTON
#define LIGHT_ON
#define LIGHTS_OFF
#define PATTERN
#define IDLE
#define PASSCODE_CHECK
#define DIFFICULTY
#define GAMEPLAY
#define LOSE
#define WIN
#define DEACTIVATE
#define MAIN

//----------------------------------------------------------------
//Pin & Button Variables
//----------------------------------------------------------------

unsigned int ledGPin = 12;
unsigned int ledRPin = 10;
unsigned int ledYPin = 8;
unsigned int ledBPin = 6;

unsigned int buttonGPin = 13;
unsigned int buttonRPin = 11;
unsigned int buttonYPin = 9;
unsigned int buttonBPin = 7;

//----------------------------------------------------------------
//Function Declarations
//----------------------------------------------------------------

#ifdef CHECK_BUTTON
unsigned int checkButton();
#endif
#ifdef LIGHT_ON
void lightOn(unsigned int light, unsigned int time_on, unsigned int wait_time);
#endif
#ifdef LIGHTS_OFF
void lightsOff(unsigned int light, unsigned int wait_time);
#endif
#ifdef PATTERN
void pattern(unsigned int pattern_number);
#endif
#ifdef IDLE
void idle();
#endif
#ifdef PASSCODE_CHECK
void passcodeCheck();
#endif
#ifdef DIFFICULTY
void difficulty();
#endif
#ifdef GAMEPLAY
void gameplay();
#endif
#ifdef LOSE
void lose();
#endif
#ifdef WIN
void win();
#endif
#ifdef DEACTIVATE
void deactivate();
#endif
#ifdef MAIN
int main();
#endif

//----------------------------------------------------------------
//Global variables
//----------------------------------------------------------------

//Passcodes (reversed to compensate for the algorithm)
unsigned int g_easy_passcode = 4321; //1234
unsigned int g_moderate_passcode = 1111; //1111
unsigned int g_difficult_passcode = 1234; //4321
unsigned int g_operator_passcode = 4444; //4444

//Difficulty variables
unsigned int g_difficulty_level = 0; //easy = 1 | moderate = 2 | difficult = 3
unsigned int g_start_amount = 0; //The amount of light flashes on the first sequence
unsigned int g_sequence_amount = 0; //The amount of sequences the game will go through
unsigned int g_between_time = 0; //Time between light flashes (miliseconds)
unsigned int g_wait_time = 0; //Time waited for a pushbutton input (miliseconds)
unsigned int g_decrement_between_time = 0; //The time that gets subtracted from between_time every time a sequence completes
unsigned int g_decrement_wait_time = 0; //The time that gets subtracted from g_wait_time every time a sequence completes

//----------------------------------------------------------------
//Function Definitions
//----------------------------------------------------------------

//Checks which button is pressed, if any
#ifdef CHECK_BUTTON
unsigned int checkButton() {
    if(digitalRead(buttonGPin) == LOW) {
        return 1;
    } else if(digitalRead(buttonRPin) == LOW) {
        return 2;
    } else if(digitalRead(buttonYPin) == LOW) {
        return 3;
    } else if(digitalRead(buttonBPin) == LOW) {
        return 4;
    } else {
        return 0;
    }
}
#endif

//Turns light on for time_on and waits for wait_time
#ifdef LIGHT_ON
void lightOn(unsigned int light, unsigned int time_on, unsigned int wait_time) {
    //1 = Light 1 || 2 = Light 2 || 3 = Light 3 || 4 = Light 4 || 5 = All lights
    switch (light) {
        case 1: //Green (12)
            light = 12;
            break;
        case 2: //Red (10)
            light = 10;
            break;
        case 3: //Yellow (8)
            light = 8;
            break;
        case 4: //Blue (6)
            light = 6;
            break;
        case 5: //All lights
            digitalWrite(ledGPin, HIGH);
            digitalWrite(ledRPin, HIGH);
            digitalWrite(ledYPin, HIGH);
            digitalWrite(ledBPin, HIGH);
            delay(time_on);
            lightsOff(5, 0);
            if(wait_time != 0) {
                delay(wait_time);
            }
            return;
        case 6:
            digitalWrite(ledGPin, HIGH);
            digitalWrite(ledRPin, HIGH);
            digitalWrite(ledYPin, HIGH);
            digitalWrite(ledBPin, HIGH);
            if(wait_time != 0) {
                delay(wait_time);
            }
            return;
    }
    digitalWrite(light, HIGH);
    delay(time_on);
    digitalWrite(light, LOW);
    if(wait_time != 0) {
        delay(wait_time);
    }
}
#endif

//Turns off all lights
#ifdef LIGHTS_OFF
void lightsOff(unsigned int light, unsigned int wait_time) {
    switch(light) {
        case 1: //Turn off green
            digitalWrite(ledGPin, LOW);
            break;
        case 2: //Turn off red
            digitalWrite(ledRPin, LOW);
            break;
        case 3: //Turn off yellow
            digitalWrite(ledYPin, LOW);
            break;
        case 4: //Turn off blue
            digitalWrite(ledBPin, LOW);
            break;
        case 5: //Turn off all lights
            digitalWrite(ledGPin, LOW);
            digitalWrite(ledRPin, LOW);
            digitalWrite(ledYPin, LOW);
            digitalWrite(ledBPin, LOW);
            break;
    }
    if(wait_time != 0) {
        delay(wait_time);
    }
}
#endif

//Plays 8 different patterns
#ifdef PATTERN
void pattern(unsigned int pattern_number) {

    unsigned int passcode_pattern = 0;

    switch(pattern_number) {
        case 1: //Easy mode passcode
            passcode_pattern = 1;
            break;
        case 2: //Moderate mode passcode
            passcode_pattern = 2;
            break;
        case 3: //Difficult mode passcode
            passcode_pattern = 3;
            break;
        case 4: //Incorrect passcode
            for(unsigned int i = 0; i < 5; i++) {
                lightOn(2, 250, 250);
            }
            break;
        case 5: //User completes all patterns in a round
            lightOn(1, 50, 50);
            lightOn(2, 50, 50);
            lightOn(3, 50, 50);
            lightOn(4, 50, 50);
            lightOn(5, 100, 100);
            lightOn(5, 100, 250);
            break;
        case 6: //Win
            for(unsigned int i = 10; i > 0; i--) {
                for(unsigned int k = 1; k <= 4; k++) {
                    lightOn(k, 100, 100);
                }
            }
            break;
        case 7: //Lose
            for(unsigned int i = 0; i < 5; i++) {
                lightOn(5, 150, 150);
            }
            lightOn(6, 0, 250);
            for(unsigned int i = 1; i <= 4; i ++) {
                lightsOff(i, 250);
            }
            break;
        case 8: //Deactivate
            for(unsigned int i = 0; i < 5; i++) {
                lightOn(5, 100, 100);
            }
            break;
    }
    //Passcode pattern (easy, moderate, and difficult)
    if(passcode_pattern != 0) {
        for(unsigned int i = 0; i < (passcode_pattern + 1); i++) {
            lightOn(5, 500, 500);
        }
    }
}
#endif

//Locks Chime-In in an infinite sequence until an input is detected
#ifdef IDLE
void idle() {
    while(true) {
        for(unsigned int i = 1; i < 5; i++) {
            lightOn(i, 200, 0);
            if(checkButton()) {
                break;
            }
        }
        if(checkButton()) {
            break;
        }
    }
    lightsOff(5, 0);
    delay(500);
}
#endif

//Compares the inputed colour sequence to the global passcodes
#ifdef PASSCODE_CHECK
void passcodeCheck() {
    unsigned int code;
    bool incorrect_passcode = false;
    unsigned int position = 1;

    do {
        code = 0;
        position = 1;
        //Once interupt
        for (unsigned int k = 0; k < 4; ++k) {
            while (true) {
                if (checkButton() == 1) {
                    code += 1*(position);
                    lightOn(1, 50, 250);
                    break;
                } else if (checkButton() == 2) {
                    code += 2*(position);
                    lightOn(2, 50, 250);
                    break;
                } else if (checkButton() == 3) {
                    code += 3*(position);
                    lightOn(3, 50, 250);
                    break;
                } else if (checkButton() == 4) {
                    code += 4*(position);
                    lightOn(4, 50, 250);
                    break;
                }
            }
            position *= 10;
        }

        if(code == g_easy_passcode) {
            g_difficulty_level = 1;
            pattern(1);
            incorrect_passcode = false;
            break;
        } else if (code == g_moderate_passcode) {
            g_difficulty_level = 2;
            pattern(2);
            incorrect_passcode = false;
            break;
        } else if (code == g_difficult_passcode) {
            g_difficulty_level = 3;
            pattern(3);
            incorrect_passcode = false;
            break;
        } else {
            pattern(4);
            incorrect_passcode = true;
        }
    }
    while(incorrect_passcode);
}
#endif

//Changes the global variables based on the difficulty of the game
#ifdef DIFFICULTY
void difficulty() {
    if(g_difficulty_level == 1) { //Easy
        g_start_amount = 1; //Start off with 1 button flash
        g_sequence_amount = 5; //5 sequences
        g_between_time = 750; //0.75 seconds
        g_wait_time = 2000; //2 seconds
        g_decrement_between_time = 50; //Decrement lowered_between_time for each sequence by 50 miliseconds 
        g_decrement_wait_time = 100; //Decrement lowered_wait_time for each sequence by 100 miliseconds
    } else if(g_difficulty_level == 3) { //Hard
        g_start_amount = 3; //Start off with 3 button flashes
        g_sequence_amount = 10; //10 sequences
        g_between_time = 500; //0.5 seconds
        g_wait_time = 1250; //1.25 seconds
        g_decrement_between_time = 25; //Decrement lowered_between_time for each sequence by 25 miliseconds
        g_decrement_wait_time = 50; //Decrement lowered_wait_time for each sequence by 50 miliseconds
    } else { //Moderate
        g_start_amount = 2; //Start off with 2 button flashes
        g_sequence_amount = 7; //7 sequences
        g_between_time = 500; //0.5 seconds
        g_wait_time = 1500; //1.5 seconds
        g_decrement_between_time = 50; //Decrement lowered_between_time for each sequence by 50 miliseconds
        g_decrement_wait_time = 100; //Decrement lowered_wait_time for each sequence by 100 miliseconds
    }
}
#endif

//Plays colour sequences and waits for corresponding player input
#ifdef GAMEPLAY
void gameplay() {

    //Button 1: Green
    //Button 2: Red
    //Button 3: Yellow
    //Button 4: Blue

    //!!!DON'T NEED??
    //srand((unsigned)time(NULL));
    //To generate a random number between 1 and 4: ((rand()%4)+1)

    //Seed random based on a static analoug input
    randomSeed(analogRead(A0));
    unsigned int sequences_completed, flash_amount, random_light, lowered_between_time,
    lowered_wait_time, time, button_input, sequence_number;
    bool player_lose;
    unsigned int sequence_data[g_sequence_amount + g_start_amount];

    do {
        //Reset all variables
        sequences_completed = 0;
        flash_amount = g_start_amount;
        random_light = 0;
        lowered_between_time = g_between_time;
        lowered_wait_time = g_wait_time;
        time = 0;
        button_input = 0;
        sequence_number = 0;
        player_lose = false;
        for(unsigned int i = 0; i < (g_sequence_amount + g_start_amount); i++) {
        sequence_data[i] = 0;
        }

        //Terminate the do while loop once the user has gone through all g_sequence_amount
        for(unsigned int i = 0; i < g_sequence_amount; ++i) {

            //Flashes the light k amount of times
            if(i == 0) {
                for(unsigned int k = 0; k < g_start_amount; ++k) { 
                    random_light = random(1, 5);
                    sequence_data[k] = random_light;
                    lightOn(random_light, lowered_between_time, 0);
                    //Creates a longer delay after presenting all light flashes (to give the user a buffer period)
                    if(k != flash_amount - 1) {
                        delay(lowered_between_time);
                    } else {
                        delay(250);
                    }
                }
            } else {
                random_light = random(1, 5);
                sequence_data[flash_amount - 1] = random_light;
                for(unsigned int k = 0; k < flash_amount; ++k) {
                    lightOn(sequence_data[k], lowered_between_time, 0);
                    //Creates a longer delay after presenting all light flashes (to give the user a buffer period)
                    if(k != flash_amount - 1) {
                        delay(lowered_between_time);
                    } else {
                        delay(250);
                    }
                }
            }

            time = 0;
            while(time <= lowered_wait_time) {
                time++;

                //Fail the player if they wait to long to press a button
                if(time == lowered_wait_time) {
                    lose();
                    player_lose = true;
                    break;
                }

                if(checkButton() == 1) {
                    lightOn(1, 50, 0);
                    button_input = 1;
                } else if(checkButton() == 2) {
                    lightOn(2, 50, 0);
                    button_input = 2;
                } else if(checkButton() == 3) {
                    lightOn(3, 50, 0);
                    button_input = 3;
                } else if(checkButton() == 4) {
                    lightOn(4, 50, 0);
                    button_input = 4;
                }

                if(checkButton()) {

                    //If the button pressed matches the button in the [INDEX] sequence, the user has inputed a the correct button
                    if(button_input == sequence_data[sequence_number]) {
                        sequence_number++;

                        //If the user has successfully completed all of the button inputs for this flash amount
                        if(sequence_number == flash_amount) {
                            delay(100);
                            pattern(5);
                            break;
                        } else { //If the user has *not* successfully completed all of the button inputs for this flash amount
                            delay(250);

                            button_input = 0;
                            time = 0; //Reset the while loop to wait for the next input
                        }

                    } else { //The user has inputted an incorrect button
                        lose();
                        player_lose = true;
                        break;
                    }
                }
                delay(1);
            }
            
            if(player_lose) {
                break;
            }

            //Increase difficult for each light sequence
            lowered_between_time -= g_decrement_between_time;
            lowered_wait_time -= g_decrement_wait_time;
            sequence_number = 0;
            time = 0;
            flash_amount++;
            sequences_completed++;
        }

        //If the player has successfully completed all of the sequences in g_sequence_amount, break out of do-while loop
        if(sequences_completed == g_sequence_amount) {
            player_lose = false;
            break;
        }
    }
    while(player_lose);

    win();
}
#endif

//Plays lose pattern
#ifdef LOSE
void lose() {
    pattern(7);
    //Return to gameplay which will restart the game
}
#endif

//Plays win sequence and calls deactivate()
#ifdef WIN
void win() {
    lightsOff(5, 0);
    pattern(6);
    delay(1000);
    deactivate();
}
#endif

//Locks game until it is manually reset
#ifdef DEACTIVATE
void deactivate() {
    while(true) {
        if(checkButton()) {
            pattern(8);
        }
    }
}
#endif

//!!!DO NOT NEED FOR NUCLEO
void setup() {
  pinMode(ledGPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  pinMode(ledYPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);

  pinMode(buttonGPin, INPUT_PULLUP);  
  pinMode(buttonRPin, INPUT_PULLUP);  
  pinMode(buttonYPin, INPUT_PULLUP);  
  pinMode(buttonBPin, INPUT_PULLUP);  
  Serial.begin(9600);
}

//Calls all functions
//!!!CHANGE TO main()
#ifdef MAIN
void loop() {
    idle();
    passcodeCheck();
    difficulty();
    gameplay();
}
#endif