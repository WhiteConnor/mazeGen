# 3D Maze Generator

The goal of this maze generator is to generate as large of a maze as quickly as possible. The standard test case is a 50x50x50 maze, this translates to 125,000 steps, and approximatly 1Mb of data.


## Current Progress

- Implemented a linked list to more quickly find an available location to walk from. Times for a test case of 50x50x50 are ~7.99 seconds, approximately 3.75x faster than the original

- Maze generator functional. Times for a test case are ~30 seconds.


## Future Upgrades

- Implement multi-threading to allow multiple cursors to run at the same time
  - Mutex followed by lock-free linked list
  - Clean up thread(s) only concerned with removing nodes from the list that are dead, or have no available moves
  - May remove ability to generate consistent mazes with seeds due to threads running at different speeds
    - Consider removing custom seeds
- Bit packing to cut down on space
  - Currently uses one byte per corridor/hallway
  - Would allow 2 corridors and 6 hallways per byte
  - Should improve efficiency with significantly greater cache locality
  - Larger mazes possible due to lowered memory constraints
- Wrapping maze option
  - Would allow corridors to wrap from left to right, top to bottom, front to back
  - Borders would be handled differently, no 'walls' around the edges would be necessary
    - Walls account for 
  - An imperfect maze that allowed wrapping could have interesting non-euclidian effects


## Walls vs workable Area
$$x = 2a + 1$$
$$y = 2b + 1$$
$$z = 2c + 1$$

$$workableArea = (x - 2) * (y - 2) * (z - 2)$$
If we assume that x, y, and z are equal we can simplify this to:
$$= (x - 2)^3$$
$$= (x^2-2x+4) * (x-2) = x^3 - 2x^2 + 4x -2x^2 + 4x - 8 = x^3 - 4x^2 + 8x - 8$$
$$walls = x^3 - workableArea$$
$$ratio=\frac{x^3-(x^3-4x^2+8x-8)}{x^3}$$
$$=\frac{4x^2-8x+8}{x^3}$$

Divide every term in the numerator by x^3:
$$\frac{4x^2-8x+8}{x^3} = \frac{4x^2}{x^3} - \frac{8x}{x^3} + \frac{8}{x^3}$$

Simplify each term:
$$= 1 - \frac{4}{x} - \frac{8}{x^2} + \frac{8}{x^3}$$

As x grows, each term with 1/x^n vanishes:
$$\lim_{x \to \infty} \left(\frac{4}{x} - \frac{8}{x^2} + \frac{8}{x^3}\right) = 0$$

The larger it gets, the smaller the ratio between the walls and the workable area

For the standard case of 50x50x50

$$x=101$$
$$r=\frac{101^2-2(101)+8}{101^3}$$
$$=\frac{10,201-202+8}{1,030,301}$$
$$=\frac{10,007}{1,030,301}$$
$$=0.0097$$
$$=0.97\%$$

At 50x50x50 this is a difference of ~0.01Mb, nearly insignificant, but it will save some space.
$$x=1000$$
$$mazeSpace=2001^2-2(2001)+8$$
$$=4,004,001 - 4002 + 8$$
$$=4,000,007$$

At 1000x1000x1000, this saves 4Mb of data, while more significant, it is also paired with a maze that takes up 8Gb. For now, I'm not worried about the space that walls take up and while removing walls completely may be relatively trivial, just modifying the size of the array and how available moves are calculated, it doesn't seem worth it.
### Third-Party Code Notice

This project uses [MurmurHash3](https://github.com/aappleby/smhasher) by Austin Appleby for deterministic 32-bit hashing.
- Source files used: [`MurmurHash3.h`](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.h), [`MurmurHash3.cpp`](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp)
- License: **Public Domain**

MurmurHash3 is a non-cryptographic hash function designed for high performance and portability.
