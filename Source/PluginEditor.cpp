/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
KissOfShameAudioProcessorEditor::KissOfShameAudioProcessorEditor (KissOfShameAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), priorProcessorTime(0), showReels(true), linkIOMode(false)
{
    
    //setSize(1000, 1000);
    
    //addKeyListener(this);
    
    backlight = new BacklightComponent;
    backlight->setTopLeftPosition(0, 703 - backlight->getHeight());
    addAndMakeVisible(backlight);
    
    faceImage = new ImageInteractor;
    faceImage->setNumFrames(1);
    faceImage->setDimensions(0, 0, 960, 703);
    String faceImagePath = GUI_PATH + "KOS_Graphics/fond_alpha.png";
    faceImage->setAnimationImage(faceImagePath);
    faceImage->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(faceImage);

    
    /////////// COMPONENTS /////////////////
    
    environmentsComponent = new EnvironmentsComponent(p);
    environmentsComponent->setTopLeftPosition(388, 654);
    addAndMakeVisible(environmentsComponent);
    
    
    ////////// KNOBS ////////////////
    
    inputSaturationKnob = new CustomKnob;
    String inputImagePath = GUI_PATH + "KOS_Graphics/06_alpha.png";
    inputSaturationKnob->setKnobImage(inputImagePath);
    inputSaturationKnob->setNumFrames(65);
    inputSaturationKnob->setKnobDimensions(104, 521, 116, 116);
    inputSaturationKnob->addListener (this);
    addAndMakeVisible(inputSaturationKnob);

    shameKnobImage = new ImageInteractor;
    shameKnobImage->setNumFrames(65);
    shameKnobImage->setDimensions(401, 491, 174, 163);
    String shameImagePath = GUI_PATH + "KOS_Graphics/09_alpha.png";
    shameKnobImage->setAnimationImage(shameImagePath);
    addAndMakeVisible(shameKnobImage);
    
    shameKnob = new CustomKnob;
    String crossImagePath = GUI_PATH + "KOS_Graphics/09_v2.png";
    shameKnob->setKnobImage(crossImagePath);
    shameKnob->setNumFrames(65);
    shameKnob->setKnobDimensions(401, 491, 174, 163);
    shameKnob->addListener (this);
    addAndMakeVisible(shameKnob);
    
    hissKnob = new CustomKnob;
    String hissImagePath = GUI_PATH + "KOS_Graphics/04_alpha.png";
    hissKnob->setKnobImage(hissImagePath);
    hissKnob->setNumFrames(65);
    hissKnob->setKnobDimensions(547, 455, 78, 72);
    hissKnob->addListener (this);
    addAndMakeVisible(hissKnob);
    
    blendKnob = new CustomKnob;
    String blendImagePath = GUI_PATH + "KOS_Graphics/05_alpha.png";
    blendKnob->setKnobImage(blendImagePath);
    blendKnob->setNumFrames(65);
    blendKnob->setKnobDimensions(705, 455, 78, 72);
    blendKnob->addListener (this);
    addAndMakeVisible(blendKnob);
    
    outputKnob = new CustomKnob;
    String outputImagePath = GUI_PATH + "KOS_Graphics/12_alpha.png";
    outputKnob->setKnobImage(outputImagePath);
    outputKnob->setNumFrames(65);
    outputKnob->setKnobDimensions(757, 521, 122, 116);
    outputKnob->addListener (this);
    addAndMakeVisible(outputKnob);
    
    ageKnob = new CustomKnob;
    String ageImagePath = GUI_PATH + "KOS_Graphics/03_alpha.png";
    ageKnob->setKnobImage(ageImagePath);
    ageKnob->setNumFrames(65);
    ageKnob->setKnobDimensions(350, 455, 74, 72);
    ageKnob->addListener (this);
    addAndMakeVisible(ageKnob);
    
    
    ///////////// BUTTONS /////////////////
    
    bypassButton = new CustomButton;
    bypassButton->setTopLeftPosition(202, 469);
    String bypassImagePath = GUI_PATH + "KOS_Graphics/01.png";
    bypassButton->setClippedCustomOnImage(bypassImagePath, 0, 68, 34, 34);
    bypassButton->setClippedCustomOffImage(bypassImagePath, 0, 0, 34, 34);
    bypassButton->addListener(this);
    bypassButton->setClickingTogglesState(true);
    addAndMakeVisible(bypassButton);
    
    tapeTypeButton = new CustomButton;
    tapeTypeButton->setTopLeftPosition(233, 610);
    String tapeTypeImagePath = GUI_PATH + "KOS_Graphics/07.png";
    tapeTypeButton->setClippedCustomOnImage(tapeTypeImagePath, 0, 0, 42, 39);
    tapeTypeButton->setClippedCustomOffImage(tapeTypeImagePath, 0, 39, 42, 39);
    tapeTypeButton->addListener(this);
    tapeTypeButton->setClickingTogglesState(true);
    addAndMakeVisible(tapeTypeButton);
    
    printThroughButton = new CustomButton;
    printThroughButton->setTopLeftPosition(698, 609);
    String printThroughImagePath = GUI_PATH + "KOS_Graphics/11.png";
    printThroughButton->setClippedCustomOnImage(printThroughImagePath, 0, 41, 47, 41);
    printThroughButton->setClippedCustomOffImage(printThroughImagePath, 0, 0, 47, 41);
    printThroughButton->addListener(this);
    printThroughButton->setClickingTogglesState(true);
    addAndMakeVisible(printThroughButton);
    
    linkIOButtonL = new CustomButton;
    //inputSaturationKnob->setKnobDimensions(104, 521, 116, 116);
    linkIOButtonL->setTopLeftPosition(137, 605);
    String linkImagePath = GUI_PATH + "KOS_Graphics/link.png";
    linkIOButtonL->setClippedCustomOnImage(linkImagePath, 0, 0, 50, 50);
    linkIOButtonL->setClippedCustomOffImage(linkImagePath, 0, 0, 50, 50);
    linkIOButtonL->resizeButton(0.6);
    linkIOButtonL->addListener(this);
    linkIOButtonL->setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonL);

    linkIOButtonR = new CustomButton;
    //outputKnob->setKnobDimensions(757, 521, 122, 116);
    linkIOButtonR->setTopLeftPosition(792, 605);
    linkIOButtonR->setClippedCustomOnImage(linkImagePath, 0, 0, 50, 50);
    linkIOButtonR->setClippedCustomOffImage(linkImagePath, 0, 0, 50, 50);
    linkIOButtonR->resizeButton(0.6);
    linkIOButtonR->addListener(this);
    linkIOButtonR->setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonR);

    
    ///////////////// Animation //////////////////
    
    String reelImagePath = GUI_PATH + "KOS_Graphics/wheels.png";
    File reelFile(reelImagePath);
    //reelAnimation = new ImageAnimator(reelFile, 31, 31);
    reelAnimation = new ImageAnimationComponent(reelFile, 31, 50); //50
    reelAnimation->setFrameDimensions(0, 0, 960, 322);
    reelAnimation->addActionListener(this);
    addAndMakeVisible(reelAnimation);
    
    vuMeterL = new ImageInteractor;
    vuMeterL->setNumFrames(65);
    vuMeterL->setDimensions(251, 518, 108, 108);
    String vuLeftImagePath = GUI_PATH + "KOS_Graphics/08.png";
    vuMeterL->setAnimationImage(vuLeftImagePath);
    addAndMakeVisible(vuMeterL);
    
    vuMeterR = new ImageInteractor;
    vuMeterR->setNumFrames(65);
    vuMeterR->setDimensions(605, 518, 110, 108);
    String vuRightImagePath = GUI_PATH + "KOS_Graphics/10.png";
    vuMeterR->setAnimationImage(vuRightImagePath);
    addAndMakeVisible(vuMeterR);
    
    //oglContext.setComponentPaintingEnabled(true);
    //oglContext.attachTo(*this);

    
    //////////////// LABELS /////////////////
    //#if DEBUG
//    String debugText = "Debug Info...";
//    debugLabel.setText(debugText, dontSendNotification);
//    debugLabel.setTopLeftPosition(100, 100);
//    debugLabel.setFont (Font (25.0f));
//    debugLabel.setSize(500, 50);
//    debugLabel.setColour(Label::textColourId, Colours::white);
//    addAndMakeVisible(debugLabel);
    //#endif

    
    int mainWidth = faceImage->getWidth();
    int mainHeight = faceImage->getHeight();// + inputSaturationKnob->getHeight() + inputLabel.getHeight();
    setSize(mainWidth, mainHeight);
    
    initializeLevels();
        
    startTimer(25);    
}

KissOfShameAudioProcessorEditor::~KissOfShameAudioProcessorEditor()
{
}

void KissOfShameAudioProcessorEditor::actionListenerCallback (const String& message)
{
    if(message == "updateFlange")
    {
        //debugLabel.setText(String(reelAnimation->getCurrentFlangeDepth()), dontSendNotification);
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::flangeParam, reelAnimation->getCurrentFlangeDepth());
        processor.aGraph->setAudioUnitParameters(eFlangeDepth, reelAnimation->getCurrentFlangeDepth());
    }
}

void KissOfShameAudioProcessorEditor::setReelMode(bool showReels)
{

    int adustment = showReels ? 437 : -437; //= height difference: 703 - 266
    
    //images
    backlight->setTopLeftPosition(backlight->getX(),backlight->getY()+adustment);
    
    //Knobs
    inputSaturationKnob->setTopLeftPosition(inputSaturationKnob->getX(),inputSaturationKnob->getY()+adustment);
    shameKnob->setTopLeftPosition(shameKnob->getX(),shameKnob->getY()+adustment);
    hissKnob->setTopLeftPosition(hissKnob->getX(),hissKnob->getY()+adustment);
    blendKnob->setTopLeftPosition(blendKnob->getX(),blendKnob->getY()+adustment);
    outputKnob->setTopLeftPosition(outputKnob->getX(),outputKnob->getY()+adustment);
    ageKnob->setTopLeftPosition(ageKnob->getX(),ageKnob->getY()+adustment);
    
    //buttons
    bypassButton->setTopLeftPosition(bypassButton->getX(),bypassButton->getY()+adustment);
    tapeTypeButton->setTopLeftPosition(tapeTypeButton->getX(),tapeTypeButton->getY()+adustment);
    printThroughButton->setTopLeftPosition(printThroughButton->getX(),printThroughButton->getY()+adustment);
    
    //Components
    environmentsComponent->setTopLeftPosition(environmentsComponent->getX(),environmentsComponent->getY()+adustment);
    
    //animation
    vuMeterL->setTopLeftPosition(vuMeterL->getX(),vuMeterL->getY()+adustment);
    vuMeterR->setTopLeftPosition(vuMeterR->getX(),vuMeterR->getY()+adustment);
    shameKnobImage->setTopLeftPosition(shameKnobImage->getX(),shameKnobImage->getY()+adustment);
    
    
    String faceImagePath;
    int faceHeight = 0;
    if(showReels)
    {
        faceImagePath = GUI_PATH + "KOS_Graphics/fond_alpha.png";
        faceHeight = 703;
    }
    else
    {
        faceImagePath = GUI_PATH + "KOS_Graphics/fond_alone2.png";
        faceHeight = 266;
    }
    faceImage->setAnimationImage(faceImagePath);
    faceImage->setDimensions(0, 0, 960, faceHeight);
}

void KissOfShameAudioProcessorEditor::timerCallback()
{
    //processor.
    //KissOfShameAudioProcessor* ourProcessor = getProcessor();
    
    //debugLabel.setText(String(reelAnimation->getCurrentFlangeDepth()), dontSendNotification);
    
    //DEBUG: message from processor
    //debugLabel.setText(String(ourProcessor->curPositionInfo.isPlaying) + ":  " + String(ourProcessor->playHeadPos), dontSendNotification);
    
    //animation of VU meters and backlighting
    //float smoothRMS = tanh(ourProcessor->curRMS*10);
    float vuLevelL = bypassButton->getToggleState() ? 0.0 : processor.curRMSL*3;
    float vuLevelR = bypassButton->getToggleState() ? 0.0 : processor.curRMSR*3;
    vuMeterL->updateImageWithValue(vuLevelL);
    vuMeterR->updateImageWithValue(vuLevelR);
    
//    if(!bypassButton->getToggleState())
//    {
//        float backlightAlpha = 1 - (0.5*processor.curRMSL + 0.5*processor.curRMSR)*3*shameKnob->getValue();
//        backlight->setAlpha(backlightAlpha);
//        shameKnob->setAlpha(backlightAlpha);
//    }
    
    //NOTE: when output level == 0, for some reason the AudioPlayhead position doesn't return to 0
    //after stopping playback. Don't know why this is... For now, only animating reels when output != 0.
    if(processor.curPositionInfo.isPlaying && processor.playHeadPos != priorProcessorTime && !processor.aGraph->isGraphBypassed())
    {
        priorProcessorTime = processor.playHeadPos;
        if(!reelAnimation->isAnimating)
        {
            reelAnimation->setFramesPerSecond(50);
            reelAnimation->isAnimating = true;
        }
    }
    else
    {
        reelAnimation->setFramesPerSecond(0);
        reelAnimation->isAnimating = false;
    }
    //else if(reelAnimation->isAnimating) reelAnimation->stopAnimation();
    
}

void KissOfShameAudioProcessorEditor::mouseDoubleClick(const MouseEvent &event)
{

    //debugLabel.setText("Double Clicked!!!!", dontSendNotification);
    
    if(showReels)
    {
        showReels = false;
        removeChildComponent(reelAnimation);
        setReelMode(false);
        setSize(faceImage->getWidth(), faceImage->getHeight());
        repaint();
    }
    else
    {
        showReels = true;
        addAndMakeVisible(reelAnimation);
        setReelMode(true);
        setSize(faceImage->getWidth(), faceImage->getHeight());
        repaint();
    }
}


void KissOfShameAudioProcessorEditor::mouseDown (const MouseEvent& event)
{
}
void KissOfShameAudioProcessorEditor::mouseUp (const MouseEvent& event)
{
}
void KissOfShameAudioProcessorEditor::mouseDrag (const MouseEvent& event)
{
}

void KissOfShameAudioProcessorEditor::initializeLevels()
{
    inputSaturationKnob->setValue(0.5);
    processor.setParameterNotifyingHost (KissOfShameAudioProcessor::inputSaturationParam, 0.5);
    processor.aGraph->setAudioUnitParameters(eInputDrive, 0.5);
    
    outputKnob->setValue(0.5);
    processor.setParameterNotifyingHost (KissOfShameAudioProcessor::outputParam, 0.5);
    processor.aGraph->setAudioUnitParameters(eOutputLevel, 0.5);
    
    blendKnob->setValue(1.0);
    processor.setParameterNotifyingHost (KissOfShameAudioProcessor::blendParam, 1.0);
    processor.aGraph->setAudioUnitParameters(eBlendLevel, 1.0);
    
    linkIOButtonL->setToggleState(false, dontSendNotification);
    linkIOButtonR->setToggleState(false, dontSendNotification);
    linkIOButtonL->setAlpha(0.25);
    linkIOButtonR->setAlpha(0.25);
    linkIOMode = false;
    
    reelAnimation->setAnimationResetThreshold(0.0);
}

void KissOfShameAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    
    // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
    // by the host, rather than just modifying them directly, otherwise the host won't know
    // that they've changed.
    
    if (slider == inputSaturationKnob)
    {
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::inputSaturationParam,
                                                   (float) inputSaturationKnob->getValue());
        processor.aGraph->setAudioUnitParameters(eInputDrive, (float) inputSaturationKnob->getValue());
        
        if(linkIOMode)
        {
            outputKnob->setValue(1.0 - inputSaturationKnob->getValue());
            processor.setParameterNotifyingHost (KissOfShameAudioProcessor::outputParam, (float) outputKnob->getValue());
            processor.aGraph->setAudioUnitParameters(eOutputLevel, (float) outputKnob->getValue());
        }
    }
    else if(slider == outputKnob)
    {
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::outputParam,
                                             (float) outputKnob->getValue());
        processor.aGraph->setAudioUnitParameters(eOutputLevel, (float) outputKnob->getValue());
        
        if(linkIOMode)
        {
            inputSaturationKnob->setValue(1.0 - outputKnob->getValue());
            processor.setParameterNotifyingHost (KissOfShameAudioProcessor::inputSaturationParam, (float) inputSaturationKnob->getValue());
            processor.aGraph->setAudioUnitParameters(eInputDrive, (float) inputSaturationKnob->getValue());
        }
    }
    else if(slider == shameKnob)
    {
        shameKnobImage->updateImageWithValue(slider->getValue());
        
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::shameParam,
                                                   (float) shameKnob->getValue());
        
        processor.aGraph->setAudioUnitParameters(eShameGlobalLevel, (float) shameKnob->getValue());
    }
    else if(slider == hissKnob)
    {
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::hissParam,
                                                   (float) hissKnob->getValue());
        
        processor.aGraph->setAudioUnitParameters(eHissLevel, (float) hissKnob->getValue());
    }
    else if(slider == blendKnob)
    {
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::blendParam,
                                                   (float) blendKnob->getValue());
        
        processor.aGraph->setAudioUnitParameters(eBlendLevel, (float) blendKnob->getValue());
    }
    else if(slider == ageKnob)
    {
        //reelAnimation->setFramesPerSecond(ageKnob->getValue()*15 + 35);
        //reelAnimation->setAnimationRate(ageKnob->getValue());
        //reelAnimation->setAnimationResetThreshold(ageKnob->getValue()*0.025);
        
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::hurricaneSandyParam,
                                             (float) ageKnob->getValue());
        
        processor.aGraph->setAudioUnitParameters(eHurricaneSandyGlobalLevel, (float) ageKnob->getValue());
    }
}

void KissOfShameAudioProcessorEditor::buttonClicked (Button* b)
{
    if(b == bypassButton)
    {
        if(b->getToggleState())
        {
            //vuMeterL->updateImageWithValue(0);
            //vuMeterR->updateImageWithValue(0);
            //vuMeterL->setDesaturate(true);
            //vuMeterR->setDesaturate(true);
            backlight->setAlpha(0.25);
            shameKnob->setAlpha(0.25);
        }
        else
        {
            //vuMeterL->setDesaturate(false);
            //vuMeterR->setDesaturate(false);
            //vuMeterL->setAlpha(1.0);
            //vuMeterR->setAlpha(1.0);
            backlight->setAlpha(1.0);
            shameKnob->setAlpha(1.0);
        }
        
        
        processor.setParameterNotifyingHost (KissOfShameAudioProcessor::bypassParam,
                                                   b->getToggleState());
        processor.aGraph->setAudioUnitParameters(eBypass, b->getToggleState());
    }
    else if(b == linkIOButtonL || b == linkIOButtonR)
    {
        linkIOButtonL->setToggleState(b->getToggleState(), dontSendNotification);
        linkIOButtonR->setToggleState(b->getToggleState(), dontSendNotification);
        linkIOMode = b->getToggleState();
 
        if(b->getToggleState())
        {
            linkIOButtonL->setAlpha(1.0);
            linkIOButtonR->setAlpha(1.0);
            
            outputKnob->setValue(1.0 - inputSaturationKnob->getValue());
            processor.setParameterNotifyingHost (KissOfShameAudioProcessor::outputParam, (float) outputKnob->getValue());
            processor.aGraph->setAudioUnitParameters(eOutputLevel, (float) outputKnob->getValue());
        }
        else
        {
            linkIOButtonL->setAlpha(0.25);
            linkIOButtonR->setAlpha(0.25);
        }
    }
}


//==============================================================================
void KissOfShameAudioProcessorEditor::paint (Graphics& g)
{
    //RGBA: 255, 55, 98, 1.0
    //g.fillAll(Colour::fromFloatRGBA(1.0f, 0.216f, 0.384f, 1.0f));
    
    g.fillAll(Colours::black.withAlpha(1.0f));
    
    //g.drawImageAt(linkIO, 0, 0);
}

void KissOfShameAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
