#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <limits>
#include <fstream> // For file output
using namespace std;

int money = 0;
int wager = 0;
bool playerLoses = false;
int total = 0;
int houseTotal = 0;
bool houseTurnActive = true;
bool isStand = false;
bool computerLoses = false;

// Strings to keep track of cards as they are rolled
string playerHand = ""; 
string houseHand = ""; 

ofstream outFile("blackjack_output.txt"); // File to write game output

// Function to simulate dice roll
int roll() {
    int roll = rand() % 13 + 2; // Values between 2 and 14
    if (roll >= 11 && roll <= 13) return 10; // Jack, Queen, King = 10
    if (roll == 14) return 11; // Ace = 11
    return roll;
}

// Function to output text to both console and file
void output(const string &text) {
    cout << text << endl;
    outFile << text << endl;
}

// Function that handles the player's rolling behavior
void playerRoll(); // Declare the playerRoll function here

void houseTurn() {
    while (houseTotal < 17) {
        int rollValue = roll();
        houseHand += to_string(rollValue) + " "; // Append the card to house hand string
        houseTotal += rollValue;
        output("House rolled a " + to_string(rollValue));
    }
    output("House's final total is " + to_string(houseTotal));
    
    if (houseTotal > 21) {
        output("House Bust! Player wins.");
        money += wager * 2;
        output("Your new balance is $" + to_string(money));
        computerLoses = true;
    } else {
        // Compare totals
        if (houseTotal > total) {
            output("House wins. You lose.");
            playerLoses = true;
            money -= wager;
            output("Your new balance is $" + to_string(money));
        } else if (houseTotal < total) {
            output("You win!");
            money += wager * 2;
            output("Your new balance is $" + to_string(money));
        } else {
            output("It's a tie!");
            output("You still have $" + to_string(money));
        }
    }
}

void playerHitOrStand() {
    char choice;
    do {
        output("Would you like to hit (h) or stand (s)? ");
        cin >> choice;
        choice = tolower(choice);
        if (choice != 'h' && choice != 's') {
            output("Invalid input! Please enter 'h' to hit or 's' to stand.");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 'h' && choice != 's');

    if (choice == 'h') {
        output("Player chooses to hit.");
        playerRoll();  // Call the playerRoll function
    } else {
        output("Player chooses to stand.");
        isStand = true;
        houseTurn();
    }
}

void playerRoll() {  // Define the playerRoll function here
    if (total == 0) {  // Only happens for the first roll
        // Roll two cards for the player initially
        int roll1 = roll();
        int roll2 = roll();
        playerHand += to_string(roll1) + " " + to_string(roll2) + " "; // Append both rolls to the hand string
        total = roll1 + roll2;  // Add both rolls to the player's total
        output("You rolled " + to_string(roll1) + " and " + to_string(roll2));
        output("Your total is " + to_string(total));

        if (total == 21) {
            output("Blackjack! You win!");
            money += wager * 2;
            output("Your new balance is $" + to_string(money));
            return;
        }
    } else {
        // For subsequent rolls (when the player chooses to hit)
        int rollValue = roll();
        playerHand += to_string(rollValue) + " "; // Append the new card to player's hand string
        total += rollValue;
        output("You rolled a " + to_string(rollValue));
        output("Your total is " + to_string(total));
    }

    if (total > 21) {
        output("Player Bust. You lose.");
        playerLoses = true;
        money -= wager;
        output("Your new balance is $" + to_string(money));
    } else if (total == 21 || isStand) {
        output("Player stands with a total of " + to_string(total));
        houseTurn();
    } else {
        playerHitOrStand();
    }
}

void play() {
    playerHand = ""; // Reset the player's hand
    houseHand = ""; // Reset the house's hand
    total = 0;
    houseTotal = 0;
    houseTurnActive = true;
    isStand = false;

        do {
        output("How much do you want to wager?");
        cin >> wager;
        if (cin.fail() || wager > money || wager <= 0) {
            output("Invalid wager! Please enter a positive amount not exceeding your balance.");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (wager > money || wager <= 0);

        // Start with house's first roll
    int houseRoll1 = roll();
    houseHand += to_string(houseRoll1) + " "; // Append the roll to house's hand string
    houseTotal += houseRoll1;
    output("House's first roll is " + to_string(houseRoll1) + " (Total: " + to_string(houseTotal) + ")");
    
    playerRoll();


    // Output final results
    output("Player's hand: " + playerHand + " (Total: " + to_string(total) + ")");
    output("House's hand: " + houseHand + " (Total: " + to_string(houseTotal) + ")");
}

void moneyInput() {
    do {
        output("How much money do you want to spend gambling?");
        cin >> money;
        if (cin.fail() || money <= 0) {
            output("Invalid input! Please enter a valid amount.");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (money <= 0);
    
    play();
}

int main() {
    srand(time(0)); // Initialize random seed
    moneyInput();

    do {
        output("Do you want to play again? (y/n)");
        char choice;
        cin >> choice;
        choice = tolower(choice);

        if (choice == 'y') {
            play();
        } else if (choice == 'n') {
            output("Thanks for playing!");
            break;
        }

    } while (money > 0);

    if (money <= 0) {
        output("You have no more money to gamble with!");
    }

    outFile.close(); // Close the file when done
    return 0;
}
