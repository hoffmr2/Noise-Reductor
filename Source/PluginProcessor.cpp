/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "fftw3.h"


fftw_complex* in; 
fftw_complex* out; 
fftw_plan fftwPlan;
//==============================================================================
NoiseReductorAudioProcessor::NoiseReductorAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                       )
{
    in = new fftw_complex[512];
    out = new fftw_complex[512];
    fftwPlan = fftw_plan_dft_1d(512, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
}

NoiseReductorAudioProcessor::~NoiseReductorAudioProcessor()
{
}

//==============================================================================
const String NoiseReductorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseReductorAudioProcessor::acceptsMidi() const
{
    return false;
}

bool NoiseReductorAudioProcessor::producesMidi() const
{
    return false;
}

bool NoiseReductorAudioProcessor::isMidiEffect() const
{
    return false;
}

double NoiseReductorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseReductorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoiseReductorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseReductorAudioProcessor::setCurrentProgram (int index)
{
}

const String NoiseReductorAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoiseReductorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NoiseReductorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NoiseReductorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseReductorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        || layouts.getMainInputChannelSet() != AudioChannelSet::stereo())
        return false;
    else
        return true;
}
#endif

void NoiseReductorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }

    fftw_execute(fftwPlan);
}

//==============================================================================
bool NoiseReductorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NoiseReductorAudioProcessor::createEditor()
{
    return new NoiseReductorAudioProcessorEditor (*this);
}

//==============================================================================
void NoiseReductorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NoiseReductorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseReductorAudioProcessor();
}
