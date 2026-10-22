#pragma once

class Node
{
public:

    int particleIndex = -1;
    int childFirstIndex = -1;
    
    double centreMassX = 0;
    double centreMassY = 0;

    double mass = 0; 

    double halfWidth;

    double squareCenterX;
    double squareCenterY;

    void PrintData() const;
};
