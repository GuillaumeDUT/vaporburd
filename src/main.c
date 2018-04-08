#include "libs.h"
#include "constants.h"

#include "basic_shape.h"
#include "entities.h"
#include "ship.h"
#include "shoot.h"
#include "obstacle.h"
#include "ennemies.h"
#include "osu_reader.h"

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
void setTexture ( char *name, int textureID, GLuint *textures ) {

  char filepath[100];
  SDL_Surface *image = NULL;
  sprintf(filepath, "./assets/%s.png", name);
  image = IMG_Load("assets/l.png");


  if ( image ) {
    glBindTexture(GL_TEXTURE_2D, textures[textureID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      image->w,
      image->h,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    // FREE
    SDL_FreeSurface( image );
  } else {
    printf("Erreur chargement image %s ( jpg )\n", name);
  }
}

int main(int argc, char** argv) {

  // Initialisation de la SDL
  if(-1 == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  // Ouverture d'une fenêtre et création d'un contexte OpenGL
  SDL_Surface *screen = NULL;
  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
  if(screen == NULL) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  SDL_WM_SetCaption("V A P O R B U R D", NULL);

  resizeViewport();

  /* Ouverture de la musique */
  /* Initialisation de l'API Mixer */
  Mix_Music *music;
  if((Mix_Init(MIX_INIT_MP3)&MIX_INIT_MP3)!=MIX_INIT_MP3) {
    printf("Mix_Init error: %s",Mix_GetError());
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
    printf("Opening MIX_AUDIO: %s\n", Mix_GetError());
  }
  music = Mix_LoadMUS("./assets/flicker.mp3");
  /* Mix_PlayMusic(music, -1); */
  int CORRECTIF = 150;
  int musicStartTime = SDL_GetTicks() + CORRECTIF;
  printf("Music start at %d ticks\n", musicStartTime);
  OSUList osu = readOsuFile("./assets/osu/Porter Robinson - Flicker (Cyllinus) [Hard].osu");
  OSUNode currentOsuNode = osu.first;

  srand(time(NULL));

  /* Chargement et traitement de la texture */
  GLuint textureID[1];
  glGenTextures(1, textureID);
  setTexture("l", 0, textureID);



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

    /* Texture */

    /* Active l'image */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    int scale = 5;
    float w = 12 / scale;
    float h = 16 / scale;

    glBegin(GL_QUADS);
    {
      glTexCoord2f(0, 0);
      glVertex2f(-w/2, h/2);

      glTexCoord2f(1, 0);
      glVertex2f(w/2, h/2);

      glTexCoord2f(1, 1);
      glVertex2f(w/2, -h/2);      

      glTexCoord2f(0, 1);
      glVertex2f(-w/2, -h/2);
    }
    glEnd();        

    /* Desactive l'image */      
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);



    /* Spawn ennemy */
    if ( currentOsuNode != NULL && 
        musicStartTime + currentOsuNode->time <= SDL_GetTicks() ) {
      currentOsuNode = currentOsuNode->next;
      newRandomEnnemy(&ennemiesList);      
    }


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


    /* Texte */

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
  glDeleteTextures(11, textureID);

  // Liberation des ressources associées à la SDL
  Mix_FreeMusic(music);
  Mix_CloseAudio();

  SDL_Quit();

  return EXIT_SUCCESS;
}