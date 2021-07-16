#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//éstas variables deben ser ESTRICTAMENTE globales para el buen funcionamiento del game ;)
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//CLASE LTEXTURE
//---------------------------------------------------------------------------------------------------------

class LTexture{
	public:
		LTexture();
		~LTexture();
		bool loadFromFile( string path );
		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect *clip = NULL );
		bool loadmenu( string );
		void loadinstructions();
		int archive( bool & );
		bool loadFromRenderedText( string textureText, SDL_Color textColor );
		void renderins( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip );
		bool loadback( string );


		SDL_Texture* mTexture, *menuu, *regresar, *registro;
		int mWidth;
		int mHeight;
};

LTexture gSpriteSheetTexture;


LTexture::LTexture(){
	mTexture = NULL;
	menuu = NULL;
	regresar = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	free();
}

bool LTexture::loadFromFile( string path ){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ){
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL ){
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free(){
	if( mTexture != NULL ){
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ){
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip ){
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

bool LTexture::loadmenu( string path ){
    SDL_Surface *cargado = IMG_Load( path.c_str() );

    if( cargado == NULL ){
        cout << "'cargado' en 'loadmenu' esta vacio, error: " << IMG_GetError() << endl;
    }
    else{
        SDL_SetColorKey( cargado, SDL_TRUE, SDL_MapRGB( cargado->format, 0, 0xFF, 0xFF ) );
        menuu = SDL_CreateTextureFromSurface( gRenderer, cargado );

        if( menuu == NULL ){ cout << "'menu' esta vacio, error: " << SDL_GetError() << endl; }
        else{
            mWidth = cargado->w;
            mHeight = cargado->h;
        }
        SDL_FreeSurface( cargado );
    }
    return menuu != NULL;
}

void LTexture::loadinstructions(){
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_RenderCopy( gRenderer, gSpriteSheetTexture.regresar, NULL, NULL );
    SDL_RenderPresent( gRenderer );
}

int LTexture::archive( bool &aux ){
    fstream archivo( "instrucciones.txt", ios::in );
    if( archivo.is_open() ){
        char linea[128];
        do{
            archivo.getline( linea, sizeof(linea) );
            cout << linea << endl;
        }while( !archivo.eof() );
        aux = false;
        archivo.close();
        return 0;

    }
    else{
        cout << "El arshivo solicitado no existe o no tiene permiso para abrirlo\n";
        return 1;
    }
}

bool LTexture::loadback( string path ){
    SDL_Surface *cargado = IMG_Load( path.c_str() );

    if( cargado == NULL ){
        cout << "'cargado' en 'loadmenu' esta vacio, error: " << IMG_GetError() << endl;
    }
    else{
        SDL_SetColorKey( cargado, SDL_TRUE, SDL_MapRGB( cargado->format, 0, 0xFF, 0xFF ) );
        regresar = SDL_CreateTextureFromSurface( gRenderer, cargado );

        if( regresar == NULL ){ cout << "'menu' esta vacio, error: " << SDL_GetError() << endl; }
        else{
            mWidth = cargado->w;
            mHeight = cargado->h;
        }
        SDL_FreeSurface( cargado );
    }
    return menuu != NULL;
}

//-------------------------------------------------------------------------------------------------------------------------
//CLASE BOTON

class boton{
    public:
        SDL_Point posicion, posicion1, posicion2, posicion3, posicion4;

        boton(){
            posicion.x = 162;
            posicion.y = 424;

            posicion1.x = 169;
            posicion1.y = 164;

            posicion2.x = 126;
            posicion2.y = 299;

            posicion3.x = 100;
            posicion3.y = 196;

            posicion4.x = 125;
            posicion4.y = 120;

        }
        void handleevent( SDL_Event *e, bool & );
        void handleevent1( SDL_Event *e, bool &, bool & );
        void handleevent2( SDL_Event *e, bool &, bool & );
        //void handleevent3( SDL_Event *e, bool & );
        void handleevent4( SDL_Event *e, bool & );
}button;

void boton::handleevent( SDL_Event *e, bool &salida ){
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ){
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;
        if( x < posicion.x ){
            inside = false;
        }
        else if( x > posicion.x + 193.9 ){
            inside = false;
        }
        else if( y < posicion.y ){
            inside = false;
        }
        else if( y > posicion.y + 48.7 ){
            inside = false;
        }
        if( inside ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){ salida = false; Mix_HaltMusic();}
        }
    }
}

void boton::handleevent1( SDL_Event *e, bool &salida, bool &op ){
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ){
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;
        if( x < posicion1.x ){
            inside = false;
        }
        else if( x > posicion1.x + 193.9 ){
            inside = false;
        }
        else if( y < posicion1.y ){
            inside = false;
        }
        else if( y > posicion1.y + 48.7 ){
            inside = false;
        }
        if( inside ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){ salida = false; op = false; Mix_HaltMusic();}
        }
    }
}

void boton::handleevent2( SDL_Event *e, bool &salida, bool &op ){
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ){
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;
        if( x < posicion2.x ){
            inside = false;
        }
        else if( x > posicion2.x + 193.9 ){
            inside = false;
        }
        else if( y < posicion2.y ){
            inside = false;
        }
        else if( y > posicion2.y + 48.7 ){
            inside = false;
        }
        if( inside ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){ salida = false; op = false; }
        }
    }
}

/*void boton::handleevent3( SDL_Event *e, bool &salida ){
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ){
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;
        if( x < posicion1.x ){
            inside = false;
        }
        else if( x > posicion1.x + 326 ){
            inside = false;
        }
        else if( y < posicion1.y ){
            inside = false;
        }
        else if( y > posicion1.y + 162 ){
            inside = false;
        }
        if( inside ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){ salida = true; }
        }
    }
}*/

void boton::handleevent4( SDL_Event *e, bool &salida ){
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ){
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;
        if( x < posicion1.x ){
            inside = false;
        }
        else if( x > posicion1.x + 244 ){
            inside = false;
        }
        else if( y < posicion1.y ){
            inside = false;
        }
        else if( y > posicion1.y + 257 ){
            inside = false;
        }
        if( inside ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){ salida = false; }
        }
    }
}
