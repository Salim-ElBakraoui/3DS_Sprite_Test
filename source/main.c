#include <citro2d.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH        400
#define SCREEN_HEIGHT       240

/*--------------------------------- GRAPHICS INITIALIZATION ---------------------------------*/
static C2D_SpriteSheet cSpriteSheet;
static C2D_Sprite      cSprite;
static size_t sizeNbSprites;
static u8 uGuileSwap;

static void _SpriteInitialization(){
    srand((unsigned int)time(NULL));
    sizeNbSprites=C2D_SpriteSheetCount(cSpriteSheet);
    C2D_SpriteFromSheet(&cSprite, cSpriteSheet, uGuileSwap); //Loads a random sprite
    C2D_SpriteSetCenter(&cSprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&cSprite, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

static void _GuileSwap(){
    uGuileSwap = !uGuileSwap;
    C2D_SpriteFromSheet(&cSprite, cSpriteSheet, uGuileSwap);
    C2D_SpriteSetCenter(&cSprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&cSprite, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

int main(int argc, char*argv[]){
    /*--------------------------------- LIBRARY INITIALIZATION ---------------------------------*/

    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    C3D_RenderTarget* pTopScrRender = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    /*----------------------------------- CORE PROCESS TASKS -----------------------------------*/

    printf("Bienvenue dans mon application !\n"); //Welcome message showing on the bottom screen

    cSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x"); //Loads the spritesheet
    if (!cSpriteSheet) svcBreak(USERBREAK_PANIC);

    _SpriteInitialization();

    printf("Nombre de sprites dans la spritesheet %d\n", sizeNbSprites);

    while(aptMainLoop()){

        hidScanInput(); //Scanning incoming inputs

        u32 kDown = hidKeysDown(); //Scanning key presses

        if(kDown & KEY_START)
            break; //If start is pressed, we'll go back to the Homebrew Menu.
        if(kDown & KEY_A)
            _GuileSwap();

        //---Scene Rendering--//
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(pTopScrRender, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));//Clears the screen 2 white
        C2D_SceneBegin(pTopScrRender);
        C2D_DrawSprite(&cSprite);
        C3D_FrameEnd(0);
    }

    /*-------------------------------- APPLICATION EXIT PROCESS --------------------------------*/

    //Graphics Deinitialization
    C2D_SpriteSheetFree(cSpriteSheet);

    //Library Deinitialization
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();

    return 0;
}
