
#include <random>
#include <time.h>
#include "Cellular_Automata.h"

Cellular_Automata::Cellular_Automata() : smoothNum(5)
{
    
}

Cellular_Automata::~Cellular_Automata()
{
}

void Cellular_Automata::Update(void)
{

}

void Cellular_Automata::GenerateMap()
{
    map = new int* [g_GRID_HEIGHT];
    MapRandomFill();

    for (int i = 0; i < smoothNum; i++) //반복이 많을수록 동굴의 경계면이 매끄러워진다.
        SmoothMap();
}

void Cellular_Automata::MapRandomFill()
{
    seed = time(NULL);

    for (int y = 0; y < g_GRID_HEIGHT; y++)
    {
        for (int x = 0; x < g_GRID_WIDTH; ++x)
        {
            if (x == 0 || x == g_GRID_WIDTH - 1 || y == 0 || y == g_GRID_HEIGHT - 1)
                map[y][x] = WALL;
            else
            {
                map[y][x] = 
            }
        }
    }
}

void Cellular_Automata::SmoothMap()
{
}


