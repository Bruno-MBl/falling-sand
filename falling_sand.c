#include "raylib.h"

#define GridHeight 100
#define GridWidth 100
#define CellSize 10

typedef enum { AIR, SAND, STONE } Element;

Vector2 GetMousePositionInGrid();
void DrawSand();
void UpdateSand();
void GenerateWithRadius(int, Element);
void set(int x, int y, Element element);
void swap(int x1, int y1, int x2, int y2);
void clear();

Element grid[GridWidth][GridHeight];
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = GridWidth * CellSize;
  const int screenHeight = GridHeight * CellSize;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "Sand falling test by Bruno");

  Vector2 ballPosition = {-100.0f, -100.0f};
  Color ballColor = DARKBLUE;
  int cursorRadius = 5;
  bool mousePressed = false;

  clear();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  HideCursor();
  //---------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    UpdateSand();

    float delta_time = GetFrameTime();
    ballPosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      mousePressed = true;
    }
    if (mousePressed) {
      GenerateWithRadius(cursorRadius, SAND);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      mousePressed = false;
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawSand();

    DrawCircleLinesV(ballPosition, cursorRadius, ballColor);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
void set(int x, int y, Element element) { grid[x][y] = element; }
void swap(int x1, int y1, int x2, int y2) {
  Element temp = grid[x1][y1];
  grid[x1][y1] = grid[x2][y2];
  grid[x2][y2] = temp;
}
void clear() {
  for (int i = 0; i < GridWidth; i++) {
    for (int j = 0; j < GridHeight; j++) {
      if (i == 0 || i == GridWidth - 1) {
        grid[i][j] = STONE;
      } else if (j == 0 || j == GridHeight) {
        grid[i][j] = STONE;
      } else {
        grid[i][j] = AIR;
      }
    }
  }
}
Vector2 GetMousePositionInGrid() {
  Vector2 mousePosition = GetMousePosition();
  return (Vector2){mousePosition.x / CellSize, mousePosition.y / CellSize};
}

// TODO: Cambiar funcion para que solo calcule dentro del grid
void GenerateWithRadius(int radius, Element element) {
  Vector2 mouse = GetMousePosition();
  int grid_x = mouse.x;
  int grid_y = mouse.y;
  for (int i = grid_x - radius; i < grid_x + radius; i++) {
    for (int j = grid_y - radius; j < grid_y + radius; j++) {
      if (GetRandomValue(1, 10) >= 7) {
        set(i / CellSize, j / CellSize, element);
      }
    }
  }
}
void DrawSand() {
  for (int i = 0; i < GridWidth; i++) {
    for (int j = 0; j < GridHeight; j++) {
      switch (grid[i][j]) {
      case SAND:
        DrawRectangle(i * CellSize, j * CellSize, CellSize, CellSize, BEIGE);
        break;
      case STONE:
        DrawRectangle(i * CellSize, j * CellSize, CellSize, CellSize, GRAY);
        break;
      }
    }
  }
}
bool IsMovable(Element element) {
  switch (element) {
  case AIR:
    return true;
  default:
    return false;
  }
}
void UpdateSand() {
  for (int i = 0; i < GridWidth; i++) {
    for (int j = GridHeight - 1; j >= 0; j--) {
      switch (grid[i][j]) {
      case SAND:
        if (IsMovable(grid[i][j + 1])) {
          swap(i, j, i, j + 1);
        } else if (IsMovable(grid[i - 1][j + 1])) {
          swap(i, j, i - 1, j + 1);
        } else if (IsMovable(grid[i + 1][j + 1])) {
          swap(i, j, i + 1, j + 1);
          break;
        }
      }
    }
  }
}
