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
#include "images/images.h"

const int default_capacity = 10;

const int redTrail[3] = {64593, 20480, 8192};
const int blueTrail[3] = {46495, 10, 4};

const unsigned short *cycleSprites[4][2] = {{UpBlueCycle, UpRedCycle},
                                {RightBlueCycle, RightRedCycle},
                                {DownBlueCycle, DownRedCycle},
                                {LeftBlueCycle, LeftRedCycle}};

class Coord
{
    int x;
    int y;

public:
    Coord(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    Coord()
    {
        x = 0;
        y = 0;
    }

    bool operator==(Coord &other)
    {
        if (other.x == this->x && other.y == this->y)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    Coord operator+(Coord &other)
    {
        Coord res;
        res.x = x + other.x;
        res.y = y + other.y;
        return res;
    }

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }
};

class Line
{
    Coord *data;
    int length;
    int capacity;

public:
    Line()
    {
        this->capacity = default_capacity;
        this->data = new Coord[this->capacity];
        this->length = 0;
    }

    int getLength()
    {
        return length;
    }

    Coord operator[](int index)
    {
        // assert((index > 0) && (index < this->length));
        return this->data[index];
    }

    bool isMember(Coord point)
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->data[i] == point)
            {
                return true;
            }
        }
        return false;
    }

    bool operator[](Coord point)
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->data[i] == point)
            {
                return false;
            }
        }

        this->capacity *= 2;
        Coord *temp = new Coord[this->capacity];
        for (int i = 0; i < this->length; i++)
        {
            temp[i] = this->data[i];
        }
        delete[] this->data;
        this->data = temp;
        this->data[this->length] = point;
        length++;
        return true;
    }
};

class Bike
{
    int speed;
    bool abilities;
    Line line;
    int lineLength;
    int lineCapacity;
    int number;
    bool dead;
    int direction;
    int lives;

    void newPoint(int x, int y)
    {
        Coord point = Coord(x, y);
        if (!line[point])
        {
            dead = true;
        }
    }

    void displayCrash()
    {
        // display crash sprite at last coordinate
        Coord last = line[line.getLength() - 7];

        ST7735_DrawBitmap(last.getX(), last.getY(), ExplosionOne, 8, 8);
        ST7735_DrawBitmap(last.getX(), last.getY(), ExplosionTwo, 8, 8);
        ST7735_DrawBitmap(last.getX(), last.getY(), ExplosionThree, 8, 8);
        ST7735_DrawBitmap(last.getX(), last.getY(), ExplosionFour, 8, 8);
    }

public:
    Bike() {}
    Bike(int speed, bool abilities, bool number)
    {
        this->speed = speed;
        this->abilities = abilities;
        this->number = number;
        if (number)
        {
            line[0] = Coord(PLAYER1_START_X, PLAYER1_START_Y);
        }
        else
        {
            line[0] = Coord(PLAYER2_START_X, PLAYER2_START_Y);
        }
    }

    void crash()
    {
        lives--;
        displayCrash();
    }

    int health()
    {
        return lives;
    }

    void updateLocation(int direction)
    {
        this->direction = direction;
        Coord point = line[line.getLength() - 1];
        switch (direction)
        {
        case DIR_UP:
            newPoint(point.getX(), point.getY() - 1);
            break;
        case DIR_RIGHT:
            newPoint(point.getX() + 1, point.getY());
            break;
        case DIR_DOWN:
            newPoint(point.getX(), point.getY() + 1);
            break;
        case DIR_LEFT:
            newPoint(point.getX() - 1, point.getY());
            break;
        }
    }

    bool collisions(Bike other)
    {
        if (dead)
        {
            return true;
        }
        Coord currentPos = this->line[this->lineLength - 1];
        if (other.line.isMember(currentPos))
        {
            dead = true;
            return true;
        }
        if (currentPos.getX() < 0 || currentPos.getY() < 0 || currentPos.getX() >= 160 || currentPos.getY() >= 128)
        {
            dead = true;
            return true;
        }
    }

    void firstDisplay()
    {
        // display bike sprite at starting coordinate
        if (!number)
        {
            ST7735_DrawBitmap(PLAYER1_START_X, PLAYER1_START_Y, DownBlueCycle, 5, 7);
        }
        else
        {
            ST7735_DrawBitmap(PLAYER2_START_X, PLAYER2_START_Y, UpRedCycle, 5, 7);
        }
    }

    void display()
    {
        // display bike sprite at current coordinate
        // if current direction up or down, display vertical line sprite at last coordinate
        // else display horizontal sprite at last coordinate
        Coord last = line[line.getLength() - 7];
        Coord current = line[line.getLength() - 1];
        const int *trailColor = !number ? blueTrail : redTrail;

        int width = direction % 2 ? 7 : 5;
        int height = direction % 2 ? 5 : 7;

        ST7735_DrawBitmap(current.getX(), current.getY(), cycleSprites[direction][number], width, height);

        switch (direction)
        {
        case DIR_UP:
        case DIR_DOWN:
            ST7735_DrawPixel(last.getX(), last.getY(), trailColor[0]);
            ST7735_DrawPixel(last.getX() - 1, last.getY(), trailColor[1]);
            ST7735_DrawPixel(last.getX() + 1, last.getY(), trailColor[1]);
            ST7735_DrawPixel(last.getX() - 2, last.getY(), trailColor[2]);
            ST7735_DrawPixel(last.getX() + 2, last.getY(), trailColor[2]);
            break;

        case DIR_LEFT:
        case DIR_RIGHT:
            ST7735_DrawPixel(last.getX(), last.getY(), trailColor[0]);
            ST7735_DrawPixel(last.getX(), last.getY() - 1, trailColor[1]);
            ST7735_DrawPixel(last.getX(), last.getY() + 1, trailColor[1]);
            ST7735_DrawPixel(last.getX(), last.getY() - 2, trailColor[2]);
            ST7735_DrawPixel(last.getX(), last.getY() + 2, trailColor[2]);
            break;
        }
    }
};

class Gameplay
{

    int speed;
    int language;
    bool abilities;
    Bike player1;
    Bike player2;

    void countDown()
    {
        // display background
        // display bikes
        // display countdown timer
        ST7735_FillRect(0, 0, 159, 113, 0);
        player1.firstDisplay();
        player2.firstDisplay();
        ST7735_DrawBitmap(60, 40, three_image, 40, 80);
        Clock_Delay(1000);
        ST7735_DrawBitmap(60, 40, two_image, 40, 80);
        Clock_Delay(1000);
        ST7735_DrawBitmap(60, 40, one_image, 40, 80);
        Clock_Delay(1000);
        ST7735_FillRect(60, 40, 40, 80, 0);
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
        }
        player1.display();
        player2.display();
        return false;
    }

    void gameOver()
    {
    }
};
