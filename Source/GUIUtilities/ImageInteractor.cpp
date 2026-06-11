#include "ImageInteractor.h"

ImageInteractor::ImageInteractor() = default;

void ImageInteractor::setNumFrames(int newNumFrames)
{
    curValue = 0;
    numFrames = jmax(1, newNumFrames);
}

void ImageInteractor::setAnimationImage(const Image& newImage)
{
    satImage = newImage;
    desatImage = newImage.createCopy();
    desatImage.desaturate();
    repaint();
}

void ImageInteractor::setDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    frameWidth = w;
    frameHeight = h;
    setSize(frameWidth, frameHeight);
}

void ImageInteractor::paint(Graphics& g)
{
    const Image& image = desaturate ? desatImage : satImage;

    if (! image.isNull())
    {
        const double normalizedValue = (curValue - minValue) / (maxValue - minValue);
        const int frameNum = (int) (normalizedValue * (numFrames - 1));
        juce::Rectangle<int> clipRect(0, frameNum * frameHeight, frameWidth, frameHeight);
        g.drawImageAt(image.getClippedImage(clipRect), 0, 0);
    }
}
