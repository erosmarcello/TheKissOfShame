#include "CustomKnob.h"

CustomKnob::CustomKnob()
{
    setSliderStyle(Rotary);
    setTextBoxStyle(NoTextBox, true, 0, 0);
    setRange(0.000, 1.000, 0.001);
    setValue(0.0);
}

void CustomKnob::setNumFrames(int numFrames)
{
    knobNumFrames = jmax(1, numFrames);
}

void CustomKnob::setKnobImage(const Image& image)
{
    knobImage = image;
    repaint();
}

void CustomKnob::setKnobDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    knobFrameWidth = w;
    knobFrameHeight = h;
    setSize(knobFrameWidth, knobFrameHeight);
}

void CustomKnob::mouseEnter(const MouseEvent& event)
{
    hovering = true;
    repaint();
    Slider::mouseEnter(event);
}

void CustomKnob::mouseExit(const MouseEvent& event)
{
    hovering = false;
    repaint();
    Slider::mouseExit(event);
}

void CustomKnob::paint(Graphics& g)
{
    const double normalizedValue = valueToProportionOfLength(getValue());

    if (era == UIEra::modern)
    {
        // The Shame knob carries the Infernal Love logo — the inverted cross
        // with the heart. In the modern era we keep the ORIGINAL 2014
        // artwork, frame-accurate and still rotating, ringed by the vector
        // value arc. The brand is not negotiable.
        if (modernCross && ! knobImage.isNull())
        {
            const auto bounds = getLocalBounds().toFloat();
            const auto centre = bounds.getCentre();
            const float radius = bounds.getWidth() * 0.5f - bounds.getWidth() * 0.06f;
            const Colour glowColour = extremeVisual ? ModernTheme::accentHot : ModernTheme::accent;

            ModernTheme::dropShadowEllipse(g, bounds.reduced(bounds.getWidth() * 0.10f), 0.6f);

            // ambient halo behind the artwork — stronger on hover, hot in Extreme
            ColourGradient halo(glowColour.withAlpha(extremeVisual ? 0.45f : (hovering ? 0.26f : 0.18f)),
                                centre.x, centre.y,
                                glowColour.withAlpha(0.0f), centre.x, bounds.getY() - 8.0f, true);
            g.setGradientFill(halo);
            g.fillEllipse(bounds.expanded(10.0f));

            const int frameNum = (int) (normalizedValue * (knobNumFrames - 1));
            juce::Rectangle<int> clipRect(0, frameNum * knobFrameHeight, knobFrameWidth, knobFrameHeight);
            g.drawImage(knobImage.getClippedImage(clipRect), bounds, RectanglePlacement::centred);

            Path track;
            track.addCentredArc(centre.x, centre.y, radius, radius, 0.0f,
                                ModernTheme::rotaryStart, ModernTheme::rotaryEnd, true);
            g.setColour(Colours::black.withAlpha(0.55f));
            g.strokePath(track, PathStrokeType(3.6f, PathStrokeType::curved, PathStrokeType::rounded));
            g.setColour(Colours::white.withAlpha(0.08f));
            g.strokePath(track, PathStrokeType(1.2f, PathStrokeType::curved, PathStrokeType::rounded));

            if (normalizedValue > 0.001)
            {
                const float angle = ModernTheme::rotaryStart
                                  + (float) normalizedValue * (ModernTheme::rotaryEnd - ModernTheme::rotaryStart);
                ModernTheme::drawBloomArc(g, centre, radius, ModernTheme::rotaryStart, angle, glowColour, 3.0f);
            }

            return;
        }

        ModernTheme::drawKnob(g, getLocalBounds().toFloat(), (float) normalizedValue,
                              false, extremeVisual, hovering);
        return;
    }

    if (! knobImage.isNull())
    {
        const int frameNum = (int) (normalizedValue * (knobNumFrames - 1));
        juce::Rectangle<int> clipRect(0, frameNum * knobFrameHeight, knobFrameWidth, knobFrameHeight);
        g.drawImageAt(knobImage.getClippedImage(clipRect), 0, 0);
    }
}

void CustomKnob::mouseDoubleClick(const MouseEvent& event)
{
    if (onDoubleClick != nullptr)
        onDoubleClick();
    else
        Slider::mouseDoubleClick(event);
}
