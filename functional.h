#ifndef IMAGES_FUNCTIONAL_H
#define IMAGES_FUNCTIONAL_H

#include <bits/stdc++.h>

template<typename F = nullptr_t> auto compose()
{
    return [=](auto x) { return x; };
}

template<typename F, typename ... Args>
auto compose(F f, Args ... args)
{
    return [=](auto x) { return compose(args ...)(f(x)); };
}

template<typename H, typename ... Fs>
auto lift(H h, Fs ... fs)
{
    return [=](auto a) { return h(fs(a) ...); };
}

#endif //IMAGES_FUNCTIONAL_H
