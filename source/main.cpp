#include <raylib.h>
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>

int main(void) {

	const std::string score_filename = "score.txt";

	std::ifstream score_file_read(score_filename);
	
	int file_max_score = 0;
	
	if (score_file_read.is_open()) {
		score_file_read >> file_max_score;
	}
	
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Eater Game");

	SetTargetFPS(60);

	struct {
		Vector2 pos = { (float) 400.0f, (float) 300.0f};
		float velocity = 4.0f;
		int score = 0; 
		int max_score = 0;
				
	} ball;

	struct {
		Vector2 pos;
		float size = 20;
		bool exists = false;		
		
	} food;

	struct {
		float time_left = 600;
		float time_decrease = 1;
		
		Vector2 pos = { (float) screenWidth - 315, 20};
		Vector2 size = { (float) time_left/2, (float) 30.0f };
				 
	} timer;

	bool game_over = false;
	ball.max_score = file_max_score;
	
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_Z)) printf("ball_x %.2f, ball_y %.2f\n",ball.pos.x, ball.pos.y);
		if (IsKeyPressed(KEY_X)) printf("max score: %i", ball.max_score);
		
			BeginDrawing();

				if (!game_over) {
				
					if (IsKeyDown(KEY_RIGHT)) ball.pos.x += ball.velocity;
					if (IsKeyDown(KEY_DOWN)) ball.pos.y += ball.velocity;
					if (IsKeyDown(KEY_LEFT)) ball.pos.x -= ball.velocity;
					if (IsKeyDown(KEY_UP)) ball.pos.y -= ball.velocity;

					if (!food.exists) {
						food.exists = true;
						
						food.pos.x = GetRandomValue(10, screenWidth - 10);
						food.pos.y = GetRandomValue(10, screenHeight - 10);
					} else { 
						DrawCircleV(food.pos, food.size, RED);
					}

					if (( ball.pos.x <= food.pos.x + food.size && ball.pos.x >= food.pos.x - food.size ) &&
						( ball.pos.y <= food.pos.y + food.size && ball.pos.y >= food.pos.y - food.size )) {
						food.exists = false;

						timer.time_left = 600;
						timer.time_decrease += 0.25;
					
						ball.score += 1;
						ball.velocity += 0.25;
					 }

					 if ( timer.time_left <= 0 ) {
					 	game_over = true;
					 }

					timer.time_left -= timer.time_decrease;
					timer.size = { (float) timer.time_left/2, (float) 20.0f };

					DrawCircleV(ball.pos, 25, BLACK);
					DrawRectangleV(timer.pos, timer.size, BLUE);
					
 					DrawText(TextFormat("Score: %i", ball.score), 20, 20, 20, BLACK);
 					DrawText(TextFormat("Max Score: %i", ball.max_score), 20, 45, 20, BLACK);

					if (ball.score >= ball.max_score ) {
						ball.max_score = ball.score;
					}

				} else {				
					DrawText("GAME OVER!", 165, 250, 70, BLACK);
					DrawText(TextFormat("Score: %i", ball.score), 310, 325, 40, BLACK);

					if (ball.score >= file_max_score) {
						std::ofstream score_file_write(score_filename);
						if (score_file_write.is_open()) {
							score_file_write << ball.max_score;
						}
					}		
				}
			
			ClearBackground(WHITE); 
				
		EndDrawing();
		}
	
	CloseWindow();
	
	return 0;
}
