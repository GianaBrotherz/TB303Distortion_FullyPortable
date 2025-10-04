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

class AcidSmileyComponent : public juce::Component, private juce::Timer
{
public:
    AcidSmileyComponent()
    {
        // Load smiley images from Resources folder
        auto resourcesDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                           .getParentDirectory().getChildFile("Resources");
        
        auto dryFile = resourcesDir.getChildFile("smiley_dry.png");
        auto wetFile = resourcesDir.getChildFile("smiley_wet.png");
        
        if (dryFile.existsAsFile())
            smileyDry = juce::ImageFileFormat::loadFrom(dryFile);
        if (wetFile.existsAsFile())
            smileyWet = juce::ImageFileFormat::loadFrom(wetFile);
        
        // If files don't exist, try relative path
        if (!smileyDry.isValid() || !smileyWet.isValid())
        {
            // Fallback: try to load from project Resources
            auto projectDir = juce::File(__FILE__).getParentDirectory().getParentDirectory();
            auto resourcesPath = projectDir.getChildFile("Resources");
            
            auto dryPath = resourcesPath.getChildFile("smiley_dry.png");
            auto wetPath = resourcesPath.getChildFile("smiley_wet.png");
            
            if (dryPath.existsAsFile())
                smileyDry = juce::ImageFileFormat::loadFrom(dryPath);
            if (wetPath.existsAsFile())
                smileyWet = juce::ImageFileFormat::loadFrom(wetPath);
        }
        
        updateMorphedImage();
        startTimerHz(60); // 60 FPS for smooth animation
    }
    
    ~AcidSmileyComponent() override
    {
        stopTimer();
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        
        if (morphedImage.isValid())
        {
            g.drawImage(morphedImage, bounds, juce::RectanglePlacement::centred);
        }
        else
        {
            // Fallback: draw programmatic smiley
            drawProgrammaticSmiley(g, bounds);
        }
    }
    
    void drawProgrammaticSmiley(juce::Graphics& g, juce::Rectangle<float> bounds)
    {
        auto center = bounds.getCentre();
        auto radius = bounds.getWidth() * 0.4f;
        
        // Face circle
        g.setColour(juce::Colours::yellow);
        g.fillEllipse(bounds.reduced(20));
        g.setColour(juce::Colours::black);
        g.drawEllipse(bounds.reduced(20), 3);
        
        // Eyes change based on dry/wet
        float eyeSize = juce::jmap(dryWetValue, 0.0f, 1.0f, 16.0f, 8.0f);
        float eyeSlant = juce::jmap(dryWetValue, 0.0f, 1.0f, 0.0f, 0.3f);
        
        // Left eye
        juce::Rectangle<float> leftEye(center.x - radius * 0.3f - eyeSize/2, 
                                      center.y - radius * 0.3f - eyeSize/2 + eyeSlant * 20, 
                                      eyeSize, eyeSize);
        g.fillEllipse(leftEye);
        
        // Right eye  
        juce::Rectangle<float> rightEye(center.x + radius * 0.3f - eyeSize/2, 
                                       center.y - radius * 0.3f - eyeSize/2 - eyeSlant * 20, 
                                       eyeSize, eyeSize);
        g.fillEllipse(rightEye);
        
        // Mouth morphs from smile to frown
        juce::Path mouth;
        float mouthY = center.y + radius * 0.2f;
        float mouthCurve = juce::jmap(dryWetValue, 0.0f, 1.0f, 25.0f, -25.0f);
        
        mouth.startNewSubPath(center.x - radius * 0.4f, mouthY);
        mouth.quadraticTo(center.x, mouthY + mouthCurve, center.x + radius * 0.4f, mouthY);
        
        g.strokePath(mouth, juce::PathStrokeType(5.0f));
        
        // Add distortion effect at high wet values
        if (dryWetValue > 0.7f)
        {
            float distortionAmount = (dryWetValue - 0.7f) / 0.3f;
            
            // Jagged mouth
            juce::Path jaggedMouth;
            jaggedMouth.startNewSubPath(center.x - radius * 0.4f, mouthY + 10);
            for (int i = 0; i < 8; ++i)
            {
                float x = center.x - radius * 0.4f + i * (radius * 0.8f / 7);
                float y = mouthY + 10 + (i % 2 == 0 ? 1 : -1) * distortionAmount * 8;
                jaggedMouth.lineTo(x, y);
            }
            g.strokePath(jaggedMouth, juce::PathStrokeType(3.0f));
        }
    }
    
    void resized() override
    {
        updateMorphedImage();
    }
    
    void mouseDown(const juce::MouseEvent& event) override
    {
        isDragging = true;
        lastMousePos = event.getPosition();
    }
    
    void mouseDrag(const juce::MouseEvent& event) override
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
    
    void setDryWetValue(float value)
    {
        targetValue = juce::jlimit(0.0f, 1.0f, value);
    }
    
    float getDryWetValue() const { return dryWetValue; }
    
    std::function<void(float)> onValueChange;
    
private:
    void timerCallback() override
    {
        // Smooth animation towards target value
        float speed = 0.15f;
        dryWetValue += (targetValue - dryWetValue) * speed;
        
        if (std::abs(dryWetValue - targetValue) > 0.001f)
        {
            updateMorphedImage();
            repaint();
        }
    }
    
    void updateMorphedImage()
    {
        if (!smileyDry.isValid() || !smileyWet.isValid())
            return;
            
        auto bounds = getLocalBounds();
        if (bounds.isEmpty())
            return;
        
        // Create morphed image by blending dry and wet smileys
        morphedImage = juce::Image(juce::Image::ARGB, bounds.getWidth(), bounds.getHeight(), true);
        
        juce::Graphics g(morphedImage);
        g.fillAll(juce::Colours::transparentBlack);
        
        // Draw dry smiley with decreasing opacity
        float dryOpacity = 1.0f - dryWetValue;
        g.setOpacity(dryOpacity);
        g.drawImage(smileyDry, bounds.toFloat(), juce::RectanglePlacement::centred);
        
        // Draw wet smiley with increasing opacity
        float wetOpacity = dryWetValue;
        g.setOpacity(wetOpacity);
        g.drawImage(smileyWet, bounds.toFloat(), juce::RectanglePlacement::centred);
    }
    
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
// NewProjectAudioProcessorEditor Implementation
//==============================================================================

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Title Label
    titleLabel.setText("TB-303 ACID DISTORTION", juce::dontSendNotification);
    titleLabel.setFont(24.0f);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Acid Smiley Component
    acidSmiley = std::make_unique<AcidSmileyComponent>();
    acidSmiley->onValueChange = [this](float value) {
        audioProcessor.dryWetParam->setValueNotifyingHost(value);
    };
    addAndMakeVisible(*acidSmiley);
    
    // Update from parameter
    startTimerHz(30);
    
    setSize(400, 450);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Acid House Background
    juce::ColourGradient gradient(juce::Colours::purple.darker(), 0, 0,
                                 juce::Colours::black, 0, getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Neon border
    g.setColour(juce::Colours::yellow);
    g.drawRect(getLocalBounds(), 4);
    
    // Inner glow effect
    g.setColour(juce::Colours::yellow.withAlpha(0.3f));
    g.drawRect(getLocalBounds().reduced(4), 2);
    g.setColour(juce::Colours::yellow.withAlpha(0.1f));
    g.drawRect(getLocalBounds().reduced(8), 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    // Title
    titleLabel.setBounds(bounds.removeFromTop(50));
    
    // Acid Smiley (main area)
    bounds.reduce(20, 0);
    acidSmiley->setBounds(bounds);
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // Sync smiley with parameter
    if (audioProcessor.dryWetParam != nullptr && acidSmiley) 
    {
        acidSmiley->setDryWetValue(audioProcessor.dryWetParam->get());
    }
}