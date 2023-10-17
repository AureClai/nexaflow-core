#ifndef FD_H
#define FD_H

class FD
{
public:
    // Constructor
    FD(float u, float C, float kx, float w) : u(u), C(C), kx(kx), w(w)
    {
    }

    // Member variables
    float u;  // Maximum speed
    float C;  // Capacity
    float kx; // Maximum density
    float w;  // Wave speed
};

#endif // FD_H
