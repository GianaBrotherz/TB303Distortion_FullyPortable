/*
  ==============================================================================

    TB-303 Distortion Plugin Editor with Acid Smiley Morphing

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// AcidSmileyComponent Implementation
//==============================================================================

AcidSmileyComponent::AcidSmileyComponent()
{
    // Load smiley images from binary data embedded in the plugin
    auto dryData = juce::ImageCache::getFromMemory(BinaryData::smiley_dry_png, BinaryData::smiley_dry_pngSize);
    auto wetData = juce::ImageCache::getFromMemory(BinaryData::smiley_wet_png, BinaryData::smiley_wet_pngSize);
    
    if (dryData.isValid()) smileyDry = dryData;
    if (wetData.isValid()) smileyWet = wetData;
    
    // Create initial morphed image
    updateMorphedImage();
    
    // Start animation timer
    startTimerHz(60); // 60 FPS for smooth animation
}

AcidSmileyComponent::~AcidSmileyComponent()
{
    stopTimer();
}

void AcidSmileyComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Draw morphed smiley
    if (morphedImage.isValid())
    {
        g.drawImage(morphedImage, bounds, juce::RectanglePlacement::centred);
    }
    else
    {
        // Fallback: draw a simple circle if images don't load
        g.setColour(juce::Colours::yellow);
        g.fillEllipse(bounds.reduced(10));
        g.setColour(juce::Colours::black);
        g.drawEllipse(bounds.reduced(10), 3);
        
        // Draw face based on dry/wet value
        auto center = bounds.getCentre();
        auto faceRadius = bounds.getWidth() * 0.3f;
        
        // Eyes
        g.fillEllipse(center.x - faceRadius * 0.3f - 8, center.y - faceRadius * 0.3f - 8, 16, 16);
        g.fillEllipse(center.x + faceRadius * 0.3f - 8, center.y - faceRadius * 0.3f - 8, 16, 16);
        
        // Mouth changes with dry/wet
        juce::Path mouth;
        float mouthY = center.y + faceRadius * 0.2f;
        float mouthCurve = juce::jmap(dryWetValue, 0.0f, 1.0f, 20.0f, -20.0f); // Happy to sad
        
        mouth.startNewSubPath(center.x - faceRadius * 0.4f, mouthY);
        mouth.quadraticTo(center.x, mouthY + mouthCurve, center.x + faceRadius * 0.4f, mouthY);
        
        g.strokePath(mouth, juce::PathStrokeType(4.0f));
    }
}

void AcidSmileyComponent::resized()
{
    updateMorphedImage();
}

void AcidSmileyComponent::mouseDown(const juce::MouseEvent& event)
{
    isDragging = true;
    lastMousePos = event.getPosition();
}

void AcidSmileyComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (isDragging)
    {
        auto deltaY = lastMousePos.y - event.getPosition().y;
        auto sensitivity = 0.005f;
        
        targetValue = juce::jlimit(0.0f, 1.0f, targetValue + deltaY * sensitivity);
        lastMousePos = event.getPosition();
        
        if (onValueChange)
            onValueChange(targetValue);
    }
}

void AcidSmileyComponent::setDryWetValue(float value)
{
    targetValue = juce::jlimit(0.0f, 1.0f, value);
}

void AcidSmileyComponent::timerCallback()
{
    // Smooth animation towards target value
    float speed = 0.1f;
    dryWetValue += (targetValue - dryWetValue) * speed;
    
    if (std::abs(dryWetValue - targetValue) > 0.001f)
    {
        updateMorphedImage();
        repaint();
    }
}

void AcidSmileyComponent::updateMorphedImage()
{
    if (!smileyDry.isValid() || !smileyWet.isValid())
        return;
        
    auto bounds = getLocalBounds();
    if (bounds.isEmpty())
        return;
    
    // Create morphed image by blending dry and wet smileys
    morphedImage = juce::Image(juce::Image::ARGB, bounds.getWidth(), bounds.getHeight(), true);
    
    juce::Graphics g(morphedImage);
    
    // Draw dry smiley with decreasing opacity
    float dryOpacity = 1.0f - dryWetValue;
    g.setOpacity(dryOpacity);
    g.drawImage(smileyDry, bounds.toFloat(), juce::RectanglePlacement::centred);
    
    // Draw wet smiley with increasing opacity
    float wetOpacity = dryWetValue;
    g.setOpacity(wetOpacity);
    g.drawImage(smileyWet, bounds.toFloat(), juce::RectanglePlacement::centred);
}

//==============================================================================
// NewProjectAudioProcessorEditor Implementation
//==============================================================================

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Title Label
    titleLabel.setText("TB-303 ACID DISTORTION", juce::dontSendNotification);
    titleLabel.setFont(24.0f);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Acid Smiley Component
    acidSmiley.onValueChange = [this](float value) {
        audioProcessor.dryWetParam->setValueNotifyingHost(value);
    };
    addAndMakeVisible(acidSmiley);
    
    // Update from parameter
    startTimerHz(30);
    
    setSize(400, 400);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Acid House Background
    juce::ColourGradient gradient(juce::Colours::darkmagenta, 0, 0,
                                 juce::Colours::black, 0, getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Rahmen
    g.setColour(juce::Colours::yellow);
    g.drawRect(getLocalBounds(), 3);
    
    // Acid vibes
    g.setColour(juce::Colours::yellow.withAlpha(0.1f));
    for (int i = 0; i < 5; ++i)
    {
        g.drawRect(getLocalBounds().reduced(i * 8), 1);
    }
}

void NewProjectAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Title
    titleLabel.setBounds(bounds.removeFromTop(50).reduced(10));
    
    // Acid Smiley (main area)
    auto smileyArea = bounds.reduced(40);
    acidSmiley.setBounds(smileyArea);
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // Sync smiley with parameter
    if (audioProcessor.dryWetParam != nullptr) 
    {
        acidSmiley.setDryWetValue(audioProcessor.dryWetParam->get());
    }
}