/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
							   public SliderListener
{
public:
    //==============================================================================
	MainContentComponent()
	  : phase(0.0f),
		phaseDelta(0.0f),
		frequency(5000.0f),
		amplitude(0.2f),
		sampleRate(0.0),
		expectedSamplesPerBlock(0)
    {
        setSize (800, 600);
		pitch = new Slider("Pitch");
		roll = new Slider("Roll");
		yaw = new Slider("Yaw");
		volume = new Slider("Volume");
		lforate = new Slider("Pan");
		pan = new Slider("LFO rate");

		addAndMakeVisible(pitch);
		addAndMakeVisible(roll);
		addAndMakeVisible(yaw);
		addAndMakeVisible(volume);
		addAndMakeVisible(pan);
		addAndMakeVisible(lforate);

		pitch->setBounds(20, 20, 260, 20);
		roll->setBounds(20, 50, 260, 20);
		yaw->setBounds(20, 80, 260, 20);
		volume->setBounds(20, 110, 260, 20);
		pan->setBounds(20, 140, 260, 20);
		lforate->setBounds(20, 170, 260, 20);

		pitch->setRange(20, 10000, 1);

		pitch->addListener(this);
		roll->addListener(this);
		yaw->addListener(this);
		volume->addListener(this);
		pan->addListener(this);
		lforate->addListener(this);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
		deleteAllChildren();
    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double newSampleRate) override
    {

		sampleRate = newSampleRate;
		expectedSamplesPerBlock = samplesPerBlockExpected;

        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

		bufferToFill.clearActiveBufferRegion();
		const float originalPhase = phase;

		for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
		{
			phase = originalPhase;

			float* const channelData = bufferToFill.buffer->getWritePointer(chan, bufferToFill.startSample);

			for (int i = 0; i < bufferToFill.numSamples; ++i)
			{
				channelData[i] = amplitude * std::sin(phase);

				// increment the phase step for the next sample
				phase = std::fmod(phase + phaseDelta, float_Pi * 2.0f);
			}
		}

        // For more details, see the help for AudioProcessor::getNextAudioBlock()
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(Colours::black);

		const float Y = getHeight() * 0.8f;
		const float radius = amplitude * 50.0f;

		// Draw an ellipse based on the mouse position and audio volume
		g.setColour(Colours::lightgreen);
		g.fillEllipse(lastMousePosition.x - radius / 2.0f,
			lastMousePosition.y - radius / 2.0f,
			radius, radius);

		// draw a representative sinewave
		Path wavePath;
		wavePath.startNewSubPath(0, Y);

		for (float x = 1.0f; x < getWidth(); ++x)
			wavePath.lineTo(x, Y + amplitude * getHeight()
			* std::sin(x * frequency * 0.0001f));

		g.setColour(Colours::grey);
		g.strokePath(wavePath, PathStrokeType(2.0f));


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

	void sliderValueChanged(Slider* slider){
		if (slider == pitch){
			frequency = slider->getValue();
		}
		if (slider == volume){
			amplitude = slider->getValue() / 100;
		}

		phaseDelta = 2.0f * float_Pi * frequency / sampleRate;

		repaint();
	}


private:
    //==============================================================================

    // Your private member variables go here...
	float phase;
	float phaseDelta;
	float frequency;
	float amplitude;
	Slider* pitch;
	Slider* roll;
	Slider* yaw;
	Slider* volume;
	Slider* pan;
	Slider* lforate;

	double sampleRate;
	int expectedSamplesPerBlock;
	Point<float> lastMousePosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
