#include "asteroids.h"
#include "raylib.h"
#include "ScreenLogo.h"
#include "GlobalContainers.h"

#include <ctime>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


void Asteroids::init()
{
    // Initialization
    //---------------------------------------------------------
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    InitWindow(GC::screenWidth, GC::screenHeight, "Asteroids");
    
    InitAudioDevice();      // Initialize audio device
    
    // Load global data (assets that must be available in all screens, i.e. font)
    GC::font = LoadFont("resources/textures/setback.png");
    GC::music = LoadMusicStream("resources/music/MainMenuMusic.ogg");
    GC::fxCoin = LoadSound("resources/coin.wav");
    
    SetMusicVolume(GC::music, 1.0f);
    PlayMusicStream(GC::music);

    m_currentScreen = m_screens[m_currentScreenId];
    auto currentScreen = m_currentScreen.lock();
    currentScreen->init();
}


int Asteroids::run()
{
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(GC::music);       // NOTE: Music keeps playing between screens

        if (m_onTransition)
            updateTransition();    // Update transition (fade-in, fade-out)
        else
        {
            updateTransitionManagement();
        }

        m_currentScreen.lock()->update(GetFrameTime());

        // Render
        BeginDrawing();

        ClearBackground(BLACK);

        m_currentScreen.lock()->render();

        // Draw full screen rectangle in front of everything
        if (m_onTransition) drawTransition();

        //DrawFPS(10, 10);

        EndDrawing();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    m_currentScreen.lock()->unload();
    
    // Unload global data loaded
    UnloadFont(GC::font);
    UnloadMusicStream(GC::music);
    UnloadSound(GC::fxCoin);
    
    CloseAudioDevice();     // Close audio context
    
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}


// Change to next screen, no transition
void Asteroids::changeToScreen(ScreenId screen)
{
    // Unload current screen
    m_screens[m_currentScreenId]->unload();

    // Init next screen
    m_screens[screen]->init();

    m_currentScreenId = screen;
}


// Request transition to next screen
void Asteroids::transitionToScreen(ScreenId screenId)
{
    m_onTransition = true;
    m_transFadeOut = false;
    m_transFromScreen = m_currentScreenId;
    m_transToScreen = m_screens.at(screenId);
    m_transToScreenId = screenId;
    m_transAlpha = 0.0f;
}


void Asteroids::updateTransitionManagement()
{
    if (!m_onTransition)
    {
        std::shared_ptr<Screen> currentScreen = m_currentScreen.lock();
        currentScreen->update(GetFrameTime());
        int finishScreen = currentScreen->finish();

        switch(m_currentScreenId)
        {
            case ScreenId::LOGO:
            {
                if (finishScreen) transitionToScreen(ScreenId::TITLE);
    
            } break;
            case ScreenId::TITLE:
            {
                if (finishScreen == 1) transitionToScreen(ScreenId::OPTIONS);
                else if (finishScreen == 2) transitionToScreen(ScreenId::GAMEPLAY);
    
            } break;
            case ScreenId::OPTIONS:
            {
                if (finishScreen) transitionToScreen(ScreenId::TITLE);
    
            } break;
            case ScreenId::GAMEPLAY:
            {
                if (finishScreen == 1) transitionToScreen(ScreenId::ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);
    
            } break;
            case ScreenId::ENDING:
            {
                if (finishScreen == 1) transitionToScreen(ScreenId::TITLE);
    
            } break;
            default: break;
        }
    }
    else updateTransition();    // Update transition (fade-in, fade-out)
}


void Asteroids::updateTransition(void)
{
    if (!m_transFadeOut)
    {
        m_transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (m_transAlpha > 1.01f)
        {
            m_transAlpha = 1.0f;

            // Unload current screen
            if (std::shared_ptr<Screen> ptrCurrentScreen = m_currentScreen.lock())
            {
                ptrCurrentScreen->unload();
            }

            // Unload current screen
            if (std::shared_ptr<Screen> ptrTransToScreen = m_transToScreen.lock())
            {
                ptrTransToScreen->init();
            }

            m_currentScreen = m_transToScreen;
            m_currentScreenId = m_transToScreenId;

            // Activate fade out effect to next loaded screen
            m_transFadeOut = true;
        }
    }
    else
    {
        m_transAlpha -= 0.02f;

        if (m_transAlpha < -0.01f)
        {
            m_transAlpha = 0.0f;
            m_transFadeOut = false;
            m_onTransition = false;
            m_transFromScreen = ScreenId::UNKNOWN;
            m_transToScreen.reset();
        }
    }
 }


// Draw transition effect (full-screen rectangle)
void Asteroids::drawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, m_transAlpha));
}