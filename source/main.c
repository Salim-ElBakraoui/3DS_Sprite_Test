#include <citro2d.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH        400
#define SCREEN_HEIGHT       240

#define COLOR_RED           0xFF0000FF
#define COLOR_BLUE          0xFFFF0000
#define COLOR_GREEN         0xFF00FF00
#define COLOR_WHITE         0xFFFFFFFF
#define COLOR_CYAN          0xFFFFFF00
#define COLOR_FUSCHIA       0xFFFF00FF
#define COLOR_YELLOW        0xFF00FFFF


#define mABGR(a,b,g,r)          ( ((u32)a<<24)|\
                                  ((u32)b<<16)|\
                                  ((u32)g<< 8)|\
                                  ((u32)r<< 0)  )

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

static void _DrawBackground(u32 colorTopLeft, u32 colorTopRight, u32 colorBotLeft, u32 colorBotRight){
        C2D_DrawRectangle(0,0,0,SCREEN_WIDTH, SCREEN_HEIGHT, colorTopLeft, colorTopRight, colorBotLeft, colorBotRight);
}

int main(int argc, char*argv[]){
    /*--------------------------------- LIBRARY INITIALIZATION ---------------------------------*/

    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    u32 colorOne, colorTwo;
    colorOne = COLOR_YELLOW;
    colorTwo = COLOR_RED;
    
    u32 colorSpeed = 0x5;
    u32 colorTransition = 0x00;
    int colorAsc = 1;
    
    C3D_RenderTarget* pTopScrRender = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    /*----------------------------------- CORE PROCESS TASKS -----------------------------------*/

    int nbCount = 0;
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
        C2D_TargetClear(pTopScrRender, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f)); //Clears the screen 2 white
        C2D_SceneBegin(pTopScrRender);
        
        
        switch(colorAsc){
        case 1:
            colorTransition+=colorSpeed;
            colorOne = mABGR(0xFF,0x00,(0xFF-colorTransition),0xFF);
            colorTwo = mABGR(0xFF,0x00,0x00,0xFF);
            printf("STEP 1\n");
            break;
        case 2:
            colorTransition-=colorSpeed;
            colorOne = mABGR(0xFF,0x00,(0xFF-colorTransition),0xFF);
            colorTwo = mABGR(0xFF,0x00,0x00,0xFF);
            printf("STEP 2\n");
            break;
        case 3:
            colorTransition+=colorSpeed;
            colorOne = mABGR(0xFF,0x00,(0xFF-colorTransition),0xFF);
            colorTwo = mABGR(0xFF,0x00,(colorTransition),0xFF);
            printf("STEP 3\n");
            break;
        case 4:
            colorTransition-=colorSpeed;
            colorOne = mABGR(0xFF,0x00,colorTransition,0xFF);
            colorTwo = mABGR(0xFF,0x00,colorTransition,0xFF);
            printf("STEP 4\n");
            break;
        case 5:
            colorAsc=0;
            break;
        }
        
        if(!(colorTransition%255)) colorAsc++;

                
        //printf("\rColorOne: %lx\tColorTwo: %lx", colorOne, colorTwo);
        
        _DrawBackground(colorOne, colorTwo, colorTwo, colorOne);
        C2D_DrawSprite(&cSprite);
        C3D_FrameEnd(0);
        nbCount++;

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
