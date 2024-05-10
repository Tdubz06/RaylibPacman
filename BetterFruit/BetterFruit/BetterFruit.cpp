#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

int main()
{


	// Set FPS
	SetTargetFPS(60);

	// Set the window dimensions
	const int widthScreen = 800;
	const int heightScreen = 800;

	// Frame counter
	int framesCounter = 0;

	// Create a player rectangle 
	Rectangle player = { widthScreen / 2, heightScreen / 2, 50, 50 };

	// Create a fruit rectangle
	Rectangle fruit = { 0, 0, 25, 25 };

	// Create a score variable
	int score = 0;

	// Create fruit counter
	int fruitCounter = 0;



	// Initialize the window
	InitWindow(widthScreen, heightScreen, "Better Fruit");

	// Start the game loop
	while (!WindowShouldClose())
	{
		// Update the game loop
		BeginDrawing();
		ClearBackground(BLACK);

		// Draw the fruit rectangle
		DrawRectangleRec(fruit, GREEN);

		// Draw the player rectangle and put the PLAYER texture on top
		DrawRectangleRec(player, RED);

		// Draw the score in the top left corner
		DrawText(("Score: " + to_string(score)).c_str(), 10, 10, 20, WHITE);

		// Draw the fruit counter in the top right corner
		DrawText(("Fruit Counter: " + to_string(fruitCounter)).c_str(), 600, 10, 20, WHITE);


		// Update the player rectangle
		if (IsKeyDown(KEY_D)) player.x += 5;
		if (IsKeyDown(KEY_A)) player.x -= 5;
		if (IsKeyDown(KEY_W)) player.y -= 5;
		if (IsKeyDown(KEY_S)) player.y += 5;

	//	if (IsKeyPressed(KEY_D)) player.x += 10;
	//	if (IsKeyPressed(KEY_A)) player.x -= 10;
	//	if (IsKeyPressed(KEY_W)) player.y -= 10;
	//	if (IsKeyPressed(KEY_S)) player.y += 10;

		// Update the fruit rectangle
		if (framesCounter % 180 == 0)
		{
			fruit.x = GetRandomValue(0, GetScreenWidth() - 25);
			fruit.y = GetRandomValue(0, GetScreenHeight() - 25);
		//fruit.x = GetScreenHeight() / 2;
		//fruit.y = GetScreenWidth() / 2;
			fruitCounter++;
		
		}

		// Check if the player rectangle collides with the fruit rectangle and then remove the fruit and add to the score
		if (CheckCollisionRecs(player, fruit))
		{
			score++;
			fruit.x = -50;
			fruit.y = -50;
		}


		// Update the frames counter
		framesCounter++;

		// End the game loop
		EndDrawing();
	}


	


}

