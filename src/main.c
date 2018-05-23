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

/* Variables super globales */
float globalTranslation;
float WINDOW_SCALE = 20.0;
int LEVEL_STATE;
int GAME_MODE;


/* DEBUG */
static const int DEBUG = 1 ;

/* DIFFICULTY */
//static const char diff[20] = "[ryuu's Easy]";

//static const char diff[20] = "[Advanced]";
//static const char diff[20] = "[Hard]";
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


  Mix_Music *music = Mix_LoadMUS("./assets/flicker.mp3");
  int musicStartTime =0;

  /* Chargement et traitement de la texture */

  GLuint textureID[25];
  glGenTextures(25, textureID);
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

  setTexture("button_easy",10,textureID);
  setTexture("button_normal",11,textureID);
  setTexture("button_advanced",12,textureID);
  setTexture("button_hard",13,textureID);
  setTexture("button_fufufu",14,textureID);
  setTexture("button_play",15,textureID);

  setTexture("bossBg",16,textureID);

  setTexture("button_menu",17,textureID);
  setTexture("text_win",18,textureID);
  setTexture("text_death",19,textureID);

  setTexture("boss",20,textureID);

  setTexture("bullet_ship",21,textureID);
  setTexture("bullet_boss",22,textureID);



  /* activation du canal Alpha */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	/* Ouverture de la map OSU de la bonne difficulté */
	char osuFileName[100] = "./assets/osu/Porter Robinson - Flicker (Cyllinus) ";
  char bufferOsuFileName[100];
  OSUList osu;
  OSUNode currentOsuNode;


  Ship ship = createShip(-4.0, 0.0, 30, 0.5);

  OList obstaclesList;
  obstaclesList.taille = 0;
  EList ennemiesList;
  ennemiesList.taille = 0;
  BList bulletsList;
  bulletsList.taille = 0;
  BList bonusesList;
  bonusesList.taille = 0;


  char diff[50] = "[Normal]";
	char ppmFileName[100] = "./assets/map ";
  char bufferPpmFileName[100];
  int mapLength;

  int triggerKeyArrowUp = 0;
  int triggerKeyArrowDown = 0;
  int triggerKeyArrowLeft = 0;
  int triggerKeyArrowRight = 0;
  int triggerKeySpace = 0;
  int triggerKeyShift = 0;

  float globalTranslationTotal = 0;

  int loop = 1;

  // 1 = Menu  ### 2 = jeu  ### 3 = fin de jeu
  GAME_MODE = GAME_MODE_MENU;

  glClearColor(0.1, 0.1, 0.1 ,1.0);

  float posButton[12] ={-6.5,-4,0,-4,6.5,-4,-3.5,-7,3.5,-7,0,0};
  int idTextureForLoop =0;
  //ordre : easy normal advanced hard fuuu  Haut gauche, bas droite
  int selectedButtonPos[20]= {20,520,260,600,280,520,520,600,540,520,780,600,140,640,380,720,420,640,660,720};
  int selectedDifficulty = 1;

  LEVEL_STATE = LEVEL_STATE_RUNNING;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT);

      if(GAME_MODE ==  GAME_MODE_MENU){
        // background
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
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID[10]);
        glPushMatrix();{
          glTranslatef(posButton[selectedDifficulty*2],posButton[selectedDifficulty*2+1],0);
          glRotated(180,0,1,0);
          glScalef(1.04,1.1,0);
          glBegin(GL_QUADS);
          {
            //glColor4f(0.4,0,0.74,0.8);
            glColor3ub(255,255,255);
            glTexCoord2f(0, 0);
            glVertex2f(-3 , +1);

            glTexCoord2f(1, 0);
            glVertex2f(+3, +1);

            glTexCoord2f(1, 1);
            glVertex2f(+3 , -1);

            glTexCoord2f(0, 1);
            glVertex2f(-3 , -1);
          }
          glEnd();
        }
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        // for de rendu bouttons
        idTextureForLoop = 0;
        for(int i =0;i<12;i=i+2){

          glPushMatrix();
          {
            //printf("button %d",i-1+10);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID[10+idTextureForLoop]);
            glTranslatef(posButton[i],posButton[i+1],0);
            if(i>=10){
              glScalef(2,2,0);
            }
            glBegin(GL_QUADS);
            {
              glColor3ub(255,255,255);
              glTexCoord2f(0, 0);
              glVertex2f(-3 , +1);

              glTexCoord2f(1, 0);
              glVertex2f(+3, +1);

              glTexCoord2f(1, 1);
              glVertex2f(+3 , -1);

              glTexCoord2f(0, 1);
              glVertex2f(-3 , -1);
            }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
          }
          glPopMatrix();
          idTextureForLoop = idTextureForLoop +1;
        }

        glPushMatrix();
          glTranslatef(0,-6,0);
          drawCircle(1);
        glPopMatrix();

      }else if(GAME_MODE == GAME_MODE_GAME){
          glTranslatef(-globalTranslation, 0, 0);
          /* Global counter */
          globalTranslationTotal += globalTranslation;


          if ( DEBUG == 1 ) {
            /* DEBUG DU BOSS */
            Bonus actuel = bonusesList.first;
            while ( actuel != NULL ) {
              acquireBonus(ship, actuel);
              supprimerList(&bonusesList, actuel->id);
              actuel = actuel->next;
            }
          }

          /* textures bg */
          glEnable(GL_TEXTURE_2D);
          glPushMatrix();
            if(LEVEL_STATE == LEVEL_STATE_BOSS_SPAWNED){
              glBindTexture(GL_TEXTURE_2D, textureID[16]);
              glTranslatef(globalTranslationTotal,0,0);
              glRotatef(20-(globalTranslationTotal*5),0,0,1);
              glBegin(GL_QUADS);
              {
                glColor3ub(255,255,255);
                glTexCoord2f(0, 0);
                glVertex2f(-1 *14, +1 *14);

                glTexCoord2f(1, 0);
                glVertex2f(+1 *14, +1 *14);

                glTexCoord2f(1, 1);
                glVertex2f(+1 *14, -1 *14);

                glTexCoord2f(0, 1);
                glVertex2f(-1 *14, -1 *14);
              }
              glEnd();

            }else{
              glBindTexture(GL_TEXTURE_2D, textureID[2]);
              glTranslatef(42+globalTranslationTotal*0.58,0,0);
              glBegin(GL_QUADS);
              {
                glColor3ub(255,255,255);
                glTexCoord2f(0, 0);
                glVertex2f(-5.33 *10, +1 *10);

                glTexCoord2f(1, 0);
                glVertex2f(+5.33 *10, +1 *10);

                glTexCoord2f(1, 1);
                glVertex2f(+5.33 *10, -1 *10);

                glTexCoord2f(0, 1);
                glVertex2f(-5.33 *10, -1 *10);
              }
              glEnd();
            }
          glPopMatrix();
          /* Desactive l'image */
          glBindTexture(GL_TEXTURE_2D, 0);
          glDisable(GL_TEXTURE_2D);

          // glPushMatrix();
          //   glScalef(10,10,1);
          //   //glRotatef(-20,0,0,-1);
          //   glPushMatrix();
          //     glTranslatef(0.1- globalTranslationTotal ,0,0);
          //     for(int i=-20;i<20;i++){
          //       glColor3f(1.0f,0.0f,1.0f);
          //       glBegin(GL_LINES);
          //         glVertex3f(-20,i*0.1,0);
          //         glVertex3f(20,i*0.1,0);
          //       glEnd();
          //     }
          //     for(int i=-20;i<20;i++){
          //       glColor3f(1.0f,0.0f,1.0f);
          //       glBegin(GL_LINES);
          //         glVertex3f(i*0.1,-20,0);
          //         glVertex3f(i*0.1,20,0);
          //       glEnd();
          //     }
          //   glPopMatrix();
          // glPopMatrix();

          /* Active l'image */
          /* dessine l'image du vaisseau en fonction de sa position */
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textureID[0]);

            glBegin(GL_QUADS);
            {
              glColor3ub(255,255,255);
              glTexCoord2f(0, 0);
              glVertex2f(ship->pos[X]-1.10 *0.4 , ship->pos[Y]+1 *0.4 );

              glTexCoord2f(1, 0);
              glVertex2f(ship->pos[X]+1.10 *0.4 , ship->pos[Y]+1 *0.4 );

              glTexCoord2f(1, 1);
              glVertex2f(ship->pos[X]+1.10 *0.4 , ship->pos[Y]-1 *0.4 );

              glTexCoord2f(0, 1);
              glVertex2f(ship->pos[X]-1.10 *0.4 , ship->pos[Y]-1 *0.4 );
            }
            glEnd();
          /* Desactive l'image */

          glBindTexture(GL_TEXTURE_2D, 0);
          glDisable(GL_TEXTURE_2D);



          /* Spawn ennemy si la prochaine node OSU a un temps supérieur au temps passé, on invoque un ennemi */
          if ( currentOsuNode != NULL &&
              musicStartTime + currentOsuNode->time <= SDL_GetTicks() &&
              LEVEL_STATE == LEVEL_STATE_RUNNING &&
              GAME_MODE == GAME_MODE_GAME)
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

            /* On supprime tous les obstacles */
            deleteList(&obstaclesList);
            deleteList(&ennemiesList);
            deleteList(&bulletsList);
            
            /* On fait apparaître un boss */
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

        /* Debug */
        printf("Nb bullets : %d\n", bulletsList.taille);
        printf("Nb ennemies : %d\n", ennemiesList.taille);
        printf("Nb obstacles : %d\n", obstaclesList.taille);


          if ( ship->hp <= 0) {
            printf("Fin de partie\n\n");
            GAME_MODE= GAME_MODE_END_GAME;
            resizeViewport();
          }
      }else if(GAME_MODE == GAME_MODE_END_GAME ){

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
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID[17]);
        glPushMatrix();
          glScalef(1.5,1.5,0);
          glTranslatef(0,-4,0);
          glBegin(GL_QUADS);
          {
            glColor3ub(255,255,255);
            glTexCoord2f(0, 0);
            glVertex2f(-3 , +1);

            glTexCoord2f(1, 0);
            glVertex2f(+3, +1);

            glTexCoord2f(1, 1);
            glVertex2f(+3 , -1);

            glTexCoord2f(0, 1);
            glVertex2f(-3 , -1);
          }
          glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);


        glEnable(GL_TEXTURE_2D);
        if(ship->hp <=0){
          glBindTexture(GL_TEXTURE_2D, textureID[19]);
          glPushMatrix();
            glScalef(1.5,1.5,0);
            glBegin(GL_QUADS);
            {
              glColor3ub(255,255,255);
              glTexCoord2f(0, 0);
              glVertex2f(-3.79 , +1);

              glTexCoord2f(1, 0);
              glVertex2f(+3.79, +1);

              glTexCoord2f(1, 1);
              glVertex2f(+3.79 , -1);

              glTexCoord2f(0, 1);
              glVertex2f(-3.79 , -1);
            }
            glEnd();
          glPopMatrix();
        }else{
          glBindTexture(GL_TEXTURE_2D, textureID[18]);
          glPushMatrix();
            glScalef(4,4,0);
            glBegin(GL_QUADS);
            {
              glColor3ub(255,255,255);
              glTexCoord2f(0, 0);
              glVertex2f(-1.89 , +1);

              glTexCoord2f(1, 0);
              glVertex2f(+1.89, +1);

              glTexCoord2f(1, 1);
              glVertex2f(+1.89 , -1);

              glTexCoord2f(0, 1);
              glVertex2f(-1.89 , -1);
            }
            glEnd();
          glPopMatrix();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        //glTranslatef(0, 0, 0);
        //drawSquare(1);
        //drawCircle(1);
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
          if(GAME_MODE == GAME_MODE_MENU ){
            if(e.button.x >= 260 && e.button.x <= 530 && e.button.y <= 430 && e.button.y >= 350){

              //TO DO une fonction init game ? vu que ça pourrait reservir pour un bouton rejouer sur l'écran de fin
              strcpy(bufferOsuFileName,osuFileName);
              strcat(bufferOsuFileName, diff);
            	strcat(bufferOsuFileName, ".osu");
              osu = readOsuFile(bufferOsuFileName);
              currentOsuNode = osu.first;

              LEVEL_STATE = LEVEL_STATE_RUNNING;

              strcpy(bufferPpmFileName,ppmFileName);
              strcat(bufferPpmFileName, diff);
            	strcat(bufferPpmFileName, ".ppm");
            	mapLength = createFromPPM(bufferPpmFileName, &obstaclesList, &bonusesList);
              globalTranslation = (float)mapLength / MUSIC_DURATION * FRAMERATE_MILLISECONDS;

              /* Suppression des ressources */
//
//              deleteList(&obstaclesList);
//              deleteList(&ennemiesList);
//              deleteList(&bulletsList);


              globalTranslationTotal =0;
              ship->hp = 30;
              ship->pos[X] = -5;
              ship->pos[Y] =0;
              GAME_MODE = GAME_MODE_GAME;

              // Debug fin jeu
              //GAME_MODE = GAME_MODE_END_GAME;
              Mix_PlayMusic(music, -1);
              int CORRECTIF = 100;
              musicStartTime = SDL_GetTicks() + CORRECTIF;
              printf("Music start at %d ticks\n", musicStartTime);

            }else if(e.button.x >= selectedButtonPos[0] && e.button.x <= selectedButtonPos[2] && e.button.y >= selectedButtonPos[1] && e.button.y <= selectedButtonPos[3]){

              selectedDifficulty = 0;
              strcpy(diff,"[ryuu's Easy]");
              printf("niveau 1 easy \n");

            }else if(e.button.x >= selectedButtonPos[4] && e.button.x <= selectedButtonPos[6] && e.button.y >= selectedButtonPos[5] && e.button.y <= selectedButtonPos[7]){
              selectedDifficulty = 1;
              strcpy(diff,"[Normal]");
              printf("niveau 2 normal\n");
            }else if(e.button.x >= selectedButtonPos[8] && e.button.x <= selectedButtonPos[10] && e.button.y >= selectedButtonPos[9] && e.button.y <= selectedButtonPos[11]){
              selectedDifficulty = 2;
              strcpy(diff,"[Advanced]");
              printf("niveau 3 advanced\n");

            }else if(e.button.x >= selectedButtonPos[12] && e.button.x <= selectedButtonPos[14] && e.button.y >= selectedButtonPos[13] && e.button.y <= selectedButtonPos[15]){
              selectedDifficulty = 3;
              strcpy(diff,"[Hard]");
              printf("niveau 4 hard \n");

            }else if(e.button.x >= selectedButtonPos[16] && e.button.x <= selectedButtonPos[18] && e.button.y >= selectedButtonPos[17] && e.button.y <= selectedButtonPos[19]){
              selectedDifficulty = 4;
              strcpy(diff,"[fufufu]");
              printf("niveau 5 fufufu\n");

            }
            // for(int i =0;i<10;i+=4){
            //   printf("bornes : %d %d %d %d ",selectedButtonPos[i],selectedButtonPos[i+1],
            //   selectedButtonPos[i+2],selectedButtonPos[i+3]);
            //   if(e.button.x >= selectedButtonPos[i] && e.button.x <= selectedButtonPos[i+2]
            //     && e.button.y >= selectedButtonPos[i+1] && e.button.y <= selectedButtonPos[i+3]){
            //     printf("niveau : %d\n",i/4);
            //
            //   }
            // }
          }

          if(GAME_MODE == GAME_MODE_END_GAME){
            if(e.button.x >= 290 && e.button.x <= 505 && e.button.y <= 660 && e.button.y >= 605){
              GAME_MODE = GAME_MODE_MENU;
            }
          }
          break;
        case SDL_MOUSEMOTION:
          //printf("oos X : %d  ||  pos Y :  %d \n",e.motion.x,e.motion.y);
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
