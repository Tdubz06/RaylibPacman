#include <iostream>
#include <raylib.h>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// Function to read the map txt file and parse through it
string MapParse(string mapFiletxt) { //Reads the map txt file
	ifstream map(mapFiletxt);

	if (!(map.is_open())) {
		cout << "Error: File not found" << endl;
	}

	string currentLine;
	int currentLineNum = 0;
	string mapChars;
	while (getline(map, currentLine)) {
		for (int i = 0; i < currentLine.length(); i++) {
			mapChars = mapChars + currentLine[i];
			if (i == currentLine.length() - 1) {
				currentLineNum++;
			}
		}
	}
	return mapChars;
}

string map = MapParse("map.txt");


int main()
{
	// Initialization (default 560x720)
	const int screenWidth = 560;
	const int screenHeight = 720;

	// check if mmap is being read
	cout << map << endl;

	// Score counter
	int score = 0;


	// Lives counter
	int lives = 3;

	// Pacman velocity (this is for constant movement)
	int velX = 0;
	int velY = 0;

	// Power pellet variables (powerPellet is a bool to check if power pellet is active)
	bool powerPellet = false;

	// Frame offset for power pellet (frameOffset is the frame counter for power pellet)
	int frameOffset = 0;

	// Speed of pacman
	int speed = 2;

	// Frame counter 
	int frameCounter = 0;

	// Fps
	int fps = 60;

	// Dot counter for fruit (to know when to spawn fruit)
	int dotCounter = 0;

	// Fruit timer (to despawn fruit after 10 seconds)
	int fruitTimer = 0;

	// Start the program
	bool start = false;

	InitWindow(screenWidth, screenHeight, "Pac Dots");

	// Set Target FPS
	SetTargetFPS(fps);
	// Create Pacman
	Rectangle pacman = { screenWidth / 2, 26 * 20 , 20, 20 };

	// Mouse position
	Vector2 mousePoint = { 0.0f, 0.0f };

	// Define end screen
	Rectangle screen = { 40, screenHeight / 2 - 200, screenWidth - 80, screenHeight / 2 + 40 };
	//Rectangle screenBorder = { 20, 20, screenWidth - 40, screenHeight - 40 };
	Rectangle screenText = { screen.width, screen.height, screen.x - 40, screen.y - 40 };

	Rectangle screenButtonText1 = { screen.width, screen.height, screen.x - 40, screen.y - 40 };
	Rectangle screenButton1 = { screenButtonText1.x / 4 - 43, screenButtonText1.y - 8, screen.x * 4 + 7, screen.y / 5 };

	Rectangle screenButtonText2 = { screen.width, screen.height, screen.x - 40, screen.y - 40 };
	Rectangle screenButton2 = { screenButtonText1.x / 2 + 60, screenButtonText1.y - 8, screen.x * 4 + 19, screen.y / 5 };

	// Fonts
	Font titleFont = LoadFont("fonts/titleFont.ttf");
	Font livesFont = LoadFont("fonts/livesFont.ttf");

	// Main game loop
	while (!WindowShouldClose())
	{

		BeginDrawing();



		ClearBackground(BLACK);

		frameCounter++;

		// Draw map from map.txt file										// Any 2 and 3 on the map is where fruit will be placed
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1') {
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, DARKBLUE);
			}

			// Draw pellets at each '7' in the map and centre them
			if (map[i] == '7') {
				DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 3, WHITE);
			}

			// Draw Power Pellets at each '8' in the map and centre them
			if (map[i] == '8') {
				DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 5, PINK);
			}


			// When pacman collides with a pellet, the pellet is removed from the map and checks with score counter
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '7') {
				map[i] = '0';
				score += 10;
				dotCounter++;
				cout << "Score: " << score << endl;
			}

			// When pacman collides with a power pellet, the power pellet is removed from the map and checks with score counter
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '8') {
				map[i] = '0';
				score += 50;
				cout << "Score: " << score << endl;
				powerPellet = true;
				frameOffset = frameCounter + fps * 10;

			}

			// Fruit spawning (first fruit spawns after 70 dots are eaten on '2' and despawns after 10 seconds)
			if (dotCounter >= 70 && map[i] == '2') {
				DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 5, RED);
				fruitTimer++;
				if (fruitTimer == fps * 10) {
					map[i] = '0';
					fruitTimer = 0;
				}
				// When pacman collides with a fruit, the fruit is removed from the map and checks with score counter
				if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20) {
					map[i] = '0';
					score += 100;
					cout << "Score: " << score << endl;
				}
			}

			// Second fruit spawn on '3' after 170 dots are eaten
			if (dotCounter >= 170 && map[i] == '3') {
				DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 5, PINK);
				fruitTimer++;
				if (fruitTimer == fps * 10) {
					map[i] = '0';
					fruitTimer = 0;
				}
				// When pacman collides with a fruit, the fruit is removed from the map and checks with score counter
				if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20) {
					map[i] = '0';
					score += 200;
					cout << "Score: " << score << endl;
				}
			}
		}

		// When Pacman leaves the screen, it reappears on the opposite side
		if (pacman.x > screenWidth) {
			pacman.x = -20;
		}
		if (pacman.x < -pacman.width) {
			pacman.x = screenWidth;
		}
		if (pacman.y > screenHeight) {
			pacman.y = 0;
		}
		if (pacman.y < 0) {
			pacman.y = screenHeight;
		}


		// Draw Pacman character with starting position in middle bottom of screen
		DrawRectangle(pacman.x, pacman.y, pacman.width, pacman.height, YELLOW);

		if (powerPellet == true && frameCounter < frameOffset) {
			//cout << "Power pellet active. Speed: " << speed << endl;				(debugging)
			DrawRectangle(pacman.x, pacman.y, pacman.width, pacman.height, PINK);
			speed = 2.999999999999;											//(this currently creates issues with collision)
		}
		else if (powerPellet == true && frameCounter >= frameOffset) {
			cout << "Power pellet effect ending." << endl;				//(debugging)
			powerPellet = false;
			speed = 2;											//(Not needed when speed isn't changed on collection)												
		}


		// This is for constant movement of pacman
		pacman.x += velX;
		pacman.y += velY;


		// Move Pacman character
		if (!(lives == 0)) {
			if (IsKeyDown(KEY_D))
			{
				velX = speed;
				velY = 0;
			}
			if (IsKeyDown(KEY_A))
			{
				velX = -speed;
				velY = 0;
			}
			if (IsKeyDown(KEY_W))
			{
				velY = -speed;
				velX = 0;
			}
			if (IsKeyDown(KEY_S))
			{
				velY = speed;
				velX = 0;
			}
			if (IsKeyPressed(KEY_PERIOD))
			{
				lives--;
			}
			if (IsKeyPressed(KEY_COMMA))
			{
				lives++;
			}
		}


		// All for testing purposes
		//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	
	//	// Draw the Score in the top left corner using the score variable without format text
	//	DrawText(("Score: " + to_string(score)).c_str(), 10, 10, 20, WHITE);
	//	
	//	// Draw the frame counter in the top right corner using the frame counter variable without format text
	//	DrawText(("Frame: " + to_string(frameCounter)).c_str(), 375, 10, 20, WHITE);
	//
	//	// Draw the dot counter in the top right corner using the dot counter variable without format text
	//	DrawText(("Dot Counter: " + to_string(dotCounter)).c_str(), 375, 40, 20, WHITE);
	//
	//	// Draw the fruit timer in the top right corner using the fruit timer variable without format text
	//	DrawText(("Fruit Timer: " + to_string(fruitTimer)).c_str(), 375, 70, 20, WHITE);
	//
	//	// Draw the power pellet bool in the top right corner using the power pellet variable without format text
	//	DrawText(("Power Pellet: " + to_string(powerPellet)).c_str(), 375, 100, 20, WHITE);
	//
	//	// Draw the frame offset in the top right corner using the frame offset variable without format text
	//	DrawText(("Frame Offset: " + to_string(frameOffset)).c_str(), 375, 130, 20, WHITE);
	//
	//	// Draw the speed in the top right corner using the speed variable without format text
	//	DrawText(("Speed: " + to_string(speed)).c_str(), 375, 160, 20, WHITE);
	//
	//
		//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



		// Check for collision with walls
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1') {
				Rectangle wall = { static_cast<int>(i % 28 * 20), static_cast<int>(i / 28 * 20), 20, 20 };
				if (CheckCollisionRecs(pacman, wall)) {
					//cout << "Collision with wall at x: " << wall.x << " y: " << wall.y << endl;     (this is for debugging)
					if (pacman.x < wall.x) {
						pacman.x -= speed;
					}
					if (pacman.x > wall.x) {
						pacman.x += speed;
					}
					if (pacman.y < wall.y) {
						pacman.y -= speed;
					}
					if (pacman.y > wall.y) {
						pacman.y += speed;
					}

				}

			}
		}
		// Get mouse position
		mousePoint = GetMousePosition();

		// Draw UI
		//void DrawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint);
		DrawTextEx(titleFont, "Pacman", { screenWidth / 2 - 60, screenHeight / 60 }, 20, 3, WHITE);
		DrawTextEx(titleFont, TextFormat("Score: %i", score), { screenWidth / 2 - 100, screenHeight / 20 }, 20, 3, WHITE);

		// Draw lives
		DrawTextEx(titleFont, TextFormat("Lives: "), { screenWidth / 40, screenHeight - 38 }, 35, 3, WHITE);
		if (lives == 3) {
			DrawTextEx(livesFont, "ccc", { screenWidth / 40 + 180, screenHeight - 32 }, 35, 3, YELLOW);
		}
		else if (lives == 2) {
			DrawTextEx(livesFont, "cc", { screenWidth / 40 + 180, screenHeight - 32 }, 35, 3, YELLOW);
		}
		else if (lives == 1) {
			DrawTextEx(livesFont, "c", { screenWidth / 40 + 180, screenHeight - 32 }, 35, 3, YELLOW);
		}
		else if (lives == 0) {
			//Stop pacman
			velY = 0;
			velX = 0;

			DrawTextEx(livesFont, "", { screenWidth / 40 + 180, screenHeight - 32 }, 35, 3, YELLOW);
			DrawRectangleRec(screen, BLACK);
			DrawRectangleLinesEx(screen, 10, BLUE);

			// Draw the end screen text and buttons
			DrawTextEx(titleFont, "Game Over", { screenText.x / 2 - 45, screenText.y / 2 }, 20, 3, WHITE);

			DrawRectangleRec(screenButton1, PINK);
			DrawRectangleLinesEx(screenButton1, 4, BLUE);
			DrawTextEx(titleFont, "Play Again", { screenButtonText1.x / 4 - 35, screenButtonText1.y }, 15, 3, WHITE);

			DrawRectangleRec(screenButton2, RED);
			DrawRectangleLinesEx(screenButton2, 4, BLUE);
			DrawTextEx(titleFont, "End Session", { screenButtonText2.x - 170, screenButtonText2.y }, 15, 3, WHITE);

			if (CheckCollisionPointRec(mousePoint, screenButton1))
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				{
					
					score = 0;
					lives = 3;
					pacman.x = screenWidth / 2;
					pacman.y = 26 * 20;
					dotCounter = 0;
					fruitTimer = 0;
					
					
					map = MapParse("map.txt");
					//main();
				}
			}

			if (CheckCollisionPointRec(mousePoint, screenButton2))
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				{
					return 0;
				}


			}
		}
		//
		EndDrawing();
		CloseWindow();
	}

}
