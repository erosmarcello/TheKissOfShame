#pragma once

#include "../shameConfig.h"

// Bitmap toggle button with separate on/off frames clipped out of the
// faceplate filmstrips. Rev 2: images come from BinaryData.
class CustomButton : public ImageButton
{
public:
    CustomButton() = default;
    ~CustomButton() override = default;

    void resizeButton(float scale)
    {
        offImage = offImage.rescaled((int) (offImage.getWidth() * scale), (int) (offImage.getHeight() * scale));
        onImage = onImage.rescaled((int) (onImage.getWidth() * scale), (int) (onImage.getHeight() * scale));
        applyImages();
    }

    void setClippedCustomOnImage(const Image& source, int topLeftX, int topLeftY, int w, int h)
    {
        if (! source.isNull())
            onImage = source.getClippedImage({ topLeftX, topLeftY, w, h });
        applyImages();
    }

    void setClippedCustomOffImage(const Image& source, int topLeftX, int topLeftY, int w, int h)
    {
        if (! source.isNull())
            offImage = source.getClippedImage({ topLeftX, topLeftY, w, h });
        applyImages();
    }

private:
    void applyImages()
    {
        setImages(true, false, true,
                  onImage, 1.0f, Colour(0x0),
                  Image(), 1.0f, Colour(0x0),
                  offImage, 1.0f, Colour(0x0));

        if (! onImage.isNull())
            setSize(onImage.getWidth(), onImage.getHeight());
    }

    Image offImage;
    Image onImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomButton)
};
