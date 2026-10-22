#include "button.hpp"

Button::Button(float x, float y, float width, float height, std::string text, Color buttonColour, bool algorithmButton, bool integrator, bool startButton, bool particleConfigButton)
    : ButtonBounds{x, y, width, height},
      text(text),
      particleConfig(particleConfigButton),
      algorithm(algorithmButton),
      integrator(integrator),
      startButton(startButton)
{
    this->buttonColour = buttonColour;
}

void Button::Draw() const
{
    DrawRectangleRec(ButtonBounds, buttonColour);
    DrawRectangleLinesEx(ButtonBounds, 3.0f, DARKGRAY);

    const Vector2 textDimensions = MeasureTextEx(GetFontDefault(), text.c_str(), 30, 1.0f);

    const float buttonTextX = ButtonBounds.x + (ButtonBounds.width - textDimensions.x) / 2;
    const float buttonTextY = ButtonBounds.y + (ButtonBounds.height - textDimensions.y) / 2;

    DrawTextEx(GetFontDefault() ,text.c_str(), Vector2{buttonTextX, buttonTextY}, 30.0f, 1.0f, WHITE);
}
