#ifndef IMAGES_IMAGES_H
#define IMAGES_IMAGES_H

#include <functional>
#include <cassert>
#include <utility>
#include "functional.h"
#include "coordinate.h"
#include "color.h"

namespace Detail {
    inline Point my_to_polar(const Point p) {
        return p.is_polar ? p : to_polar(p);
    }

    inline Point my_from_polar(const Point p) {
        return !p.is_polar ? p : from_polar(p);
    }

    inline double my_distance(const Point p, const Point q = {0, 0}) {
        return distance(my_from_polar(p), my_from_polar(q));
    }
}

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t) {
    return [=](const Point p) {
        (void) (p);
        return t;
    };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
    return compose(Detail::my_to_polar, [phi](const Point p) { return Point(p.first, p.second - phi, true); },
                   Detail::my_from_polar, image);
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
    return compose(Detail::my_from_polar, [v](const Point p) { return Point(p.first - v.first, p.second - v.second); },
                   image);
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
    return compose(Detail::my_from_polar, [s](const Point p) { return Point(p.first / s, p.second / s); }, image);
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
    return [=](const Point p) { return Detail::my_distance(p, q) <= r ? inner : outer; };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
    return compose([=](const Point p) {
        return static_cast<int>(std::floor((p.first / d)) + std::floor((p.second / d))) % 2 ? that_way : this_way;
    });
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    return compose(Detail::my_to_polar, [=](const Point p) {
        return Point(p.first, p.second * static_cast<double>(n) * d / (2 * M_PI), true);
    }, checker(d, this_way, that_way));
}


template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    return [=](const Point p) { return static_cast<int>(Detail::my_distance(p, q) / d) % 2 ? that_way : this_way; };
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [=](const Point p) { return std::abs(p.first) <= d / 2 ? this_way : that_way; };
}

inline Image cond(Region region, Image this_way, Image that_way) {
    return lift([=](bool b, Color this_color, Color that_color) { return b ? this_color : that_color; },
                std::move(region), std::move(this_way), std::move(that_way));
}

inline Image lerp(Blend blend, Image this_way, Image that_way) {
    return lift([=](Fraction f, Color this_color, Color that_color) { return this_color.weighted_mean(that_color, f); },
                std::move(blend), std::move(this_way), std::move(that_way));
}

inline Image darken(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::black));
}

inline Image lighten(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::white));
}

#endif //IMAGES_IMAGES_H
