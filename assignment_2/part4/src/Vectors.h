#include <iostream>

using namespace std;

#ifndef VECTORS_H_
#define VECTORS_H_

class Vectors
{
    public:
    float x_comp;
    float y_comp;

    Vectors()   {}

    Vectors(float x, float y)
    {
        x_comp = x;
        y_comp = y;
    }
    void set(float x_comp, float y_comp);

    void vectorAdd(const Vectors& a);
    void scalarAdd(float x);

    void vectorSub(const Vectors& a);
    Vectors vvSub(const Vectors& a, const Vectors& b);
    void scalarSub(float a);

    void vectorMult(const Vectors& a);
    void scalarMult(float a);

    void vectorDivi(const Vectors& a);
    void scalarDivi(float a);

    void limit(double m);

    float dist(const Vectors& a);
    float dot(const Vectors& a);
    float magnitude();
    void setMag(float a);
    float angleB(const Vectors& a);
    void normalize();

    Vectors copyTo(const Vectors& a);
};

#endif