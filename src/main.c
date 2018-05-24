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
#include "menu.h"

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
int DEBUG;

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
	GAME_MODE = GAME_MODE_MENU;
	DEBUG = 0;

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
		printf("Mix_Init error: %s\n",Mix_GetError());
	}
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1) {
		printf("Opening MIX_AUDIO: %s\n", Mix_GetError());
	}
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);

	/* Chargement des musiques */
	Mix_Music *musicGame = Mix_LoadMUS("./assets/flicker.mp3");
	Mix_Music *musicBoss = Mix_LoadMUS("./assets/boss.mp3");
	Mix_Music *musicMenu = Mix_LoadMUS("./assets/menu.mp3");
	Mix_PlayMusic(musicMenu, -1);   
	int musicStartTime = 0;

	/* Chargement et traitement de la texture */
	GLuint textureID[TEXTURE_NUMBER];
	glGenTextures(TEXTURE_NUMBER, textureID);
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

	/* Activation du canal Alpha */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1, 0.1, 0.1 ,1.0);

	/* Ouverture de la map OSU de la bonne difficulté */
	char osuFileName[100] = "./assets/osu/Porter Robinson - Flicker (Cyllinus) ";
	char bufferOsuFileName[100];
	OSUList osu;
	OSUNode currentOsuNode = NULL;

	/* Creation des entitees */
	Ship ship = NULL;
	OList obstaclesList;     obstaclesList.taille = 0;
	EList ennemiesList;    ennemiesList.taille = 0;
	BList bulletsShipList;    bulletsShipList.taille = 0;
	BList bulletsEnnemyList;    bulletsEnnemyList.taille = 0;
	BList bonusesList;    bonusesList.taille = 0;


	/* Création des variables */
	char diff[50] = "[Normal]";
	char ppmFileName[100] = "./assets/map ";
	char bufferPpmFileName[100];
	int mapLength;

	/* Variables qui enregistrent si les touches sont appuyées ou non */
	int triggerKeyArrowUp = 0;
	int triggerKeyArrowDown = 0;
	int triggerKeyArrowLeft = 0;
	int triggerKeyArrowRight = 0;
	int triggerKeySpace = 0;
	int triggerKeyShift = 0;
	int triggerKeyB = 0;

	/* Contient le décalage sur l'axe X de l'écran */
	float globalTranslationTotal = 0;

	int loop = 1;

	/* Variables des textures */
	/* ordre : easy normal advanced hard fuuu Haut gauche, bas droite */
	int selectedButtonPos[20] = {20, 520, 260, 600, 280, 520, 520, 600, 540, 520, 780, 600, 140, 640, 380, 720, 420, 640, 660, 720};
	float posButton[12] = {-6.5, -4, 0, -4, 6.5, -4, -3.5, -7, 3.5, -7, 0, 0};
	int selectedDifficulty = 1;
	int idTextureForLoop = 0;

	LEVEL_STATE = LEVEL_STATE_RUNNING;
	while(loop) {
		Uint32 startTime = SDL_GetTicks();
		glClear(GL_COLOR_BUFFER_BIT);

		/* ========================== MENU ========================== */
		if(GAME_MODE == GAME_MODE_MENU){   

			glEnable(GL_TEXTURE_2D);      
			/* Affichage du background */
			drawMenuBackground(textureID);
			/* Affichage du cadre de selection */
			drawSelectionBox(textureID, posButton, selectedDifficulty);
			/* Boucle de rendu des buttons */
			idTextureForLoop = 0;
			for(int i =0;i<12;i=i+2){
				/* Affichage d'un button */
				drawButton(textureID, posButton, i, 10+idTextureForLoop);
				idTextureForLoop = idTextureForLoop +1;
			}
			glDisable(GL_TEXTURE_2D);
		} 
		/* ========================== GAME ========================== */
		else if(GAME_MODE == GAME_MODE_GAME) {
			/* Global counter */
			glTranslatef(-globalTranslation, 0, 0);
			globalTranslationTotal += globalTranslation;

			/* DEBUG DU BOSS */
			if ( DEBUG == 1 ) {
				Bonus actuel = bonusesList.first;
				while ( actuel != NULL ) {
					acquireBonus(ship, actuel);
					supprimerList(&bonusesList, actuel->id);
					actuel = actuel->next;
				}
			}

			/* Affichage des background */
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			if(LEVEL_STATE == LEVEL_STATE_BOSS_SPAWNED){
				drawBossBackground(textureID, globalTranslationTotal);
			}else{
				drawGameBackground(textureID, globalTranslationTotal);
			}
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);

			/* 
				Spawn ennemy si la prochaine node OSU a un temps supérieur au temps passé, on invoque un ennemi
			*/
			if ( currentOsuNode != NULL &&
					musicStartTime + currentOsuNode->time <= SDL_GetTicks() &&
					LEVEL_STATE == LEVEL_STATE_RUNNING &&
					GAME_MODE == GAME_MODE_GAME) {
				createOSUNodeEnnemy(
					&ennemiesList,
					currentOsuNode,
					globalTranslationTotal);
				currentOsuNode = currentOsuNode->next;
			} else if ( LEVEL_STATE == LEVEL_STATE_BOSS_INIT ) {
				/* On est arrivé au boss => Initiation de plusieurs variables */
				/* On supprime tous les obstacles */
				deleteList(&obstaclesList);
				deleteList(&ennemiesList);
				deleteList(&bulletsShipList);
				deleteList(&bulletsEnnemyList);

				/* On charge la musique */
				Mix_PlayMusic(musicBoss, -1);

				/* On déplace le vaisseau pour eviter les problèmes */
				ship->pos[Y] = 0;
				ship->pos[X] = globalTranslationTotal - WINDOW_SCALE / 4;

				/* On fait apparaître un boss */
				createBoss(&ennemiesList, globalTranslation, globalTranslationTotal, selectedDifficulty+1);

				LEVEL_STATE = LEVEL_STATE_BOSS_SPAWNED;
			} else if ( LEVEL_STATE == LEVEL_STATE_ENDED ) {
				printf(CYAN "Fin de partie\n\n" RESET);
				Mix_PlayMusic(musicMenu, -1);   
				GAME_MODE = GAME_MODE_END_GAME;
				resizeViewport();				
			}


			/* >>> UPDATE DU SHIP <<< */
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
					shoot(ship, &bulletsShipList);
				}
			}

			/* Deplacement et Affichage */
			moveShip(ship, globalTranslation, globalTranslationTotal, triggerKeyShift); 
			drawShip(ship, 0, textureID);


			/* Boucle d'update et affichage des objets */
			updateShip(ship, &bulletsEnnemyList);
			updateEnnemies(ship, &bulletsEnnemyList, &bulletsShipList, &ennemiesList, globalTranslationTotal, textureID, selectedDifficulty+1);
			updateBullets(ship, &bulletsShipList, globalTranslationTotal,textureID);
			updateBullets(ship, &bulletsEnnemyList, globalTranslationTotal,textureID);
			updateBonuses(ship, &bonusesList,textureID);
			updateObstacles(ship, &obstaclesList,textureID);

			/* Debug */
			/*
      printf("Nb bullets ship : %d\n", bulletsShipList.taille);
      printf("Nb bullets ennemy : %d\n", bulletsEnnemyList.taille);
      printf("Nb ennemies : %d\n", ennemiesList.taille);
      printf("Nb obstacles : %d\n", obstaclesList.taille);
      */

			/* Si l'on est a cours de points de vies, game over on affiche le menu de fin */
			if ( ship->hp <= 0) LEVEL_STATE = LEVEL_STATE_ENDED;
		}
		/* ========================== END ========================== */
		else if(GAME_MODE == GAME_MODE_END_GAME ){
			/* >>> Affichage <<< */

			glEnable(GL_TEXTURE_2D);
			/* Affichage du background */
			drawMenuBackground(textureID);      
			/* Affigage du button reset */
			drawEndMenuButton(textureID);      
			/* Affichage du texte de fin */
			if(ship->hp <= 0){
				drawTextLose(textureID);
			} else {
				drawTextWin(textureID);
			}      
			glDisable(GL_TEXTURE_2D);

		}

		/* ========================== BOUCLE D'EVENEMENTS ========================== */
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
					// printf("touche pressée (code = %d)\n", e.key.keysym.sym);
					// if spacebar
					if(GAME_MODE == GAME_MODE_GAME){
						if(e.key.keysym.sym == SDLK_UP) triggerKeyArrowUp = 1;
						if(e.key.keysym.sym == SDLK_DOWN) triggerKeyArrowDown = 1;
						if(e.key.keysym.sym == SDLK_RIGHT) triggerKeyArrowRight = 1;
						if(e.key.keysym.sym == SDLK_LEFT) triggerKeyArrowLeft = 1;
						if(e.key.keysym.sym == SDLK_SPACE) triggerKeySpace = 1;
						if(e.key.keysym.sym == SDLK_LSHIFT || e.key.keysym.sym == SDLK_RSHIFT) triggerKeyShift = 1;
					}
					if(e.key.keysym.sym == 'b') triggerKeyB = 1;
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
					if(e.key.keysym.sym == 'b') triggerKeyB = 0;

					break;

				case SDL_MOUSEBUTTONDOWN:
					if( GAME_MODE == GAME_MODE_MENU ){
						if(e.button.x >= 260 &&
							 e.button.x <= 530 &&
							 e.button.y <= 430 &&
							 e.button.y >= 350){

							if ( triggerKeyB == 1 ) DEBUG = 1;

							if ( DEBUG == 1 ) {
								printf(CYAN);
								printf("========================================\n");
								printf(">>>>>>>>>> MODE DEBUG BOSS ON <<<<<<<<<<\n");
								printf("========================================\n");
								printf(RESET);
								MUSIC_DURATION = MUSIC_DURATION / 100;
							}

							/* >>> BUTTON PLAY <<< */
							/* Suppression des ressources */
							deleteList(&obstaclesList);
							deleteList(&ennemiesList);
							deleteList(&bulletsShipList);
							deleteList(&bulletsEnnemyList);

							/* Creation des entitees */
							obstaclesList.taille = 0;
							ennemiesList.taille = 0;
							bulletsShipList.taille = 0;
							bulletsEnnemyList.taille = 0;
							bonusesList.taille = 0;


							/* Variables qui enregistrent si les touches sont appuyées ou non */
							triggerKeyArrowUp = 0;
							triggerKeyArrowDown = 0;
							triggerKeyArrowLeft = 0;
							triggerKeyArrowRight = 0;
							triggerKeySpace = 0;
							triggerKeyShift = 0;
							triggerKeyB = 0;




							//TO DO une fonction init game ? vu que ça pourrait reservir pour un bouton rejouer sur l'écran de fin

							/* Chargement des ennemies */
							strcpy(bufferOsuFileName,osuFileName);
							strcat(bufferOsuFileName, diff);
							strcat(bufferOsuFileName, ".osu");
							osu = readOsuFile(bufferOsuFileName);
							currentOsuNode = osu.first;


							/* Chargement des obstacles / bonus */
							strcpy(bufferPpmFileName,ppmFileName);
							strcat(bufferPpmFileName, diff);
							strcat(bufferPpmFileName, ".ppm");
							mapLength = createFromPPM(bufferPpmFileName, &obstaclesList, &bonusesList);

							/* Modification de la vitesse de deplacement de la map */
							globalTranslation = (float) mapLength / MUSIC_DURATION * FRAMERATE_MILLISECONDS;
							globalTranslationTotal = 0;

							/* Creation du vaisseau */
							ship = NULL;
							ship = createShip(-4.0, 0.0, 30, 0.5);
							ship->hp = 30;
							ship->pos[X] = -5;
							ship->pos[Y] = 0;

							// Debug fin jeu
							// GAME_MODE = GAME_MODE_END_GAME;

							/* Lancement de la musique */
							Mix_PlayMusic(musicGame, -1);
							int CORRECTIF = 100;
							musicStartTime = SDL_GetTicks() + CORRECTIF;

							/* Changement d'état */
							GAME_MODE = GAME_MODE_GAME;
							LEVEL_STATE = LEVEL_STATE_RUNNING;
							resizeViewport();

						} else if (e.button.x >= selectedButtonPos[0] &&
											 e.button.x <= selectedButtonPos[2] &&
											 e.button.y >= selectedButtonPos[1] &&
											 e.button.y <= selectedButtonPos[3]) {
							selectedDifficulty = 0;
							strcpy(diff,"[ryuu's Easy]");
							printf(YELLOW "Niveau 1 : 'easy'  \n" RESET);
						} else if (e.button.x >= selectedButtonPos[4] &&
											 e.button.x <= selectedButtonPos[6] &&
											 e.button.y >= selectedButtonPos[5] &&
											 e.button.y <= selectedButtonPos[7]) {
							selectedDifficulty = 1;
							strcpy(diff,"[Normal]");
							printf(YELLOW "Niveau 2 : 'normal' \n" RESET);
						} else if (e.button.x >= selectedButtonPos[8] &&
											 e.button.x <= selectedButtonPos[10] &&
											 e.button.y >= selectedButtonPos[9] &&
											 e.button.y <= selectedButtonPos[11]) {
							selectedDifficulty = 2;
							strcpy(diff,"[Advanced]");
							printf(YELLOW "Niveau 3 : 'advanced' \n" RESET);
						} else if (e.button.x >= selectedButtonPos[12] &&
											 e.button.x <= selectedButtonPos[14] &&
											 e.button.y >= selectedButtonPos[13] &&
											 e.button.y <= selectedButtonPos[15]) {
							selectedDifficulty = 3;
							strcpy(diff,"[Hard]");
							printf(YELLOW "Niveau 4 : 'hard'  \n" RESET);
						} else if (e.button.x >= selectedButtonPos[16] &&
											 e.button.x <= selectedButtonPos[18] &&
											 e.button.y >= selectedButtonPos[17] &&
											 e.button.y <= selectedButtonPos[19]) {
							selectedDifficulty = 4;
							strcpy(diff,"[fufufu]");
							printf(YELLOW "Niveau 5 : 'fufufu' \n" RESET);
						}
					}

					if(GAME_MODE == GAME_MODE_END_GAME){
						if(e.button.x >= 290 &&
							 e.button.x <= 505 &&
							 e.button.y <= 660 &&
							 e.button.y >= 605){
							/* >>> BUTTON RESET <<< */
							GAME_MODE = GAME_MODE_MENU;
						}
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

	/* SUPPRESSION DES RESSOURCES */
	/* Libération des données GPU */
	int i;
	for ( i=0; i<TEXTURE_NUMBER; i++) {
		glDeleteTextures(i, textureID);    
	}

	/* Liberation des ressources de la musique */
	Mix_FreeMusic(musicGame);
	Mix_FreeMusic(musicBoss);
	Mix_FreeMusic(musicMenu);
	Mix_CloseAudio();

	/* Liberation des structures allouées */
	if ( ship ) free(ship);
	deleteList(&obstaclesList);
	deleteList(&ennemiesList);
	deleteList(&bulletsEnnemyList);
	deleteList(&bulletsShipList);

	SDL_Quit();

	return EXIT_SUCCESS;
}
