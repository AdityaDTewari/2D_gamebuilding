#include <math.h>
#include "Vectors.h"

void Vectors::set(float a, float b)
{
    x_comp = a;
    y_comp = b;
}

void Vectors::vectorAdd(const Vectors& a)
{
    x_comp += a.x_comp;
    y_comp += a.y_comp;
}

void Vectors::scalarAdd(float a)
{
    x_comp += a;
    y_comp += a;
}

void Vectors::vectorSub(const Vectors& a)
{
    x_comp -= a.x_comp;
    y_comp -= a.y_comp;
}

Vectors Vectors::vvSub(const Vectors& a, const Vectors& b)
{
    Vectors tmp(a.x_comp - b.x_comp, a.y_comp - b.y_comp);
    return std::move(tmp);
}

void Vectors::scalarSub(float a)
{
    x_comp -= a;
    y_comp -= a;
}

void Vectors::vectorMult(const Vectors& a)
{
    x_comp *= a.x_comp;
    y_comp *= a.y_comp;
}

void Vectors::scalarMult(float a)
{
    x_comp *= a;
    y_comp *= a;
}

void Vectors::vectorDivi(const Vectors& a)
{
    x_comp /= a.x_comp;
    y_comp /= a.y_comp;
}

void Vectors::scalarDivi(float a)
{
    x_comp /= a;
    y_comp /= a;
}

void Vectors::limit(double max)
{
    double size = magnitude();

    if (size > max) {
        set(x_comp / size, y_comp / size);
    }
}

float Vectors::dist(const Vectors& a)
{
    float dx = x_comp - a.x_comp;
    float dy = y_comp - a.y_comp;
    float dist = sqrt(dx*dx + dy*dy);
    return dist;
}

float Vectors::dot(const Vectors& a)
{
    float dot = x_comp * a.x_comp + y_comp * a.y_comp;
    return dot;
}

float Vectors::magnitude()
{
    return sqrt(x_comp*x_comp + y_comp*y_comp);
}

void Vectors::setMag(float a)
{
    normalize();
    scalarMult(a);
}

float Vectors::angleB(const Vectors& a)
{
    if (x_comp == 0 && y_comp == 0) return 0.0f;
    if (a.x_comp == 0 && a.y_comp == 0) return 0.0f;

    double dot = x_comp * a.x_comp + y_comp * a.y_comp;
    double v1 = sqrt(x_comp * x_comp + y_comp * y_comp);
    double v2 = sqrt(a.x_comp * a.x_comp + a.y_comp * a.y_comp);
    double amt = dot / (v1 * v2);
    if (amt <= -1) {
        return M_PI;
    } else if (amt >= 1) {
        return 0;
    }
    float tmp = acos(amt);
    return tmp;
}

void Vectors::normalize()
{
    float m = magnitude();

    if (m > 0) {
        set(x_comp / m, y_comp / m);
    } else {
        set(x_comp, y_comp);
    }
}

Vectors Vectors::copyTo(const Vectors& a)
{
    Vectors copy(a.x_comp, a.y_comp);
    return copy;
}