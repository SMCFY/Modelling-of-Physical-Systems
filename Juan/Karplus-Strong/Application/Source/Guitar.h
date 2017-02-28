/*
  ==============================================================================

    Guitar.h
    Created: 19 Feb 2017 11:21:42pm
    Author:  Juan Gil

  ==============================================================================
*/

#ifndef GUITAR_H_INCLUDED
#define GUITAR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Editor.h"
#include "MIDIController.h"

//==============================================================================
/*
*/
class Guitar :
    public AudioAppComponent,
    public TextButton::Listener
{
public:
    Guitar() :
        string1 (300),
        string2 (500)
    {
        addAndMakeVisible (pluckAllButton);
        pluckAllButton.setButtonText ("Pluck all!");
        pluckAllButton.addListener (this);

        addAndMakeVisible (string1);
        addAndMakeVisible (string2);
        addAndMakeVisible (midiControl);

        setAudioChannels (0, 2);
    }

    ~Guitar()
    {
        shutdownAudio();
    }

    //==========================================================================
    void resized() override
    {
        Rectangle<int> localBounds = getLocalBounds();
        pluckAllButton.setBounds(localBounds.removeFromTop (35).reduced (5));

        int heightChild = localBounds.getHeight() / 3;
        string1.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
        string2.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
        midiControl.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
    }

    //==========================================================================
    void prepareToPlay (int samplesPerBlockExpected,
                        double sampleRate) override
    {
        string1.karplus.prepareToPlay (samplesPerBlockExpected, sampleRate);
        string2.karplus.prepareToPlay (samplesPerBlockExpected, sampleRate);
        midiControl.karplus.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void releaseResources() override
    {
        string1.karplus.releaseResources();
        string2.karplus.releaseResources();
        midiControl.karplus.releaseResources();
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        string1.karplus.getNextAudioBlock (bufferToFill);
        string2.karplus.getNextAudioBlock (bufferToFill);
        midiControl.karplus.getNextAudioBlock (bufferToFill);
    }

    void buttonClicked (Button* button) override
    {
        if (&pluckAllButton == button) {
            string1.karplus.pluckString (2);
            string2.karplus.pluckString (2);
        }
    }

    //==========================================================================

private:
    TextButton pluckAllButton;
    Editor string1;
    Editor string2;
    MIDIController midiControl;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Guitar)
};


#endif  // GUITAR_H_INCLUDED
