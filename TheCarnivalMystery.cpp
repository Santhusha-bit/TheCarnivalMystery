// TheCarnivalMystery.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Standgard libraries
#include <iostream>
#include <string>
#include <array>
#include <map>
using namespace std;

// For number gusssing game
#include <cstdlib>
#include <ctime>

class Player {
public:
    // Default constructor
    Player() {
        string story = "********** Story **********\n\n- Liza is a girl who came to a carnival.\n- Liza's ring is missing.\n- Rumor has it that Liza's ring has a very expensive diamond.\n- The thief is believed to be hiding out somewhere at the carnival.\n- Liza has no clue who the thief is.\n- The object of the game is to find Liza's ring and the thief before he has a chance to leave the carnival.";
        points = 50; // Initial points
        string welcomeMsg = "Welcome to Liza Mystery!"; // Welcome message
        string thankMsg = "Thank you for coming to help Liza!"; // Thank you message
        cout << welcomeMsg << endl << endl << story << endl << endl << thankMsg << endl; // Display welcome message
    }

    // Basic Characteristics fo the player
    string name;
    int points;
    bool isFound = 0;

    //Storing tokens and clues that the player collects in an array
    array < string, 3 > tokens;
    array < string, 3 > clues;
    int products = 0;
    string* playerLocation = &name;

    // Setters
    void setPoints(int p) {
        points = p;
    }

    void setPlayerLocation(string l) {
        *playerLocation = l;
    }

    // Getters
    int getPoints() {
        return points;
    }

    // Getters
    string getPlayerLocation() {
        return *playerLocation;
    }
};

// Base class for locations
class TheLocation {
public:
    string type;
    string name;
};

// Derived class from TheLocation class
class GameBooth : public TheLocation {
private:
    // Game variables
    int secretNumber;
    int minRange;
    int maxRange;
    int pointsForWin;  // Points earned for winning this game

public:
    string type = "Game Booth";

    // Constructor that accepts min, max range, and points for winning
    GameBooth(int minR, int maxR, int points) : minRange(minR), maxRange(maxR), pointsForWin(points) {
        srand(time(0)); // Seed random number generator
        secretNumber = (rand() % (maxRange - minRange + 1)) + minRange;
    }

    // Function to play the game
    void play(Player& player) {
        int guess;
        cout << "Guess the number (" << minRange << " to " << maxRange << "): ";
        cin >> guess;

        // Check if the guess is correct
        if (guess == secretNumber) {
            // If the guess is correct
            cout << "Congratulations! You guessed the correct number." << endl;
            player.setPoints(player.getPoints() + pointsForWin);  // Add points for winning the game
            player.tokens[0] = "Game - Won";
        }

        // If the guess is wrong
        else {
            cout << "Sorry, Try again!" << endl;
        }
    }
};


// Derived class from TheLocation class
class FoodStand : public TheLocation {
public:
    string type = "Food Stand";
    int price; // Price of the product

    // Constructor to set the price
    FoodStand(string standName, int standPrice) {
        name = standName;
        price = standPrice;
    }

    // Function to buy a product
    void buyProduct(Player& player) {
        string userInput;
        cout << "Do you want to buy a product from " << name << " for " << price << " points? (Y/N)" << endl;
        cin >> userInput;

        // Check if the player has enough points to buy the product
        if (userInput == "Y") {

            // If the player has enough points
            if (player.getPoints() >= price) {

                // Deduct the points
                player.setPoints(player.getPoints() - price);
                cout << "You bought a product from " << name << ". " << price << " points have been deducted." << endl;
                cout << "You have " << player.getPoints() << " points remaining." << endl;
                player.products++; // Increase product count
                updateClues(player); // Update clues based on the number of products purchased
            }
            else {
                cout << "You don't have enough points to buy a product from " << name << "!" << endl;
            }
        }
    }

    // Function to update clues based on the number of products purchased
    void updateClues(Player& player) {

        // Update clues based on the number of products purchased
        if (player.products == 2) {
            player.clues[0] = "Clue 1";
            cout << "You have the thief's Name!" << endl;
        }

        else if (player.products == 4) {
            player.clues[1] = "Clue 2";
            cout << "You have a photo of the thief!" << endl;
        }
        else if (player.products == 5) {
            player.clues[2] = "Clue 3";
            cout << "You know where he is hiding at!" << endl;
        }
    }
};

// Derived class from TheLocation class
class Ride : public TheLocation {
public:
    string type = "Ride";
    int funPoints; // Points awarded for experiencing the ride

    // Constructor to set ride name and fun points
    Ride(string rideName, int points) {
        name = rideName;
        funPoints = points;
    }

    // Function to enjoy the ride
    void enjoy(Player& player, map<string, string>& contacts) {
        cout << "You are enjoying the " << name << "!" << endl;

        // Random chance for ride to break down
        if (rand() % 4 == 0) { // 25% chance
            cout << "Uh-oh! The " << name << " broke down!" << endl;
            string contactName = contacts[name];
            cout << "You remember you're supposed to contact: " << contactName << "." << endl;

            // Simulate issues
            if (player.products < 2) {
                cout << "Oh no! Your phone was stolen at the Cotton Candy Booth!" << endl;
                cout << "You had to climb down using a rope you found earlier... what an adventure!" << endl;
            }
            else {
                cout << "You manage to call " << contactName << " and they arrive to fix the ride. You're safe!" << endl;
                player.setPoints(player.getPoints() + funPoints); // Award points only if safe
                cout << "You gained " << funPoints << " points. You now have " << player.getPoints() << " points." << endl;
            }
        }
        else {
            cout << "The ride was smooth and fun!" << endl;
            player.setPoints(player.getPoints() + funPoints);
            cout << "You gained " << funPoints << " points. You now have " << player.getPoints() << " points." << endl;
        }
    }

};



int main()
{
    // Variables
    string userInput;
    string userName;
    string command;
    string howToPlay = "- You will move through different locations, play games, and try to solve the mystery.\n-To win, you must collect the 3 clues and 5 products (must include 3 different products) from the Food stands.\n- You have to use your points to buy products they offer to collect clues.\n- After using points, you should have 100 points remaining finally to find Liza's ring.\n- You have to plan the game to win ahead, because you can't go back.\n";

	// Emergency contacts for rides
    map<string, string> rideEmergencyContacts = {
    {"Ferris Wheel", "Sam - The Mechanic"},
    {"Merry Go Round", "Anna - The Operator"}
    };

    // Game Booths
    GameBooth guessAndWin1(1, 2, 30);
    guessAndWin1.name = "Guess n Win (1-2)";

    GameBooth guessAndWin2(1, 3, 40);
    guessAndWin2.name = "Guess n Win (1-3)";

    GameBooth guessAndWin3(1, 5, 50);
    guessAndWin3.name = "Guess n Win (1-5)";

    // Food Stands
    FoodStand energyDrinkStand("Energy Drink Stand", 30);

    FoodStand cookieStand("Cookie Stand", 50);

    FoodStand iceCreamStand("Ice Cream Stand", 40);

    // Rides
    Ride ferrisWheel("Ferris Wheel", 10);

    Ride merryGoRound("Merry Go Round", 15);

    // Ask the user to start the game
    cout << "Do you want to play the game? (Y/N)" << endl;
    cin >> userInput;

    // If the user wants to play the game
    if (userInput == "Y") {
        cout << "What name do you like to use for your character?" << endl;
        cin >> userName;

        // Display the instructions
        cout << "Hey " << userName << ",\n" << endl;
        cout << "********** How To Play? **********\n\n" << howToPlay << endl;

        // Create a player object
        Player player1;
        player1.setPlayerLocation("Carnival Entrance");

        // Display the commands
        cout << "* Type >S to start the game" << endl;
        cout << "* Type >H to learn how to play the game" << endl;
        cin >> command;

        if (command == "S") {
            string playAgain, buyMore, rideAgain;

            // **Step 1: Game Booth 1**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin1.name << " ###" << endl;
            player1.setPlayerLocation(guessAndWin1.name);

            // Play the game
            do {

                guessAndWin1.play(player1);
                // Display the points
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                // Ask the user if they want to play again
                cout << "Do you want to play again? (Y/N): ";
                // Get the user input
                cin >> playAgain;
                // Check if the user wants to play again
            } while (playAgain == "Y");

            cout << "********** Finish **********" << endl;

            // **Step 2: Food Stand 1**
            cout << "********** Start **********" << endl;
            cout << "### " << energyDrinkStand.name << " ###" << endl;

            // Set the player location
            player1.setPlayerLocation(energyDrinkStand.name);

            // Buy a product
            do {
                energyDrinkStand.buyProduct(player1);

                // Ask the user if they want to buy more
                cout << "Do you want to buy one more? (Y/N): ";

                // Get the user input
                cin >> buyMore;

                // Check if the user wants to buy more
            } while (buyMore == "Y" && player1.getPoints() >= energyDrinkStand.price);

            cout << "********** Finish **********" << endl;

            // **Step 2.5: Ride 1 - Ferris Wheel**
            cout << "********** Start **********" << endl;
            cout << "### " << ferrisWheel.name << " ###" << endl;

            // Set the player location
            player1.setPlayerLocation(ferrisWheel.name);

            // Go for ride
            do {

                ferrisWheel.enjoy(player1, rideEmergencyContacts);
                // Display the points
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                // Ask the user if they want to ride again
                cout << "Do you want to ride again? (Y/N): ";
                // Get the user input
                cin >> rideAgain;
                // Check if the user wants to ride again
            } while (rideAgain == "Y");

            cout << "********** Finish **********" << endl;

            // **Step 3: Game Booth 2**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin2.name << " ###" << endl;
            player1.setPlayerLocation(guessAndWin2.name);

            do {
                guessAndWin2.play(player1);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to play again? (Y/N): ";
                cin >> playAgain;
            } while (playAgain == "Y");

            cout << "********** Finish **********" << endl;

            // **Step 4: Food Stand 2**
            cout << "********** Start **********" << endl;
            cout << "### " << cookieStand.name << " ###" << endl;
            player1.setPlayerLocation(cookieStand.name);

            do {
                cookieStand.buyProduct(player1);
                cout << "Do you want to buy one more? (Y/N): ";
                cin >> buyMore;
            } while (buyMore == "Y" && player1.getPoints() >= cookieStand.price);

            cout << "********** Finish **********" << endl;

            // **Step 4.5: Ride 2 - Merry Go Round**
            cout << "********** Start **********" << endl;
            cout << "### " << merryGoRound.name << " ###" << endl;

            // Set the player location
            player1.setPlayerLocation(merryGoRound.name);

            // Go for ride
            do {

                merryGoRound.enjoy(player1, rideEmergencyContacts);
                // Display the points
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                // Ask the user if they want to ride again
                cout << "Do you want to ride again? (Y/N): ";
                // Get the user input
                cin >> rideAgain;
                // Check if the user wants to ride again
            } while (rideAgain == "Y");

            cout << "********** Finish **********" << endl;

            // **Step 5: Game Booth 3**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin3.name << " ###" << endl;
            player1.setPlayerLocation(guessAndWin3.name);

            do {
                guessAndWin3.play(player1);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to play again? (Y/N): ";
                cin >> playAgain;
            } while (playAgain == "Y");

            cout << "********** Finish **********" << endl;

            // **Step 6: Food Stand 3**
            cout << "********** Start **********" << endl;
            cout << "### " << iceCreamStand.name << " ###" << endl;
            player1.setPlayerLocation(iceCreamStand.name);

            do {
                iceCreamStand.buyProduct(player1);
                cout << "Do you want to buy one more? (Y/N): ";
                cin >> buyMore;
            } while (buyMore == "Y" && player1.getPoints() >= iceCreamStand.price);

            cout << "********** Finish **********" << endl;

            // **Step 7: Finish the game**
            if (player1.getPoints() >= 100 && player1.products == 5 && player1.clues[2] == "Clue 3") {
                // If the player has more than 100 points and has collected all the clues and products
                cout << "Congratulations! You have found all the clues, collected all 3 products and have more than 100 points remaining!" << endl;

                // Display the final message
                cout << "You have successfully solved the mystery and found Liza's ring!" << endl;

                // Set the player as found
                player1.isFound = 1;

                // Set the player location
                player1.setPlayerLocation("Carnival Entrance with Liza");
            }
            else {
                // If the player has not collected all the clues and products
                cout << "You have not collected all the clues and products yet!" << endl;
            }

            // Display the final message from Liza
            cout << "Thank you, " << userName << ". I'm Liza here. Appreciate your help.\n!!!LOVE YOU!!!" << endl;
        }
        // Display the Help instructions
        else if (command == "H") {
            cout << howToPlay;
        }
    }
    else {
        // If the user does not want to play the game
        cout << "Sad to see you go! Come back later . . .";
    }
}

// End of the game
