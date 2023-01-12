/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
OneShotSamplerAudioProcessorEditor::OneShotSamplerAudioProcessorEditor(
    OneShotSamplerAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      keyboardComponent(p.getMidiKeyboardState(),
                        MidiKeyboardComponent::horizontalKeyboard) {
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    sineWaveButton.setButtonText("Sine Wave");
    sineWaveButton.addListener(this);
    sampleSelectButton.setButtonText("Sample Select");
    sampleSelectButton.addListener(this);

    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(sampleSelectButton);
    addAndMakeVisible(keyboardComponent);

    setSize(800, 600);
}

OneShotSamplerAudioProcessorEditor::~OneShotSamplerAudioProcessorEditor() {}

//==============================================================================
void OneShotSamplerAudioProcessorEditor::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(),
                     juce::Justification::centred, 1);
}

void OneShotSamplerAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sineWaveButton.setBoundsRelative(0.2, 0.2, 0.2, 0.2);
    sampleSelectButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
    keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSamplerAudioProcessorEditor::buttonClicked(Button *button) {
    if (button == &sineWaveButton) {
        processor.loadSineWave();
    } else if (button == &sampleSelectButton) {
        processor.loadSampleFile();
    }
}

