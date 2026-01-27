#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int Ncasuale(int min, int max);

int main() {
    // Forza la disattivazione di alcune funzioni che possono causare GLXBadFBConfig
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); 
    InitWindow(800, 450, "Flappy Shooter");
    SetTargetFPS(60);
    int i;
    int punteggio = 0;
    float flappy_x = 200, flappy_y = 200, *rectFlappy_y = &flappy_y, *rectFlappy_x = &flappy_x;
    float flappy2_x = 200, flappy2_y = 200, *rectFlappy2_y = &flappy2_y, *rectFlappy2_x = &flappy2_x;
    float frecciaD_x = flappy_x, frecciaD_y = flappy_y;
    float timer, contoallarovescia = 25.0;
    float hit_x = 1000, hit_y = 1000;
    bool frecciaD_visibile = false;
    bool sparatoD = false;
    bool stop = false;

    Texture2D flappy = LoadTexture("img/flappy.png");
    Texture2D flappy2 = LoadTexture("img/flappy.png");
    Texture2D frecciaD = LoadTexture("img/frecciaD.png");
    Texture2D sfondo = LoadTexture("img/flappySfondo.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
            timer += GetFrameTime();
            contoallarovescia -= GetFrameTime();
            
            if(timer >= 2.0){
                flappy2_x = Ncasuale(-30, 700);
                flappy2_y = Ncasuale(-100, 200);
                timer = 0.0;
            }
            
            if(contoallarovescia <= 0.0){
                stop = true;
            }
            if(!stop){
              ClearBackground(RAYWHITE);
              DrawTexture(sfondo, 0, 0, WHITE);
              if (sfondo.id == 0) {
                  printf("ATTENZIONE: Impossibile caricare img/flappySfondo.png\n");
              } else {
                  printf("Sfondo caricato correttamente! ID: %u - Dim: %dx%d\n", sfondo.id, sfondo.width, sfondo.height);
              }
              DrawTexture(flappy, flappy_x, flappy_y, WHITE);
              DrawTexture(flappy2, flappy2_x, flappy2_y, WHITE);
              DrawText(TextFormat("Punti: %d", punteggio), 10, 10, 20, BLACK);
              DrawText(TextFormat("Tempo rimasto: %.2f", contoallarovescia), 10, 50, 20, BLACK);

              
              Rectangle hitboxFrecciaD = { 
                  hit_x ,     
                  hit_y + 147, 
                  (float)frecciaD.width - 160, 
                  (float)frecciaD.height - 150
              };
              //DrawRectangleRec(hitboxFrecciaD, BLACK);

              if(sparatoD){
                  DrawText("Ricarica...", 10, 30, 20, RED);
                  hit_x = frecciaD_x;
                  hit_y = frecciaD_y;
                  frecciaD_x = frecciaD_x + 5;
                  Vector2 frecciaD_xy = {frecciaD_x, frecciaD_y + 115};
                  DrawTextureEx(frecciaD, frecciaD_xy, 0, 0.5, WHITE);
                  if(frecciaD_x >= 800){
                      sparatoD = false;
                  }
              }else{
                  DrawText("Colpo disponibile", 10, 30, 20, BLACK);
                  frecciaD_x = flappy_x;
                  frecciaD_y = flappy_y;
                  hit_x = 1000;
                  hit_y = 1000;
              }

              Rectangle hitboxFlappy = { 
                  *rectFlappy_x + 30,     
                  *rectFlappy_y + 120, 
                  (float)flappy.width - 130, 
                  (float)flappy.height - 150
              };
              Rectangle hitboxFlappy2 = { 
                  *rectFlappy2_x + 30, 
                  *rectFlappy2_y + 120, 
                  (float)flappy2.width - 130,     
                  (float)flappy2.height -150
              };
              //DrawRectangleRec(hitboxFlappy2, BLACK);
              if (IsKeyPressed(KEY_SPACE)) {
                  sparatoD = true;
              }
              if(IsKeyDown(KEY_D) == true){
                  flappy_x = flappy_x + 5;
              }
              if(IsKeyDown(KEY_A) == true){
                  flappy_x = flappy_x - 5;
              }
              if(IsKeyDown(KEY_W) == true){
                  flappy_y = flappy_y - 5;
              }
              if(IsKeyDown(KEY_S) == true){
                  flappy_y = flappy_y + 5;
              }
              if (CheckCollisionRecs(hitboxFrecciaD, hitboxFlappy2)) {
                  punteggio++;
                  sparatoD = false;
                  flappy2_x = Ncasuale(0, 800 - 50); 
                  flappy2_y = Ncasuale(0, 450 - 50);
                  timer = 0.0;
              }
            }else{
                ClearBackground(RAYWHITE);
                DrawText("Tempo scaduto!", 180, 165, 50, RED);
                DrawText(TextFormat("Punti totalizzati: %d", punteggio), 180, 215, 50, RED);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

int Ncasuale(int min, int max) {
    return rand() % (max - min + 1) + min;
}
