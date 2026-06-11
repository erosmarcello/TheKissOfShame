#pragma once

#include "../shameConfig.h"

// The glow behind the reels. Stock: the signature pink. Extreme mode runs
// hotter — the backlight shifts toward red so you always know the machine
// is past its stops.
class BacklightComponent : public Component
{
public:
    BacklightComponent()
    {
        setSize(960, 266);
        setInterceptsMouseClicks(false, false);
    }

    ~BacklightComponent() override = default;

    void setExtreme(bool shouldBeExtreme)
    {
        extreme = shouldBeExtreme;
        repaint();
    }

    void paint(Graphics& g) override
    {
        if (extreme)
            g.fillAll(Colour::fromFloatRGBA(1.0f, 0.10f, 0.16f, 1.0f));
        else
            g.fillAll(Colour::fromFloatRGBA(1.0f, 0.216f, 0.384f, 1.0f));
    }

private:
    bool extreme = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BacklightComponent)
};
