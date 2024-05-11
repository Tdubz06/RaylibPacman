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

	// Initialization
	const int screenWidth = 560;
	const int screenHeight = 720;

	// Score counter
	int score = 0;

	// Pacman velocity
	int velX = 0;
	int velY = 0;
	
	// Power pellet variables
	bool powerPellet = false;

	// Frame offset for power pellet
	int frameOffset = 0;

	// Speed of pacman
	int speed = 2;

	// Frame counter
	int frameCounter = 0;

	// fps
	int fps = 60;



	InitWindow(screenWidth, screenHeight, "Pac Dots");

	SetTargetFPS(fps);

	// Create Pacman character
	Rectangle pacman = { screenWidth / 2, 26 * 20 , 20, 20 };

	
	// Main game loop

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		frameCounter++;

		// Draw map
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
				score++;
				cout << "Score: " << score << endl;
			}

			// When pacman collides with a power pellet, the power pellet is removed from the map and checks with score counter
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '8') {
				map[i] = '0';
				score += 10;
				cout << "Score: " << score << endl;
				powerPellet = true;
				frameOffset = frameCounter + fps *5;

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
			cout << "Power pellet active. Speed: " << speed << endl;
			DrawRectangle(pacman.x, pacman.y, pacman.width, pacman.height, PINK);
			speed = 4;
		}
		else if (powerPellet == true && frameCounter >= frameOffset) {
			cout << "Power pellet effect ending." << endl;
			powerPellet = false;
			speed = 2;
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


		// Check for collision with walls
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1' ) {
				Rectangle wall = { static_cast<int>(i % 28 * 20), static_cast<int>(i / 28 * 20), 20, 20 };
				if (CheckCollisionRecs(pacman, wall)) {
						cout << "Collision with wall at x: " << wall.x << " y: " << wall.y << endl;
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


