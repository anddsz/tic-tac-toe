#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>


unsigned char checkDraw(unsigned char* gameBoard);

int main(int argc, char* argv[]){
    unsigned char* gameBoard = calloc(3 * 3, sizeof(unsigned char));
    unsigned char playerTurn = 1;
    unsigned char gameState = 0;
    
    SDL_Point mousePosition;
    unsigned char mouseLeftClick;

    unsigned char shouldQuit = 0;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 320, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture_x = IMG_LoadTexture(renderer, "res/x.png");
    SDL_Texture* texture_o = IMG_LoadTexture(renderer, "res/o.png");

    SDL_RenderSetLogicalSize(renderer, 16 * 3, 16 * 3);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while(!shouldQuit){
        mouseLeftClick = 0;
        
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_MOUSEBUTTONDOWN:
                    if(!mouseLeftClick && event.button.button == SDL_BUTTON_LEFT){
                        mouseLeftClick = 1;
                    }
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_R){
                        memset(gameBoard, 0, 3 * 3 * sizeof(unsigned char));
                        playerTurn = 1;
                        gameState = 0;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mousePosition.x = event.motion.x;
                    mousePosition.y = event.motion.y;
                    break;
                case SDL_QUIT:
                    shouldQuit = 1;
                    break;
            }
        }

        if(gameState == 0){
            if(mouseLeftClick && gameBoard[(mousePosition.y / 16) * 3 + mousePosition.x / 16] == 0){
                gameBoard[(mousePosition.y / 16)  * 3 + mousePosition.x / 16] = playerTurn;
                playerTurn = (playerTurn == 1) ? 2 : 1;
            }
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if((gameBoard[i * 3 + 0] == gameBoard[i * 3 + 1] && gameBoard[i * 3 + 1] != 0 && gameBoard[i * 3 + 1] == gameBoard[i * 3 + 2])){
                        gameState = gameBoard[i * 3 + 1];
                    } 
                    if((gameBoard[0 * 3 + j] == gameBoard[1 * 3 + j] && gameBoard[1 * 3 + j] != 0 && gameBoard[1 * 3 + j] == gameBoard[2 * 3 + j])){
                        gameState = gameBoard[1 * 3 + j];
                    }
                }
            }
            if((gameBoard[0 * 3 + 0] == gameBoard[1 * 3 + 1] && gameBoard[1 * 3 + 1] != 0 && gameBoard[1 * 3 + 1] == gameBoard[2 * 3 + 2])){
                gameState = gameBoard[1 * 3 + 1];
            }
            if((gameBoard[2 * 3 + 0] == gameBoard[1 * 3 + 1] && gameBoard[1 * 3 + 1] != 0 && gameBoard[1 * 3 + 1] == gameBoard[0 * 3 + 2])){
                gameState = gameBoard[1 * 3 + 1];
            }

            if(checkDraw(gameBoard)){
                gameState = 3;
            }
        }

        SDL_RenderClear(renderer);

        if((mousePosition.y < 48 && mousePosition.y > 0 && mousePosition.x < 48 && mousePosition.x > 0)){
            if(gameBoard[(mousePosition.y / 16) * 3 + (mousePosition.x / 16)] == 0){
                SDL_Rect destRect = { (mousePosition.x / 16) * 16, (mousePosition.y / 16) * 16, 16, 16 };

                if(gameState == 0){
                    switch(playerTurn){
                        case 1:
                            SDL_SetTextureAlphaMod(texture_x, 128);
                            SDL_RenderCopy(renderer, texture_x, NULL, &destRect);
                            break;
                        case 2:
                            SDL_SetTextureAlphaMod(texture_o, 128);
                            SDL_RenderCopy(renderer, texture_o, NULL, &destRect);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                SDL_Rect dstRect = { j * 16, i * 16, 16, 16 };
                switch(gameBoard[i * 3 + j]){
                    case 1:
                        SDL_SetTextureAlphaMod(texture_x, 255);
                        SDL_RenderCopy(renderer, texture_x, NULL, &dstRect);
                        break;
                    case 2:
                        SDL_SetTextureAlphaMod(texture_o, 255);
                        SDL_RenderCopy(renderer, texture_o, NULL, &dstRect);
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        
        for(int i = 1; i < 3; i++){
            SDL_RenderDrawLine(renderer, 0, i * 16, 3 * 16, i * 16);
            SDL_RenderDrawLine(renderer, i * 16, 0, i * 16, 3 * 16);
        }

        switch(gameState){
            case 1:
                SDL_SetRenderDrawColor(renderer, 64, 16, 16, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(renderer, 16, 16, 64, 255);
                break;
            case 3:
                SDL_SetRenderDrawColor(renderer, 16, 64, 16, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
                break;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture_o);
    SDL_DestroyTexture(texture_x);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

unsigned char checkDraw(unsigned char* gameBoard){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(gameBoard[i * 3 + j] == 0){
                return 0;
            }
        }
    }
    return 1;
}