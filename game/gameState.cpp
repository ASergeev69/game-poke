#include "gameState.h"
#include "raylib.h"

GameScreen currentScreen = SCREEN_MENU;
GameScreen nextScreen = SCREEN_MENU;

bool transitioning = false;
bool showSettingsWindow = false;

bool selectedPokemons[10] = { false };

float transitionAlpha = 0.0f;
float transitionSpeed = 0.05f;
float musicVolume = 0.5f;

int difficultyActive = 1;
int difficultyChoice = 0;

void StartTransition(GameScreen target) {
    transitioning = true;
    transitionAlpha = 0.0f;
    nextScreen = target;
}

void DrawTransition() {
    if (transitioning) {
        transitionAlpha += transitionSpeed;
        if (transitionAlpha >= 1.0f) {
            transitionAlpha = 1.0f;
            currentScreen = nextScreen;
            transitioning = false;
        }
    }
    else if (transitionAlpha > 0.0f) {
        transitionAlpha -= transitionSpeed;
        if (transitionAlpha < 0.0f) transitionAlpha = 0.0f;
    }

    if (transitionAlpha > 0.0f)
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
            Color{ 0, 0, 0, (unsigned char)(transitionAlpha * 255) });
}
