#ifndef IMAGES_IMAGES_H
#define IMAGES_IMAGES_H

#include <functional>
#include <cassert>
#include "functional.h"
#include "coordinate.h"
#include "color.h"

namespace Detail
{
    inline Point my_to_polar(const Point p)
    {
        return p.is_polar ? p : to_polar(p);
    }

    inline Point my_from_polar(const Point p)
    {
        return !p.is_polar ? p : from_polar(p);
    }
}

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t)
{
    return [=](const Point p){ return t; };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi)
{
    return compose(Detail::my_to_polar, [=](const Point p) { return Point(p.first, p.second - phi, true); }, image);
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v)
{
    return compose(Detail::my_from_polar, [=](const Point p) { return Point(p.first + v.first, p.second + v.second); }, image);
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s)
{
    return compose(Detail::my_from_polar, [=](const Point p) { return Point(p.first * s, p.second * s); }, image);
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer)
{
    return [=](const Point p) { return (distance(q)) <= r ? inner : outer; };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way)
{

}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way)
{

}


template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way)
{

}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way)
{

}

inline Image cond(Region region, Image this_way, Image that_way)
{

}

inline Image lerp(Blend blend, Image this_way, Image that_way)
{

}

inline Image darken(Image image, Blend blend)
{

}

inline Image lighten(Image image, Blend blend)
{

}

#endif //IMAGES_IMAGES_H
