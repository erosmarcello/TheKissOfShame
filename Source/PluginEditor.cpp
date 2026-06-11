#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ShameResources.h"

//==============================================================================
KissOfShameAudioProcessorEditor::KissOfShameAudioProcessorEditor(KissOfShameAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    using namespace ShameResources;

    faceWithReels    = getImage(BinaryData::fond_alpha_png, BinaryData::fond_alpha_pngSize);
    faceWithoutReels = getImage(BinaryData::fond_alone2_png, BinaryData::fond_alone2_pngSize);

    backlight.setTopLeftPosition(0, 703 - backlight.getHeight());
    addAndMakeVisible(backlight);

    faceImage.setNumFrames(1);
    faceImage.setDimensions(0, 0, 960, 703);
    faceImage.setAnimationImage(faceWithReels);
    faceImage.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(faceImage);

    /////////// COMPONENTS /////////////////

    environmentsComponent.setAnimationImage(getImage(BinaryData::_00_png, BinaryData::_00_pngSize));
    environmentsComponent.setTopLeftPosition(388, 654);
    addAndMakeVisible(environmentsComponent);

    ////////// KNOBS ////////////////

    inputSaturationKnob.setKnobImage(getImage(BinaryData::_06_alpha_png, BinaryData::_06_alpha_pngSize));
    inputSaturationKnob.setNumFrames(65);
    inputSaturationKnob.setKnobDimensions(104, 521, 116, 116);
    addAndMakeVisible(inputSaturationKnob);

    shameKnobImage.setNumFrames(65);
    shameKnobImage.setDimensions(401, 491, 174, 163);
    shameKnobImage.setAnimationImage(getImage(BinaryData::_09_alpha_png, BinaryData::_09_alpha_pngSize));
    addAndMakeVisible(shameKnobImage);

    shameKnob.setKnobImage(getImage(BinaryData::_09_v2_png, BinaryData::_09_v2_pngSize));
    shameKnob.setNumFrames(65);
    shameKnob.setKnobDimensions(401, 491, 174, 163);
    shameKnob.onValueChange = [this] { shameKnobImage.updateImageWithValue((float) shameKnob.getValue()); };
    shameKnob.onDoubleClick = [this] { toggleExtreme(); };
    addAndMakeVisible(shameKnob);

    hissKnob.setKnobImage(getImage(BinaryData::_04_alpha_png, BinaryData::_04_alpha_pngSize));
    hissKnob.setNumFrames(65);
    hissKnob.setKnobDimensions(547, 455, 78, 72);
    addAndMakeVisible(hissKnob);

    blendKnob.setKnobImage(getImage(BinaryData::_05_alpha_png, BinaryData::_05_alpha_pngSize));
    blendKnob.setNumFrames(65);
    blendKnob.setKnobDimensions(705, 455, 78, 72);
    addAndMakeVisible(blendKnob);

    outputKnob.setKnobImage(getImage(BinaryData::_12_alpha_png, BinaryData::_12_alpha_pngSize));
    outputKnob.setNumFrames(65);
    outputKnob.setKnobDimensions(757, 521, 122, 116);
    addAndMakeVisible(outputKnob);

    ageKnob.setKnobImage(getImage(BinaryData::_03_alpha_png, BinaryData::_03_alpha_pngSize));
    ageKnob.setNumFrames(65);
    ageKnob.setKnobDimensions(350, 455, 74, 72);
    addAndMakeVisible(ageKnob);

    // The input/output link: invert-couples the two gain knobs.
    inputSaturationKnob.onValueChange = [this]
    {
        if (linkIOMode)
            outputKnob.setValue(1.0 - inputSaturationKnob.getValue());
    };
    outputKnob.onValueChange = [this]
    {
        if (linkIOMode)
            inputSaturationKnob.setValue(1.0 - outputKnob.getValue());
    };

    ///////////// BUTTONS /////////////////

    const auto bypassImage = getImage(BinaryData::_01_png, BinaryData::_01_pngSize);
    bypassButton.setTopLeftPosition(202, 469);
    bypassButton.setClippedCustomOnImage(bypassImage, 0, 68, 34, 34);
    bypassButton.setClippedCustomOffImage(bypassImage, 0, 0, 34, 34);
    bypassButton.setClickingTogglesState(true);
    bypassButton.onClick = [this]
    {
        const bool bypassed = bypassButton.getToggleState();
        backlight.setAlpha(bypassed ? 0.25f : 1.0f);
        shameKnob.setAlpha(bypassed ? 0.25f : 1.0f);
    };
    addAndMakeVisible(bypassButton);

    const auto tapeTypeImage = getImage(BinaryData::_07_png, BinaryData::_07_pngSize);
    tapeTypeButton.setTopLeftPosition(233, 610);
    tapeTypeButton.setClippedCustomOnImage(tapeTypeImage, 0, 0, 42, 39);
    tapeTypeButton.setClippedCustomOffImage(tapeTypeImage, 0, 39, 42, 39);
    tapeTypeButton.setClickingTogglesState(true);
    addAndMakeVisible(tapeTypeButton);

    const auto printThroughImage = getImage(BinaryData::_11_png, BinaryData::_11_pngSize);
    printThroughButton.setTopLeftPosition(698, 609);
    printThroughButton.setClippedCustomOnImage(printThroughImage, 0, 41, 47, 41);
    printThroughButton.setClippedCustomOffImage(printThroughImage, 0, 0, 47, 41);
    printThroughButton.setClickingTogglesState(true);
    addAndMakeVisible(printThroughButton);

    const auto linkImage = getImage(BinaryData::link_png, BinaryData::link_pngSize);
    linkIOButtonL.setTopLeftPosition(137, 605);
    linkIOButtonL.setClippedCustomOnImage(linkImage, 0, 0, 50, 50);
    linkIOButtonL.setClippedCustomOffImage(linkImage, 0, 0, 50, 50);
    linkIOButtonL.resizeButton(0.6f);
    linkIOButtonL.setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonL);

    linkIOButtonR.setTopLeftPosition(792, 605);
    linkIOButtonR.setClippedCustomOnImage(linkImage, 0, 0, 50, 50);
    linkIOButtonR.setClippedCustomOffImage(linkImage, 0, 0, 50, 50);
    linkIOButtonR.resizeButton(0.6f);
    linkIOButtonR.setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonR);

    auto handleLinkClick = [this](CustomButton& clicked)
    {
        const bool linked = clicked.getToggleState();
        linkIOButtonL.setToggleState(linked, dontSendNotification);
        linkIOButtonR.setToggleState(linked, dontSendNotification);
        linkIOMode = linked;

        linkIOButtonL.setAlpha(linked ? 1.0f : 0.25f);
        linkIOButtonR.setAlpha(linked ? 1.0f : 0.25f);

        if (linked)
            outputKnob.setValue(1.0 - inputSaturationKnob.getValue());
    };
    linkIOButtonL.onClick = [this, handleLinkClick] { handleLinkClick(linkIOButtonL); };
    linkIOButtonR.onClick = [this, handleLinkClick] { handleLinkClick(linkIOButtonR); };

    linkIOButtonL.setAlpha(0.25f);
    linkIOButtonR.setAlpha(0.25f);

    ///////////////// Animation //////////////////

    reelAnimation = std::make_unique<ImageAnimationComponent>(
        getImage(BinaryData::wheels_png, BinaryData::wheels_pngSize), 31, 50);
    reelAnimation->setFrameDimensions(0, 0, 960, 322);
    addAndMakeVisible(*reelAnimation);

    vuMeterL.setNumFrames(65);
    vuMeterL.setDimensions(251, 518, 108, 108);
    vuMeterL.setAnimationImage(getImage(BinaryData::_08_png, BinaryData::_08_pngSize));
    addAndMakeVisible(vuMeterL);

    vuMeterR.setNumFrames(65);
    vuMeterR.setDimensions(605, 518, 110, 108);
    vuMeterR.setAnimationImage(getImage(BinaryData::_10_png, BinaryData::_10_pngSize));
    addAndMakeVisible(vuMeterR);

    ///////////////// Parameter bindings //////////////////

    auto& apvts = processor.apvts;

    inputAttachment  = std::make_unique<SliderAttachment>(apvts, ParamIDs::input, inputSaturationKnob);
    shameAttachment  = std::make_unique<SliderAttachment>(apvts, ParamIDs::shame, shameKnob);
    hissAttachment   = std::make_unique<SliderAttachment>(apvts, ParamIDs::hiss, hissKnob);
    blendAttachment  = std::make_unique<SliderAttachment>(apvts, ParamIDs::blend, blendKnob);
    outputAttachment = std::make_unique<SliderAttachment>(apvts, ParamIDs::output, outputKnob);
    ageAttachment    = std::make_unique<SliderAttachment>(apvts, ParamIDs::age, ageKnob);

    bypassAttachment       = std::make_unique<ButtonAttachment>(apvts, ParamIDs::bypass, bypassButton);
    tapeTypeAttachment     = std::make_unique<ButtonAttachment>(apvts, ParamIDs::tapeType, tapeTypeButton);
    printThroughAttachment = std::make_unique<ButtonAttachment>(apvts, ParamIDs::printThrough, printThroughButton);

    if (auto* flangeParam = apvts.getParameter(ParamIDs::flange))
    {
        flangeAttachment = std::make_unique<ParameterAttachment>(*flangeParam, [](float) {});

        reelAnimation->onFlangeGestureStart = [this] { flangeAttachment->beginGesture(); };
        reelAnimation->onFlangeDepthChanged = [this](float depth) { flangeAttachment->setValueAsPartOfGesture(depth); };
        reelAnimation->onFlangeGestureEnd   = [this] { flangeAttachment->endGesture(); };
    }

    if (auto* envParam = apvts.getParameter(ParamIDs::environment))
    {
        environmentAttachment = std::make_unique<ParameterAttachment>(*envParam,
            [this](float newValue) { environmentsComponent.setDisplayedEnvironment((int) std::lround(newValue)); });
        environmentAttachment->sendInitialUpdate();

        environmentsComponent.onEnvironmentChanged = [this](int index)
        { environmentAttachment->setValueAsCompleteGesture((float) index); };
    }

    ///////////////// Initial state //////////////////

    shameKnobImage.updateImageWithValue((float) shameKnob.getValue());
    backlight.setExtreme(processor.isShameExtreme());

    showReels = processor.getShowReels();
    setSize(960, showReels ? 703 : 266);
    if (! showReels)
    {
        setReelMode(false);
        applyReelVisibility();
    }

    startTimer(25);
}

//==============================================================================
void KissOfShameAudioProcessorEditor::toggleExtreme()
{
    const bool extreme = ! processor.isShameExtreme();
    processor.setShameExtreme(extreme);
    backlight.setExtreme(extreme);
    repaint();
}

//==============================================================================
void KissOfShameAudioProcessorEditor::setReelMode(bool shouldShowReels)
{
    const int adjustment = shouldShowReels ? 437 : -437; // height difference: 703 - 266

    auto shift = [adjustment](Component& c)
    { c.setTopLeftPosition(c.getX(), c.getY() + adjustment); };

    shift(backlight);

    shift(inputSaturationKnob);
    shift(shameKnob);
    shift(hissKnob);
    shift(blendKnob);
    shift(outputKnob);
    shift(ageKnob);

    shift(bypassButton);
    shift(tapeTypeButton);
    shift(printThroughButton);
    shift(linkIOButtonL);
    shift(linkIOButtonR);

    shift(environmentsComponent);

    shift(vuMeterL);
    shift(vuMeterR);
    shift(shameKnobImage);

    if (shouldShowReels)
    {
        faceImage.setAnimationImage(faceWithReels);
        faceImage.setDimensions(0, 0, 960, 703);
    }
    else
    {
        faceImage.setAnimationImage(faceWithoutReels);
        faceImage.setDimensions(0, 0, 960, 266);
    }
}

void KissOfShameAudioProcessorEditor::applyReelVisibility()
{
    if (showReels)
        addAndMakeVisible(*reelAnimation);
    else
        removeChildComponent(reelAnimation.get());
}

void KissOfShameAudioProcessorEditor::mouseDoubleClick(const MouseEvent&)
{
    showReels = ! showReels;
    processor.setShowReels(showReels);

    setReelMode(showReels);
    applyReelVisibility();
    setSize(faceImage.getWidth(), faceImage.getHeight());
    repaint();
}

//==============================================================================
void KissOfShameAudioProcessorEditor::timerCallback()
{
    // VU meters track the processed level unless bypassed.
    const float vuLevelL = bypassButton.getToggleState() ? 0.0f : processor.getCurrentRMSL() * 3.0f;
    const float vuLevelR = bypassButton.getToggleState() ? 0.0f : processor.getCurrentRMSR() * 3.0f;
    vuMeterL.updateImageWithValue(vuLevelL);
    vuMeterR.updateImageWithValue(vuLevelR);

    // Reels spin only while the host transport runs and audio is flowing.
    const int blockCount = processor.getBlockCounter();
    if (processor.isTransportPlaying() && blockCount != priorBlockCount && ! processor.isGraphBypassed())
    {
        priorBlockCount = blockCount;
        if (! reelAnimation->isAnimating)
        {
            reelAnimation->setFramesPerSecond(50);
            reelAnimation->isAnimating = true;
        }
    }
    else if (reelAnimation->isAnimating)
    {
        reelAnimation->setFramesPerSecond(0);
        reelAnimation->isAnimating = false;
    }
}

//==============================================================================
void KissOfShameAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}

void KissOfShameAudioProcessorEditor::paintOverChildren(Graphics& g)
{
    // Extreme mode: the Shame knob runs hot.
    if (processor.isShameExtreme())
    {
        const auto knobBounds = shameKnob.getBounds().toFloat().expanded(10.0f);
        const auto centre = knobBounds.getCentre();

        ColourGradient glow(Colour::fromFloatRGBA(1.0f, 0.1f, 0.15f, 0.45f), centre.x, centre.y,
                            Colour::fromFloatRGBA(1.0f, 0.1f, 0.15f, 0.0f), centre.x, knobBounds.getY(), true);
        g.setGradientFill(glow);
        g.fillEllipse(knobBounds);
    }
}
