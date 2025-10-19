#ifndef MAZEUTILS_H
#define MAZEUTILS_H

#include "ThreeCoord.h"
#include "Node.h"
#include "ThreeCoord.h"
#include <immintrin.h>
#include "LinkedNode.h"
#include <atomic>

ThreeCoord<int> corridorToFull(ThreeCoord<int> corridorCursor);

ThreeCoord<int> corridorToFull(int x, int y, int z);

int access(ThreeCoord<int> cursor, ThreeCoord<int> d);

int access(int x, int y, int z, ThreeCoord<int> d);

uint8_t availableLocations(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor);

bool hasAvailable(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor);

void printFlatMaze(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor);
int getDirection(int dir, int offset);
int translateDirection(uint8_t available, int chosenDirection);

ThreeCoord<int> step(std::atomic<uint8_t> *maze, ThreeCoord<int> cursor, ThreeCoord<int> d, std::mt19937 gen, std::uniform_int_distribution<> dis, LinkedNode &list);

void walk(std::atomic<uint8_t> *maze, ThreeCoord<int> d, LinkedNode &list, std::mt19937 gen, std::uniform_int_distribution<> dis, ThreeCoord<int> halls);

#endif