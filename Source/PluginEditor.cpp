/*
  ==============================================================================

    TB-303 Distortion Plugin Editor with Acid Smiley Morphing

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
// AcidSmileyComponent Implementation
//==============================================================================

class AcidSmileyComponent : public juce::Component, private juce::Timer
{
public:
    AcidSmileyComponent()
    {
        loadSpriteSheet();
        updateCurrentFrame();
        startTimerHz(60); // 60 FPS for smooth animation
    }
    
    ~AcidSmileyComponent() override
    {
        stopTimer();
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        
        if (currentFrame.isValid())
        {
            g.drawImage(currentFrame, bounds, juce::RectanglePlacement::centred);
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
        updateCurrentFrame();
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
            updateCurrentFrame();
            repaint();
        }
    }
    
    void loadSpriteSheet()
    {
        // Load sprite sheet from embedded binary resources
        int resourceSize = 0;
        const char* resourceData = BinaryData::getNamedResource("smiley_mega_sprite_sheet_png", resourceSize);
        
        if (resourceData != nullptr && resourceSize > 0)
        {
            spriteSheet = juce::ImageFileFormat::loadFrom(resourceData, resourceSize);
        }
        
        // Fallback: try to load from file system if binary data not available
        if (!spriteSheet.isValid())
        {
            auto resourcesDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                               .getParentDirectory().getChildFile("Resources");
            
            auto spriteFile = resourcesDir.getChildFile("smiley_mega_sprite_sheet.png");
            
            if (spriteFile.existsAsFile())
            {
                spriteSheet = juce::ImageFileFormat::loadFrom(spriteFile);
            }
            else
            {
                // Try relative path as last resort
                auto projectDir = juce::File(__FILE__).getParentDirectory().getParentDirectory();
                auto resourcesPath = projectDir.getChildFile("Resources");
                auto spritePath = resourcesPath.getChildFile("smiley_mega_sprite_sheet.png");
                
                if (spritePath.existsAsFile())
                    spriteSheet = juce::ImageFileFormat::loadFrom(spritePath);
            }
        }
        
        // If sprite sheet loaded, calculate frame dimensions
        if (spriteSheet.isValid())
        {
            frameWidth = spriteSheet.getWidth() / numFrames;  // Horizontal layout
            frameHeight = spriteSheet.getHeight();           // Full height
        }
    }
    
    void updateCurrentFrame()
    {
        if (!spriteSheet.isValid())
            return;
            
        // Calculate which frame to show based on dry/wet value
        int frameIndex = (int)(dryWetValue * (numFrames - 1));
        frameIndex = juce::jlimit(0, numFrames - 1, frameIndex);
        
        // Calculate position in horizontal sprite sheet
        int frameX = frameIndex * frameWidth;
        int frameY = 0;  // All frames in one row
        
        // Extract current frame from sprite sheet
        currentFrame = spriteSheet.getClippedImage(juce::Rectangle<int>(frameX, frameY, frameWidth, frameHeight));
    }
    
    juce::Image spriteSheet;
    juce::Image currentFrame;
    
    static const int numFrames = 14;
    static const int framesPerRow = 14;  // All frames in one horizontal row
    int frameWidth = 0;
    int frameHeight = 0;
    
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
    // No title label needed
    
    // Acid Smiley Component
    acidSmiley = std::make_unique<AcidSmileyComponent>();
    acidSmiley->onValueChange = [this](float value) {
        audioProcessor.dryWetParam->setValueNotifyingHost(value);
    };
    addAndMakeVisible(*acidSmiley);
    
    // Info Button (round question mark in bottom right)
    infoButton.setButtonText("?");  // Show the question mark directly
    infoButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xfff3d43c));
    infoButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    infoButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xfff3d43c).brighter(0.2f));
    infoButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    infoButton.onClick = [this] { showInfoPanel(); };
    addAndMakeVisible(infoButton);
    
    // Info Text Area (initially hidden)
    infoTextArea.setMultiLine(true);
    infoTextArea.setReadOnly(true);
    infoTextArea.setScrollbarsShown(true);
    infoTextArea.setCaretVisible(false);
    infoTextArea.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black.withAlpha(0.95f));
    infoTextArea.setColour(juce::TextEditor::textColourId, juce::Colours::lightgrey);
    infoTextArea.setFont(juce::Font("Monaco", 11.0f, juce::Font::plain));
    
    // Set the info text content
    infoTextArea.setText(R"(TB-303 Distortion Plugin 
Author: Frank Hahn aka Giana Brotherz
Support me on Patreon: https://www.patreon.com/c/GianaBrotherz
Release Date: October 4, 2025
Copyright: All rights reserved

Overview
The TB-303 Distortion Plugin is a specialized audio effect designed to simulate and enhance the characteristic aggressive distortion sound of Roland TB-303 basslines. It is aimed at Acid House, Techno, and electronic music producers seeking the authentic acid sound.

Key Features

Asymmetric Tube Distortion
- Positive Half-Wave: Smooth tanh saturation for warmth and compression
- Negative Half-Wave: Hard clipping for the distinctive TB-303 bite
- Multi-Stage Processing: 3-stage distortion for maximum control

Dynamic Mid-Boost EQ
- Resonance Simulation: Automatic boost around 1.5 kHz (TB-303 sweet spot)
- Envelope Following: Dynamic gain adjustment depending on input signal
- Acid Character: Emphasizes the typical TB-303 frequency range

2x Oversampling
- Anti-Aliasing: Prevents digital artifacts at aggressive distortion levels
- Studio Quality: Clean harmonics without aliasing issues
- Optimized Performance: Efficient filters for minimal CPU load

Extended Parameters
- Dry/Wet Mix: Default at 85% wet for strong effect
- Aggression: Set at 80% for maximum TB-303 bite
- Drive: 4.5x gain for intense saturation
- Warmth: Analog warmth simulation

Under the Hood - Technical Details

Signal Processing Chain:
1. Envelope Follower → Dynamic adjustment to signal loudness
2. Dynamic Mid-EQ → Resonance boost at 1.5 kHz
3. Pre-Filtering → Lowpass filter for analog warmth
4. Multi-Stage Distortion → 3-stage aggressive distortion
5. Harmonic Enhancement → 2nd, 3rd, 5th harmonics
6. Frequency-Dependent Saturation → Additional overtone enrichment
7. Post High-Pass → Removes mud in the bass range
8. Multi-Stage Limiting → Controlled output loudness

Oversampling Architecture:
- Linear interpolation for 2x upsampling
- Anti-aliasing filter (2-pole lowpass)
- Decimation with lowpass filtering
- CPU-optimized using simple IIR filters

Harmonic Enrichment:
// 2nd Harmonic (Warmth)  
sin(driven * 2.1f) * warmth * 0.2f  

// 3rd Harmonic (Musicality)  
sin(driven * 3.7f) * warmth * 0.15f  

// 5th Harmonic (Aggression)  
sin(driven * 5.3f) * aggression * 0.1f  

Bit-Crushing (at high Aggression):
- Subtle quantization for digital roughness
- Dynamically activated above 70% aggression
- Variable bit depth (64-32 bit) depending on setting

Use Cases

Primary Target Group:
- Acid House Producers seeking authentic TB-303 sound
- Techno Artists focusing on aggressive bassline shaping
- Electronic Musicians looking for vintage analog simulation
- Sound Designers who need characterful distortion

Typical Applications:
- Enhancing and shaping TB-303 basslines
- Adding aggression to analog synthesizers
- Giving lead sounds an acid character
- Adding grit to drums, especially kick and snare

Technical Specifications:
- Format: VST3 (macOS bundle)
- Bit Depth: 32-bit float processing
- Sample Rate: 44.1 kHz to 192 kHz
- Latency: Minimal due to oversampling
- CPU: Optimized for real-time performance

Sound Characteristics:
- Warm and aggressive with tube-like warmth and digital bite
- Dynamic response to playing strength and transients
- Authentic TB-303 resonance character
- Modern studio-grade quality without artifacts

The plugin combines the nostalgia of the classic TB-303 sound with modern DSP technology for professional music production.

License:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This project uses the JUCE framework (https://juce.com), which is
licensed under the terms of the GNU General Public License v3.)", false);
    
    infoTextArea.setVisible(false);
    addAndMakeVisible(infoTextArea);
    
    // Close Button (X in top right of info panel)
    closeButton.setButtonText("");  // Empty text, we'll draw it ourselves
    closeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    closeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    closeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    closeButton.onClick = [this] { hideInfoPanel(); };
    closeButton.setVisible(false);
    addAndMakeVisible(closeButton);
    
    // Update from parameter
    startTimerHz(30);
    
    setSize(400, 400);  // Square aspect ratio
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Simple black background
    g.fillAll(juce::Colours::black);
    
    // Custom paint for round close button
    if (showingInfo && closeButton.isVisible()) {
        auto buttonBounds = closeButton.getBounds().toFloat();
        
        // Round background
        g.setColour(closeButton.isMouseOver() ? juce::Colours::red : juce::Colours::red.withAlpha(0.8f));
        g.fillEllipse(buttonBounds);
        
        // Border
        g.setColour(juce::Colours::darkred);
        g.drawEllipse(buttonBounds, 2.0f);
        
        // X symbol (proper X)
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(16.0f, juce::Font::bold));
        g.drawText("X", buttonBounds, juce::Justification::centred);
    }
}

void NewProjectAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    if (!showingInfo) {
        // Normal mode: Smiley takes full area
        int size = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto centeredBounds = juce::Rectangle<int>(
            (bounds.getWidth() - size) / 2,
            (bounds.getHeight() - size) / 2,
            size, size
        );
        acidSmiley->setBounds(centeredBounds);
        
        // Info button in bottom right corner (round)
        infoButton.setBounds(bounds.getWidth() - 40, bounds.getHeight() - 40, 30, 30);
    } else {
        // Info mode: Text area takes most space
        infoTextArea.setBounds(bounds.reduced(20).withTrimmedTop(30));
        
        // Close button in top right
        closeButton.setBounds(bounds.getWidth() - 35, 10, 25, 25);
    }
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    // Sync smiley with parameter
    if (audioProcessor.dryWetParam != nullptr && acidSmiley) 
    {
        acidSmiley->setDryWetValue(audioProcessor.dryWetParam->get());
    }
}

void NewProjectAudioProcessorEditor::showInfoPanel()
{
    showingInfo = true;
    
    // Hide smiley interface
    acidSmiley->setVisible(false);
    infoButton.setVisible(false);
    
    // Show info interface
    infoTextArea.setVisible(true);
    closeButton.setVisible(true);
    
    resized();
    repaint();
}

void NewProjectAudioProcessorEditor::hideInfoPanel()
{
    showingInfo = false;
    
    // Hide info interface
    infoTextArea.setVisible(false);
    closeButton.setVisible(false);
    
    // Show smiley interface
    acidSmiley->setVisible(true);
    infoButton.setVisible(true);
    
    resized();
    repaint();
}