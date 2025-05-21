#include "raylib.h"
#include <stdio.h>

#define GridHeight 100
#define GridWidth 100
#define CellSize 10

typedef enum { AIR, SAND, STONE } Element;

Vector2 GetMousePositionInGrid();
Color CycleElement(Element* currentElement);
void DrawSand();
void UpdateSand();
void GenerateWithRadius(int, Element);
void set(int x, int y, Element element);
void swap(Element* element1, Element* element2);
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
  Color ballColor = BEIGE;
  int cursorRadius = 5;
  Element currentElement=SAND;
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

    ballPosition = GetMousePosition();
    cursorRadius += (int)(GetMouseWheelMove());
    if (cursorRadius<1){
      cursorRadius=1;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      ballColor=CycleElement(&currentElement);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      mousePressed = true;
    }
    if (mousePressed) {
      GenerateWithRadius(cursorRadius, currentElement);
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
void swap(Element* element1, Element* element2) {
  Element temp =*element1;
  *element1=*element2;
  *element2=temp;
}
void clear() {
  for (int i = 0; i < GridWidth; i++) {
    for (int j = 0; j < GridHeight; j++) {
      if (i == 0 || i == GridWidth - 1) {
        grid[i][j] = STONE;
      } else if (j == 0 || j == GridHeight - 1) {
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
      if (GetRandomValue(0, 1) == 1) {
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
      Element* currentCell=&grid[i][j];
      Element* belowCell=&grid[i][j+1];
      Element* belowLCell=&grid[i-1][j+1];
      Element* belowRCell=&grid[i+1][j+1];
      switch (*currentCell) {
      case SAND:
        if (IsMovable(*belowCell)) {
          swap(currentCell, belowCell);
        } else if (IsMovable(*belowLCell)) {
          swap(currentCell, belowLCell);
        } else if (IsMovable(*belowRCell)) {
          swap(currentCell, belowRCell);
          break;
        }
      }
    }
  }
}

Color GetElementColor(Element element){
  switch (element) {
    case SAND:
      return BEIGE;
    case STONE:
      return GRAY;
    case AIR:
      return LIGHTGRAY;
}
}

Color CycleElement(Element* currentElement){
  switch (*currentElement) {
    case SAND:
      *currentElement=STONE;
      break;
    case STONE:
      *currentElement=AIR;
      break;
    case AIR:
      *currentElement=SAND;
      break;
  }
  return GetElementColor(*currentElement);
}
