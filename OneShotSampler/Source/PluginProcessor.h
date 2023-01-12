/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 */
class OneShotSamplerAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    ,
                                     public juce::AudioProcessorARAExtension
#endif
{
  public:
    //==============================================================================
    OneShotSamplerAudioProcessor();
    ~OneShotSamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    // シンセサイザーのセットアップ処理
    // サンプル音源のロード、SamplerSoundオブジェクトの生成とセット、SamplerVoiceオブジェクトの生成とセット
    void setupSampler(AudioFormatReader& newReader);

    // サンプル音源としてバイナリリソースから正弦波を読み込んでセットアップ処理を実行
    void loadSineWave();

    // MidiKeyboardState オブジェクトの参照を返す
    MidiKeyboardState& getMidiKeyboardState() { return keyboardState; };

  private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OneShotSamplerAudioProcessor)
    Synthesiser synth;
    MidiKeyboardState keyboardState;
    bool isChaging; // processBlock関数内の処理をスキップするかどうか
};

