#pragma once

#include "../shameConfig.h"

// Value-driven filmstrip display (VU meters, shame ring, face background).
// Rev 2: images come from BinaryData.
class ImageInteractor : public Component
{
public:
    ImageInteractor();
    ~ImageInteractor() override = default;

    void paint(Graphics& g) override;

    void updateImageWithValue(float value)
    {
        setCurrentValue(value);
        repaint();
    }

    void setCurrentValue(float value)
    {
        curValue = jlimit(minValue, maxValue, value);
    }

    void setMinMaxValues(float min, float max)
    {
        minValue = min;
        maxValue = max;
    }

    void setDesaturate(bool shouldDesaturate)
    {
        desaturate = shouldDesaturate;
        repaint();
    }

    void setNumFrames(int numFrames);
    void setAnimationImage(const Image& newImage);
    void setDimensions(int topLeftX, int topLeftY, int w, int h);

private:
    bool desaturate = false;
    Image satImage;
    Image desatImage;
    int frameWidth = 0;
    int frameHeight = 0;
    int numFrames = 128;

    float maxValue = 1.0f, minValue = 0.0f, curValue = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageInteractor)
};
