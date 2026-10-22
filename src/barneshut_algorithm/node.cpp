#include "node.hpp"
#include <iostream>

void Node::PrintData() const
{
    std::cout << "Particle Index: " << particleIndex << '\n';
    std::cout << "First ChildNode Index: " << childFirstIndex << '\n';
    std::cout << "CenterMass Coordinate: (" << centreMassX<< ", " << centreMassY << ")\n";
    std::cout << "Node Mass:  " << mass << '\n';
    std::cout << "HalfWidth:  " << halfWidth << '\n';
    std::cout << "Square Center Coordinate: (" << squareCenterX << ", " << squareCenterY << ")\n";
}
