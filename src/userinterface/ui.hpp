#pragma once
#include <raylib.h>
#include "button.hpp"
#include "statistics/statistics.hpp"

class UserInterface
{
public:
    const Font font;
    Button BarnesHutButton;
    Button PairwiseButton;

    Button EulerButton;
    Button VerletButton;
    Button RK4Button;
    Button StartButton;
    Button GalaxyButton;
    Button SingleStarButton;
    Button BinaryStarButton;
    Button StatisticsButton;

    bool simulationStart;

    UserInterface(Font font, double dt, double G);

    void Draw() const;
    void UpdateButtons(Vector2 mousePosition);
    void ButtonPressed(Vector2 mousePosition, Button &button);

    void ToggleStatistics();

    void SetActiveParticleConfiguration(Button &button);
    void SetActiveIntegrator(Button &button);
    void SetActiveAlgorithm(Button &button);
    void SimulationBegin();

    void DrawStatistics(const Statistics &Stats) const;

private:
};
