/*
  ==============================================================================

    TB-303 Distortion Plugin Editor with Acid Smiley Morphing

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class AcidSmileyComponent;

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
    std::unique_ptr<AcidSmileyComponent> acidSmiley;
    juce::Label titleLabel;
    
    // Info system
    juce::TextButton infoButton;
    juce::TextEditor infoTextArea;
    juce::TextButton closeButton;
    bool showingInfo = false;
    
    void showInfoPanel();
    void hideInfoPanel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};