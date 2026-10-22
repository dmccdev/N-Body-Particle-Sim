#pragma once

#include "particlestate/ParticlesState.hpp"
#include "node.hpp"
#include <vector>
#include <raylib.h>
#include <cmath>

class QuadTree
{
public:

    QuadTree();

    int numberOfParticles;
    std::vector<Node> nodes;
    
    void buildTree(const ParticlesState &particles, int centerScreenX, int centerScreenY);
    void printData(const ParticlesState &particles) const;

    void calculateNodeCentresOfMass(int nodeIndex, const ParticlesState &particles);
    void resetTree();
    bool empty(int nodeIndex) const;
    bool external(int nodeIndex) const;
    void insert(int nodeIndex, int particleIndex, const ParticlesState &particles);
    void subdivide(int nodeIndex);
    int assignQuadrant(int nodeIndex , double particleX, double particleY) const;
    bool checkInsideNodeQuadrant(int nodeIndex, double particleX, double particleY) const;

private:

    static constexpr int rootIndex = 0;
};

