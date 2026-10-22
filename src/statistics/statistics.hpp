#pragma once

#include "particlestate/ParticlesState.hpp"



class Statistics
{
public:
int frame_rate;
const double frameCalculationInterval;

const double dt;
const double G;
const double epsilon;
const double epsilonSquared;

double totalEnergy;
double energyError;

double linearMomentumX;
double linearMomentumY;
double linearMomentumErrorX;
double linearMomentumErrorY;


double angularMomentum;
double angularMomentumError;

double totalKineticEnergy;
double totalPotentialEnergy;

double initialTotalKineticEnergy;
double initialTotalPotentialEnergy;
double initialTotalEnergy;

double initialLinearMomentumX;
double initialLinearMomentumY;

double initialAngularMomentum;


Statistics(double dt, double G, double frameCalculationInterval, double epsilon);

void calculateStatistics(const ParticlesState &particles);
void updateStatistics(const ParticlesState &particles, int &frameCounter);
void getInitialParticleState(const ParticlesState &particles);

private:
Vec initialPositionX;
Vec initialPositionY;
Vec initialVelocityX;
Vec initialVelocityY;
Vec initialMass;
bool initialStateCaptured = false;
bool initialStatisticsCalculated = false;

void calculateInitialStatistics();
};
