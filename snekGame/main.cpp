#include <iostream>
#include <ncurses.h> //input and output
#include <cstdlib>   // rand() and srand()
#include <ctime>     // time()
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;

int x, y, fruitX, fruitY; // coordinates for head and fruit
int score;

int tailX[100], tailY[100];
int nTail; // length of tail

int gameSpeed = 250;

enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

eDirection dir;

void Setup()
{
    gameOver = false;
    dir = STOP;
    // snake head centered on map
    x = width / 2;
    y = height / 2;

    // distribute fruit randomly on map
    fruitX = rand() % width;
    fruitY = rand() % height;

    score = 0;
}

void Draw()
{
    // system("clear"); // system(clear) on Linux
    clear();

    // Top boundary
    for (int i = 0; i < width + 2; i++)
    {
        mvprintw(0, i, "#");
        // cout << "#";
    }
    // cout << endl;

    // Setting up map
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                mvprintw(i + 1, j, "#");

            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "O"); // Snake head
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "F"); // Fruit
            else
            {

                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        mvprintw(i + 1, j + 1, "o");
                        print = true;
                    }
                }
                if (!print)
                {
                    mvprintw(i + 1, j + 1, " ");
                }
            }

            if (j == width - 1)
                mvprintw(i + 1, j + 2, "#");
        }
    }

    // Bottom boundary
    for (int i = 0; i < width + 2; i++)
    {
        mvprintw(height + 1, i, "#");
        // cout << "#";
    }
    // cout << endl;

    mvprintw(height + 3, 0, "Score: %d", score);

    refresh(); // update the screen
}

void Input()
{
    nodelay(stdscr, TRUE);
    int ch = getch();
    if (ch != ERR)
    {
        // A key was pressed, 'ch' holds the character value
        // Process the key press
        switch (ch)
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic()
{
    // Tail growth
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    // Head
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // if snake out of bounds
    if (x < 0 || x > width || y < 0 || y > height)
        gameOver = true;

    // if snake head runs into tail
    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
        }
    }

    // if eaten a fruit
    if (x == fruitX && y == fruitY)
    {
        gameSpeed -= 50;
        nTail++;
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
    }

    // add new snake head?
}

int main()
{
    srand(time(0));

    initscr();   // ncurses.h
    noecho();    // ncurses.h
    curs_set(0); // ncurses.h

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        napms(gameSpeed); // delay 100ms (ncurses function)
        // Sleep(10); — slows the game (no implemented)
    }

    clear();
    mvprintw(height / 2, width / 2 - 5, "Game Over!");
    mvprintw(height / 2 + 1, width / 2 - 7, "Final Score: %d", score);
    refresh();
    nodelay(stdscr, FALSE);
    getch();

    endwin(); // ncurses.h

    return 0;
}