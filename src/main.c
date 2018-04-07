#include "libs.h"
#include "constants.h"

#include "basic_shape.h"
#include "entities.h"
#include "ship.h"
#include "shoot.h"
#include "obstacle.h"
#include "ennemies.h"

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

  OList obstaclesList;
  obstaclesList.taille = 0;
  ajouterFinOList(&obstaclesList, createObstacle(4.0, 4.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, 3.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, 2.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, 1.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, 0.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, -1.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, -2.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, -3.0, 20, 1));
  ajouterFinOList(&obstaclesList, createObstacle(4.0, -4.0, 20, 1));

  
  EList ennemiesList;
  ennemiesList.taille = 0;
  ajouterFinEList(&ennemiesList, createEnnemy(2.0, 1.0, 5, 0.5));
  ajouterFinEList(&ennemiesList, createEnnemy(2.0, 2.0, 5, 0.5));
  ajouterFinEList(&ennemiesList, createEnnemy(2.0, 3.0, 5, 0.5));
  ajouterFinEList(&ennemiesList, createEnnemy(2.0, 4.0, 5, 0.5));
  
  int triggerKeyArrowUp = 0;
  int triggerKeyArrowDown = 0;
  int triggerKeyArrowLeft = 0;
  int triggerKeyArrowRight = 0;
  int triggerKeyArrowSpace = 0;

  int shootCooldown = 0;

  BList bulletsList;
  bulletsList.taille = 0;

  int loop = 1;
  glClearColor(0.1, 0.1, 0.1 ,1.0);
  while(loop) {

    Uint32 startTime = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT);


    /* Si on reste appuyé sur les flêches, on se déplace */
    if(triggerKeyArrowUp) moveUp(ship);
    if(triggerKeyArrowDown) moveDown(ship);
    if(triggerKeyArrowLeft) moveLeft(ship);
    if(triggerKeyArrowRight) moveRight(ship);

    /* Tir */
    shootCooldown = shootCooldown > 0 ? shootCooldown-1 : 0;
    if ( triggerKeyArrowSpace ) {
      if ( shootCooldown <= 0 ) {
        printf("Shot\n");
        shootCooldown = SHOOT_COOLDOWN;
        shoot(ship, &bulletsList);
      } 
    }

    /* Boucle affichage des objets */
    moveShip(ship);
    drawShip(ship, 0);
    loopOList(ship, &obstaclesList);
    loopBList(ship, &bulletsList);
    loopEList(ship, &bulletsList, &ennemiesList);
    
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
          if(e.key.keysym.sym == SDLK_UP) triggerKeyArrowUp = 1;
          if(e.key.keysym.sym == SDLK_DOWN) triggerKeyArrowDown = 1;
          if(e.key.keysym.sym == SDLK_RIGHT) triggerKeyArrowRight = 1;
          if(e.key.keysym.sym == SDLK_LEFT) triggerKeyArrowLeft = 1;
          if(e.key.keysym.sym == SDLK_SPACE) triggerKeyArrowSpace = 1;
          break;
        case SDL_KEYUP:
          if(e.key.keysym.sym == SDLK_UP) triggerKeyArrowUp = 0;
          if(e.key.keysym.sym == SDLK_DOWN) triggerKeyArrowDown = 0;
          if(e.key.keysym.sym == SDLK_RIGHT) triggerKeyArrowRight = 0;
          if(e.key.keysym.sym == SDLK_LEFT) triggerKeyArrowLeft = 0;
          if(e.key.keysym.sym == SDLK_SPACE) triggerKeyArrowSpace = 0;
          if(e.key.keysym.sym == 'a' || e.key.keysym.sym == 'q') loop = 0;
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
