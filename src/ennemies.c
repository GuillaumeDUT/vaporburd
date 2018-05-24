#include "libs.h"

#include "ennemies.h"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
float WINDOW_SCALE;
int GAME_MODE;

void createRandomEnnemy(EList *ennemies, float globalTranslation) {
	int limit = WINDOW_SCALE/2 - 1;
	ajouterFinList(ennemies, createEnnemy(
		frand_a_b(limit, limit-5) + globalTranslation,
		frand_a_b(-limit, limit),
		5, /* HP */
		0.5 /* SIZE */));
}
void createOSUNodeEnnemy(EList *ennemies, OSUNode oNode, float globalTranslation)
{
	int limit = WINDOW_SCALE/2 - 1;
	ajouterFinList(ennemies, createEnnemy(
		limit + globalTranslation,
		fmap(oNode->y, 0, 384, -limit, limit),
		15, /* HP */
		1.5 /* SIZE */));
}
void createHommingEnnemy(EList *ennemies, float y, Ennemy boss) {
	int limit = WINDOW_SCALE/2 - 2;
	Ennemy homming = createEnnemy(
		limit + boss->pos[X],
		y, /* Y */
		300, /* HP */
		1 /* SIZE */
	);

	homming->speed[X] = boss->speed[X] - 0.5;
	homming->type = ENNEMY_HOMMING;
	homming->damages = 5;
	homming->attackPerSecond = 0;

	ajouterFinList(ennemies, homming);
}
void createBoss(EList *ennemies, float globalTranslation, float globalTranslationTotal, int diff) {
	int limit = WINDOW_SCALE/2 - 2;
	Ennemy boss = createEnnemy(
		limit + globalTranslationTotal,
		0, /* Y */
		BOSS_HP * diff, /* HP */
		2 /* SIZE */
	);

	boss->speed[X] = globalTranslation;
	boss->type = ENNEMY_BOSS;
	boss->damages = 2;
	boss->attackPerSecond = 0.4;
	boss->missileLevel = 3;

	ajouterFinList(ennemies, boss);
}
void updateEnnemies(Ship ship, BList *bulletsEnnemy, BList *bulletsShip, EList *ennemies, float globalTranslationTotal, GLuint textureID[], int diff) {
	if(ennemies->taille == 0){
		return ;
	}
	Ennemy eActuel = ennemies->first;
	Ennemy eNext;
	while ( eActuel != NULL ) {
		eNext = eActuel->next;
		if ( eActuel->type == ENNEMY_BOSS ) {

			/* Patterns du boss */
			if ( eActuel->hp > BOSS_HP * 3 ) {
				bossPattern2( eActuel, ship, bulletsEnnemy, ennemies);
			} else if ( eActuel->hp > BOSS_HP * 2 ) {
				bossPattern3( eActuel, ship, bulletsEnnemy );
			} else if ( eActuel->hp > BOSS_HP ) {
				bossPattern1( eActuel, ship, bulletsEnnemy );
			} else {

				/* Deplacement */
				if ( eActuel->pos[Y] < ship->pos[Y] ) {
					eActuel->speed[Y] += ACC/5;
				} else if ( eActuel->pos[Y] > ship->pos[Y] ) {
					eActuel->speed[Y] -= ACC/5;
				} else {
					eActuel->speed[Y] = 0;
				}

				shootEnnemy( eActuel, bulletsEnnemy );
			}

		}
		else if ( eActuel->type == ENNEMY_HOMMING ) {

			/* Deplacement */
			float deltaY;

			deltaY = ship->pos[Y] - eActuel->pos[Y];

			eActuel->speed[Y] = deltaY / 25;
			eActuel->speed[X] = -WINDOW_SCALE / 100;

		}
		else if ( eActuel->type == ENNEMY_SIMPLE ) {

			/* Deplacement */
			float freq = 0.01;
			float depth = 4;
			float offX = perlin2d(eActuel->perlinOffsetX,
														eActuel->perlinOffsetY,
														freq, depth);
			float offY = perlin2d(eActuel->perlinOffsetY,
														eActuel->perlinOffsetX,
														freq, depth);

			eActuel->perlinOffsetX += 1;
			eActuel->perlinOffsetY += 1;

			offX = fmap(offX, 0, 1, -0.1, 0.1);
			offY = fmap(offY, 0, 1, -0.1, 0.1);

			eActuel->speed[X] = offX;
			eActuel->speed[Y] = offY;
			shootEnnemy( eActuel, bulletsEnnemy );

		}
		
		moveEnnemy( eActuel, globalTranslationTotal );
		drawEnnemy( eActuel, 0 , textureID, diff);

		/* Collision avec les bullets du ship */
		if(bulletsShip->taille != 0){
			Bullet bulletActuel = bulletsShip->first;
			Bullet bulletNext;
			while ( bulletActuel != NULL ) {
				bulletNext = bulletActuel->next;
				if ( bulletActuel->type == BULLET_SHIP && collision(bulletActuel, eActuel) ) {
					/*displayEntity(eActuel);*/
					drawEnnemy( eActuel, 1 ,textureID, diff);
					getDamage(bulletActuel, eActuel);
					supprimerList(bulletsShip, bulletActuel->id);
				}
				bulletActuel = bulletNext;
			}
		}

		/* Collision avec le ship */
		if ( collision(ship, eActuel) ) {			
			if ( eActuel->type == ENNEMY_BOSS ) {
				ship->hp = 0;
			} else {
				drawEnnemy(eActuel, 1,textureID, diff);
				getDamage(eActuel, ship);
				eActuel->hp = 0;
			}
		}

		/* Verification de la mort */
		/* Le boss est vaincu */
		if ( eActuel->hp <= 0 )  {
			if ( eActuel->type == ENNEMY_BOSS ) {
				LEVEL_STATE = LEVEL_STATE_ENDED;
			}
			supprimerList(ennemies, eActuel->id);
		}
		eActuel = eNext;
	}
}
void drawEnnemy(Ennemy ennemy, int full, GLuint textureID[], int diff) {
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		if(ennemy->type == ENNEMY_BOSS){
			float scale;
			scale = ennemy->hp / BOSS_HP;
			scale = scale / diff;
			scale += 0.2;
			glBindTexture(GL_TEXTURE_2D, textureID[20]);
			glBegin(GL_QUADS);
			{
				glColor3ub(255,255,255);
				glTexCoord2f(0, 0);
				glVertex2f(ennemy->pos[X]-1 *scale, ennemy->pos[Y]+1.77 *scale);

				glTexCoord2f(1, 0);
				glVertex2f(ennemy->pos[X]+1 *scale, ennemy->pos[Y]+1.77 *scale);

				glTexCoord2f(1, 1);
				glVertex2f(ennemy->pos[X]+1 *scale, ennemy->pos[Y]-1.77 *scale);

				glTexCoord2f(0, 1);
				glVertex2f(ennemy->pos[X]-1 *scale, ennemy->pos[Y]-1.77 *scale);
			}
			glEnd();
		}else{
			glBindTexture(GL_TEXTURE_2D, textureID[8]);
			glBegin(GL_QUADS);
			{
				glColor3ub(255,255,255);
				glScalef(0.5,0.5,1);
				glTexCoord2f(0, 0);
				glVertex2f(ennemy->pos[X] -0.5 *1.2,ennemy->pos[Y] +0.5 *1.2);

				glTexCoord2f(1, 0);
				glVertex2f(ennemy->pos[X]+0.5 *1.2,ennemy->pos[Y] +0.5 *1.2);

				glTexCoord2f(1, 1);
				glVertex2f(ennemy->pos[X]+0.5 *1.2,ennemy->pos[Y] -0.5 *1.2);

				glTexCoord2f(0, 1);
				glVertex2f(ennemy->pos[X]-0.5 *1.2,ennemy->pos[Y] -0.5 *1.2);
			}
			glEnd();
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	if ( ennemy->type == ENNEMY_BOSS ) {
		/* Affichage de la barre de vie */
		float percent = (float)ennemy->hp/(BOSS_HP*diff);
		glPushMatrix();
		{
			glLoadIdentity();
			glColor3ub(112, 208, 69);

			/* Outline */
			glTranslatef(0, 0.92, 0);
			glScalef(1.9, 0.05, 1);

			glPushMatrix();
			{
				/* Plain */
				glTranslatef(-(1-percent)/2, 0, 0);
				glScalef(percent, 1, 1);
				drawSquare(1);
			}
			glPopMatrix();

			drawSquare(0);
		}
		glPopMatrix();
	}
}

void moveEnnemy(Ennemy ennemy, int globalTranslationTotal) {
	int LIMIT = WINDOW_SCALE/2;
	ennemy->pos[X] += ennemy->speed[X];
	ennemy->pos[Y] += ennemy->speed[Y];

	ennemy->speed[Y] = absFloat(ennemy->speed[Y]) > 0.0001 ?
		ennemy->speed[Y] * 0.7
		: 0;

	/* Contraint l'ennemi a rester à l'ecran */
	if ( ennemy->pos[X] > LIMIT + globalTranslationTotal) {
		ennemy->pos[X] = LIMIT + globalTranslationTotal;
		ennemy->speed[X] = 0;
	}

	/* Deplacement du bas en haut */
	if ( ennemy->pos[Y] > LIMIT ) ennemy->hp = 0;
	if ( ennemy->pos[Y] < -LIMIT ) ennemy->hp = 0;
}

void shootEnnemy(Ennemy ennemy, BList *bullets) {
	ennemy->cooldown = ennemy->cooldown > 0 ? ennemy->cooldown-1 : 0;
	if ( ennemy->cooldown <= 0 ) {
		ennemy->cooldown = 1 + FRAMERATE_MILLISECONDS/(ennemy->attackPerSecond);
		shoot(ennemy, bullets);
	}
}

void bossPattern1(Ennemy boss, Ship ship, BList *bullets) {
	boss->cooldown = boss->cooldown > 0 ? boss->cooldown-1 : 0;

	/* Tir par raffale de 5 coups */
	if ( boss->cooldown < 50 && boss->cooldown % 10 == 0 ) {
		bossAttack1(boss, ship, bullets, boss->cooldown);
	}

	/* Modifie le cooldown */
	if ( boss->cooldown <= 0 ) {
		boss->cooldown = 100;
	}

}
void bossPattern2(Ennemy boss, Ship ship, BList *bullets, EList *ennemies) {
	boss->cooldown = boss->cooldown > 0 ? boss->cooldown-1 : 0;


	switch (boss->cooldown) {
		case 20:
			createHommingEnnemy(ennemies, 4.0, boss);
			break;
		case 40:
			createHommingEnnemy(ennemies, 0.0, boss);
			break;
		case 80:
			createHommingEnnemy(ennemies, -4.0, boss);
			break;
	}


	if ( boss->cooldown <= 0 ) {
		boss->cooldown = 160;
	}

}
void bossPattern3(Ennemy boss, Ship ship, BList *bullets) {
	boss->cooldown = boss->cooldown > 0 ? boss->cooldown-1 : 0;

	if ( boss->cooldown <= 0 ) {
		boss->cooldown = 30;
		bossAttack3(boss, ship, bullets);
	}

}
