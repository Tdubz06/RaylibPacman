#include <iostream>
#include <raylib.h>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <random>

using namespace std;


// Function to read the map txt file and parse through it //
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
string mapGhost = MapParse("mapGhost.txt");


// Struct for the ghosts
struct Ghost {
	Rectangle rect;
	int velX, velY;
	Texture2D texture;
};

vector<Ghost> ghosts;

void InitGhosts() {
	Image ghostImages[5] = {
		LoadImage("inky.png"),
		LoadImage("blinky.png"),
		LoadImage("pinky.png"),
		LoadImage("Dave.png"),
		LoadImage("ScaredGhost.png")
	};

	for (auto& img : ghostImages) {
		ImageResize(&img, 20, 20);
	}

	Texture2D ghostTextures[4];
	for (int i = 0; i < 4; i++) {
		ghostTextures[i] = LoadTextureFromImage(ghostImages[i]);
	}

	// Initialize ghosts with starting positions
	ghosts = {
		{ { 240, 360, 20, 20 }, 2, 0, ghostTextures[0] },
		{ { 260, 360, 20, 20 }, 2, 0, ghostTextures[1] },
		{ { 280, 360, 20, 20 }, 2, 0, ghostTextures[2] },
		{ { 300, 360, 20, 20 }, 2, 0, ghostTextures[3] }
	};
}

void MoveGhosts() {
	for (auto& ghost : ghosts) {
		ghost.rect.x += ghost.velX;
		ghost.rect.y += ghost.velY;

		// Ghost collision with walls
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1') {
				Rectangle wall = { static_cast<int>(i % 28 * 20), static_cast<int>(i / 28 * 20), 20, 20 };
				if (CheckCollisionRecs(ghost.rect, wall)) {
					ghost.rect.x -= ghost.velX;
					ghost.rect.y -= ghost.velY;
					
					random_device rd;

					// Use the Mersenne Twister 19937 generator
					mt19937 gen(rd());

					// Define the range [1, 4]
					uniform_int_distribution<> dis(0, 3);
					
					// Change direction randomly upon collision
					int direction = dis(gen);
					switch (direction) {
					case 0: ghost.velX = 2; ghost.velY = 0; break; // Right
					case 1: ghost.velX = -2; ghost.velY = 0; break; // Left
					case 2: ghost.velX = 0; ghost.velY = 2; break; // Down
					case 3: ghost.velX = 0; ghost.velY = -2; break; // Up
					}
				}
			}
		}
		// Ghost direction change when on mapGhost '4'
		for (int i = 0; i < mapGhost.length(); i++) {
			if (mapGhost[i] == '4') {
				//Rectangle wall = { static_cast<int>(i % 28 * 20), static_cast<int>(i / 28 * 20), 20, 20 };
				if (ghost.rect.x == i % 28 * 20 && ghost.rect.y == i / 28 * 20 && mapGhost[i] == '4'){
				//if (CheckCollisionRecs(ghost.rect, wall))
					
					random_device rd;
					
					// Use the Mersenne Twister 19937 generator
					mt19937 gen(rd());

					// Define the range [1, 4]
					uniform_int_distribution<> dis(0, 3);

					
					// Change direction randomly upon collision
                    int direction = dis(gen);
					switch (direction) {
					case 0: ghost.velX = 2; ghost.velY = 0; break; // Right
					case 1: ghost.velX = -2; ghost.velY = 0; break; // Left
					case 2: ghost.velX = 0; ghost.velY = 2; break; // Down
					case 3: ghost.velX = 0; ghost.velY = -2; break; // Up
					}

					
					mapGhost[i] = '0';


				
				}
				 
					mapGhost[i] = '4';
				
			}
		}

		// When ghost collide with 5 go up
		for (int i = 0; i < mapGhost.length(); i++) {
			if (mapGhost[i] == '5') {
				//Rectangle wall = { static_cast<int>(i % 28 * 20), static_cast<int>(i / 28 * 20), 20, 20 };
				if (ghost.rect.x == i % 28 * 20 && ghost.rect.y == i / 28 * 20 && mapGhost[i] == '5') {
					//if (CheckCollisionRecs(ghost.rect, wall))

					ghost.velX = 0; ghost.velY = -2; // Up
					mapGhost[i] = '0';
				}
				mapGhost[i] = '5';
			}
		}
	}
}
int main()
{
	// check if mmap is being read
	cout << map << endl;

	// Initialization (default 560x720)
	const int screenWidth = 560;
	const int screenHeight = 720;

	// Score counter
	int score = 0;

	// Lives Counter
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


	// Ghost timer 
	int ghostTimer = 0;

	// Start the program
	bool start = false;
	 

	InitWindow(screenWidth, screenHeight, "Pac Dots");

	// Image/Texture loading
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	// Ghost image loading and resizing
	Image Inky = LoadImage("inky.png");
	ImageResize(&Inky, 20, 20);
	Texture2D inkyTexture = LoadTextureFromImage(Inky);

	Image Blinky = LoadImage("blinky.png");
	ImageResize(&Blinky, 20, 20);
	Texture2D blinkyTexture = LoadTextureFromImage(Blinky);

	Image Pinky = LoadImage("pinky.png");
	ImageResize(&Pinky, 20, 20);
	Texture2D pinkyTexture = LoadTextureFromImage(Pinky);

	Image DaveChapelle = LoadImage("Dave.png");
	ImageResize(&DaveChapelle, 20, 20);
	Texture2D daveChapelleTexture = LoadTextureFromImage(DaveChapelle);

		// Scared Ghost image loading and resizing
	Image ScaredGhost = LoadImage("ScaredGhost.png");
	ImageResize(&ScaredGhost, 20, 20);
	Texture2D scaredGhostTexture = LoadTextureFromImage(ScaredGhost);
	
	// Fruit image loading and resizing
	Image Cherry = LoadImage("cherry.png");
	ImageResize(&Cherry, 25, 25);
	Texture2D cherryTexture = LoadTextureFromImage(Cherry);

	Image Strawberry = LoadImage("strawberry.png");
	ImageResize(&Strawberry, 18, 18);
	Texture2D strawberryTexture = LoadTextureFromImage(Strawberry);


	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	SetTargetFPS(fps);

	// Create Pacman
	Rectangle pacman = { screenWidth / 2, 26 * 20 , 20, 20 };

	// Initialize ghosts
	InitGhosts();
	// Random start movement for ghosts
	for (auto& ghost : ghosts) {
		int direction = GetRandomValue(0, 3);
		switch (direction) {
		case 0: ghost.velX = 2; ghost.velY = 0; break; // Right
		case 1: ghost.velX = -2; ghost.velY = 0; break; // Left
		}
	}

	// Mouse Position
	Vector2 mousePoint = { 0.0f, 0.0f };

	// Define end screen
	Rectangle screen = { 40, screenHeight / 2 - 200, screenWidth - 80, screenHeight / 2 + 40 };
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

		// Draw map from map.txt file							// Any 2 and 3 on the map is where fruit will be placed
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

			// Draw blue squares at each '9' in the map and centre them
			if (map[i] == '9') {
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}
			
			// When pacman collides with a pellet, the pellet is removed from the map and checks with score counter
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '7') {
				map[i] = '0';
				score += 10;
				dotCounter++;
				cout << "Score: " << score << endl;
				cout << "Dots eaten: " << dotCounter << endl;

				// When 244 dots are eaten reset everything but the score
				if (dotCounter >= 244) {
					lives = 3;
					pacman.x = screenWidth / 2;
					pacman.y = 26 * 20;
					dotCounter = 0;
					fruitTimer = 0;

					// Reset ghosts
					InitGhosts();

					// Reset map
					map = MapParse("map.txt");


				}
			}
			
			// When pacman collides with a power pellet, the power pellet is removed from the map and checks with score counter
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '8') {
				map[i] = '0';
				score += 50;
				cout << "Score: " << score << endl;
				powerPellet = true;
				frameOffset = frameCounter + fps *10;
				dotCounter++;
				cout << dotCounter << endl;

				// When 244 dots are eaten reset everything but the score
				if (dotCounter >= 244) {
					lives = 3;
					pacman.x = screenWidth / 2;
					pacman.y = 26 * 20;
					dotCounter = 0;
					fruitTimer = 0;

					// Reset ghosts
					InitGhosts();

					// Reset map
					map = MapParse("map.txt");


				}
			}

			// Fruit spawning (first fruit spawns after 70 dots are eaten on '2' and despawns after 10 seconds)
			if (dotCounter >= 70 && map[i] == '2') {
				//DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 5, RED);
				DrawTextureEx(cherryTexture, { static_cast<float>(i % 28 * 20), static_cast<float>(i / 28 * 20) }, 0, 1, WHITE);
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
				//DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 5, PINK);
				DrawTextureEx(strawberryTexture, { static_cast<float>(i % 28 * 20), static_cast<float>(i / 28 * 20) }, 0, 1, WHITE);
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
		ghostTimer++;
		// Pacman collision with ghosts
		for (const auto& ghost : ghosts) {
			if (CheckCollisionRecs(pacman, ghost.rect)) {
				if (powerPellet == true) {
					
					// Move only the ghost that collides with pacman
					for (auto& ghost : ghosts) {
						if (CheckCollisionRecs(pacman, ghost.rect)) {
							ghostTimer = 0;
								ghost.rect.x = 240;
								ghost.rect.y = 360;
								
								score += 200;
						}
					}
		
				}
				else {
					lives--;
					pacman.x = screenWidth / 2;
					pacman.y = 26 * 20;
					for (auto& ghost : ghosts) {
						ghost.rect.x = 240;
						ghost.rect.y = 360;
					}
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

	
		// When Ghost leave the screen, they reappear on the opposite side
		for (auto& ghost : ghosts) {
			if (ghost.rect.x > screenWidth) {
				ghost.rect.x = -20;
			}
			if (ghost.rect.x < -ghost.rect.width) {
				ghost.rect.x = screenWidth;
			}
			if (ghost.rect.y > screenHeight) {
				ghost.rect.y = 0;
			}
			if (ghost.rect.y < 0) {
				ghost.rect.y = screenHeight;
			}
		}

		
		// Draw Pacman as a circle in starting position
		DrawCircle(pacman.x + 10, pacman.y + 10, 10, YELLOW);

		// Every other frame, draw Pacman with a pink rectangle to simulate blinking when power pellet is active
		if (powerPellet == true) {
			DrawCircle(pacman.x + 10, pacman.y + 10, 10, PINK);
		}
		
		if (powerPellet == true && frameCounter < frameOffset) {
			//cout << "Power pellet active. Speed: " << speed << endl;				(debugging)
			//DrawRectangle(pacman.x, pacman.y, pacman.width, pacman.height, PINK);
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

		// Check for collision with walls
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1' || map[i] == '9') {
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

		

		// Map for ghosts
		for (int i = 0; i < mapGhost.length(); i++) {
			if (mapGhost[i] == '4') {
				// Draw a 1x1 rectangle at each '4' in the map and centre them
				//DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 1, GREEN);
				
			}
		}

		// Map for ghost box collision (where ghosts spawn)
		for (int i = 0; i < mapGhost.length(); i++) {
			if (mapGhost[i] == '5') {
				// Draw a 1x1 rectangle at each '5' in the map and centre them
				//DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 1, RED);
			}
		}
	
		// Move and draw ghosts
		MoveGhosts();
		for (const auto& ghost : ghosts) {
			DrawTexture(ghost.texture, ghost.rect.x, ghost.rect.y, WHITE);
		}
		
		// Draw scared ghost texture on ghosts when power pellet is active
		if (powerPellet == true) {
			for (const auto& ghost : ghosts) {
				DrawTexture(ghost.texture, ghost.rect.x, ghost.rect.y, WHITE);
				DrawTexture(scaredGhostTexture, ghost.rect.x, ghost.rect.y, WHITE);
			}
		}
		else {
			for (const auto& ghost : ghosts) {
				DrawTexture(ghost.texture, ghost.rect.x, ghost.rect.y, WHITE);
			}
		}

		// Draw the UI
		DrawTextEx(titleFont, "Pacman", { screenWidth / 2 - 60, screenHeight / 60 }, 20, 3, WHITE);
		DrawTextEx(titleFont, TextFormat("Score: %i", score), { screenWidth / 2 - 100, screenHeight / 20 }, 20, 3, WHITE);

		// Draw lives at the bottom left corner
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
		// When lives reach 0, Pacman stops moving and the game over screen is displayed
		else if (lives == 0) {
			velY = 0;
			velX = 0;
			// Set ghost to VelX = 0 and VelY = 0
			for (auto& ghost : ghosts) {
				ghost.velX = 0;
				ghost.velY = 0;
			}

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

			
			// When power pellet is true draw the scared ghost texture on the ghosts
			if (powerPellet == true) {
				for (const auto& ghost : ghosts) {
					DrawTexture(ghost.texture, ghost.rect.x, ghost.rect.y, WHITE);
					DrawTexture(scaredGhostTexture, ghost.rect.x, ghost.rect.y, WHITE);
				}
			}
			else {
				for (const auto& ghost : ghosts) {
					DrawTexture(ghost.texture, ghost.rect.x, ghost.rect.y, WHITE);
				}
			}



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

					// Reset ghosts
					InitGhosts();


					map = MapParse("map.txt");
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

		EndDrawing();
		CloseWindow();
	}




}


