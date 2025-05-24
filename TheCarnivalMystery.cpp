// The Carnival Mystery Game
 
// Standgard libraries
#include <iostream> // Input/Output stream
#include <string>   // String manipulation
#include <array>    // Array manipulation
#include <map>      // Map for storing emergency contacts
#include <list>     // List for storing visited places
#include <limits>   // For numeric limits
using namespace std;

// For number guessing game
#include <cstdlib>  // Standard library for random number generation
#include <ctime>    // For seeding the random number generator

// Helper function to convert input to uppercase
string toUpper(const string& str) {
    string result = str;
    for (char& c : result) c = toupper(c);
    return result;
}

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
	array < string, 3 > tokens;        // Tokens collected by the player
	array < string, 3 > clues;         // Clues collected by the player
	int products = 0;                  // Number of products purchased  
	string* playerLocation = &name;    // Pointer to the player's current location

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
		secretNumber = (rand() % (maxRange - minRange + 1)) + minRange; // Generate a random number within the specified range
    }

    // Function to play the game
    void play(Player& player) {
		int guess;   // Variable to store the player's guess
        while (true) {
            cout << "Guess the number (" << minRange << " to " << maxRange << "): ";
            cin >> guess;

			// Validate input
            if (cin.fail() || guess < minRange || guess > maxRange) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between " << minRange << " and " << maxRange << "." << endl;
            }
            else {
                break;
            }
        }

        // Check if the guess is correct
        if (guess == secretNumber) {
            cout << "Congratulations! You guessed the correct number." << endl;
            player.setPoints(player.getPoints() + pointsForWin);  // Add points for winning the game
			player.tokens[0] = "Game - Won";   // Store token for winning the game
        }
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
        do {
            cout << "Do you want to buy a product from " << name << " for " << price << " points? (Y/N)" << endl;
            cin >> userInput;
            userInput = toUpper(userInput);
            
			// Validate input
            if (userInput != "Y" && userInput != "N") {
                cout << "Invalid input. Please enter Y or N: ";
            }
        } while (userInput != "Y" && userInput != "N");

        // Check if the player has enough points to buy the product
        if (userInput == "Y") {

            // If the player has enough points
            if (player.getPoints() >= price) {

                // Deduct the points
                player.setPoints(player.getPoints() - price);
				cout << "You bought a product from " << name << ". " << price << " points have been deducted." << endl;   // Display purchase message
				cout << "You have " << player.getPoints() << " points remaining." << endl;   // Display remaining points
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
 
		else if (player.products == 4) {    // If the player has purchased 4 products
            player.clues[1] = "Clue 2";
            cout << "You have a photo of the thief!" << endl;
        }
		else if (player.products == 5) {    // If the player has purchased 5 products
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

				// If the player has not collected enough products
                cout << "Oh no! Your phone was stolen at the Cotton Candy Booth!" << endl;
				// Ask the player to climb down
                cout << "You had to climb down using a rope you found earlier... what an adventure!" << endl;
            }
            else {
                
				// If the player has collected enough products
                cout << "You manage to call " << contactName << " and they arrive to fix the ride. You're safe!" << endl;
				// Award points for a safe ride
                player.setPoints(player.getPoints() + funPoints); // Award points only if safe
				// Display points gained
                cout << "You gained " << funPoints << " points. You now have " << player.getPoints() << " points." << endl;
            }
        }
        else {
			// If the ride was smooth
            cout << "The ride was smooth and fun!" << endl;
			// Award points for a smooth ride
            player.setPoints(player.getPoints() + funPoints);
			// Display points gained
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
    string howToPlay = "- You will move through different locations, play games, and try to solve the mystery.\n- To win, you must collect the 3 clues and 5 products (must include 3 different products) from the Food stands.\n- You have to use your points to buy products they offer to collect clues.\n- After using points, you should have 100 points remaining finally to find Liza's ring.\n- You have to plan the game to win ahead, because you can't go back.\n";

    // List to keep track of visited places
    list<string> visitedPlaces;

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
    do {
        cin >> userInput;
        userInput = toUpper(userInput);
        if (userInput != "Y" && userInput != "N") {
            cout << "Invalid input. Please enter Y or N: ";
        }
    } while (userInput != "Y" && userInput != "N");

    // If the user wants to play the game
    if (userInput == "Y") {
        cout << "What name do you like to use for your character?" << endl;
        cin >> userName;

        // Display the instructions
        cout << "Hey " << userName << ",\n" << endl;
        cout << "********** How To Play? **********\n\n" << howToPlay << endl;

        // Create a player object
        Player player1;

		player1.setPlayerLocation("Carnival Entrance");   // Set initial location
		visitedPlaces.push_back("Carnival Entrance");     // Add initial location to visited places

        // Display the commands
        cout << "* Type S to start the game" << endl;
        cout << "* Type H to learn how to play the game" << endl;
        do {
            cin >> command;
            command = toUpper(command);
            
			// Validate input
            if (command != "S" && command != "H") {
				// Display error message
                cout << "Invalid input. Please enter S to start or H for help: ";
            }

        } while (command != "S" && command != "H");

        if (command == "S") {
            string playAgain, buyMore, rideAgain;

            // **Step 1: Game Booth 1**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin1.name << " ###" << endl;

			// Set the player's location to the game booth
            player1.setPlayerLocation(guessAndWin1.name);
            visitedPlaces.push_back(guessAndWin1.name);

            // Play the game
            do {

				// Play the guessing game
                guessAndWin1.play(player1);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;

                cout << "Do you want to play again? (Y/N): ";
                do {
                    cin >> playAgain;
                    playAgain = toUpper(playAgain);
                    
					// Validate input
                    if (playAgain != "Y" && playAgain != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }
                } while (playAgain != "Y" && playAgain != "N");

            } while (playAgain == "Y");

            cout << "********** Finish **********\n" << endl;

            // **Step 2: Food Stand 1**
            cout << "********** Start **********" << endl;
            cout << "### " << energyDrinkStand.name << " ###" << endl;

			// Set the player's location to the food stand
            player1.setPlayerLocation(energyDrinkStand.name);
            visitedPlaces.push_back(energyDrinkStand.name);

            do {
				// Buy a product from the food stand
                energyDrinkStand.buyProduct(player1);
                cout << "Do you want to buy one more? (Y/N): ";
                do {
                    cin >> buyMore;
                    buyMore = toUpper(buyMore);

					// Validate input
                    if (buyMore != "Y" && buyMore != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (buyMore != "Y" && buyMore != "N");

            } while (buyMore == "Y" && player1.getPoints() >= energyDrinkStand.price);

            cout << "********** Finish **********\n" << endl;

            // **Step 2.5: Ride 1 - Ferris Wheel**
            cout << "********** Start **********" << endl;
            cout << "### " << ferrisWheel.name << " ###" << endl;

			// Set the player's location to the ride
            player1.setPlayerLocation(ferrisWheel.name);
            visitedPlaces.push_back(ferrisWheel.name);

            do {
                string rideInput;
                cout << "Enter R and Hit enter to ride the " << ferrisWheel.name << ": ";
                do {
                    cin >> rideInput;
                    rideInput = toUpper(rideInput);

					// Validate input
                    if (rideInput != "R") {
                        cout << "Invalid input. Please enter R to ride: ";
                    }

                } while (rideInput != "R");

                ferrisWheel.enjoy(player1, rideEmergencyContacts);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to ride again? (Y/N): ";
                do {
                    cin >> rideAgain;
                    rideAgain = toUpper(rideAgain);
                    
					// Validate input
                    if (rideAgain != "Y" && rideAgain != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (rideAgain != "Y" && rideAgain != "N");

            } while (rideAgain == "Y");

            cout << "********** Finish **********\n" << endl;

            // **Step 3: Game Booth 2**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin2.name << " ###" << endl;
            
			// Set the player's location to the game booth
            player1.setPlayerLocation(guessAndWin2.name);
            visitedPlaces.push_back(guessAndWin2.name);

            do {
                guessAndWin2.play(player1);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to play again? (Y/N): ";
                do {
                    cin >> playAgain;
                    playAgain = toUpper(playAgain);
                    
					// Validate input
                    if (playAgain != "Y" && playAgain != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (playAgain != "Y" && playAgain != "N");

            } while (playAgain == "Y");

            cout << "********** Finish **********\n" << endl;

            // **Step 4: Food Stand 2**
            cout << "********** Start **********" << endl;
            cout << "### " << cookieStand.name << " ###" << endl;
            
			// Set the player's location to the food stand
            player1.setPlayerLocation(cookieStand.name);
            visitedPlaces.push_back(cookieStand.name);

            do {
                cookieStand.buyProduct(player1);
                cout << "Do you want to buy one more? (Y/N): ";
                do {
                    cin >> buyMore;
                    buyMore = toUpper(buyMore);
                    
					// Validate input
                    if (buyMore != "Y" && buyMore != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (buyMore != "Y" && buyMore != "N");

            } while (buyMore == "Y" && player1.getPoints() >= cookieStand.price);

            cout << "********** Finish **********\n" << endl;

            // **Step 4.5: Ride 2 - Merry Go Round**
            cout << "********** Start **********" << endl;
            cout << "### " << merryGoRound.name << " ###" << endl;
            
			// Set the player's location to the ride
            player1.setPlayerLocation(merryGoRound.name);
            visitedPlaces.push_back(merryGoRound.name);

            do {
                string rideInput;
                cout << "Enter R and Hit enter to ride the " << merryGoRound.name << ": ";
                do {
                    cin >> rideInput;
                    rideInput = toUpper(rideInput);

					// Validate input
                    if (rideInput != "R") {
                        cout << "Invalid input. Please enter R to ride: ";
                    }

                } while (rideInput != "R");

                merryGoRound.enjoy(player1, rideEmergencyContacts);

				// Check if the player has enough points to ride again
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to ride again? (Y/N): ";
                do {
                    cin >> rideAgain;
                    rideAgain = toUpper(rideAgain);
                    
                    if (rideAgain != "Y" && rideAgain != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (rideAgain != "Y" && rideAgain != "N");

            } while (rideAgain == "Y");

            cout << "********** Finish **********\n" << endl;

            // **Step 5: Game Booth 3**
            cout << "********** Start **********" << endl;
            cout << "### " << guessAndWin3.name << " ###" << endl;
            
			// Set the player's location to the game booth
            player1.setPlayerLocation(guessAndWin3.name);
            visitedPlaces.push_back(guessAndWin3.name);

            do {
                guessAndWin3.play(player1);
                cout << "You have " << player1.getPoints() << " points remaining." << endl;
                cout << "Do you want to play again? (Y/N): ";
                do {
                    cin >> playAgain;
                    playAgain = toUpper(playAgain);
                    
					// Validate input
                    if (playAgain != "Y" && playAgain != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (playAgain != "Y" && playAgain != "N");

            } while (playAgain == "Y");

            cout << "********** Finish **********\n" << endl;

            // **Step 6: Food Stand 3**
            cout << "********** Start **********" << endl;
            cout << "### " << iceCreamStand.name << " ###" << endl;
            
			// Set the player's location to the food stand
            player1.setPlayerLocation(iceCreamStand.name);
            visitedPlaces.push_back(iceCreamStand.name);

            do {
                iceCreamStand.buyProduct(player1);
                cout << "Do you want to buy one more? (Y/N): ";
                do {
                    cin >> buyMore;
                    buyMore = toUpper(buyMore);
                    
					// Validate input
                    if (buyMore != "Y" && buyMore != "N") {
                        cout << "Invalid input. Please enter Y or N: ";
                    }

                } while (buyMore != "Y" && buyMore != "N");

            } while (buyMore == "Y" && player1.getPoints() >= iceCreamStand.price);

            cout << "********** Finish **********\n" << endl;

            // **Step 7: Finish the game**
            if (player1.getPoints() >= 100 && player1.products == 5 && player1.clues[2] == "Clue 3") {

				// If the player has enough points and has collected all clues
                cout << "Congratulations! You have found all the clues, collected all 3 products and have more than 100 points remaining!" << endl;
                cout << "You have successfully solved the mystery and found Liza's ring!" << endl;
                
				// Set the player's location to the carnival entrance with Liza
                player1.isFound = 1;
                player1.setPlayerLocation("Carnival Entrance with Liza");
            }
            else {
                cout << "You have not collected all the clues and products yet!" << endl;
            }

            // Display all places visited
            cout << "\nPlaces you visited during your adventure:" << endl;
            for (const auto& place : visitedPlaces) {
                cout << "- " << place << endl;
            }
            cout << "Thank you, " << userName << ". I'm Liza here. Appreciate your help.\n!!!LOVE YOU!!!" << endl;
        }
        else if (command == "H") {
            cout << howToPlay;
        }
    }
    else {
        // Display all places visited
        cout << "\nPlaces you visited during your adventure:" << endl;
        for (const auto& place : visitedPlaces) {
            cout << "- " << place << endl;
        }
        cout << "Sad to see you go! Come back later . . .";
    }
}

// End of the game
