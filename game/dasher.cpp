#include "raylib.h"

struct AnimationData
{
	Rectangle rect;
	Vector2 position;
	int frame;
	float updateTime;
	float runningTime;
};

// Ground checking function
bool IsGrounded(AnimationData animData, int windowHeight)
{
	return (animData.position.y >= (windowHeight - animData.rect.height)) ;
}



int main()
{
	// window dimensions
	int windowDiamensions[]{ 512,380 };

	const int targetFPS{ 60 };
	const int numberSprites{ 6 };

	const int numberNebulaSprites_x{ 8 };
	const int numberNebulaSprites_y{ 8 };
	const int sizeOfNebulae{ 6 };

	// acceleration due to gravity (pixels/s)/s
	const int gravity{ 1000 };

	InitWindow(windowDiamensions[0], windowDiamensions[1], "Dapper Dasher");
	SetTargetFPS(targetFPS);

	//Nebula variables
	Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

	//nebula x velocity (pixels/second
	int nebulaVelocity{ -200 }; //  travels right to left

	// Create and initalise nublae animation data.
	AnimationData nebulae[sizeOfNebulae]{ };
	for (int i = 0; i < sizeOfNebulae; i++)
	{
		nebulae[i].rect.x = 0.0;
		nebulae[i].rect.y = 0.0;
		nebulae[i].rect.width = nebula.width / numberNebulaSprites_x;
		nebulae[i].rect.height = nebula.height / numberNebulaSprites_y;
		nebulae[i].position.x = windowDiamensions[0] + (300 * i);
		nebulae[i].position.y = windowDiamensions[1] - nebula.height / numberNebulaSprites_y;
		nebulae[i].frame = 0;
		nebulae[i].updateTime = 1.0 / (2 * numberNebulaSprites_x);
		nebulae[i].runningTime = 0.0;
	}

	// Scarfy variables
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimationData scarfyData{
		{0,0,scarfy.width / 6, scarfy.height * 1,},
		{windowDiamensions[0] / 2 - scarfy.width / 12,windowDiamensions[1] - scarfy.height},
		0,
		 1.0 / (targetFPS / (numberSprites - 1)), // Animation frame update time 1/12 second
		0.0
	};

	int jumpVelocity{ 600 }; // pixels/second

	// initialise the window

	int velocity{ 0 };

	bool hasJumped{ false };

	while (!WindowShouldClose())
	{
		// Start drawing
		float deltaTime{ GetFrameTime() };
		scarfyData.runningTime += deltaTime;
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i].runningTime += deltaTime;
		}
		BeginDrawing();

		ClearBackground(WHITE);

		// Perform ground check
		bool grounded = IsGrounded(scarfyData, windowDiamensions[1]);
		if (!grounded)
		{
			// Apply gravity if not grounded.
			velocity += gravity * deltaTime;
		}
		else
		{
			// Is grounded.
			velocity = 0;
		}
		if (IsKeyPressed(KEY_SPACE) && grounded)
		{
			velocity -= jumpVelocity;
			hasJumped = true;
		}

		//Update positions
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i].position.x += nebulaVelocity * deltaTime;
		}

		scarfyData.position.y += velocity * deltaTime;

		//Update scarfy animation frame
		if (scarfyData.runningTime >= scarfyData.updateTime && grounded)
		{
			scarfyData.rect.x = scarfyData.frame * scarfyData.rect.width;
			scarfyData.frame++;
			if (scarfyData.frame > 5)
			{
				scarfyData.frame = 0;
			}
			scarfyData.runningTime = 0.0;
		}

		// Update nebula animation
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			if (nebulae[i].runningTime >= nebulae[i].updateTime)
			{
				nebulae[i].rect.x = nebulae[i].frame * nebulae[i].rect.width;
				nebulae[i].frame++;
				if (nebulae[i].frame > nebula.width - 1)
				{
					nebulae[i].frame = 0;
				}
				nebulae[i].runningTime = 0.0;
			}
		}

		if (nebulae[1].runningTime >= nebulae[1].updateTime)
		{
			nebulae[1].rect.x = nebulae[1].frame * nebulae[1].rect.width;
			nebulae[1].frame++;
			if (nebulae[1].frame > nebula.width - 1)
			{
				nebulae[1].frame = 0;
			}
			nebulae[1].runningTime = 0.0;
		}
		//Draw nebula
		//DrawTextureRec(nebula, nebulae[1].rect, nebulae[1].position, RED);
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].position, WHITE);
		}


		// Draw scarfy
		DrawTextureRec(scarfy, scarfyData.rect, scarfyData.position, WHITE);

		// Stop drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	CloseWindow();
}
