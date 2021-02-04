#ifndef TRAINDYNAMIC_H
#define TRAINDYNAMIC_H

#endif // TRAINDYNAMIC_H

#include<iostream>

using namespace std;

class Train
{
public:
    Train();

    void init();
    void PowerDistribution(float F, float v);
    void BasicRunningForce();
    
public:
    float Controlspeed = 0.01;

   
private:
    
};
