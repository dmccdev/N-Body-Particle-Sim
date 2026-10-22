#pragma once

#include "particlestate/ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"
#include "statistics/statistics.hpp"



class Pairwise

{
public:

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;
    Statistics Stats;

    ParticlesState particles;
    Pairwise(int particlesCount, int integratorNumber, int particleConfigNumber, double dt);
    void InitialiseParticles();
    void Update(bool statisticsEnabled);

    void CalculateAccelerations(const Vec &positionX, const Vec &positionY, Vec &accelerationX, Vec &accelerationY, const Vec &mass) const;
    std::array<double, 2> CalculateAcceleration(double displacementX, double displacementY) const;

    const int particlesCount;
    const int integratorNumber;
    const int particleConfigNumber;
    int frameCounter = 0;

    const double dt;
    static constexpr double G = 1.0;
    static constexpr double epsilon = 0.5;
    static constexpr double epsilonSquared = epsilon*epsilon;


};
