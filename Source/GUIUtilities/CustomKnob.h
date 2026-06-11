#pragma once

#include "../shameConfig.h"

// Filmstrip rotary knob. Rev 2: images come from BinaryData (no file
// paths), and a double-click hook is exposed — the Shame knob uses it to
// enter Extreme mode.
class CustomKnob : public Slider
{
public:
    CustomKnob();
    ~CustomKnob() override = default;

    void setNumFrames(int numFrames);
    void setKnobImage(const Image& image);
    void setKnobDimensions(int topLeftX, int topLeftY, int w, int h);

    void paint(Graphics& g) override;
    void mouseDoubleClick(const MouseEvent& event) override;

    std::function<void()> onDoubleClick;

private:
    int knobNumFrames = 128;

    Image knobImage;
    int knobFrameWidth = 0;
    int knobFrameHeight = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomKnob)
};
