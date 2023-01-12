/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class OneShotSamplerAudioProcessorEditor : public juce::AudioProcessorEditor {
  public:
    OneShotSamplerAudioProcessorEditor(OneShotSamplerAudioProcessor &);
    ~OneShotSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OneShotSamplerAudioProcessor &audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        OneShotSamplerAudioProcessorEditor)

    void buttonClicked(Button *button) override;
    TextButton sineWaveButton;
    TextButton sampleSelectButton;
    MidiKeyboardComponent keyboardComponent;

    OneShotSamplerAudioProcessor &processor;
};

