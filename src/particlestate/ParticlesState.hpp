#pragma once
#include <Eigen/Dense>
#include <raylib.h>

using Vec = Eigen::VectorXd;

class ParticlesState
{
public:
    Vec X;
    Vec Y;
    Vec velocityX;
    Vec velocityY;
    Vec accelerationX;
    Vec accelerationY;
    Vec mass;
    Vec speedSquared;

    const int particlesCount;
    static constexpr double G = 1.0;
    static constexpr double pi = 3.14159265358979323846;
    ParticlesState(int particlesCount);
    ~ParticlesState();
    void PrintData() const;
    void BinaryStar();
    void SingleStar();
    void Galaxy();
    void Draw();
private:
    Color GetParticleColor(double speedSquared, double maxSpeedSquared) const;
    Texture2D particleTexture{};
};
