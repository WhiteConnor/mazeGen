#include "mazeUtils.h"
#include "Node.h"
#include "ThreeCoord.h"
#include <immintrin.h>
#include "LinkedNode.h"
#include <atomic>
#include <stdio.h>


ThreeCoord<int> corridorToFull(ThreeCoord<int> corridorCursor) {
  return ThreeCoord<int>(2 * corridorCursor.x + 1, 2 * corridorCursor.y + 1, 2 * corridorCursor.z + 1);
}

ThreeCoord<int> corridorToFull(int x, int y, int z) {
  return ThreeCoord<int>(2 * x + 1, 2 * y + 1, 2 * z + 1);
}

int access(ThreeCoord<int> cursor, ThreeCoord<int> d){
  return ((cursor.z) * d.y * d.x) + ((cursor.y) * d.x) + (cursor.x);
}

int access(int x, int y, int z, ThreeCoord<int> d) {
  return ((z)*d.y * d.x) + ((y)*d.x) + (x);
}

uint8_t availableLocations(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor)
{
  /*
  na na Z- Z+ Y- Y+ X- X+
  origin *
                        Y-
           . *------------------------+
         .'  |                      .'|
       .'    |                   .-'  |
      +------+-----------------+'     |
      |      |                 |      |
      |      |                 |      |
      |      |           Z-    |      | X+
      |      |                 |      |
    X-|      |   Z+            |      |
      |      |                 |      |
      |      |                 |      |
      |      +-----------------+------+
      |    .'                  |    .'
      |  .'                    |  .'
      +------------------------+'
                  Y+
                    -Y -Z
                     |/
                -X---+---X+
                    /|
                  +Z +Y
*/

  uint8_t flags{0b00000000};
  // DIR_RIGHT = 1 << 0 X+
  // DIR_LEFT = 1 << 1  X-
  // DIR_UP = 1 << 2    Y+
  // DIR_DOWN = 1 << 3  Y-
  // DIR_FWD = 1 << 4   Z+
  // DIR_BACK = 1 << 5  Z-
  // flags |= (isAvailable << i);
  if (!cursor)
  {
    return flags;
  }
  if (cursor.x < d.x - 2)
  {
    int index = access(cursor.x + 2, cursor.y, cursor.z, d);
    flags |= (!(*(maze + index)) << 0); // X+
  }
  if (cursor.x >= 2)
  {
    int index = access(cursor.x - 2, cursor.y, cursor.z, d);
    flags |= (!(*(maze + index)) << 1); // X-
  }

  if (cursor.y < d.y - 2)
  {
    int index = access(cursor.x, cursor.y + 2, cursor.z, d);
    flags |= (!(*(maze + index)) << 2); // Y+
  }
  if (cursor.y >= 2)
  {
    int index = access(cursor.x, cursor.y - 2, cursor.z, d);
    flags |= (!(*(maze + index)) << 3); // Y-
  }

  if (cursor.z < d.z - 2)
  {
    int index = access(cursor.x, cursor.y, cursor.z + 2, d);
    flags |= (!(*(maze + index)) << 4); // Z+
  }
  if (cursor.z >= 2)
  {
    int index = access(cursor.x, cursor.y, cursor.z - 2, d);
    flags |= (!(*(maze + index)) << 5); // Z-
  }
  return flags;
}

bool hasAvailable(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor)
{
  if (!cursor)
  {
    return false;
  }
  if (cursor.x < d.x - 2)
  {
    int index = access(cursor.x + 2, cursor.y, cursor.z, d);
    if (!(*(maze + index)))
    {
      return true;
    } // X+
  }

  if (cursor.x >= 2)
  {
    int index = access(cursor.x - 2, cursor.y, cursor.z, d);
    if (!(*(maze + index)))
    {
      return true;
    } // X-
  }

  if (cursor.y < d.y - 2)
  {
    int index = access(cursor.x, cursor.y + 2, cursor.z, d);
    if (!(*(maze + index)))
    {
      return true;
    } // Y+
  }

  if (cursor.y >= 2)
  {
    int index = access(cursor.x, cursor.y - 2, cursor.z, d);
    if (!(*(maze + index)))
    {
      return true;
    } // Y-
  }

  if (cursor.z < d.z - 2)
  {
    int index = access(cursor.x, cursor.y, cursor.z + 2, d);
    if (!(*(maze + index)))
    {
      return true;
    } // Z+
  }

  if (cursor.z >= 2)
  {
    int index = access(cursor.x, cursor.y, cursor.z - 2, d);
    if (!(*(maze + index)))
    {
      return true;
    } // Z-
  }

  return false;
}

void printFlatMaze(std::atomic<uint8_t> *maze, ThreeCoord<int> d, ThreeCoord<int> cursor)
{
  for (int y = 0; y < d.y; y++)
  {
    for (int z = 0; z < d.z; z++)
    {
      for (int x = 0; x < d.x; x++)
      {
        int index = x + y * d.x + z * d.x * d.y;
        if (cursor.x == x && cursor.y == y && cursor.z == z)
        {
          printf("X ");
        }
        else
        {
          printf("%d ", (*(maze + index)).load());
        }
      }
      printf("  "); // space between z-slices
    }
    printf("\n"); // line per Y-row
  }
}

int getDirection(int dir, int offset)
{
  return (2 * (dir - offset)) - 1;
}

int translateDirection(uint8_t available, int chosenDirection)
{
  int i = 0;
  int j = 0;
  while (i < 6)
  {
    if ((available & (1 << i)) > 0)
    {
      j++;
      if (j >= chosenDirection)
      {
        return i;
      }
    }
    i++;
  }
  return i;
}

ThreeCoord<int> step(std::atomic<uint8_t> *maze, ThreeCoord<int> cursor, ThreeCoord<int> d, std::mt19937 gen, std::uniform_int_distribution<> dis, LinkedNode &list)
{
  // printf("Cursor before: (%d, %d, %d)\n", cursor.x, cursor.y, cursor.z);
  uint8_t available = availableLocations(maze, d, cursor); // test available locations at cursor
  if (!available)
  {
    list.removeAt(cursor);           // remove this cursor from the list
    return ThreeCoord<int>(0, 0, 0); // no available moves
  }
  uint8_t popCount = _mm_popcnt_u32((uint32_t)available); // total options available
  // printf("Total options: %d\n", popCount);
  uint8_t chosenDirection = dis(gen) % popCount; // random number 0 - popCount-1

  int i = translateDirection(available, chosenDirection);
  int delta = 0;
  // printf("i: %d\nChosen Direction: %d\n", i, chosenDirection);
  if (i < 2)
  {
    *(maze + access(cursor.x - getDirection(i, 0), cursor.y, cursor.z, d)) = 1; // open the wall
    delta = getDirection(i, 0) * -2;
    // printf("moving cursor along x axis by %d\n", delta);
    cursor.x += delta; // move the cursor
  }
  else if (i < 4)
  {
    *(maze + access(cursor.x, cursor.y - getDirection(i, 2), cursor.z, d)) = 1; // open the wall
    delta = getDirection(i, 2) * -2;
    // printf("moving cursor along y axis by %d\n", delta);
    cursor.y += delta; // move the cursor
  }
  else if (i < 6)
  {
    *(maze + access(cursor.x, cursor.y, cursor.z - getDirection(i, 4), d)) = 1; // open the wall
    delta = getDirection(i, 4) * -2;
    // printf("moving cursor along z axis by %d\n", delta);
    cursor.z += delta; // move the cursor
  }
  else
  {
    printf("\n\n\nFAIL\n\n\n");
  }
  uint8_t expected = 0;
  uint8_t desired = 1;
  // if able to move, then move, otherwise return failed step
  if ((*(maze + access(cursor, d))).compare_exchange_strong(expected, desired, std::memory_order_acq_rel))
  {
    return cursor;
  }
  else
  {
    return ThreeCoord<int>(0, 0, 0);
  }
}

void walk(std::atomic<uint8_t> *maze, ThreeCoord<int> d, LinkedNode &list, std::mt19937 gen, std::uniform_int_distribution<> dis, ThreeCoord<int> halls) {
  ThreeCoord<int> cursor = ThreeCoord<int>(0, 0, 0);
  for (int i = 0; i < halls.vol() - 1; i++)
  {
    if ((i % 10) == 9)
    {
      cursor = step(maze, cursor, d, gen, dis, list);
    }
    else
    {
      cursor = ThreeCoord<int>(0, 0, 0);
    }
    while (!cursor)
    {
      if (list.getNodeCount() == 0)
      {
        i = halls.vol();
        break;
      }
      cursor = list.getRandom(maze, d, gen);
      cursor = step(maze, cursor, d, gen, dis, list);
    }
    if (cursor)
    {
      list.append(cursor);
    }
  }
}