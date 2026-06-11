#pragma once

#include "ImageInteractor.h"

// The storage-environment selector strip. Rev 2: decoupled from the
// processor — it reports clicks through onEnvironmentChanged and is told
// what to display via setDisplayedEnvironment, so the parameter system
// remains the single source of truth (and host automation moves the strip).
class EnvironmentsComponent : public ImageInteractor
{
public:
    EnvironmentsComponent()
    {
        setNumFrames(6);
        setMinMaxValues(0, 5);
        setDimensions(0, 0, 183, 32);
    }

    ~EnvironmentsComponent() override = default;

    void setDisplayedEnvironment(int index)
    {
        currentIndex = jlimit(0, 5, index);
        updateImageWithValue((float) currentIndex);
    }

    void mouseUp(const MouseEvent& event) override
    {
        ignoreUnused(event);

        const int next = (currentIndex + 1) % 6;
        setDisplayedEnvironment(next);

        if (onEnvironmentChanged != nullptr)
            onEnvironmentChanged(next);
    }

    std::function<void(int)> onEnvironmentChanged;

private:
    int currentIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvironmentsComponent)
};
