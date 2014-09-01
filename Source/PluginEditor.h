/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class KissOfShameAudioProcessorEditor  : public AudioProcessorEditor,
                                         public Timer,
                                         public SliderListener
{
public:
    KissOfShameAudioProcessorEditor (KissOfShameAudioProcessor* ownerFilter);
    ~KissOfShameAudioProcessorEditor();
    
    void timerCallback() override;
    
    void sliderValueChanged (Slider*) override;
    
    ScopedPointer<Slider> inputSaturationSlider;
    ScopedPointer<Slider> shameSlider;
    ScopedPointer<Slider> hissSlider;
        
private:
    
    KissOfShameAudioProcessor* getProcessor() const
    {
        return static_cast <KissOfShameAudioProcessor*> (getAudioProcessor());
    }
    
    


    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
};


#endif  // PLUGINEDITOR_H_INCLUDED
