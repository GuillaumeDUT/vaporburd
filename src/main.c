#include "libs.h"
#include "constants.h"

#include "basic_shape.h"
#include "helpers.h"
#include "entities.h"
#include "ship.h"
#include "shoot.h"
#include "obstacle.h"
#include "ennemies.h"
#include "osu_reader.h"
#include "ppm_reader.h"
#include "bonus.h"

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
static int MUSIC_DURATION = 279000;
float WINDOW_SCALE = 20.0;
float globalTranslation;
int LEVEL_STATE;

static const int GAME_MODE_MENU = 1;
static const int GAME_MODE_GAME = 2;
static const int GAME_MODE_END_GAME = 3;

/* DEBUG */
static const int DEBUG = 0;

/* DIFFICULTY */
//static const char diff[20] = "[ryuu's Easy]";
//static const char diff[20] = "[Normal]";
//static const char diff[20] = "[Advanced]";
static const char diff[20] = "[Hard]";
//static const char diff[20] = "[fufufu]";

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
  sprintf(filepath, "./assets/img/%s.png", name);
  image = IMG_Load(filepath);
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
  LEVEL_STATE = LEVEL_STATE_INIT;

  if ( DEBUG == 1 ) {
    printf(" // // MODE DEBUG BOSS ON // // \n");
    /* DEBUG DU BOSS */
    MUSIC_DURATION = MUSIC_DURATION / 100;
  }

  // Initialisation de la SDL
  if(-1 == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  // Ouverture d'une fenêtre et création d'un contexte OpenGL
  if(SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE) == NULL) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  SDL_WM_SetCaption("V A P O R B U R D", NULL);

  resizeViewport();
  srand(time(NULL));

  /* Ouverture de la musique */
  /* Initialisation de l'API Mixer */
  if((Mix_Init(MIX_INIT_MP3)&MIX_INIT_MP3)!=MIX_INIT_MP3) {
    printf("Mix_Init error: %s",Mix_GetError());
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
    printf("Opening MIX_AUDIO: %s\n", Mix_GetError());
  }


  /* Chargement et traitement de la texture */


  GLuint textureID[20];



  glGenTextures(20, textureID);
  setTexture("ship", 0, textureID);
  setTexture("bullet", 1, textureID);
  setTexture("bg", 2, textureID);
  setTexture("wall",3,textureID);
  setTexture("ennemy_bullet",4,textureID);
  setTexture("bonus1",5,textureID);
  setTexture("bonus2",6,textureID);
  setTexture("bonus3",7,textureID);
  setTexture("basic_ennemy",8,textureID);

  setTexture("background_menu",9,textureID);


  /* activation du canal Alpha */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Mix_Music *music = Mix_LoadMUS("./assets/flicker.mp3");
  Mix_PlayMusic(music, -1);
  int CORRECTIF = 100;
  int musicStartTime = SDL_GetTicks() + CORRECTIF;
  printf("Music start at %d ticks\n", musicStartTime);


	/* Ouverture de la map OSU de la bonne difficulté */
	char osuFileName[100] = "./assets/osu/Porter Robinson - Flicker (Cyllinus) ";
	strcat(osuFileName, diff);
	strcat(osuFileName, ".osu");
  OSUList osu = readOsuFile(osuFileName);
  OSUNode currentOsuNode = osu.first;

  Ship ship = createShip(-4.0, 0.0, 30, 0.5);

  OList obstaclesList;
  obstaclesList.taille = 0;
  EList ennemiesList;
  ennemiesList.taille = 0;
  BList bulletsList;
  bulletsList.taille = 0;
  BList bonusesList;
  bonusesList.taille = 0;


	/* Open PPM MAP */
	char ppmFileName[100] = "./assets/map ";
	strcat(ppmFileName, diff);
	strcat(ppmFileName, ".ppm");
	int mapLength = createFromPPM(ppmFileName, &obstaclesList, &bonusesList);


  int triggerKeyArrowUp = 0;
  int triggerKeyArrowDown = 0;
  int triggerKeyArrowLeft = 0;
  int triggerKeyArrowRight = 0;
  int triggerKeySpace = 0;
  int triggerKeyShift = 0;

  globalTranslation = (float)mapLength / MUSIC_DURATION * FRAMERATE_MILLISECONDS;
  float globalTranslationTotal = 0;

  int loop = 1;

  // 1 = Menu  ### 2 = jeu  ### 3 = fin de jeu
  int GAME_MODE = GAME_MODE_MENU;

  glClearColor(0.1, 0.1, 0.1 ,1.0);

  if ( DEBUG == 1 ) {
    /* DEBUG DU BOSS */
    Bonus actuel = bonusesList.first;
    while ( actuel != NULL ) {
      acquireBonus(ship, actuel);
      supprimerList(&bonusesList, actuel->id);
      actuel = actuel->next;
    }

  }



  LEVEL_STATE = LEVEL_STATE_RUNNING;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT);

      if(GAME_MODE ==  GAME_MODE_MENU){

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID[9]);

        glBegin(GL_QUADS);
        {
          glColor3ub(255,255,255);
          glTexCoord2f(0, 0);
          glVertex2f(-1.77 * 10, +1 *10);

          glTexCoord2f(1, 0);
          glVertex2f(+1.77 * 10, +1 *10);

          glTexCoord2f(1, 1);
          glVertex2f(+1.77 * 10, -1 *10);

          glTexCoord2f(0, 1);
          glVertex2f(-1.77 * 10, -1 *10);
        }
        glEnd();
        /* Desactive l'image */
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        drawCircle(1);


      }else if(GAME_MODE == GAME_MODE_GAME){
          glTranslatef(-globalTranslation, 0, 0);
          /* Global counter */
          globalTranslationTotal += globalTranslation;
          /* textures bg */
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textureID[2]);
          glBegin(GL_QUADS);
          {
            glColor3ub(255,255,255);
            glTexCoord2f(0, 0);
            glVertex2f(-1.77 *10+globalTranslationTotal, +1 *10);

            glTexCoord2f(1, 0);
            glVertex2f(+1.77 *10+globalTranslationTotal, +1 *10);

            glTexCoord2f(1, 1);
            glVertex2f(+1.77 *10+globalTranslationTotal, -1 *10);

            glTexCoord2f(0, 1);
            glVertex2f(-1.77 *10+globalTranslationTotal, -1 *10);
          }
          glEnd();
          /* Desactive l'image */
          glBindTexture(GL_TEXTURE_2D, 0);
          glDisable(GL_TEXTURE_2D);

          /* Active l'image */
          /* dessine l'image du vaisseau en fonction de sa position */
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textureID[0]);

          glBegin(GL_QUADS);
          {
            glColor3ub(255,255,255);
            glTexCoord2f(0, 0);
            glVertex2f(ship->pos[X]-1, ship->pos[Y]+1.76);

            glTexCoord2f(1, 0);
            glVertex2f(ship->pos[X]+1, ship->pos[Y]+1.76);

            glTexCoord2f(1, 1);
            glVertex2f(ship->pos[X]+1, ship->pos[Y]-1.76);

            glTexCoord2f(0, 1);
            glVertex2f(ship->pos[X]-1, ship->pos[Y]-1.76);
          }
          glEnd();
          /* Desactive l'image */

          glBindTexture(GL_TEXTURE_2D, 0);
          glDisable(GL_TEXTURE_2D);



          /* Spawn ennemy si la prochaine node OSU a un temps supérieur au temps passé, on invoque un ennemi */
          if ( currentOsuNode != NULL &&
              musicStartTime + currentOsuNode->time <= SDL_GetTicks() &&
              LEVEL_STATE == LEVEL_STATE_RUNNING)
          {
            /*
           createRandomEnnemy(&ennemiesList, globalTranslationTotal);
            */
            createOSUNodeEnnemy(
              &ennemiesList,
              currentOsuNode,
              globalTranslationTotal);
            currentOsuNode = currentOsuNode->next;
          } else if ( LEVEL_STATE == LEVEL_STATE_BOSS_INIT ) {
            createBoss(&ennemiesList, globalTranslation, globalTranslationTotal);
            LEVEL_STATE = LEVEL_STATE_BOSS_SPAWNED;
            printf("Boss spawned\n");
          }

          /* Si on reste appuyé sur les flêches, on se déplace */
          if(triggerKeyArrowUp) moveUp(ship);
          if(triggerKeyArrowDown) moveDown(ship);
          if(triggerKeyArrowLeft) moveLeft(ship);
          if(triggerKeyArrowRight) moveRight(ship);


          /* Tir */
          ship->cooldown = ship->cooldown > 0 ?
            ship->cooldown-1
            : 0;
          if ( triggerKeySpace ) {
            if ( ship->cooldown <= 0 ) {
              ship->cooldown = 1 + FRAMERATE_MILLISECONDS/(ship->attackPerSecond);
              shoot(ship, &bulletsList);
            }
          }



          /* Boucle d'update et affichage des objets */
          updateShip(ship, &bulletsList, globalTranslation, globalTranslationTotal, triggerKeyShift);
          updateObstacles(ship, &obstaclesList,textureID);
          updateBullets(ship, &bulletsList, globalTranslationTotal,textureID);
          updateEnnemies(ship, &bulletsList, &ennemiesList, globalTranslationTotal,textureID);
          updateBonuses(ship, &bonusesList,textureID);


          if ( ship->hp <= 0) {
            printf("Fin de partie\n\n");
            GAME_MODE= GAME_MODE_END_GAME;
          }
      }else if(GAME_MODE == GAME_MODE_END_GAME ){
        drawSquare(1);
      }



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
          if(GAME_MODE == GAME_MODE_GAME){
            if(e.key.keysym.sym == SDLK_UP) triggerKeyArrowUp = 1;
            if(e.key.keysym.sym == SDLK_DOWN) triggerKeyArrowDown = 1;
            if(e.key.keysym.sym == SDLK_RIGHT) triggerKeyArrowRight = 1;
            if(e.key.keysym.sym == SDLK_LEFT) triggerKeyArrowLeft = 1;
            if(e.key.keysym.sym == SDLK_SPACE) triggerKeySpace = 1;
            if(e.key.keysym.sym == SDLK_LSHIFT || e.key.keysym.sym == SDLK_RSHIFT) triggerKeyShift = 1;
          }
          break;
        case SDL_KEYUP:
          if(GAME_MODE == GAME_MODE_GAME){
            if(e.key.keysym.sym == SDLK_UP) triggerKeyArrowUp = 0;
            if(e.key.keysym.sym == SDLK_DOWN) triggerKeyArrowDown = 0;
            if(e.key.keysym.sym == SDLK_RIGHT) triggerKeyArrowRight = 0;
            if(e.key.keysym.sym == SDLK_LEFT) triggerKeyArrowLeft = 0;
            if(e.key.keysym.sym == SDLK_SPACE) triggerKeySpace = 0;
            if(e.key.keysym.sym == SDLK_LSHIFT || e.key.keysym.sym == SDLK_RSHIFT) triggerKeyShift = 0;
            if(e.key.keysym.sym == '1') {
              ship->missileLevel++;
              printf("[DEBUG] Missile level:%d\n", ship->missileLevel);
            }
            if(e.key.keysym.sym == '2') {
              ship->missileLevel--;
              printf("[DEBUG] Missile level:%d\n", ship->missileLevel);
            }
          }
          if(e.key.keysym.sym == 'a' || e.key.keysym.sym == 'q') loop = 0;

          break;

        case SDL_MOUSEBUTTONDOWN:
          if(GAME_MODE == GAME_MODE_MENU){

            GAME_MODE = GAME_MODE_GAME;
          }
          break;
        // case SDL_MOUSEMOTION:
        //   printf("oos X : %f  ||  pos Y :  %f \n",e.motion.x,e.motion.y);
        //   break;

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

  /* SUPPRESSION DES RESSOURCES */
  {
    /* TODO: Libération des données GPU */
    /*
  glDeleteTextures(11, textureID);
*/
    /* Liberation des ressources associées à la SDL */
    Mix_FreeMusic(music);
    Mix_CloseAudio();

    /* Liberation des structures allouées */
    deleteList(&obstaclesList);
    deleteList(&ennemiesList);
    deleteList(&bulletsList);
  }

  SDL_Quit();

  return EXIT_SUCCESS;
}
