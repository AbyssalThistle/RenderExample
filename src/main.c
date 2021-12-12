#define _GNU_SOURCE

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

#define N_MODELS 5

void InitCamera(Camera *c)
{
    c->position = (Vector3){ 10.0f, 10.0f, 10.0f };
    c->target = (Vector3){ 0.0f, 0.0f, 0.0f };
    c->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    c->fovy = 45.0f;
    c->projection = CAMERA_PERSPECTIVE;
    SetCameraMode(*c, CAMERA_FREE);
}

float Vector3Mag(Vector3 vec)
{
	return sqrtf(	(vec.x * vec.x) +
					(vec.y * vec.y) +
					(vec.z * vec.z) );
}

int Compare(const void *a, const void *b, void *c)
{

	const Vector3 *posA = (Vector3 *)a;
	const Vector3 *posB = (Vector3 *)b;
	Vector3 *cameraPos = (Vector3 *)c;

	float aDistance = Vector3Mag(Vector3Subtract(*cameraPos, *posA));
	float bDistance = Vector3Mag(Vector3Subtract(*cameraPos, *posB));

	//return (int)aDistance - bDistance;
	return (int)bDistance - aDistance;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
	bool renderBackface = false;

	Model models[N_MODELS];
	Vector3 positions[N_MODELS];

    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
	InitCamera(&camera);

	for(int i = 0; i < N_MODELS; ++i){
		models[i] = LoadModel("assets/wall.obj");
		positions[i] = (Vector3){0.f, 0.f, i};
	}

    // Main game loop
    while (!WindowShouldClose())
    {
		// Update
        UpdateCamera(&camera);
        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
		if (IsKeyPressed('R')) renderBackface = !renderBackface;

		qsort_r(models, N_MODELS, sizeof(Model), Compare, &(camera.position));
		qsort_r(positions, N_MODELS, sizeof(Vector3), Compare, &(camera.position));

		// Render
        BeginDrawing();
            ClearBackground(DARKGRAY);

            BeginMode3D(camera);

				if(!renderBackface) {
					rlDisableBackfaceCulling();
				}
				for(int i = 0; i < N_MODELS; ++i) {
					DrawModel(models[i], positions[i], 1.f, WHITE);
				}
				rlEnableBackfaceCulling();

                DrawGrid(10, 1.0f);
            EndMode3D();

            DrawRectangle( 10, 10, 320, 153, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 320, 153, BLUE);

            DrawText("Free camera default controls:", 20, 20, 10, BLACK);
            DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
            DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
            DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
            DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
			DrawText("- R to toggle backface culling", 40, 140, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
