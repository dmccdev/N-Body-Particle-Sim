#pragma once
#include <raylib.h>
#include <string>

class Button
{
public:
Button(float x, float y, float width, float height, std::string text, Color buttonColour,bool algorithmButton, bool integratorButton, bool startButton, bool particleConfigButton);
const Rectangle ButtonBounds;
const std::string text;
Color buttonColour;
bool buttonPressed = false;
const bool particleConfig;
const bool algorithm;
const bool integrator;
const bool startButton;
void Draw() const;

};
