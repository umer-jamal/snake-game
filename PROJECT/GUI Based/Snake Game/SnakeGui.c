#include <stdio.h>
#include "raylib.h"
#include <time.h>
#include<stdlib.h>
void display();
void update();
void generator();
void logic();
void input();
void showOpeningScreen();
struct body{
    int x[100];
    int y[100];
};
struct head{
    int x;
    int y;
};
struct body tail;
struct head h;

char inp, *text, *textm;
const int mx = 50, my = 40;
int len = 0, ex = 1, checkinp = 8, fx, fy, score = 0;
Texture2D fruit, hu, hd, hl, hr, openingImage;
Color lightGreen = (Color){0xA9, 0xD1, 0x6A, 0xFF};
Color darkGreen = (Color){0x5C, 0x8B, 0x44, 0xFF};
Color grassyGreen = (Color){ 124, 252, 0, 255 }; 
Color lightGray = (Color){0xD3, 0xD3, 0xD3, 0xFF};
Color darkGray = (Color){0x7B, 0xA5, 0x5C, 0xFF};
Sound point, highScore, gameOver, turn;
int main() {
    h.x=mx/2;
    h.y=my/2;
    text=(char*)malloc(20*sizeof(char));
    textm=(char*)malloc(20*sizeof(char));
    srand(time(NULL));
    //Fetching previous high score from Score.txt file
    int max=0;
    FILE *maxScore=fopen("Score.txt","r");
//    char ch;
  //  while((ch=fgetc(maxScore))!=EOF){
    //    int temp=ch;
     //   max= max*10 + (temp-'0');
   // }
    fscanf(maxScore,"%d",&max);
    fclose(maxScore);
    inp = 'w';//Initializing default motion of the snake
    sprintf(textm,"High score : %d",max);
    sprintf(text, "Score : %d", score);
    generator();  //Generating fruit position  
    //Initializing windows and audio device
    InitWindow((mx + 1) * 20, (my + 10) * 20, "Snake Game");
    SetTargetFPS(60);
    InitAudioDevice();
    //Loading all sound data from the folder
    point=LoadSound("food.wav");
    highScore=LoadSound("highscore.wav");
    gameOver=LoadSound("gameover.wav");
    turn=LoadSound("move.wav");
    //Setting necessary volume
    SetSoundVolume(point, 1.0f);
    SetSoundVolume(highScore, 1.0f);
    SetSoundVolume(turn, 0.1f);
    //Loading textures/images that are to be used
    fruit = LoadTexture("apple.png");
    hu = LoadTexture("snakeheadU.png");
    hd = LoadTexture("snakeheadD.png");
    hl = LoadTexture("snakeheadL.png");
    hr = LoadTexture("snakeheadR.png");
    openingImage = LoadTexture("Opening.png");     
    showOpeningScreen();//Opening screen before game
    //PlaySound(bgmusic);
    int soundcount=0;//This variable was made to ensure that the gameover sound only plays one times
    //Main Loop for game processing
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(grassyGreen);
        if(ex){
            display();
            Sleep(120 - score);
            input();
            update();
            logic();
        }else{
            if(!soundcount){
                PlaySound(gameOver);
                Sleep(1000);
            }
            if(score<=max){
            DrawText(text,mx*8,my*12,40,BLACK);
            }else{
                if(!soundcount){
                    PlaySound(highScore);
                }
            DrawText("NEW HIGH SCORE!",mx*7,my*10,40,BLACK);
            DrawText(text,mx*8,my*13,40,BLACK);
            }
            soundcount++;
        }
        EndDrawing();
    }
    //Updating high score
    if(score>max){
        FILE *newScore=fopen("Score.txt","w");
        fprintf(newScore,"%d",score);
        fclose(newScore);
    }
    //Unloading all resources
    UnloadTexture(fruit);
    UnloadTexture(hu);
    UnloadTexture(hd);
    UnloadTexture(hl);
    UnloadTexture(hr);
    UnloadTexture(openingImage);
    UnloadSound(point);
    UnloadSound(highScore);
    UnloadSound(gameOver);
    CloseAudioDevice();
    CloseWindow();  
    return 0;
}

void showOpeningScreen() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        Vector2 position = {(float)(GetScreenWidth() - openingImage.width) / 2, (float)(GetScreenHeight() - openingImage.height) / 2};
        DrawTexture(openingImage, position.x, position.y, WHITE);
        DrawText("Click anywhere or press any key to start", 240, GetScreenHeight() - 100, 20, RAYWHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            break;  
        }
        EndDrawing();
    }
}

void display() {
    //To produce checkers pattern
    for(int i=1;i<mx;i++){
        for(int j=1;j<my;j++){
            if((i+j)%2)         DrawRectangle(i*20, j * 20, 20, 20, lightGreen);
        }
    }
    if (fruit.width > 0 && fruit.height > 0) {
        Rectangle sourceRect = {0, 0, (float)fruit.width, (float)fruit.height};
        Rectangle destRect = {fx * 20 - 10, fy * 20 - 10, 40, 40};  
        DrawTexturePro(fruit, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawRectangle(fx * 20, fy * 20, 20, 20, DARKGRAY);
    }
    Rectangle sourceRect = {0, 0, (float)hu.width, (float)hu.height};
    Rectangle destRect = {h.x * 20 - 10, h.y * 20 - 10, 40, 40};
    switch (inp) {
        case 'w':
            DrawTexturePro(hu, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        case 'a':
            DrawTexturePro(hl, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        case 'd':
            DrawTexturePro(hr, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        case 's':
            DrawTexturePro(hd, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
    }
    // Showing body
    for (int i = 0; i < len; i++) {
        DrawCircle(tail.x[i] * 20 + 10, tail.y[i] * 20 + 10, 12, darkGreen);
    }
    //Drawing borders
    for (int i = 0; i <= my; i++) {
        DrawRectangle(0, i * 20, 20, 20, BLACK);
        DrawRectangle(mx * 20, i * 20, 20, 20, BLACK);
    }
    for (int i = 0; i <= mx; i++) {
        DrawRectangle(i * 20, 0, 20, 20, BLACK);
        DrawRectangle(i * 20, my * 20, 20, 20, BLACK);
    }
    //Drawing scores
    DrawText(text, mx * 13, (my + 3) * 20, 25, BLACK);
    DrawText(textm, mx * 3, (my + 3) * 20, 25, BLACK);
}

void update() {
    //Updating tail/body co-ordinates
    for (int i = len; i > 0; i--) {
        tail.x[i - 1] = tail.x[i - 2];
        tail.y[i - 1] = tail.y[i - 2];
    }
    tail.x[0] = h.x;
    tail.y[0] = h.y;
    //Updating head co-ordinates
    switch (inp) {
        case 'w': {
            if (checkinp != 2) {
                if(checkinp !=8)    PlaySound(turn);
                checkinp = 8;
                h.y--;
            } else {
                inp = 's';
                h.y++;
            }
            break;
        }
        case 'a': {
            if (checkinp != 6) {
                if(checkinp !=4)    PlaySound(turn);
                h.x--;
                checkinp = 4;
            } else {
                inp = 'd';
                h.x++;
            }
            break;
        }
        case 'd': {
            if (checkinp != 4) {
                if(checkinp !=6)    PlaySound(turn);
                h.x++;
                checkinp = 6;
            } else {
                h.x--;
                inp = 'a';
            }
            break;
        }
        case 's': {
            if (checkinp != 8) {
                if(checkinp !=2)    PlaySound(turn);
                h.y++;
                checkinp = 2;
            } else {
                h.y--;
                inp = 'w';
            }
            break;
        }
        case 'q': {
            ex = 0;
            break;
        }
    }
}

void generator() {
B:
    int tempx=fx, tempy=fy;
    do {
        fx = rand() % (mx - 1);
        fy = rand() % (my - 1);
    } while (fx <= 1 || fy <= 1);
    if(fx==tempx && fy==tempy)  goto B;
    for (int i = 0; i < len; i++) {
        if (tail.x[i] == fx && tail.y[i] == fy) {
            goto B;
        }
    }
}

void logic() {
    if (!h.x || !h.y || h.y >= my || h.x >= mx) ex = 0; //Checking if head hit any boundary
    //Checking if head hits its body
    for (int i = 0; i < len; i++) {
        if (tail.x[i] == h.x && tail.y[i] == h.y) {
            ex = 0;
            break;
        }
    }
    //if(!ex) PauseSound(bgmusic);
    if (h.x == fx && h.y == fy) {
        score++;
        PlaySound(point);
        len++;
        sprintf(text, "Score : %d", score);
        generator();
    }
}
//checking input to update inp variable
void input() {
    if (IsKeyDown(KEY_UP)) {
        inp = 'w';
    } else if (IsKeyDown(KEY_LEFT)) {
        inp = 'a';
    } else if (IsKeyDown(KEY_RIGHT)) {
        inp = 'd';
    } else if (IsKeyDown(KEY_DOWN)) {
        inp = 's';
    } else if (IsKeyDown(KEY_Q)) {
        inp = 'q';
    }
}