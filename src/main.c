#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include "physics/body/body.h"
#include "physics/world/world.h"

<<<<<<< HEAD
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SUBSTEPS 50
=======
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SUBSTEPS 5
>>>>>>> 8b31124 (a)

int main(int argc, char** argv) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plunyisto");

    World world = CreateWorld();
    AddWorldBounds(&world, (Vector2){ 30.0f, 30.0f }, (float)SCREEN_WIDTH - 60.0f, (float)SCREEN_HEIGHT - 60.0f);
/*
    AddBodyToWorld(
        &world,
        CreateSoftBodyCircle(
<<<<<<< HEAD
            (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f }, 
            12, 
=======
            (Vector2){ (SCREEN_WIDTH / 2.0f) - 100.0f, SCREEN_HEIGHT / 2.0f }, 
            150, 
>>>>>>> 8b31124 (a)
            100.0f, 
            20.0f, 
            (Vector2){ 1000.0f, 0.0f }, 
            5.0f, 
            true, 
            true
        )
    );

    AddBodyToWorld(
        &world,
<<<<<<< HEAD
        CreateSoftBodyChain(
            (Vector2){ (SCREEN_WIDTH / 2.0f) - 300.0f, SCREEN_HEIGHT * 0.75f },
            (Vector2){(SCREEN_WIDTH / 2.0f) + 100.0f, SCREEN_HEIGHT * 0.75f },
            10,
            10.0f,
            Vector2Zero(), 
            1000.0f, 
            false
        )
    );
    */

    AddBodyToWorld(
        &world,
        CreateSoftBodyGrid(
            (Vector2){ (SCREEN_WIDTH * 0.02), SCREEN_HEIGHT  * 0.05f },
            16,
            9,
            70.0f,
            8.0f,
            Vector2Zero(),
            10000.0f,
            true,
            false
        )
    );

=======
        CreateSoftBodyCircle(
            (Vector2){ (SCREEN_WIDTH / 2.0f) + 100.0f, SCREEN_HEIGHT / 2.0f }, 
            150, 
            100.0f, 
            20.0f, 
            (Vector2){ 1000.0f, 0.0f }, 
            10.0f, 
            true, 
            true
        )
    );


>>>>>>> 8b31124 (a)
    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        for (int i = 0; i < SUBSTEPS; i++) {
            ResolveWorldCollisions(&world);
            UpdateWorld(&world, delta / SUBSTEPS);
        }

        printf("Collision Checks: %lld\n", collisionChecks);
        collisionChecks = 0;

        BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawFPS(10.0f, 10.0f);

            DrawWorldSegments(&world);
            DrawWorldBodies(&world);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}