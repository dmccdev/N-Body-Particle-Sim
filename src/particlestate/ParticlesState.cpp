#include "ParticlesState.hpp"
#include <random>
#include <raylib.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <rlgl.h>
#include <raymath.h>



ParticlesState::ParticlesState(int particlesCount) :
X(particlesCount), Y(particlesCount),
velocityX(particlesCount), velocityY(particlesCount),
accelerationX(particlesCount), accelerationY(particlesCount),
mass(particlesCount), speedSquared(particlesCount),
particlesCount(particlesCount)

{
    int textureSize = 32;
    Image glowImage = GenImageColor(textureSize, textureSize, BLANK);
    Color *pixels = static_cast<Color *>(glowImage.data);

    for (int y = 0; y < textureSize; ++y)
    {
        for (int x = 0; x < textureSize; ++x)
        {
            float dx = (x + 0.5f - textureSize * 0.5f) / (textureSize * 0.5f);
            float dy = (y + 0.5f - textureSize * 0.5f) / (textureSize * 0.5f);
            float radiusSquared = dx * dx + dy * dy;
            unsigned char alpha = static_cast<unsigned char>(255.0f * std::exp(-5.0f * radiusSquared));
            pixels[y * textureSize + x] = Color{255, 255, 255, alpha};
        }
    }

    particleTexture = LoadTextureFromImage(glowImage);
    SetTextureFilter(particleTexture, TEXTURE_FILTER_BILINEAR);
    UnloadImage(glowImage);
}

ParticlesState::~ParticlesState()
{
    if (particleTexture.id != 0) 
    {
        UnloadTexture(particleTexture);
    }
}

void ParticlesState::PrintData()// Debug

{
    std::cout << "Acceleration: " << '\n';  
    std::cout << accelerationX << '\n';  
    std::cout << accelerationY << '\n';  

    std::cout << "Velocity: " << '\n';  
    std::cout << velocityX << '\n';  
    std::cout << velocityY << '\n';  

    std::cout << "Position: " << '\n';  
    std::cout << X << '\n';  
    std::cout << Y << '\n';  

}

void ParticlesState::SingleStar()
{

    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(150.0, 300.0);
    std::uniform_real_distribution<double> distributionTheta(0.0, 2.0 * pi);
    std::uniform_real_distribution<double> distributionPerturbation(-5.0, 5.0);


    //Initialise Center Mass
    X[0] = 450.0;
    Y[0] = 450.0;
    mass[0] = 10000000.0;
    velocityX[0] = 0.0;
    velocityY[0] = 0.0;

    for(int i = 1; i < particlesCount; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);
        double randomSpeedPerturbation = distributionPerturbation(generator64);


        X[i] = X[0] + radius * std::cos(theta);
        Y[i] = Y[0] + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - X[0];
        double displacementY = Y[i] - Y[0];

        double distance = std::sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = std::sqrt(G * mass[0] / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed + randomSpeedPerturbation;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;
        mass[i] = 1.0;
    }
}

void ParticlesState::BinaryStar()
{

    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(150.0, 300.0);
    std::uniform_real_distribution<double> distributionTheta(0.0, 2.0 * pi);
    std::uniform_real_distribution<double> distributionPerturbation(-30.0, 30.0);


    //Initialise Central Masses
    X[0] = 480.0;
    Y[0] = 450.0;
    mass[0] = 10000000.0;


    X[1] = 420.0;
    Y[1] = 450.0;
    mass[1] = 10000000.0;

    double centerMassX = (X[0] + X[1])/2;
    double centerMassY = (Y[0] + Y[1])/2;
    double centerMassMass = mass[0] + mass[1];

    //Calculating their velocities
    double distance = X[0] - X[1];
    double radius = distance/2;
    double speed = std::sqrt(G * mass[0] /( 4 * radius));

    velocityX[0] = 0.0;
    velocityX[1] = 0.0;

    velocityY[0] = speed; //Tangential velocity to center of mass in opposite directions
    velocityY[1] = -speed;

    for(int i = 2; i < particlesCount; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);
        double randomSpeedPerturbation = distributionPerturbation(generator64);

        X[i] = centerMassX + radius * std::cos(theta);
        Y[i] = centerMassY + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - centerMassX;
        double displacementY = Y[i] - centerMassY;

        double distance = std::sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = std::sqrt(G * centerMassMass / distance);

        double tangentialVelocityX = -displacementY/distance * speed + randomSpeedPerturbation;
        double tangentialVelocityY = displacementX/distance * speed;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;

        //Mass
        mass[i] = 1.0;
    }
}

void ParticlesState::Galaxy()
{
    std::random_device rd;
    std::mt19937 generator64(rd());

    double blackHoleX = 450.0;
    double blackHoleY = 450.0;
    double blackHoleMass = 5000000.0;

    X[0] = blackHoleX;
    Y[0] = blackHoleY;
    mass[0] = blackHoleMass;

    velocityX[0] = 0.0;
    velocityY[0] = 0.0;

    //2D Random Polar Co ordiantes 
    std::uniform_real_distribution<double> distributionRadius(175.0, 300.0); 

    std::uniform_real_distribution<double> distributionTheta(0.0, 2.0 * pi);
    std::normal_distribution<double> diskThickness(0.0, 12.00);
    std::uniform_real_distribution<double> velocityVariation(0.95, 1.05);

    for (int i = 1; i < particlesCount; i++)
    {
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        // Circular position
        double offsetX = radius * std::cos(theta);
        double offsetY = radius * std::sin(theta);

        // Make the disk thinner vertically
        offsetY *= 0.25;

        X[i] = blackHoleX + offsetX;
        Y[i] = blackHoleY + offsetY + diskThickness(generator64);

        // Displacement from black hole
        double displacementX = X[i] - blackHoleX;
        double displacementY = Y[i] - blackHoleY;

        double distanceSquared = displacementX * displacementX + displacementY * displacementY;
        double distance = std::sqrt(distanceSquared);

        // Circular orbital velocity
        double orbitalSpeed = std::sqrt(G * blackHoleMass / distance);

        orbitalSpeed *= velocityVariation(generator64);

        // Tangential velocity
        velocityX[i] = -displacementY / distance * orbitalSpeed;
        velocityY[i] = displacementX / distance * orbitalSpeed;
        mass[i] = 1.0;
    }
}




void ParticlesState::Draw()
{
    if (particlesCount <= 0)
        return;

    speedSquared = velocityX.array().square() + velocityY.array().square();
    double maxSpeedSquared = speedSquared.maxCoeff();

    BeginBlendMode(BLEND_ADDITIVE);
    rlSetTexture(particleTexture.id);
    rlBegin(RL_QUADS);

    auto drawGlowQuad = [](float centerX, float centerY, float halfSize, Color tint)
    {
        rlColor4ub(tint.r, tint.g, tint.b, tint.a);
        rlTexCoord2f(0.0f, 0.0f);
        rlVertex2f(centerX - halfSize, centerY - halfSize);
        rlTexCoord2f(0.0f, 1.0f);
        rlVertex2f(centerX - halfSize, centerY + halfSize);
        rlTexCoord2f(1.0f, 1.0f);
        rlVertex2f(centerX + halfSize, centerY + halfSize);
        rlTexCoord2f(1.0f, 0.0f);
        rlVertex2f(centerX + halfSize, centerY - halfSize);
    };

    for (int i = 0; i < particlesCount; i++)
    {
        if (i > 0 && i % 1000 == 0)
        {
            rlEnd();
            rlSetTexture(particleTexture.id);
            rlBegin(RL_QUADS);
        }

        Color color = GetParticleColor(speedSquared[i], maxSpeedSquared);
        float px = static_cast<float>(X[i]);
        float py = static_cast<float>(Y[i]);

        Color halo = color;
        halo.a = 155;
        drawGlowQuad(px, py, 6.0f, halo);

        Color core = ColorLerp(color, WHITE, 0.68f);
        drawGlowQuad(px, py, 1.45f, core);
    }

    rlEnd();
    rlSetTexture(0);
    EndBlendMode();
}


Color ParticlesState::GetParticleColor(double speedSquared,double maxSpeedSquared)
{
    Color palette[] = {
        {45, 105, 255, 255},
        {0, 220, 255, 255},
        {75, 255, 190, 255},
        {210, 255, 90, 255},
        {255, 175, 55, 255},
        {255, 75, 170, 255},
        {255, 245, 225, 255}
    };

    if (maxSpeedSquared <= 0.0)
    {
        return palette[0];
    }

    double normalizedSpeedSquared = std::clamp(speedSquared / maxSpeedSquared, 0.0, 1.0);
    float palettePosition = static_cast<float>(std::sqrt(normalizedSpeedSquared) * 6.0);
    int paletteIndex = std::min(static_cast<int>(palettePosition), 5);
    float localT = palettePosition - static_cast<float>(paletteIndex);
    return ColorLerp(palette[paletteIndex], palette[paletteIndex + 1], localT);
}
