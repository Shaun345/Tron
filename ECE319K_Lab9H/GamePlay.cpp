/*
 * GamePlay.cpp
 *
 *  Created on: Apr 19, 2024
 *      Author: Mark  Helwig
 */

#include <assert.h>
#include "GamePlay.h"
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "images/images.h"
#include "Sound.h"
#include "LED.h"
#include "GameSettings.h"
extern int bikeSpeed;
extern bool abilityEnabled;
extern int currLang;

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);

const int default_capacity = 10;

bool visitedMatrix[160][110];

const unsigned short redTrail[3] = {ST7735_Color565(255, 136, 136), ST7735_Color565(152, 0, 0), ST7735_Color565(87, 0, 0)};
const unsigned short blueTrail[3] = {ST7735_Color565(178, 178, 255), ST7735_Color565(0, 0, 152), ST7735_Color565(0, 0, 87)};

const unsigned short *cycleSprites[4][2] = {{UpBlueCycle, UpRedCycle},
                                            {RightBlueCycle, RightRedCycle},
                                            {DownBlueCycle, DownRedCycle},
                                            {LeftBlueCycle, LeftRedCycle}};

class Bike
{
    int speed;
    bool abilities;
    int lineLength;
    int lineCapacity;
    int number;
    bool dead;
    int last_direction;
    int direction;
    int lives;
    bool slowMode = false;
    bool slowModeCount = false;

    int curPos[2];
    int lastPos[2];

    void newPoint(int delx, int dely)
    {
        lastPos[0] = curPos[0];
        lastPos[1] = curPos[1];

        curPos[0] += delx;
        curPos[1] += dely;

        if (visitedMatrix[curPos[0]][curPos[1]])
            dead = true;

        if (curPos[0] < 1 || curPos[1] < 7 || curPos[0] >= 153 || curPos[1] >= 110)
            dead = true;
    }

    void displayCrash()
    {
        ST7735_DrawBitmap(lastPos[0] - 5, lastPos[1], ExplosionOne, 8, 8);
        Clock_Delay(10000000);
        ST7735_DrawBitmap(lastPos[0] - 5, lastPos[1], ExplosionTwo, 8, 8);
        Clock_Delay(10000000);
        ST7735_DrawBitmap(lastPos[0] - 5, lastPos[1], ExplosionThree, 8, 8);
        Clock_Delay(10000000);
        ST7735_DrawBitmap(lastPos[0] - 5, lastPos[1], ExplosionFour, 8, 8);
        Clock_Delay(10000000);
    }

public:
    Bike() {}
    Bike(int speed, bool abilities, bool number)
    {
        this->speed = speed;
        this->abilities = abilities;
        this->number = number;
        this->dead = false;
        this->lives = 3;
        reset();
    }

    void reset()
    {
        if (!number)
        {
            curPos[0] = lastPos[0] = PLAYER1_START_X;
            curPos[1] = lastPos[1] = PLAYER1_START_Y;
            this->direction = last_direction = DIR_DOWN;
        }
        else
        {
            curPos[0] = lastPos[0] = PLAYER2_START_X;
            curPos[1] = lastPos[1] = PLAYER2_START_Y;
            this->direction = last_direction = DIR_UP;
        }

        this->dead = false;
    }

    void crash()
    {
        lives--;
        // Sound_Explosion();
        displayCrash();
    }

    void turnOffLine(bool toggle)
    {
        this->abilities = toggle;
    }

    void turnOnSlowmode(bool toggle)
    {
        this->slowMode = toggle;
    }

    int health()
    {
        return lives;
    }

    void clear()
    {
        if (!(last_direction % 2))
        {
            ST7735_FillRect(curPos[0] - 2, curPos[1] - 6, 5, 7, ST7735_BLACK);
        }
        else
        {
            ST7735_FillRect(curPos[0], curPos[1] - 2, 7, 5, ST7735_BLACK);
        }
    }

    void updateCorner(const unsigned short *trailColor)
    {
        if (last_direction == DIR_DOWN && (this->direction % 2))
        {
            for (int row = 3; row < 8; row++)
            {
                ST7735_DrawPixel(lastPos[0], lastPos[1] - row, trailColor[0]);
                ST7735_DrawPixel(lastPos[0] - 1, lastPos[1] - row, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + 1, lastPos[1] - row, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] - 2, lastPos[1] - row, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + 2, lastPos[1] - row, trailColor[2]);

                if (this->direction == DIR_RIGHT)
                {
                    visitedMatrix[lastPos[0] - 1][lastPos[1] + row] = true;
                    visitedMatrix[lastPos[0] - 2][lastPos[1] + row] = true;
                }
                else
                {
                    visitedMatrix[lastPos[0] + 1][lastPos[1] + row] = true;
                    visitedMatrix[lastPos[0] + 2][lastPos[1] + row] = true;
                }
            }
        }
        if (last_direction == DIR_UP && (this->direction % 2))
        {
            for (int row = 0; row < 4; row++)
            {
                ST7735_DrawPixel(lastPos[0], lastPos[1] - row, trailColor[0]);
                ST7735_DrawPixel(lastPos[0] - 1, lastPos[1] - row, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + 1, lastPos[1] - row, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] - 2, lastPos[1] - row, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + 2, lastPos[1] - row, trailColor[2]);

                visitedMatrix[lastPos[0] + 0][lastPos[1] + row] = true;

                if (this->direction == DIR_RIGHT)
                {
                    visitedMatrix[lastPos[0] - 1][lastPos[1] + row] = true;
                    visitedMatrix[lastPos[0] - 2][lastPos[1] + row] = true;
                }
                else
                {
                    visitedMatrix[lastPos[0] + 1][lastPos[1] + row] = true;
                    visitedMatrix[lastPos[0] + 2][lastPos[1] + row] = true;
                }
            }
        }
        if (last_direction == DIR_RIGHT && !(this->direction % 2))
        {
            for (int col = 0; col < 4; col++)
            {
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1], trailColor[0]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] - 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] + 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] - 2, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] + 2, trailColor[2]);

                visitedMatrix[lastPos[0] + col][lastPos[1]] = true;

                if (this->direction == DIR_DOWN)
                {
                    visitedMatrix[lastPos[0] + col][lastPos[1] - 1] = true;
                    visitedMatrix[lastPos[0] + col][lastPos[1] - 2] = true;
                }
                else
                {
                    visitedMatrix[lastPos[0] + col][lastPos[1] + 1] = true;
                    visitedMatrix[lastPos[0] + col][lastPos[1] + 2] = true;
                }
            }
        }
        if (last_direction == DIR_LEFT && !(this->direction % 2))
        {
            for (int col = 3; col < 8; col++)
            {
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1], trailColor[0]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] - 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] + 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] - 2, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + col, lastPos[1] + 2, trailColor[2]);

                visitedMatrix[lastPos[0] + col][lastPos[1]] = true;

                if (this->direction == DIR_DOWN)
                {
                    visitedMatrix[lastPos[0] + col][lastPos[1] - 1] = true;
                    visitedMatrix[lastPos[0] + col][lastPos[1] - 2] = true;
                }
                else
                {
                    visitedMatrix[lastPos[0] + col][lastPos[1] + 1] = true;
                    visitedMatrix[lastPos[0] + col][lastPos[1] + 2] = true;
                }
            }
        }
    }

    void updateLocation(int direction)
    {
        int conversion = direction;
        switch (direction)
        {
        case 0:
            conversion = -1;
            break;
        case 1:
            conversion = DIR_LEFT;
            break;
        case 2:
            conversion = DIR_RIGHT;
            break;
        case 4:
            conversion = DIR_DOWN;
            break;
        case 8:
            conversion = DIR_UP;
            break;
        }

        last_direction = this->direction;
        switch (conversion)
        {
        case DIR_UP:
        case DIR_DOWN:
        case DIR_LEFT:
        case DIR_RIGHT:
            if ((last_direction - conversion) != 2 && (last_direction - conversion) != -2)
            {
                this->direction = conversion;
            }

        default:
            break;
        }

        switch (this->direction)
        {
        // Updated
        case DIR_UP:
            if (last_direction % 2)
            {
                clear();
                newPoint(3, -1);
            }
            else
                newPoint(0, -1);
            break;
        // Updated
        case DIR_RIGHT:
            if (!(last_direction % 2))
            {
                clear();
                newPoint(1, -3);
            }
            else
                newPoint(1, 0);
            break;
        // Updated
        case DIR_DOWN:
            if (last_direction % 2)
            {
                clear();
                newPoint(3, 8);
            }
            else
                newPoint(0, 1);
            break;
        // Updated
        case DIR_LEFT:
            if (!(last_direction % 2))
            {
                clear();
                newPoint(-8, -3);
            }
            else
                newPoint(-1, 0);
            break;
        }
    }

    bool collisions(Bike other)
    {
        if (other.curPos[0] == curPos[0] && other.curPos[1] == curPos[1])
            return true;

        return dead;
    }

    void firstDisplay()
    {
        // display bike sprite at starting coordinate
        if (!number)
            ST7735_DrawBitmap(PLAYER1_START_X - 2, PLAYER1_START_Y, DownBlueCycle, 5, 7);
        else
            ST7735_DrawBitmap(PLAYER2_START_X - 2, PLAYER2_START_Y, UpRedCycle, 5, 7);
    }

    void display()
    {
        // display bike sprite at current coordinate
        // if current direction up or down, display vertical line sprite at last coordinate
        // else display horizontal sprite at last coordinate
        const unsigned short *trailColor = (!number) ? blueTrail : redTrail;

        int xOff = 0;
        int yOff = 0;

        switch (last_direction)
        {
        case DIR_LEFT:
            xOff = (abilities && abilityEnabled) ? 6 : 7;
            break;

        case DIR_DOWN:
            yOff = (abilities && abilityEnabled) ? -6 : -7;
            break;

        case DIR_UP:
        case DIR_RIGHT:
            break;
        }

        switch (this->direction)
        {
        case DIR_UP:
        case DIR_DOWN:
            ST7735_DrawBitmap(curPos[0] - 2, curPos[1], cycleSprites[direction][number], 5, 7);
            if (abilities && abilityEnabled)
                ST7735_DrawPixel(lastPos[0], lastPos[1] + yOff, ST7735_BLACK);
            break;

        case DIR_LEFT:
        case DIR_RIGHT:
            ST7735_DrawBitmap(curPos[0], curPos[1] + 2, cycleSprites[direction][number], 7, 5);
            if (abilities && abilityEnabled)
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1], ST7735_BLACK);
            break;
        }

        if (!(abilities && abilityEnabled))
        {
            switch (last_direction)
            {
            case DIR_UP:
            case DIR_DOWN:
                ST7735_DrawPixel(lastPos[0], lastPos[1] + yOff, trailColor[0]);
                ST7735_DrawPixel(lastPos[0] - 1, lastPos[1] + yOff, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + 1, lastPos[1] + yOff, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] - 2, lastPos[1] + yOff, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + 2, lastPos[1] + yOff, trailColor[2]);

                visitedMatrix[lastPos[0] + 0][lastPos[1] + yOff] = true;
                visitedMatrix[lastPos[0] - 1][lastPos[1] + yOff] = true;
                visitedMatrix[lastPos[0] + 1][lastPos[1] + yOff] = true;
                visitedMatrix[lastPos[0] - 2][lastPos[1] + yOff] = true;
                visitedMatrix[lastPos[0] + 2][lastPos[1] + yOff] = true;
                break;

            case DIR_LEFT:
            case DIR_RIGHT:
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1], trailColor[0]);
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1] - 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1] + 1, trailColor[1]);
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1] - 2, trailColor[2]);
                ST7735_DrawPixel(lastPos[0] + xOff, lastPos[1] + 2, trailColor[2]);

                visitedMatrix[lastPos[0] + xOff][lastPos[1] + 0] = true;
                visitedMatrix[lastPos[0] + xOff][lastPos[1] + 1] = true;
                visitedMatrix[lastPos[0] + xOff][lastPos[1] - 1] = true;
                visitedMatrix[lastPos[0] + xOff][lastPos[1] + 2] = true;
                visitedMatrix[lastPos[0] + xOff][lastPos[1] - 2] = true;
                break;
            }

            updateCorner(trailColor);
        }
    }
};

class Gameplay
{

    int speed;
    int language;
    bool abilities;

    bool gameDone = false;

    Bike player1;
    Bike player2;

    void countDown()
    {
        // display background
        // display bikes
        // display countdown timer
        ST7735_DrawFastVLine(0, 0, 110, 65289);
        ST7735_DrawFastVLine(159, 0, 110, 65289);
        ST7735_DrawFastHLine(0, 0, 160, 65289);
        ST7735_DrawFastHLine(0, 110, 160, 65289);

        player1.firstDisplay();
        player2.firstDisplay();

        for (int col = 0; col < 160; col++)
            for (int row = 0; row < 110; row++)
                visitedMatrix[col][row] = false;

        LED_Off(7);
        LED_On(1);
        Sound_Select();
        ST7735_DrawBitmap(60, 79, three_image, 40, 40);
        Clock_Delay(50000000);

        Sound_Select();
        ST7735_DrawBitmap(60, 79, two_image, 40, 40);
        Clock_Delay(50000000);

        LED_Off(7);
        LED_On(2);
        Sound_Select();
        ST7735_DrawBitmap(60, 79, one_image, 40, 40);
        Clock_Delay(50000000);

        LED_Off(7);
        LED_On(4);
        Sound_Select();
        ST7735_FillRect(60, 40, 40, 40, ST7735_BLACK);
    }

    void gameOver()
    {
        if (player1.health() <= 0 || player2.health() <= 0)
        {
            gameDone = true;
            __disable_irq();
            TimerG12_IntArm(80000000 / 30, 2);
            __enable_irq();
        }
    }

    void updateLives(void)
    {
        for (int life = 0; life < 3; life++)
        {
            if (player1.health() > life)
            {
                ST7735_DrawBitmap(5 + 12 * life, 125, BlueHeart, 9, 9);
            }
            else
            {
                ST7735_FillRect(5 + 12 * life, 117, 9, 9, ST7735_BLACK);
            }

            if (player2.health() > life)
            {
                ST7735_DrawBitmap(145 - 12 * life, 125, RedHeart, 9, 9);
            }
            else
            {
                ST7735_FillRect(145 - 12 * life, 117, 9, 9, ST7735_BLACK);
            }
        }
    }

public:
    int winner;

    Gameplay(int speed, int language, bool abilities)
    {
        this->speed = speed;
        this->language = language;
        this->abilities = abilities;
        player1 = Bike(speed, abilities, 0);
        player2 = Bike(speed, abilities, 1);
    }

    void reset()
    {
        delete &player1;
        delete &player2;
        player1 = Bike(speed, abilities, 0);
        player2 = Bike(speed, abilities, 1);
        gameDone = false;

        newRound();
    }

    void newRound()
    {
        ST7735_FillRect(0, 0, 160, 110, ST7735_BLACK);
        player1.reset();
        player2.reset();
        updateLives();
        countDown();
    }

    bool update(int player1Direction, int player2Direction)
    {
        player1.updateLocation(player1Direction);
        player2.updateLocation(player2Direction);
        if (player1.collisions(player2))
        {
            player1.crash();
            if (!player1.health())
            {
                winner = 1;
                gameOver();
                return true;
            }
            else
                newRound();
        }
        if (player2.collisions(player1))
        {
            player2.crash();
            if (!player2.health())
            {
                winner = 0;
                gameOver();
                return true;
            }
            else
                newRound();
        }
        player1.display();
        player2.display();
        return false;
    }

    bool isGameOver()
    {
        return gameDone;
    }

    void toggleAbilities(bool p1, bool p2)
    {
        player1.turnOffLine(p1);
        player2.turnOffLine(p2);
    }
};

Gameplay gameRunner(bikeSpeed, currLang, abilityEnabled);

void gameInit(void)
{
    __disable_irq();
    TimerG12_IntArm(80000000 / (15 * (bikeSpeed + 1)), 2);
    __enable_irq();

    ST7735_FillScreen(ST7735_BLACK);
    gameRunner.reset();
}

void gameUpdate(int input)
{
    // gameRunner.update(0x000F & (input >> 8), input >> 12);
    gameRunner.update(0x000F & (input >> 8), 0);
    gameRunner.toggleAbilities((input & 0x0020) >> 5, (input & 0x1000) >> 12);
}

bool gameFinished()
{
    return gameRunner.isGameOver();
}
