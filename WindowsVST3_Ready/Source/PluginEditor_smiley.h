/*
  ==============================================================================

    TB-303 Distortion Plugin Editor with Acid Smiley Morphing

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class AcidSmileyComponent : public juce::Component, private juce::Timer
{
public:
    AcidSmileyComponent();
    ~AcidSmileyComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void setDryWetValue(float value);
    float getDryWetValue() const { return dryWetValue; }
    
    std::function<void(float)> onValueChange;
    
private:
    void timerCallback() override;
    void updateMorphedImage();
    
    juce::Image smileyDry;
    juce::Image smileyWet;
    juce::Image morphedImage;
    
    float dryWetValue = 0.75f;
    float targetValue = 0.75f;
    
    bool isDragging = false;
    juce::Point<int> lastMousePos;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AcidSmileyComponent)
};

//==============================================================================
class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    NewProjectAudioProcessor& audioProcessor;
    
    // GUI Components
    AcidSmileyComponent acidSmiley;
    juce::Label titleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};