/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class TB303Distortion
{
private:
    float sampleRate = 44100.0f;
    float dryWet = 0.75f;
    float lowpassState = 0.0f;
    float highpassState = 0.0f;
    
public:
    void setSampleRate(float sr) { sampleRate = sr; }
    void setDryWet(float value) { dryWet = juce::jlimit(0.0f, 1.0f, value); }
    
    float processSample(float input)
    {
        float dry = input;
        
        // Pre-Filter für analoge Wärme
        lowpassState += (input - lowpassState) * 0.75f;
        float preFiltered = lowpassState;
        
        // TB-303 Asymmetrische Distortion
        float driven = preFiltered * 3.8f;
        float distorted;
        
        if (driven > 0.0f) {
            distorted = std::tanh(driven * 0.7f) * 1.4f;
        } else {
            distorted = juce::jmax(driven * 1.3f, -0.88f);
        }
        
        // Harmonics für Acid Character
        distorted += std::sin(driven * 2.1f) * 0.15f;
        distorted += std::sin(driven * 3.7f) * 0.08f;
        
        // Post High-pass
        highpassState += (distorted - highpassState) * 0.09f;
        distorted = distorted - highpassState;
        
        // Limiting
        distorted = std::tanh(distorted * 0.82f);
        
        return dry * (1.0f - dryWet) + distorted * dryWet * 0.85f;
    }
    
    void reset() {
        lowpassState = 0.0f;
        highpassState = 0.0f;
    }
};

class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    // Public access to parameters for GUI
    juce::AudioParameterFloat* dryWetParam;

private:
    //==============================================================================
    TB303Distortion tb303DistortionL, tb303DistortionR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};