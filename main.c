#include "./libs/map.h"
#include "math.h"
#include "raylib.h"
#include "stdio.h"
const int screenWidth = 800, screenHeight = 600;

enum Direction { N, S, W, E };

Camera camera = {0};

char Direction[100];

void move(int, int);
void drawMap();

int main() {
    InitWindow(screenWidth, screenHeight, "shooter");
    const char* file = "test/test.map";

    loadMap(file);
    camera.position = (Vector3){0.0f, 2.0f, 0.0f};
    camera.target = (Vector3){-3.62f, 2.0f, 1.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
    // HideCursor();

    SetCameraMode(camera,
                  CAMERA_FIRST_PERSON);  // Set a first person camera mode
    SetCameraMoveControls(0, 0, 0, 0, 0, 0);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_W)) {
            move(KEY_W, N);
        }

        if (IsKeyDown(KEY_A)) {
            move(KEY_A, W);
        }
        if (IsKeyDown(KEY_D)) {
            move(KEY_D, E);
        }
        if (IsKeyDown(KEY_S)) {
            move(KEY_S, S);
        }

        UpdateCamera(&camera);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
                  LIGHTGRAY);  // Draw ground
        DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
                 BLUE);  // Draw a blue wall
        DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
                 LIME);  // Draw a green wall
        DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f,
                 GOLD);  // Draw a yellow wall

        drawMap();

        EndMode3D();
        DrawRectangle(0, 0, 220, 70, WHITE);
        DrawRectangleLines(0, 0, 220, 70, BLUE);

        char X[100], Y[100], Z[100];
        sprintf(X, "X: %.2f", camera.target.x);
        sprintf(Y, "Y: %.2f", camera.target.y);
        sprintf(Z, "Z: %.2f", camera.target.z);
        DrawText(X, 10, 10, 10, BLACK);
        DrawText(Y, 10, 30, 10, BLACK);
        DrawText(Z, 10, 50, 10, BLACK);
        char X1[100], Y1[100], Z1[100];
        sprintf(X1, "X: %.2f", camera.position.x);
        sprintf(Y1, "Y: %.2f", camera.position.y);
        sprintf(Z1, "Z: %.2f", camera.position.z);
        DrawText(X1, 50, 10, 10, BLACK);
        DrawText(Y1, 50, 30, 10, BLACK);
        DrawText(Z1, 50, 50, 10, BLACK);
        DrawText(Direction, 100, 10, 10, BLACK);

        EndDrawing();
    }

    CloseWindow();  // Close window and OpenGL context

    return 0;
}

void move(int key, int dir) {
    static float Ndist = 0;
    static float Sdist = 0;
    static float Wdist = 0;
    static float Edist = 0;
    const float maxSpeed = 0.3f;
    const float advance = 0.05f;

    if (IsKeyReleased(key)) {
        switch (dir) {
            case N:
                Ndist = 0;
                break;
            case W:
                Wdist = 0;
                break;
            case S:
                Sdist = 0;
                break;
            case E:
                Edist = 0;
                break;
        }
    }

    float dist = 0;
    switch (dir) {
        case N:
            dist = Ndist += Ndist < maxSpeed ? advance : 0;
            Sdist = 0;
            break;
        case S:
            dist = Sdist -= Sdist > -maxSpeed ? advance : 0;
            Ndist = 0;
            break;
        case E:
            dist = Edist -= Edist > -maxSpeed ? advance : 0;
            Wdist = 0;
            break;
        case W:
            dist = Wdist += Wdist < maxSpeed ? advance : 0;
            Edist = 0;
            break;
    }

    float actualX = camera.target.x - camera.position.x;
    float actualZ = camera.target.z - camera.position.z;
    if (dir > S) {
        const float temp = actualX;
        actualX = actualZ;
        actualZ = -temp;
    }
    if (dist > maxSpeed)
        dist = maxSpeed;
    else if (dist < -maxSpeed)
        dist = -maxSpeed;

    const float total = fabs(actualX) + fabs(actualZ);

    camera.position.x += (actualX * dist) / total;
    camera.position.z += (actualZ * dist) / total;

    sprintf(Direction, "D: %.2f", dist);
}

void drawMap() {
    if (m_MAP_size == 0)
        return;

    for (int i = 0; i < m_MAP_size; i++) {
        struct s_MESH rn = getsMesh(m_MAP[i]);
        if (rn.type == t_MESH) {
            DrawCube((Vector3){rn.posX, rn.posY, rn.posZ}, rn.width, rn.height,
                     rn.length, (Color){rn.R, rn.G, rn.B, rn.A});
        }
    }
}
