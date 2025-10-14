#ifndef THREECOORD_H
#define THREECOORD_H

template <typename T>
class ThreeCoord {
public:
  T x;
  T y;
  T z;

  ThreeCoord() {}

  ThreeCoord(T _x, T _y, T _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }

  operator bool() const
  {
    return (x > 0 && y > 0 && z > 0);
  }

  int vol()
  {
    return x * y * z;
  }

  bool operator==(const ThreeCoord<T> &other) const
  {
    return (x == other.x && y == other.y && z == other.z);
  }
};

#endif