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
	// check if mmap is being read
	cout << map << endl;

	// Initialization (default 560x720)
	const int screenWidth = 560;
	const int screenHeight = 720;

	// Score counter
	int score = 0;

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



	InitWindow(screenWidth, screenHeight, "Pac Dots");

	SetTargetFPS(fps);

	// Create Pacman
	Rectangle pacman = { screenWidth / 2, 26 * 20 , 20, 20 };

	
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
				frameOffset = frameCounter + fps *10;

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
		if (pacman.x < - pacman.width) {
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
			//speed = 4;																			//(this currently creates issues with collision)
		}
		else if (powerPellet == true && frameCounter >= frameOffset) {
			cout << "Power pellet effect ending." << endl;				//(debugging)
			powerPellet = false;
			//speed = 2;																			//(Not needed when speed isn't changed on collection)												
		}


		// This is for constant movement of pacman
		pacman.x += velX;
		pacman.y += velY;


		// Move Pacman character
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
			if (map[i] == '1' ) {
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




		EndDrawing();
	}




}


