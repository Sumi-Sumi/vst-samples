/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
OneShotSamplerAudioProcessor::OneShotSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
}

OneShotSamplerAudioProcessor::~OneShotSamplerAudioProcessor() {}

OneShotSamplerAudioProcessor::setupSampler(AudioFormatReader &newReader) {
    isChaging = true;

    // Synthesiserオブジェクトに登録されたSamplerVoiceオブジェクトとSamplerSoundオブジェクトをすべて破棄
    synth.clearSounds();
    synth.clearVoices();

    // 読み込んだサンプル音源を割り当てるノート番号の範囲を定義
    BigInteger allNotes;
    allNotes.setRange(0, 128, true);

    // SamplerSoundをSynthesiserオブジェクトに追加
    synth.addSound(
        new SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0))

        // SamplerVoiceオブジェクトをSynthesiserオブジェクトに追加
        for (int i = 0; i < 128; i++){synth.addVoice(new SamplerVoice())}

    isChaging false;
}

//==============================================================================
const juce::String OneShotSamplerAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool OneShotSamplerAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool OneShotSamplerAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool OneShotSamplerAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double OneShotSamplerAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int OneShotSamplerAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs, so this should be at least 1, even if you're not
              // really implementing programs.
}

int OneShotSamplerAudioProcessor::getCurrentProgram() { return 0; }

void OneShotSamplerAudioProcessor::setCurrentProgram(int index) {}

const juce::String OneShotSamplerAudioProcessor::getProgramName(int index) {
    return {};
}

void OneShotSamplerAudioProcessor::changeProgramName(
    int index, const juce::String &newName) {}

//==============================================================================
void OneShotSamplerAudioProcessor::prepareToPlay(double sampleRate,
                                                 int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    keyboardState.reset();
}

void OneShotSamplerAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.allNotesOff(0);
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneShotSamplerAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void OneShotSamplerAudioProcessor::processBlock(
    juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    // juce::ScopedNoDenormals noDenormals;
    // auto totalNumInputChannels = getTotalNumInputChannels();
    // auto totalNumOutputChannels = getTotalNumOutputChannels();
    if (isChaging) {
        return;
    }
    ScopedNoDenormals noDenormals;
    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),
                                        true);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to
    // keep this code if your algorithm always overwrites all the output
    // channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto *channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool OneShotSamplerAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *OneShotSamplerAudioProcessor::createEditor() {
    return new OneShotSamplerAudioProcessorEditor(*this);
}

//==============================================================================
void OneShotSamplerAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OneShotSamplerAudioProcessor::setStateInformation(const void *data,
                                                       int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new OneShotSamplerAudioProcessor();
}

