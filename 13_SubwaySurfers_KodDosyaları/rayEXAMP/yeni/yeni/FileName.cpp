#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include<time.h>
#define MAX_BLUE_OBJECTIVES 10
#define max_tree_number 1000
#define max_bush_num 1000

float SPEED = 0.2;

int jump_function(void) {

    if (IsKeyDown(KEY_SPACE)) {
        return 0;
    }
    else {
        return 1;
    }
}

void is_stopped(Vector3* cubePositionPtr) {
    *cubePositionPtr = (Vector3){ 0, 0, 0 };
}

void is_stopped_camera(Vector3* camera_positionPtr, Vector3* camera_targetPtr) {
    *camera_positionPtr = (Vector3){ 0.0f,7.0f,10.0f };
    *camera_targetPtr = (Vector3){ 0.0f,0.0f,0.0f };
}

void is_ended(float* speed_ptr, Vector3* road_ptr, Vector3* left_planePtr, Vector3* right_plane_Ptr) {
    *speed_ptr = 0.0;
    *road_ptr = (Vector3){ 0.0,0.0,0.0 };
    *right_plane_Ptr = (Vector3){ 18.5, -2, 0 };
    *left_planePtr = (Vector3){ -18.5, -2, 0 };
}

int is_over(bool check) {
    if (check) {
        return 1;
    }
}

bool ending_condition(void) {
    if (is_over) {
        return true;
    }
}



typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - RAPID RUSH");

    GameScreen currentScreen = LOGO;

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f,7.0f,10.0f }; //CAMERA POSITION
    camera.target = (Vector3){ 0.0f,0.0f,0.0f }; //CAMERA LOOKING AT POINT
    camera.up = (Vector3){ 0.0f,1.0f,0.0f }; //rotation towards target
    camera.fovy = 45.0f; //camera field of view y
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 blueObjectives[MAX_BLUE_OBJECTIVES];

    int numBlueObjectives = 0;
    float timeSinceLastSpawn = 0.0f;

    int tree_number = 0;
    float timeSinceLastSpawn1 = 0.0f;
    float timeSinceLastSpawn2 = 0.0f;

    float timeSinceLastSpawn3 = 0.0f;
    float timeSinceLastSpawn4 = 0.0f;


    Model model = LoadModel("resources/models/iqm/guy.iqm");
    Texture2D texture = LoadTexture("resources/models/iqm/guytex.png");
    SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);

    Vector3 modelPosition = { 0.0f, 0.0f, 0.0f };
    Vector3 modelSize = { 0.3f, 0.3f, 0.3f };

    Model model1 = LoadModel("resources/models/obj1/mycube.glb");
    Vector3 modelSize1 = { 0.5f,0.5f,0.5f };

    Model tree = LoadModel("resources/models/obj1/tree3.glb");
    Vector3 treeSize = { 0.8f, 0.8f, 0.8f };
    Vector3 trees[max_tree_number];

    Model bushes = LoadModel("resources/models/obj1/Bushes.glb");
    Vector3 bush_size = { 0.04f, 0.04f, 0.04f };
    Vector3 bush[max_bush_num];
    int bush_num = 0;

    Model tunnel = LoadModel("resources/models/obj1/boru.glb");
    Vector3 tunnel_size = { 6.0,7.9,5.0 };
    Vector3 tunnel_position = { 0,0,-12 };

    Vector3 cube_position = { 0,0,0 };
    Vector3 cubeSýze = { 6,0.000001,100 };
    Vector3 plane_pos = (Vector3){ -18.5, -2, 0 };
    Vector3 right_plane_pos = (Vector3){ 18.5, -2, 0 };

    int animsCount = 0;
    ModelAnimation* anims = LoadModelAnimations("resources/models/iqm/guyanim.iqm", &animsCount);
    int animFrameCounter = 0;

    DisableCursor();

    int framesCounter = 0;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {

        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framesCounter > 120)
            {
                currentScreen = TITLE;
            }
        } break;
        case TITLE:
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
            }


        } break;
        case GAMEPLAY:
        {

        } break;
        case ENDING:
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }

        } break;
        default: break;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            DrawText("RAPID RUSH", 20, 20, 40, DARKBLUE);
            DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

        } break;
        case TITLE:
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
            DrawText("RAPID RUSH", 180, 150, 60, DARKBLUE);
            DrawText("PRESS ENTER to PLAY the GAME", 200, 240, 20, DARKGREEN);

        } break;
        case GAMEPLAY:
        {
            int score = modelPosition.z / 10 * (-1);
            bool game_over = false;

            DrawRectangle(0, 0, screenWidth, screenHeight, RAYWHITE);
            DrawText("RAPID RUSH", 20, 20, 30, BLUE);
            DrawPlane(plane_pos, (Vector2) { 30, 100 }, DARKGREEN);
            DrawText(TextFormat("SCORE: %i", score), 20, 50, 25, MAROON);

            timeSinceLastSpawn += GetFrameTime();
            timeSinceLastSpawn1 += GetFrameTime();
            timeSinceLastSpawn2 += GetFrameTime();
            timeSinceLastSpawn3 += GetFrameTime();
            timeSinceLastSpawn3 += GetFrameTime();

            if (IsKeyDown(KEY_SPACE))
            {
                animFrameCounter += 3;
                UpdateModelAnimation(model, anims[0], animFrameCounter);
                if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                modelPosition.x += 2;
            }

            if (IsKeyPressed(KEY_LEFT)) {
                modelPosition.x -= 2;
            }
            if (modelPosition.x > 2)
                modelPosition.x = 2;

            if (modelPosition.x < -2)
                modelPosition.x = -2;


            for (int i = 0; i < numBlueObjectives; i++) {

                if (jump_function()) {

                    if (CheckCollisionBoxes(
                        (BoundingBox) {
                        (Vector3) {
                        modelPosition.x - modelSize.x / 2,
                            modelPosition.y - modelSize.y / 2,
                            modelPosition.z - modelSize.z / 2,

                    },
                            (Vector3) {
                            modelPosition.x + modelSize.x / 2,
                                modelPosition.y + modelSize.y / 2,
                                modelPosition.z + modelSize.z / 2,
                        }
                    },

                        (BoundingBox) {
                        (Vector3) {
                        blueObjectives[i].x - modelSize1.x / 2,
                            blueObjectives[i].y - modelSize1.y / 2,
                            blueObjectives[i].z - modelSize1.z / 2,
                    },
                            (Vector3) {
                            blueObjectives[i].x + modelSize1.x / 2,
                                blueObjectives[i].y + modelSize1.y / 2,
                                blueObjectives[i].z + modelSize1.z / 2,
                        }
                    })) {

                        DrawText("END", 300, 100, 100, RED);
                        is_stopped(&modelPosition);
                        is_stopped_camera(&camera.position, &camera.target);
                        is_ended(&SPEED, &cube_position, &plane_pos, &right_plane_pos);
                        game_over = true;
                        is_over(game_over);
                        if (ending_condition()) {
                            currentScreen = ENDING;
                        }
                    }
                }
            }


            BeginMode3D(camera);

            if (timeSinceLastSpawn >= 1.5f && numBlueObjectives < MAX_BLUE_OBJECTIVES)
            {
                blueObjectives[numBlueObjectives].z = modelPosition.z - 30.0f;
                blueObjectives[numBlueObjectives].x = GetRandomValue(-1, 1) * 2.0f;
                blueObjectives[numBlueObjectives].y = 0.4f;
                numBlueObjectives++;
                timeSinceLastSpawn = 0.0f;
            }

            for (int i = 0; i < numBlueObjectives; i++)
            {
                if (blueObjectives[i].z > modelPosition.z + 10)
                {
                    for (int j = i; j < numBlueObjectives - 1; j++)
                    {
                        blueObjectives[j] = blueObjectives[j + 1];
                    }
                    numBlueObjectives--;
                    i--;
                }
                else
                {
                    DrawModelEx(model1, (Vector3) { blueObjectives[i].x, blueObjectives[i].y, blueObjectives[i].z }, (Vector3) { 0.0f, 1.0f, 0.0f }, 180.0f, modelSize1, WHITE);
                }
            }


            if (timeSinceLastSpawn1 >= 1.5f && tree_number < max_tree_number)
            {
                trees[tree_number].z = modelPosition.z - 30.0f;
                trees[tree_number].x = GetRandomValue(-10, -5);
                trees[tree_number].y = 0.5;
                tree_number++;
                timeSinceLastSpawn1 = 0.0f;
            }

            for (int i = 0; i < tree_number; i++)
            {
                if (trees[tree_number].z > modelPosition.z + 10)
                {
                    for (int j = i; j < max_tree_number - 1; j++)
                    {
                        trees[j] = trees[j + 1];
                    }
                    tree_number--;
                    i--;
                }
                else
                {
                    DrawModelEx(tree, (Vector3) { trees[i].x, trees[i].y, trees[i].z }, (Vector3) { 0.0f, 1.0f, 0.0f }, 180.0f, treeSize, WHITE);
                }
            }

            if (timeSinceLastSpawn2 >= 1.2f && tree_number < max_tree_number)
            {
                trees[tree_number].z = modelPosition.z - 30.0f;
                trees[tree_number].x = GetRandomValue(5, 10);
                trees[tree_number].y = 0.5;
                tree_number++;
                timeSinceLastSpawn2 = 0.0f;
            }

            for (int i = 0; i < tree_number; i++)
            {
                if (trees[tree_number].z > modelPosition.z + 10)
                {
                    for (int j = i; j < max_tree_number - 1; j++)
                    {
                        trees[j] = trees[j + 1];
                    }
                    tree_number--;
                    i--;
                }
                else
                {
                    DrawModelEx(tree, (Vector3) { trees[i].x, trees[i].y, trees[i].z }, (Vector3) { 0.0f, 1.0f, 0.0f }, 180.0f, treeSize, WHITE);
                }
            }

            if (timeSinceLastSpawn3 >= 1.5f && bush_num < max_bush_num)
            {
                bush[bush_num].z = modelPosition.z - 30.0f;
                bush[bush_num].x = GetRandomValue(-15, 15);
                if (bush[bush_num].x >= -4 && bush[bush_num].x <= 4)
                {
                    bush[bush_num].x + 8;
                }
                bush[bush_num].y = -1;
                bush_num++;
                timeSinceLastSpawn3 = 0.0f;
            }

            for (int i = 0; i < bush_num; i++)
            {
                if (bush[bush_num].z > modelPosition.z + 10)
                {
                    for (int j = i; j < max_bush_num - 1; j++)
                    {
                        bush[j] = bush[j + 1];
                    }
                    bush_num--;
                    i--;
                }
                else
                {
                    DrawModelEx(bushes, (Vector3) { bush[i].x, bush[i].y, bush[i].z }, (Vector3) { 0.0f, 1.0f, 0.0f }, 180.0f, bush_size, WHITE);
                }
            }



            DrawModelEx(tunnel, tunnel_position, (Vector3) { 0.0f, 1.0f, 0.0f }, 90.0f, tunnel_size, WHITE);
            DrawModelEx(model, modelPosition, (Vector3) { 1.0f, 0.0f, 0.0f }, -90.0f, modelSize, WHITE);

            DrawCube(cube_position, cubeSýze.x, cubeSýze.y, cubeSýze.z, DARKGRAY);
            DrawCube((Vector3) { -3, cube_position.y, cube_position.z }, cubeSýze.x / 12, cubeSýze.y, cubeSýze.z, YELLOW);
            DrawCube((Vector3) { 3, cube_position.y, cube_position.z }, cubeSýze.x / 12, cubeSýze.y, cubeSýze.z, YELLOW);
            DrawPlane(plane_pos, (Vector2) { 30, 100 }, DARKGREEN);
            DrawPlane(right_plane_pos, (Vector2) { 30, 100 }, DARKGREEN);

            modelPosition.z -= SPEED;
            camera.position.z -= SPEED;
            camera.target.z -= SPEED;
            cube_position.z -= SPEED;
            plane_pos.z -= SPEED;
            right_plane_pos.z -= SPEED;
            SPEED += 0.00015;

        }

        break;

        case ENDING:
        {

            DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
            DrawText("RAPID RUSH", 20, 20, 40, DARKBLUE);
            DrawText("GAME OVER", 320, 150, 30, RED);
            DrawText("PRESS ENTER to RETURN to TITLE", 210, 220, 20, DARKBLUE);

        } break;
        default:
            break;
        }

        EndMode3D();
        EndDrawing();

    }

    UnloadTexture(texture);
    UnloadModelAnimations(anims, animsCount);
    UnloadModel(model);
    UnloadModel(model1);
    UnloadModel(tree);
    UnloadModel(bushes);
    UnloadModel(tunnel);

    CloseWindow();


    return 0;

}
