//
//  ImageAnimationComponent.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/4/14.
//  Rev 2: plain Timer-driven Component (the AnimatedAppComponent +
//  OpenGLContext stack is gone, along with the 1-pixel seam workaround it
//  required). Flange gestures are reported through std::function callbacks
//  instead of ActionBroadcaster strings.
//
//  These are the interactive reels: they spin with the transport, and
//  click-dragging them leans on the tape for a real-time flange.
//

#pragma once

#include "Theme.h"

class ImageAnimationComponent : public Component, private Timer
{
public:
    ImageAnimationComponent(const Image& image, int numFrames, int framesPerSecond)
        : animationNumFrames(jmax(1, numFrames))
    {
        animationImage = image;
        imageFrameWidth = animationImage.getWidth();
        imageFrameHeight = animationImage.getHeight() / animationNumFrames;
        setSize(imageFrameWidth, imageFrameHeight);

        endFrame = animationNumFrames - 1;

        setFramesPerSecond(framesPerSecond);
    }

    ~ImageAnimationComponent() override = default;

    //==========================================================================
    void setFramesPerSecond(int fps)
    {
        if (fps > 0)
            startTimerHz(fps);
        else
            stopTimer();
    }

    void mouseDown(const MouseEvent&) override
    {
        // Touching the reels drags them: animation slows against the thumb.
        setAnimationResetThreshold(0.015f);
        if (onFlangeGestureStart != nullptr)
            onFlangeGestureStart();
    }

    void mouseUp(const MouseEvent&) override
    {
        setFlangeDepth = curFlangeDepth;
        setAnimationResetThreshold(0.0f);
        if (onFlangeGestureEnd != nullptr)
            onFlangeGestureEnd();
    }

    void mouseDrag(const MouseEvent& event) override
    {
        const float dragDist = (float) event.getDistanceFromDragStartY() / 100.0f;
        curFlangeDepth = jlimit(0.0f, 1.0f, setFlangeDepth + dragDist);

        if (onFlangeDepthChanged != nullptr)
            onFlangeDepthChanged(curFlangeDepth);
    }

    float getCurrentFlangeDepth() const { return curFlangeDepth; }

    std::function<void()> onFlangeGestureStart;
    std::function<void(float)> onFlangeDepthChanged;
    std::function<void()> onFlangeGestureEnd;

    void setEra(UIEra newEra)
    {
        era = newEra;
        repaint();
    }

    //==========================================================================
    void paint(Graphics& g) override
    {
        if (era == UIEra::modern)
        {
            paintModernReels(g);
            return;
        }

        g.fillAll(Colours::black);

        if (! animationImage.isNull())
        {
            juce::Rectangle<int> clipRect(0, currentFrame * imageFrameHeight, imageFrameWidth, imageFrameHeight);
            g.drawImageAt(animationImage.getClippedImage(clipRect), 0, 0);
        }
    }

    void setFrameDimensions(int topLeftX, int topLeftY, int w, int h)
    {
        setTopLeftPosition(topLeftX, topLeftY);
        imageFrameWidth = w;
        imageFrameHeight = h;
        setSize(imageFrameWidth, imageFrameHeight);
    }

    void setStartingFrame(int frameNumber)
    {
        startFrame = frameNumber;
        if (currentFrame < startFrame) currentFrame = startFrame;
    }

    void setEndingFrame(int frameNumber)            { endFrame = frameNumber; }
    void setAnimationRate(float rate)               { incrRate = rate; }
    void setAnimationResetThreshold(float thresh)   { resetThresh = thresh; }

    bool isAnimating = false;

private:
    void paintModernReels(Graphics& g)
    {
        auto r = getLocalBounds().toFloat();

        // transport bay: vertical light falloff + vignette + grain
        ColourGradient bay(ModernTheme::panel.brighter(0.02f), r.getCentreX(), r.getY(),
                           ModernTheme::panelDeep, r.getCentreX(), r.getBottom(), false);
        g.setGradientFill(bay);
        g.fillAll();

        ColourGradient vignette(Colours::transparentBlack, r.getCentreX(), r.getCentreY(),
                                Colours::black.withAlpha(0.45f), r.getX(), r.getY(), true);
        vignette.addColour(0.62, Colours::transparentBlack);
        g.setGradientFill(vignette);
        g.fillRect(r);

        ModernTheme::fillGrain(g, r, 0.5f);

        // The same rotation the filmstrip would show, derived from the frame
        // counter so transport/drag behavior is identical in both eras.
        const float angle = (float) currentFrame * MathConstants<float>::twoPi / (float) jmax(1, animationNumFrames);

        const Point<float> leftHub(r.getWidth() * 0.26f, r.getHeight() * 0.46f);
        const Point<float> rightHub(r.getWidth() * 0.74f, r.getHeight() * 0.46f);
        const float radius = r.getHeight() * 0.40f;
        const float tapeY = leftHub.y + radius * 0.94f;

        // tape path: dark band with a moving sheen, threaded under both reels
        {
            g.setColour(Colour(0xff060607));
            g.fillRect(leftHub.x, tapeY - 2.5f, rightHub.x - leftHub.x, 5.0f);
            g.setColour(Colours::white.withAlpha(0.10f));
            g.drawLine(leftHub.x, tapeY - 2.5f, rightHub.x, tapeY - 2.5f, 0.8f);

            // guide rollers
            for (float gx : { leftHub.x + radius * 0.55f, rightHub.x - radius * 0.55f })
            {
                ColourGradient roller(Colour(0xff4a4a50), gx, tapeY - 7.0f, Colour(0xff19191b), gx, tapeY + 7.0f, false);
                g.setGradientFill(roller);
                g.fillEllipse(gx - 5.5f, tapeY - 5.5f, 11.0f, 11.0f);
                g.setColour(Colours::black.withAlpha(0.6f));
                g.drawEllipse(gx - 5.5f, tapeY - 5.5f, 11.0f, 11.0f, 1.0f);
                g.setColour(Colours::white.withAlpha(0.25f));
                g.fillEllipse(gx - 1.5f, tapeY - 1.5f, 3.0f, 3.0f);
            }
        }

        // head block between the reels
        {
            juce::Rectangle<float> head(r.getCentreX() - 36.0f, tapeY - 26.0f, 72.0f, 30.0f);
            g.setColour(Colours::black.withAlpha(0.5f));
            g.fillRoundedRectangle(head.translated(0, 2.0f), 5.0f);
            ColourGradient headFill(Colour(0xff37373c), head.getCentreX(), head.getY(),
                                    Colour(0xff141416), head.getCentreX(), head.getBottom(), false);
            g.setGradientFill(headFill);
            g.fillRoundedRectangle(head, 5.0f);
            g.setColour(Colours::white.withAlpha(0.10f));
            g.drawLine(head.getX() + 4.0f, head.getY() + 1.0f, head.getRight() - 4.0f, head.getY() + 1.0f, 1.0f);

            // the gap windows
            g.setColour(Colour(0xff0a0a0b));
            for (int i = 0; i < 3; ++i)
                g.fillRoundedRectangle(head.getX() + 9.0f + 19.0f * (float) i, head.getY() + 8.0f, 14.0f, 14.0f, 2.0f);

            ModernTheme::drawScrew(g, { head.getX() + 6.0f, head.getBottom() - 6.0f }, 2.2f, 0.9f);
            ModernTheme::drawScrew(g, { head.getRight() - 6.0f, head.getBottom() - 6.0f }, 2.2f, 2.4f);
        }

        ModernTheme::drawReel(g, leftHub, radius, angle);
        ModernTheme::drawReel(g, rightHub, radius, -angle);

        // pink ember rising from the deck below — the machine is alive
        ColourGradient ember(ModernTheme::accent.withAlpha(hovering ? 0.14f : 0.08f),
                             r.getCentreX(), r.getBottom() + 30.0f,
                             ModernTheme::accent.withAlpha(0.0f), r.getCentreX(), r.getBottom() - 90.0f, true);
        g.setGradientFill(ember);
        g.fillRect(r.withTop(r.getBottom() - 90.0f));

        g.setColour(hovering ? ModernTheme::textPrimary.withAlpha(0.8f) : ModernTheme::textDim.withAlpha(0.55f));
        g.setFont(ModernTheme::labelFont(9.5f));
        g.drawText("DRAG THE REELS TO LEAN ON THE TAPE",
                   r.removeFromBottom(16.0f), Justification::centred, false);
    }

    void mouseEnter(const MouseEvent&) override { hovering = true; repaint(); }
    void mouseExit(const MouseEvent&) override  { hovering = false; repaint(); }

    void timerCallback() override
    {
        // The increment/threshold gate slows the reels while they're being
        // touched — preserved from the original update() logic.
        if (curIncrement == 0.0f)
        {
            if (! animationImage.isNull())
            {
                if (currentFrame >= endFrame)
                    currentFrame = startFrame;
                ++currentFrame;
            }
        }

        curIncrement += incrRate;
        if (curIncrement >= resetThresh)
            curIncrement = 0.0f;

        repaint();
    }

    UIEra era = UIEra::heritage;
    bool hovering = false;

    float resetThresh = 1.0f;
    float curIncrement = 0.0f;
    float incrRate = 0.01f;

    Image animationImage;
    int imageFrameWidth = 0;
    int imageFrameHeight = 0;

    float curFlangeDepth = 0.0f;
    float setFlangeDepth = 0.0f;

    int animationNumFrames = 1;
    int currentFrame = 0;
    int startFrame = 0;
    int endFrame = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageAnimationComponent)
};
