//Copyright © 2026 Luigi Corvasce

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int Ncasuale(int min, int max);

int main() {
    // Forza la disattivazione di alcune funzioni che possono causare GLXBadFBConfig
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); 
    InitWindow(800, 450, "Flappy Shooter");
    InitAudioDevice();
    SetTargetFPS(60);
    int punteggio = 0, punteggiomax = -1, ripetizioni = 0;
    float flappy_x = 200, flappy_y = 200, *rectFlappy_y = &flappy_y, *rectFlappy_x = &flappy_x;
    float flappy2_x = 650, flappy2_y = 230, *rectFlappy2_y = &flappy2_y, *rectFlappy2_x = &flappy2_x;
    float frecciaD_x = flappy_x, frecciaD_y = flappy_y;
    float timer, contoallarovescia = 25.0, istante, tempo = 0.0;
    float hit_x = 1000, hit_y = 1000;
    bool frecciaD_visibile = false;
    bool sparatoD = false;
    bool stop = false;
    bool conferma = false;

    Texture2D flappy = LoadTexture("assets/img/flappy.png");
    Texture2D flappy2 = LoadTexture("assets/img/flappy.png");
    Texture2D frecciaD = LoadTexture("assets/img/frecciaD.png");
    Texture2D sfondo = LoadTexture("assets/img/flappySfondo.png");
    Texture2D X = LoadTexture("assets/img/x.png");
    Sound sparo = LoadSound("assets/sounds/sparo.mp3");

    while (!WindowShouldClose()) {
        BeginDrawing();
            if(conferma){
                timer += GetFrameTime();
                tempo += GetFrameTime();
                contoallarovescia -= GetFrameTime();
                
                if(timer >= 2.0){
                    flappy2_x = Ncasuale(0, 650);
                    flappy2_y = Ncasuale(-100, 230);
                    timer = 0.0;
                }
                
                if(contoallarovescia <= 0.0){
                    stop = true;
                }
                if(!stop){
                    ClearBackground(RAYWHITE);
                    DrawTexture(sfondo, 0, 0, WHITE);
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
                        PlaySound(sparo);
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
                        istante = tempo;
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
                        contoallarovescia++;
                        sparatoD = false;
                        flappy2_x = Ncasuale(0, 650);
                        flappy2_y = Ncasuale(-100, 230);
                        timer = 0.0;
                    }
                    if(flappy_x <= -5){
                        flappy_x = -5;
                    }
                    if(flappy_x >= 650){
                        flappy_x = 650;
                    }
                    if(flappy_y <= -100){
                        flappy_y = -100;
                    }
                    if(flappy_y >= 230){
                        flappy_y = 230;
                    }
                }else{
                    ClearBackground(RAYWHITE);
                    if(punteggio > punteggiomax){
                        punteggiomax = punteggio;
                    }
                    DrawText("Tempo scaduto!", 180, 120, 50, RED);
                    DrawText(TextFormat("Punti totalizzati: %d", punteggio), 180, 170, 50, RED);
                    DrawText(TextFormat("Miglior punteggio: %d",punteggiomax), 170, 215, 50, RED);
                    Rectangle Restart = { 
                        252 ,     
                        350, 
                        250, 
                        50
                    };
                    Vector2 mousePos = GetMousePosition();
                    bool CollRst = CheckCollisionPointRec(mousePos, Restart);
                    DrawRectangleRec(Restart, BLACK);
                    DrawText("Restart", 280, 350, 50, WHITE);
                    if (CollRst && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        contoallarovescia = 25.0;
                        punteggio = 0;
                        stop = false;
                    }
                }
            }else{
                ClearBackground(YELLOW);
                DrawText("Per muoversi usare WASD", 145, 180, 40, RED);
                DrawText("e per sparare usare SPAZIO", 115, 215, 40, RED);
                DrawTexture(X, 720, 20, WHITE);
                Rectangle OK = { 
                    720 ,     
                    20, 
                    50, 
                    50
                };
                //DrawRectangleRec(OK, BLACK);
                Vector2 mousePos = GetMousePosition();
                bool CollOK = CheckCollisionPointRec(mousePos, OK);
                if (CollOK && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    conferma = true;
                }
            }
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

int Ncasuale(int min, int max) {
    return rand() % (max - min + 1) + min;
}

