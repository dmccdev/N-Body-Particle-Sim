#pragma once

#include <vector>
#include <Eigen/Dense>
#include "particlestate/ParticlesState.hpp"


using Vec = Eigen::VectorXd;

class RK4
{
public:
    Vec virtualX;
    Vec virtualY;
    Vec Kv1X, Kv1Y, Kr1X, Kr1Y;
    Vec Kv2X, Kv2Y, Kr2X, Kr2Y;
    Vec Kv3X, Kv3Y, Kr3X, Kr3Y;
    Vec Kv4X, Kv4Y, Kr4X, Kr4Y;
    RK4(int numberOfParticles);
    void Update(ParticlesState &particles, double dt);
    void UpdateVirtualPosition(const ParticlesState &particles, const Vec &particleSlopeX, const Vec &particleSlopeY, double dt);
private:
    void CalculateSlopes(const ParticlesState &particles, double dt);
};
