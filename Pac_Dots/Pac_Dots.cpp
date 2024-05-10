#include <iostream>
#include <raylib.h>
#include <fstream>
#include <string>


using namespace std;

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

	cout << map << endl;

	// Initialization
	const int screenWidth = 560;
	const int screenHeight = 720;

	int score = 0;



	InitWindow(screenWidth, screenHeight, "Pac Dots");



	SetTargetFPS(60);

	// Create Pacman character
	Rectangle pacman = { screenWidth / 2, 26*20 , 20, 20 };

	// Main game loop

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		// Draw map
		for (int i = 0; i < map.length(); i++) {
			if (map[i] == '1') {
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}												
			if (map[i] == '2') {							
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}												
			if (map[i] == '3') {							
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}													
			if (map[i] == '4') {								
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}													
			if (map[i] == '5') {								
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}													
			if (map[i] == '6') {								
				DrawRectangle(i % 28 * 20, i / 28 * 20, 20, 20, BLUE);
			}

			// Draw pellets at each '7' in the map and centre them
			if (map[i] == '7') {
				DrawCircle(i % 28 * 20 + 10, i / 28 * 20 + 10, 3, WHITE);
			}

			// When pacman collides with a pellet, the pellet is removed from the map
			if (pacman.x == i % 28 * 20 && pacman.y == i / 28 * 20 && map[i] == '7') {
				map[i] = '0';
				score++;
				cout << "Score: " << score << endl;
			}


		}

		
			
		// Draw Pacman character with starting position in middle bottom of screen
		DrawRectangle(pacman.x, pacman.y, pacman.width, pacman.height, YELLOW);

	
		// Move Pacman character
		if (IsKeyDown(KEY_D))
		{
			pacman.x += 2;
		}
		if (IsKeyDown(KEY_A))
		{
			pacman.x -= 2;
		}
		if (IsKeyDown(KEY_W))
		{
			pacman.y -= 2;
		}
		if (IsKeyDown(KEY_S))
		{
			pacman.y += 2;
		}
	
		

	
		EndDrawing();
	}

	


}

