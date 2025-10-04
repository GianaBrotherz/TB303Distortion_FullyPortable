/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Title Label
    titleLabel.setText("TB-303 DISTORTION", juce::dontSendNotification);
    titleLabel.setFont(24.0f);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Dry/Wet Slider
    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setRange(0.0f, 1.0f, 0.01f);
    dryWetSlider.setValue(0.75f);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    dryWetSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    dryWetSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
    dryWetSlider.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
    dryWetSlider.onValueChange = [this] {
        audioProcessor.dryWetParam->setValueNotifyingHost((float)dryWetSlider.getValue());
    };
    addAndMakeVisible(dryWetSlider);
    
    // Dry/Wet Label
    dryWetLabel.setText("DRY/WET", juce::dontSendNotification);
    dryWetLabel.setFont(16.0f);
    dryWetLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    dryWetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dryWetLabel);
    
    // Update slider from parameter
    startTimerHz(30);
    
    setSize (400, 350);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // TB-303 Hintergrund
    juce::ColourGradient gradient(juce::Colours::darkslategrey, 0, 0,
                                 juce::Colours::black, 0, getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Rahmen
    g.setColour(juce::Colours::silver);
    g.drawRect(getLocalBounds(), 2);
    
    // TB-303 Styling
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(20, 80, getWidth() - 40, 200, 10);
    
    g.setColour(juce::Colours::silver);
    g.drawRoundedRectangle(20, 80, getWidth() - 40, 200, 10, 2);
}

void NewProjectAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Title
    titleLabel.setBounds(bounds.removeFromTop(60).reduced(10));
    
    // Dry/Wet Bereich
    auto knobArea = bounds.removeFromTop(220);
    knobArea = knobArea.reduced(50, 20);
    
    // Dry/Wet Slider (großer Knob)
    auto sliderBounds = knobArea.removeFromTop(180);
    dryWetSlider.setBounds(sliderBounds);
    
    // Dry/Wet Label
    dryWetLabel.setBounds(knobArea);
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // Slider mit Parameter synchronisieren
    if (audioProcessor.dryWetParam != nullptr) {
        dryWetSlider.setValue(audioProcessor.dryWetParam->get(), juce::dontSendNotification);
    }
}