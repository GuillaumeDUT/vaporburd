#include "libs.h"

#include "basic_shape.h"
#include "entities.h"
#include "ship.h"
#include "shoot.h"
#include "obstacle.h"

#define ZOOM 1
#define DIVIDETIME 5

#define WINDOW_SCALE 10

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WINDOW_SCALE/2.0, WINDOW_SCALE/2., -WINDOW_SCALE/2., WINDOW_SCALE/2.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

int main(int argc, char** argv) {

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("", NULL);

    resizeViewport();

    Ship ship = createShip(-4.0, 0.0, 10, 0.5);

		OList obstacles;
		obstacles.taille = 0;
		ajouterFinOList(&obstacles, createObstacle(4.0, 4.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, 3.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, 2.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, 1.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, 0.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, -1.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, -2.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, -3.0, 20, 1));
		ajouterFinOList(&obstacles, createObstacle(4.0, -4.0, 20, 1));

    int triggerKeyArrowUp = 0;
    int triggerKeyArrowDown = 0;
    int triggerKeyArrowLeft = 0;
    int triggerKeyArrowRight = 0;

    BList bulletsList;
    bulletsList.taille = 0;

    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);


        //Si on reste appuyé sur la barre espace on tire, same pour les flêches

        if(triggerKeyArrowUp){
          moveUp(ship);
        }
        if(triggerKeyArrowDown){
          moveDown(ship);
        }
        if(triggerKeyArrowLeft){
          moveLeft(ship);
        }
        if(triggerKeyArrowRight){
          moveRight(ship);
        }

				/* Boucle affichage des objets */
				moveShip(ship);
				drawShip(ship);
				loopOList(ship, &obstacles);
        afficherBList(&bulletsList);
				
				SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

								case SDL_KEYDOWN:
											//printf("touche pressée (code = %d)\n", e.key.keysym.sym);
											// if spacebar
											// if arrow up
											if(e.key.keysym.sym == SDLK_UP){
                        triggerKeyArrowUp = 1;
											}
											//if arrow down
											if(e.key.keysym.sym == SDLK_DOWN){
                        triggerKeyArrowDown = 1;
											}
											//if arrow right
											if(e.key.keysym.sym == SDLK_RIGHT){
                        triggerKeyArrowRight = 1;
											}
											//if arrow left
											if(e.key.keysym.sym == SDLK_LEFT){
                        triggerKeyArrowLeft = 1;
											}
										break;
                case SDL_KEYUP:
                      if(e.key.keysym.sym == SDLK_SPACE){
          							shoot(ship, &bulletsList);
                      }
                      if(e.key.keysym.sym == SDLK_UP){
                        triggerKeyArrowUp = 0;
                      }
                      //if arrow down
                      if(e.key.keysym.sym == SDLK_DOWN){
                        triggerKeyArrowDown = 0;
                      }
                      //if arrow right
											if(e.key.keysym.sym == SDLK_RIGHT){
                        triggerKeyArrowRight = 0;
											}
											//if arrow left
											if(e.key.keysym.sym == SDLK_LEFT){
                        triggerKeyArrowLeft = 0;
											}
                    break;

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }


    // TODO: Libération des données GPU
    // ...

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
