#include "libs.h"

#include "ennemies.h"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
float WINDOW_SCALE;

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
void updateEnnemies(Ship ship, BList *bullets, EList *ennemies, int globalTranslationTotal,GLuint textureID[]) {
  if(ennemies->taille == 0){
    return ;
  }
  Ennemy eActuel = ennemies->first;
  Ennemy eNext;
  while ( eActuel != NULL ) {
    eNext = eActuel->next;
    if ( eActuel->ennemyType == ENNEMY_TYPE_BOSS ) {

      /* Deplacement */
      if ( eActuel->pos[Y] < ship->pos[Y] ) {
        eActuel->speed[Y] += ACC/5;
      } else if ( eActuel->pos[Y] > ship->pos[Y] ) {
        eActuel->speed[Y] -= ACC/5;        
      } else {
        eActuel->speed[Y] = 0;
      }

      /* Patterns du boss */
      if ( eActuel->hp / BOSS_HP >= 0.8 ) {
        /* PATTERN 1 */
        bossPattern1( eActuel, ship, bullets );
        //        shootEnnemy( eActuel, bullets );     
      } else {
        shootEnnemy( eActuel, bullets );        
      }

    } else if ( eActuel->ennemyType == ENNEMY_TYPE_BASIC ) {

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
      shootEnnemy( eActuel, bullets );

    }    
    moveEnnemy( eActuel, globalTranslationTotal );
    drawEnnemy( eActuel, 0 , textureID);

    /* Collision avec les bullets */
    if(bullets->taille != 0){
      Bullet bulletActuel = bullets->first;
      Bullet bulletNext;
      while ( bulletActuel != NULL ) {
        bulletNext = bulletActuel->next;
        if ( bulletActuel->ennemyType == NOT_AN_ENNEMY && collision(bulletActuel, eActuel) ) {
          /*displayEntity(eActuel);*/
          drawEnnemy( eActuel, 1 ,textureID);
          getDamage(bulletActuel, eActuel);
          supprimerList(bullets, bulletActuel->id);
        }
        bulletActuel = bulletNext;
      }
    }

    /* Collision avec le ship */
    if ( collision(ship, eActuel) ) {
      drawShip(ship, 1);
      drawEnnemy(eActuel, 1,textureID);
      getDamage(ship, eActuel);
      getDamage(eActuel, ship);
      /*displayEntity(ship);*/
    }

    /* Verification de la mort */
    if ( eActuel->hp <= 0 )  {
      supprimerList(ennemies, eActuel->id);
    }
    eActuel = eNext;
  }
}
void drawEnnemy(Ennemy ennemy, int full,GLuint textureID[]) {
  glPushMatrix();
  {    
    glColor3f(255, 0, 120);
    glTranslatef(
      ennemy->pos[X],
      ennemy->pos[Y],
      0);
    glScalef(
      ennemy->size,
      ennemy->size,
      1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID[8]);
    // if(ennemy->ennemyType == ENNEMY_TYPE_BASIC){
      
    // }else if(ennemy->ennemyType == ENNEMY_TYPE_BOSS){
    //   glBindTexture(GL_TEXTURE_2D, textureID[8]);
    // } 
    glBegin(GL_QUADS);
      {
        glColor3ub(255,255,255);
        glScalef(0.5,0.5,1);
        glTexCoord2f(0, 0);
        glVertex2f(-0.5, +0.5);

        glTexCoord2f(1, 0);
        glVertex2f(+0.5, +0.5);

        glTexCoord2f(1, 1);
        glVertex2f(+0.5, -0.5);

        glTexCoord2f(0, 1);
        glVertex2f(-0.5, -0.5);
      }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    //drawCircle(full);
  }
  glPopMatrix();

  if ( ennemy->ennemyType == ENNEMY_TYPE_BOSS ) {
    /* Affichage de la barre de vie */
    float percent = (float)ennemy->hp/BOSS_HP;
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

  /* Deplacement du bas en h*(deltaTime-25);
  bullet->size = 2.5; aut */
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

void bossPattern1(Ennemy ennemy, Ship ship, BList *bullets) {
  ennemy->cooldown = ennemy->cooldown > 0 ? ennemy->cooldown-1 : 0;
  if ( ennemy->cooldown < 50 && ennemy->cooldown % 10 == 0 ) {
    bossAttack1(ennemy, ship, bullets, ennemy->cooldown);    
  }
  if ( ennemy->cooldown <= 0 ) {
    ennemy->cooldown = 100;
  }

}

void createBoss(EList *ennemies, float globalTranslation, float globalTranslationTotal) {
  int limit = WINDOW_SCALE/2 - 2;
  Ennemy boss = createEnnemy(
    limit + globalTranslationTotal,
    0, /* Y */
    BOSS_HP, /* HP */
    2 /* SIZE */
  );

  boss->speed[X] = globalTranslation;
  boss->ennemyType = ENNEMY_TYPE_BOSS;
  boss->damages = 2;
  boss->attackPerSecond = 0.4;
  boss->missileLevel = 3;

  ajouterFinList(ennemies, boss);
}