#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Funciones.h"
using namespace std;

bool init( int, int );
bool loadMedia( SDL_Rect [] );
void close();
void intro( int, SDL_Rect [], int, int );
void musica( Mix_Music * );
void menu();

//--------------------------------------------------------------------------------------------------------
//FUNCIÓN MAIN()

int main( int argc, char* args[] ){
    system("color f0");
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;
    const int WALKING_ANIMATION_FRAMES = 16;
    SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];


	if( !init( SCREEN_WIDTH, SCREEN_HEIGHT ) ){
		printf( "Failed to initialize!\n" );
	}
	else{
		if( !loadMedia( gSpriteClips ) ){
			printf( "Failed to load media!\n" );
		}
		else{
            //DECLARACION DE VARIABLES
			bool quit = false, salida = true, aux = true, opcion = true, opcion1 = true, opcion2 = true, registro = true;
			SDL_Event e;
			int frame = 0;
			Mix_Music *mintro = NULL;
            mintro = Mix_LoadMUS( "intro.wav" );
            if( mintro == NULL ){
                cout << "No se cargo la musica! Error: " << Mix_GetError() << endl;
            }
            Mix_Music *mmenu = NULL;
            mmenu = Mix_LoadMUS( "menu.wav" );
            if( mmenu == NULL ){
                cout << "No se cargo la musica de menu! Error: " << Mix_GetError() << endl;
            }

            if( !gSpriteSheetTexture.loadmenu("imag_menu.png") ){
                cout << "Menu sin imagen, error: " << IMG_GetError() << endl;
            }
            if( !gSpriteSheetTexture.loadback( "registro.png") ){
                cout << "registro no esta cargado, error: " << IMG_GetError() << endl;
            }
            //EMPIEZA EL CICLO
			while( !quit ){
				while( SDL_PollEvent( &e ) != 0 ){
					if( e.type == SDL_QUIT ){  quit = true;  }

				}
                button.handleevent( &e, salida );
				if( salida ){
                    intro( frame, gSpriteClips, SCREEN_WIDTH, SCREEN_HEIGHT );
                    frame++;
                    if( frame/8 >= WALKING_ANIMATION_FRAMES ){  frame = 0;  }
                    if( Mix_PlayingMusic() == 0){
                        Mix_PlayMusic( mintro, -1 );
                    }

				}
				if( !salida ){
                    button.handleevent1( &e, opcion, opcion1 );
                    button.handleevent2( &e, opcion2, opcion );
                    if( opcion ){
                        if( Mix_PlayingMusic() == 0 ){  Mix_PlayMusic( mmenu, -1 ); }
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        SDL_RenderClear( gRenderer );
                        SDL_RenderCopy( gRenderer, gSpriteSheetTexture.menuu, NULL, NULL );
                        SDL_RenderPresent( gRenderer );
                    }
                    else{
                        if( !opcion1 ){
                            if( registro ){
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                                SDL_RenderClear( gRenderer );
                                SDL_RenderCopy( gRenderer, gSpriteSheetTexture.regresar, NULL, NULL );
                                SDL_RenderPresent( gRenderer );
                            }
                            button.handleevent4( &e, registro );


                        }
                        else{
                            //button.handleevent3( &e, opcion );
                            if( aux ){
                                gSpriteSheetTexture.archive( aux );
                            }
                            //gSpriteSheetTexture.loadinstructions();

                        }
                    }
				}
			}
		}
	}

	close();

	return 0;
}

//OTRAS FUNCIONES------------------------------------------------------------------------------------
void intro( int frame, SDL_Rect gSpriteClips[], int SCREEN_WIDTH, int SCREEN_HEIGHT ){
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_Rect *currentClip = &gSpriteClips[ frame/8];
    gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w), ( SCREEN_HEIGHT - currentClip->h)/2, currentClip );
    SDL_RenderPresent( gRenderer );
}

void musica( Mix_Music *mintro ){
    Mix_PlayMusic( mintro, -1 );
}

void menu(){
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_RenderCopy( gRenderer, gSpriteSheetTexture.menuu, NULL, NULL );
    SDL_RenderPresent( gRenderer );
}

//-------------------------------------------------------------------------------------------------------------------------
//FUNCIONES PRIMARIAS

bool init( int SCREEN_WIDTH, int SCREEN_HEIGHT ){
	bool success = true;
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		gWindow = SDL_CreateWindow( "Dominos Algebraicos", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1782579 ) < 0 ){
                    cout << "SDL_mixer No pudo iniciar! Error: " << Mix_GetError() << endl; success = false;
                    success = false;
                }
                if( TTF_Init() == -1 ){
                    cout <<  "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
                    success = false;
                }
			}
		}
	}

	return success;
}

bool loadMedia( SDL_Rect gSpriteClips[] ){
	bool success = true;
	if( !gSpriteSheetTexture.loadFromFile( "logo.png" ) ){
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else{
		gSpriteClips[ 0 ].x = 0;
		gSpriteClips[ 0 ].y = 0;
		gSpriteClips[ 0 ].w =  500;
		gSpriteClips[ 0 ].h = 500;

		gSpriteClips[ 1 ].x = 500;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w =  500;
		gSpriteClips[ 1 ].h = 500;

		gSpriteClips[ 2 ].x = 1000;
		gSpriteClips[ 2 ].y =   0;
		gSpriteClips[ 2 ].w =  500;
		gSpriteClips[ 2 ].h = 500;

		gSpriteClips[ 3 ].x = 1500;
		gSpriteClips[ 3 ].y =   0;
		gSpriteClips[ 3 ].w =  500;
		gSpriteClips[ 3 ].h = 500;

		gSpriteClips[ 4 ].x = 2000;
		gSpriteClips[ 4 ].y =   0;
		gSpriteClips[ 4 ].w =  500;
		gSpriteClips[ 4 ].h = 500;

		gSpriteClips[ 5 ].x = 2500;
		gSpriteClips[ 5 ].y =   0;
		gSpriteClips[ 5 ].w =  500;
		gSpriteClips[ 5 ].h = 500;

		gSpriteClips[ 6 ].x = 3000;
		gSpriteClips[ 6 ].y =   0;
		gSpriteClips[ 6 ].w =  500;
		gSpriteClips[ 6 ].h = 500;

		gSpriteClips[ 7 ].x = 3500;
		gSpriteClips[ 7 ].y =   0;
		gSpriteClips[ 7 ].w =  500;
		gSpriteClips[ 7 ].h = 500;

		gSpriteClips[ 8 ].x = 4000;
		gSpriteClips[ 8 ].y =   0;
		gSpriteClips[ 8 ].w =  500;
		gSpriteClips[ 8 ].h = 500;

		gSpriteClips[ 9 ].x = 4500;
		gSpriteClips[ 9 ].y =   0;
		gSpriteClips[ 9 ].w =  500;
		gSpriteClips[ 9 ].h = 500;

		gSpriteClips[ 10 ].x = 5000;
		gSpriteClips[ 10 ].y =   0;
		gSpriteClips[ 10 ].w =  500;
		gSpriteClips[ 10 ].h = 500;

		gSpriteClips[ 11 ].x = 5500;
		gSpriteClips[ 11 ].y =   0;
		gSpriteClips[ 11 ].w =  500;
		gSpriteClips[ 11 ].h = 500;

		gSpriteClips[ 12 ].x = 6000;
		gSpriteClips[ 12 ].y =   0;
		gSpriteClips[ 12 ].w =  500;
		gSpriteClips[ 12 ].h = 500;

		gSpriteClips[ 13 ].x = 6500;
		gSpriteClips[ 13 ].y =   0;
		gSpriteClips[ 13 ].w =  500;
		gSpriteClips[ 13 ].h = 500;

		gSpriteClips[ 14 ].x = 7000;
		gSpriteClips[ 14 ].y =   0;
		gSpriteClips[ 14 ].w =  500;
		gSpriteClips[ 14 ].h = 500;

        gSpriteClips[ 15 ].x = 7500;
		gSpriteClips[ 15 ].y =   0;
		gSpriteClips[ 15 ].w =  500;
		gSpriteClips[ 15 ].h = 500;


	}

	return success;
}

void close(){
	gSpriteSheetTexture.free();
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}
