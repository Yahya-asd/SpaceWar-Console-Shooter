#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include"help.h"
#include<string>
#include<cstring>
#include <fstream>
using namespace std;

int toggle = 1;// for the pause game
const int SCREEN_WIDTH = 1900;
const int SCREEN_HEIGHT = 1110;
const int MAX_BULLETS = 15;
const int MAX_ALIENS = 5000;
const int BULLET_SPEED = 20;
using namespace std;

void saveState(
    int GRID_ROWS, int GRID_COLS,int ship_x, int ship_y,
    int bullet_x[], int bullet_y[], bool bulletActive[],
    int alien_x[][10], int alien_y[][10], bool alienActive[][10],
    int gridOffsetX, int gridOffsetY, int verticalDirection, int score, int lives
) {
    
    std::ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        // Save ship position
        saveFile << ship_x << " " << ship_y << "\n";

        // Save bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            saveFile << bullet_x[i] << " " << bullet_y[i] << " " << bulletActive[i] << "\n";
        }

        // Save alien grid
        for (int row = 0; row < GRID_ROWS; row++) {
            for (int col = 0; col < GRID_COLS; col++) {
                saveFile << alien_x[row][col] << " " << alien_y[row][col] << " " << alienActive[row][col] << "\n";
            }
        }

        // Save grid and game variables
        saveFile << gridOffsetX << " " << gridOffsetY << " " << verticalDirection << "\n";
        saveFile << score << " " << lives << "\n";

        saveFile.close();
        std::cout << "Game saved successfully!\n";
    }
    else {
        std::cerr << "Error saving game state.\n";
    }
}

void continueGame(
    int GRID_ROWS, int GRID_COLS, int& ship_x, int& ship_y,
    int bullet_x[], int bullet_y[], bool bulletActive[],
    int alien_x[][10], int alien_y[][10], bool alienActive[][10],
    int& gridOffsetX, int& gridOffsetY, int& verticalDirection, int& score, int& lives
) {
    std::ifstream loadFile("savegame.txt");
    if (loadFile.is_open()) {
        // Load ship position
        loadFile >> ship_x >> ship_y;

        // Load bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            loadFile >> bullet_x[i] >> bullet_y[i] >> bulletActive[i];
        }

        // Load alien grid
        for (int row = 0; row < GRID_ROWS; row++) {
            for (int col = 0; col < GRID_COLS; col++) {
                loadFile >> alien_x[row][col] >> alien_y[row][col] >> alienActive[row][col];
            }
        }

        // Load grid and game variables
        loadFile >> gridOffsetX >> gridOffsetY >> verticalDirection;
        loadFile >> score >> lives;

        loadFile.close();
        std::cout << "Game loaded successfully!\n";
    }
    else {
        std::cerr << "Error loading game state.\n";
    }
}


// New function to replace system("cls")

// Add a function to check if all aliens are dead
bool areAllAliensDead(bool alienActive[][10], int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (alienActive[row][col]) {
                return false;  // If any alien is still active, return false
            }
        }
    }
    return true;  // All aliens are dead
}
// Function to store and update high scores
void updateHighScores(int Score) {
    const int maxScores = 5;
    int scores[maxScores + 1] = { 0 }; // Array to hold up to 5 scores + 1 for the new score
    int count = 0;

    // Read existing scores from the file
    ifstream inputFile("highscores.txt");
    while (inputFile >> scores[count] && count < maxScores) {
        count++;
    }
    inputFile.close();

    // Add the new score
    scores[count] = Score;
    count++;

    // Sort scores in descending order using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                // Swap scores[j] and scores[j+1]
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // Write the top 5 scores back to the file
    ofstream outputFile("highscores.txt");
    for (int i = 0; i < maxScores && i < count; i++) {
        outputFile << scores[i] << endl;
    }
    outputFile.close();
}
void displayHighScores() {
    const int maxScores = 5;
    int scores[maxScores] = { 0 };
    int count = 0;

    // Read existing scores from the file
    ifstream inputFile("highscores.txt");

    // Clear the array first
    memset(scores, 0, sizeof(scores));// memset to clear the array first//

    // Read scores from the file
    while (!inputFile.eof() && count < maxScores) {
        if (inputFile >> scores[count]) {
            count++;
        }
    }
    inputFile.close();

    // Display high scores
    cout << "===== HIGH SCORES =====" << endl;
    for (int i = 0; i < maxScores; i++) {
        cout << (i + 1) << ". " << scores[i] << endl;
    }

    cout << "======================" << endl;
    Sleep(5000);

}
// Drawing functions
void Grid(int x, int y) {
    myRect(0, 0, x, y, 0, 0, 0, 0, 0, 0);
}

void startmenu() {
	drawText(70, 550, 120, 255, 255, 0, "SPACEWAR GAME");
	drawText(30, 550, 370, 255, 0, 255, " PRESS A FOR  ARCADE   MODE ");
	drawText(30, 550, 450, 255, 0, 255, " PRESS H FOR  HARD(main) MODE ");
    drawText(30, 550, 530, 255, 0, 255, " PRESS M FOR  MEDIUM   MODE ");
    drawText(30, 550, 610, 255, 0, 255, " PRESS E FOR  EASY     MODE ");
	drawText(30, 550, 690, 255, 0, 255, " PRESS L TO VIEW HIGH SCORES ");
}

void Ship(int x, int y, int z) {
    myRect(x, y, x + 75, y + 50, 0, 0, z, 0, 0, z);
}

void Bullet(int x, int y, int z) {
    myRect(x, y, x + 50, y + 5, z, 0, 0, 0, 0, 0);
}

void Alien(int x, int y) {
    myEllipse(x, y, x - 60, y - 60, 0, 0, 0, 255, 0, 0);
}

void drawScore(int score, int lives) {
    // Draw "SCORE:" in white
    drawText(20, 10, 20, 255, 255, 255, "SCORE:");

    // Draw score number in green
    drawText(20, 10 * 15, 20, 0, 255, 0, "", true, score);

    // Draw "Lives:" in white
    drawText(20, 230, 20, 255, 255, 255, "LIVES:");

    // Draw lives number in green
    drawText(20, 230 + 7 * 20, 20, 0, 255, 0, "", true, lives);
}
void Gameovermessage(int score) {

    updateHighScores(score);
    drawText(100, 100, 200, 255, 255, 0, "Game over!!");
    drawText(50, 250, 450, 255, 255, 0, "Game will be restarting  in 5 seconds");
    Sleep(5000);

}



// Arcade Game Logic
void PlayArcadeGame() {
    const int ALIEN_SPEED = 25;
    int ship_x = 20, ship_y = SCREEN_HEIGHT / 2;
    int bullet_x[MAX_BULLETS] = { 0 };
    int bullet_y[MAX_BULLETS] = { 0 };
    bool bulletActive[MAX_BULLETS] = { false };

    int alien_x[MAX_ALIENS];
    int alien_y[MAX_ALIENS];
    bool alienActive[MAX_ALIENS] = { false };
    int directionToggle[MAX_ALIENS] = { 1 }; // 1 for down, -1 for up
    int activeAliens = 0;
    int lives = 3;
    int score = 0; // Initialize score
    srand(static_cast<unsigned>(time(0)));

    while (true) {
        Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
        drawScore(score, lives); // Display the score

        // Spawn aliens
        if (activeAliens < MAX_ALIENS && rand() % 20 == 0)//random spawn timer with srand 
        {
            int newAlienY = 100 + rand() % (SCREEN_HEIGHT - 200);
            alien_x[activeAliens] = SCREEN_WIDTH;
            alien_y[activeAliens] = newAlienY;
            alienActive[activeAliens] = true;
            activeAliens++;
        }

        // Move and draw aliens
        for (int i = 0; i < activeAliens; i++) {
            if (alienActive[i]) {
                Alien(alien_x[i], alien_y[i]);
                alien_x[i] -= ALIEN_SPEED;

                // Alternate vertical direction
                if (directionToggle[i] == 1) {
                    alien_y[i] += 10;
                    directionToggle[i] = -1;
                }
                else {
                    alien_y[i] -= 10;
                    directionToggle[i] = 1;
                }

                // Check bounds
                if (alien_y[i] <= 50) {
                    directionToggle[i] = 1;
                    alien_y[i] = 50;
                }
                else if (alien_y[i] >= SCREEN_HEIGHT - 50) {
                    directionToggle[i] = -1;
                    alien_y[i] = SCREEN_HEIGHT - 50;
                }

                // Check Game Over condition
                if (alien_x[i] <= 0) {
                    Gameovermessage(score);
                    return;
                }
            }
        }

        // Handle bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                Bullet(bullet_x[i], bullet_y[i], 0);
                bullet_x[i] += BULLET_SPEED;
                if (bullet_x[i] >= SCREEN_WIDTH) {
                    bulletActive[i] = false;
                }
                else {
                    Bullet(bullet_x[i], bullet_y[i], 255);
                }
            }
        }

        // Collision detection(BULLET AND ALIENS)
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                for (int j = 0; j < activeAliens; j++) {
                    if (alienActive[j] &&
                        bullet_x[i] >= alien_x[j] - 60 &&
                        bullet_x[i] <= alien_x[j] &&
                        bullet_y[i] >= alien_y[j] - 60 &&
                        bullet_y[i] <= alien_y[j]) {
                        alienActive[j] = false;
                        bulletActive[i] = false;
                        score += 15; // Increment score on hit
                        break;
                    }
                }
            }
        }
        // collision check for ship and aliens

        for (int j = 0; j < activeAliens; j++) {
            if (alienActive[j] &&
                ship_x >= alien_x[j] - 60 &&
                ship_x <= alien_x[j] &&
                ship_y >= alien_y[j] - 60 &&
                ship_y <= alien_y[j]) {
                alienActive[j] = false;
                lives -= 1; // decrement score on hit
                if (lives == 0) {
                    Gameovermessage(score);
                    drawScore(score, lives);// LATEST LIVES
                    return;
                }
                break;
            }
        }




        // Display ship
        Ship(ship_x, ship_y, 255);

        // Player input
        int whichKey;
        if (isKeyPressed(whichKey)) {
            switch (whichKey) {
            case 1: // Left
                if (ship_x > 20) ship_x -= 30;
                break;
            case 2: // Up
                if (ship_y > 20) ship_y -= 30;
                break;
            case 3: // Right
                if (ship_x < SCREEN_WIDTH - 95) ship_x += 30;
                break;
            case 4: // Down
                if (ship_y < SCREEN_HEIGHT - 70) ship_y += 30;
                break;
            case 5: // Fire bullet
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        bullet_x[i] = ship_x + 75;
                        bullet_y[i] = ship_y + 25;
                        bulletActive[i] = true;
                        break;
                    }
                }
                break;
            case 6:

             return;

            case 7:
                toggle = toggle * (-1);
                while (toggle == -1) {
                    if (isKeyPressed(whichKey)) {
                        switch (whichKey) {
                        case 7:
                            toggle = toggle * -1;
                            break;
                        }
                    }

                }

            }


        }

        Sleep(50); // Game speed
    }
}

// Main Game Logic
void PlayHardGame() {
    const int ALIEN_SPEED = 8;
    int ship_x = 20, ship_y = SCREEN_HEIGHT / 2;
    int bullet_x[MAX_BULLETS] = { 0 };
    int bullet_y[MAX_BULLETS] = { 0 };
    bool bulletActive[MAX_BULLETS] = { false };

    const int GRID_ROWS = 8;
    const int GRID_COLS = 10;
    const int ALIEN_SIZE = 60;
    const int ALIEN_SPACING = 50;

    int alien_x[GRID_ROWS][GRID_COLS];
    int alien_y[GRID_ROWS][GRID_COLS];
    bool alienActive[GRID_ROWS][GRID_COLS];

    int gridOffsetX = SCREEN_WIDTH - 300;
    int gridOffsetY = 100;
    int verticalDirection = 1;

    int score = 0; // Initialize score
    int lives = 3; // initialize lives

    // Initialize alien grid
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            alien_x[row][col] = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
            alien_y[row][col] = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);
            alienActive[row][col] = true;
        }
    }

    int lastFrameTime = GetTickCount64();
    int frameDelay = 1000 / 60;  // Target 60 FPS

    while (true) {
        int currentTime = GetTickCount64();// to avoid blinking   ,
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;

            system("cls");


            // Move aliens
            gridOffsetX -= ALIEN_SPEED;
            gridOffsetY += verticalDirection * 2;

            if (gridOffsetY <= 50 || gridOffsetY + GRID_ROWS * (ALIEN_SIZE + ALIEN_SPACING) >= SCREEN_HEIGHT) {
                verticalDirection *= -1;
            }

            // Draw aliens and check bounds
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (alienPosX <= 0) {
                            Gameovermessage(score);
                            return;
                        }

                        Alien(alienPosX, alienPosY);  // Draw the alien
                    }
                }
            }

            // Handle bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    Bullet(bullet_x[i], bullet_y[i], 255);  // Draw active bullet
                    bullet_x[i] += BULLET_SPEED;

                    if (bullet_x[i] >= SCREEN_WIDTH) {
                        bulletActive[i] = false;
                    }
                }
            }

            // Collision detection: bullets and aliens
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    for (int row = 0; row < GRID_ROWS; row++) {
                        for (int col = 0; col < GRID_COLS; col++) {
                            if (alienActive[row][col]) {
                                int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                                int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                                if (bullet_x[i] >= alienPosX - ALIEN_SIZE &&
                                    bullet_x[i] <= alienPosX &&
                                    bullet_y[i] >= alienPosY - ALIEN_SIZE &&
                                    bullet_y[i] <= alienPosY) {
                                    alienActive[row][col] = false;
                                    bulletActive[i] = false;
                                    score += 1; // Increment score on hit
                                    // game over if any conditons matches for winning 
                                    if (lives == 0 || areAllAliensDead(alienActive, GRID_ROWS, GRID_COLS)) {
                                        Gameovermessage(score);
                                        return;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Collision detection: ship and aliens
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (ship_x >= alienPosX - ALIEN_SIZE &&
                            ship_x <= alienPosX &&
                            ship_y >= alienPosY - ALIEN_SIZE &&
                            ship_y <= alienPosY) {
                            alienActive[row][col] = false;
                            lives -= 1; // Decrement lives on hit

                            if (lives == 0) {
                                Gameovermessage(score);
                                return;
                            }
                            break;
                        }
                    }
                }
            }

            drawScore(score, lives);  // Display the score and lives
            Ship(ship_x, ship_y, 255);  // Draw the ship

            // Player input
            int whichKey;
            if (isKeyPressed(whichKey)) {
                switch (whichKey) {
                case 1: // Left
                    if (ship_x > 20) ship_x -= 30;
                    break;
                case 2: // Up
                    if (ship_y > 20) ship_y -= 30;
                    break;
                case 3: // Right
                    if (ship_x < SCREEN_WIDTH - 95) ship_x += 30;
                    break;
                case 4: // Down
                    if (ship_y < SCREEN_HEIGHT - 70) ship_y += 30;
                    break;
                case 5: // Fire bullet
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bulletActive[i]) {
                            bullet_x[i] = ship_x + 75;
                            bullet_y[i] = ship_y + 25;
                            bulletActive[i] = true;
                            break;
                        }
                    }
                    break;
                case 6:saveState(
                    GRID_ROWS, GRID_COLS,  // Number of rows and columns for aliens
                    ship_x, ship_y,        // Player ship position
                    bullet_x, bullet_y, bulletActive,  // Bullet arrays
                    alien_x, alien_y, alienActive,     // Alien arrays
                    gridOffsetX, gridOffsetY, verticalDirection,  // Grid-related variables
                    score, lives           // Game variables
                );

                    break;

                case 7: // Toggle mode
                    toggle = toggle * (-1);
                    while (toggle == -1) {
                        if (isKeyPressed(whichKey)) {
                            if (whichKey == 7) {
                                toggle = toggle * (-1);
                            }
                        }
                    }
                    break;
                    case 8:
                        continueGame(GRID_ROWS, GRID_COLS, ship_x, ship_y, bullet_x, bullet_y, bulletActive,
                            alien_x, alien_y, alienActive, gridOffsetX, gridOffsetY, verticalDirection, score, lives);
                        break;
                }
            }
        }
    }
}
// Medium Game Logic
void PlayMediumGame() {
    const int ALIEN_SPEED = 6;
    int ship_x = 20, ship_y = SCREEN_HEIGHT / 2;
    int bullet_x[MAX_BULLETS] = { 0 };
    int bullet_y[MAX_BULLETS] = { 0 };
    bool bulletActive[MAX_BULLETS] = { false };

    const int GRID_ROWS = 6;
    const int GRID_COLS = 10;
    const int ALIEN_SIZE = 60;
    const int ALIEN_SPACING = 50;

    int alien_x[GRID_ROWS][GRID_COLS];
    int alien_y[GRID_ROWS][GRID_COLS];
    bool alienActive[GRID_ROWS][GRID_COLS];

    int gridOffsetX = SCREEN_WIDTH - 300;
    int gridOffsetY = 100;
    int verticalDirection = 1;

    int score = 0; // Initialize score
    int lives = 3; // initialize lives

    // Initialize alien grid
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            alien_x[row][col] = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
            alien_y[row][col] = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);
            alienActive[row][col] = true;
        }
    }

    int lastFrameTime = GetTickCount64();
    int frameDelay = 1000 / 60;  // Target 60 FPS

    while (true) {
        int currentTime = GetTickCount64();// to avoid blinking   ,
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;

            // Clear screen
            system("cls");

            // Move aliens
            gridOffsetX -= ALIEN_SPEED;
            gridOffsetY += verticalDirection * 2;

            if (gridOffsetY <= 50 || gridOffsetY + GRID_ROWS * (ALIEN_SIZE + ALIEN_SPACING) >= SCREEN_HEIGHT) {
                verticalDirection *= -1;
            }

            // Draw aliens and check bounds
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (alienPosX <= 0) {
                            Gameovermessage(score);
                            return;
                        }

                        Alien(alienPosX, alienPosY);  // Draw the alien
                    }
                }
            }

            // Handle bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    Bullet(bullet_x[i], bullet_y[i], 255);  // Draw active bullet
                    bullet_x[i] += BULLET_SPEED;

                    if (bullet_x[i] >= SCREEN_WIDTH) {
                        bulletActive[i] = false;
                    }
                }
            }

            // Collision detection: bullets and aliens
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    for (int row = 0; row < GRID_ROWS; row++) {
                        for (int col = 0; col < GRID_COLS; col++) {
                            if (alienActive[row][col]) {
                                int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                                int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                                if (bullet_x[i] >= alienPosX - ALIEN_SIZE &&
                                    bullet_x[i] <= alienPosX &&
                                    bullet_y[i] >= alienPosY - ALIEN_SIZE &&
                                    bullet_y[i] <= alienPosY) {
                                    alienActive[row][col] = false;
                                    bulletActive[i] = false;
                                    score += 1; // Increment score on hit
                                    // game over if any conditons matches for winning 
                                    if (lives == 0 || areAllAliensDead(alienActive, GRID_ROWS, GRID_COLS)) {
                                        Gameovermessage(score);
                                        return;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Collision detection: ship and aliens
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (ship_x >= alienPosX - ALIEN_SIZE &&
                            ship_x <= alienPosX &&
                            ship_y >= alienPosY - ALIEN_SIZE &&
                            ship_y <= alienPosY) {
                            alienActive[row][col] = false;
                            lives -= 1; // Decrement lives on hit

                            if (lives == 0) {
                                Gameovermessage(score);
                                return;
                            }
                            break;
                        }
                    }
                }
            }

            drawScore(score, lives);  // Display the score and lives
            Ship(ship_x, ship_y, 255);  // Draw the ship

            // Player input
            int whichKey;
            if (isKeyPressed(whichKey)) {
                switch (whichKey) {
                case 1: // Left
                    if (ship_x > 20) ship_x -= 30;
                    break;
                case 2: // Up
                    if (ship_y > 20) ship_y -= 30;
                    break;
                case 3: // Right
                    if (ship_x < SCREEN_WIDTH - 95) ship_x += 30;
                    break;
                case 4: // Down
                    if (ship_y < SCREEN_HEIGHT - 70) ship_y += 30;
                    break;
                case 5: // Fire bullet
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bulletActive[i]) {
                            bullet_x[i] = ship_x + 75;
                            bullet_y[i] = ship_y + 25;
                            bulletActive[i] = true;
                            break;
                        }
                    }
                    break;
                case 7: // Toggle mode
                    toggle = toggle * (-1);
                    while (toggle == -1) {
                        if (isKeyPressed(whichKey)) {
                            if (whichKey == 7) {
                                toggle = toggle * (-1);
                            }
                        }
                    }
                    break;

                }
            }
        }
    }
}
// Medium Game Logic
void PlayEasyGame() {
    const int ALIEN_SPEED = 4;
    int ship_x = 20, ship_y = SCREEN_HEIGHT / 2;
    int bullet_x[MAX_BULLETS] = { 0 };
    int bullet_y[MAX_BULLETS] = { 0 };
    bool bulletActive[MAX_BULLETS] = { false };

    const int GRID_ROWS = 4;
    const int GRID_COLS = 10;
    const int ALIEN_SIZE = 60;
    const int ALIEN_SPACING = 50;

    int alien_x[GRID_ROWS][GRID_COLS];
    int alien_y[GRID_ROWS][GRID_COLS];
    bool alienActive[GRID_ROWS][GRID_COLS];

    int gridOffsetX = SCREEN_WIDTH - 300;
    int gridOffsetY = 100;
    int verticalDirection = 1;

    int score = 0; // Initialize score
    int lives = 3; // initialize lives

    // Initialize alien grid
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            alien_x[row][col] = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
            alien_y[row][col] = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);
            alienActive[row][col] = true;
        }
    }

    int lastFrameTime = GetTickCount64();
    int frameDelay = 1000 / 60;  // Target 60 FPS

    while (true) {
        int currentTime = GetTickCount64();// to avoid blinking   ,
        if (currentTime - lastFrameTime >= frameDelay) {
            lastFrameTime = currentTime;

            // Clear screen
            system("cls");

            // Move aliens
            gridOffsetX -= ALIEN_SPEED;
            gridOffsetY += verticalDirection * 2;

            if (gridOffsetY <= 50 || gridOffsetY + GRID_ROWS * (ALIEN_SIZE + ALIEN_SPACING) >= SCREEN_HEIGHT) {
                verticalDirection *= -1;
            }

            // Draw aliens and check bounds
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (alienPosX <= 0) {
                            Gameovermessage(score);
                            return;
                        }

                        Alien(alienPosX, alienPosY);  // Draw the alien
                    }
                }
            }

            // Handle bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    Bullet(bullet_x[i], bullet_y[i], 255);  // Draw active bullet
                    bullet_x[i] += BULLET_SPEED;

                    if (bullet_x[i] >= SCREEN_WIDTH) {
                        bulletActive[i] = false;
                    }
                }
            }

            // Collision detection: bullets and aliens
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    for (int row = 0; row < GRID_ROWS; row++) {
                        for (int col = 0; col < GRID_COLS; col++) {
                            if (alienActive[row][col]) {
                                int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                                int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                                if (bullet_x[i] >= alienPosX - ALIEN_SIZE &&
                                    bullet_x[i] <= alienPosX &&
                                    bullet_y[i] >= alienPosY - ALIEN_SIZE &&
                                    bullet_y[i] <= alienPosY) {
                                    alienActive[row][col] = false;
                                    bulletActive[i] = false;
                                    score += 1; // Increment score on hit
                                    // game over if any conditons matches for winning 
                                    if (lives == 0 || areAllAliensDead(alienActive, GRID_ROWS, GRID_COLS)) {
                                        Gameovermessage(score);
                                        return;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Collision detection: ship and aliens
            for (int row = 0; row < GRID_ROWS; row++) {
                for (int col = 0; col < GRID_COLS; col++) {
                    if (alienActive[row][col]) {
                        int alienPosX = gridOffsetX + col * (ALIEN_SIZE + ALIEN_SPACING);
                        int alienPosY = gridOffsetY + row * (ALIEN_SIZE + ALIEN_SPACING);

                        if (ship_x >= alienPosX - ALIEN_SIZE &&
                            ship_x <= alienPosX &&
                            ship_y >= alienPosY - ALIEN_SIZE &&
                            ship_y <= alienPosY) {
                            alienActive[row][col] = false;
                            lives -= 1; // Decrement lives on hit

                            if (lives == 0) {
                                Gameovermessage(score);
                                return;
                            }
                            break;
                        }
                    }
                }
            }

            drawScore(score, lives);  // Display the score and lives
            Ship(ship_x, ship_y, 255);  // Draw the ship

            // Player input
            int whichKey;
            if (isKeyPressed(whichKey)) {
                switch (whichKey) {
                case 1: // Left
                    if (ship_x > 20) ship_x -= 30;
                    break;
                case 2: // Up
                    if (ship_y > 20) ship_y -= 30;
                    break;
                case 3: // Right
                    if (ship_x < SCREEN_WIDTH - 95) ship_x += 30;
                    break;
                case 4: // Down
                    if (ship_y < SCREEN_HEIGHT - 70) ship_y += 30;
                    break;
                case 5: // Fire bullet
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bulletActive[i]) {
                            bullet_x[i] = ship_x + 75;
                            bullet_y[i] = ship_y + 25;
                            bulletActive[i] = true;
                            break;
                        }
                    }
                    break;
                case 7: // Toggle mode
                    toggle = toggle * (-1);
                    while (toggle == -1) {
                        if (isKeyPressed(whichKey)) {
                            if (whichKey == 7) {
                                toggle = toggle * (-1);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}





int main() {
    Sleep(5000);

    while (1) {
        // Clear screen
        system("cls");

        startmenu();
        char choice;
        cin >> choice;
        if (choice == 'A' || choice == 'a') {
            PlayArcadeGame();
        }
        else if (choice == 'H' || choice == 'h') {
            PlayHardGame();
        }
        else if (choice == 'M' || choice == 'm') {
            PlayMediumGame();
        }
        else if (choice == 'E' || choice == 'e') {
            PlayEasyGame();
        }
        else if (choice == 'l' || choice == 'L') {
            displayHighScores();
        }
        else {
            cout << "Invalid choice. Exiting game." << endl;
        }
    }
    return 0;
}