/*
 * GamePlay.h
 *
 *  Created on: Apr 20, 2024
 *      Author: helwi
 */

#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define PLAYER1_START_X 10
#define PLAYER1_START_Y 10
#define PLAYER2_START_X 150
#define PLAYER2_START_Y 100

void gameInit(void);
void gameUpdate(int);
void gameOver(void);
bool gameFinished(void);
int getAbility();

#endif /* GAMEPLAY_H_ */
