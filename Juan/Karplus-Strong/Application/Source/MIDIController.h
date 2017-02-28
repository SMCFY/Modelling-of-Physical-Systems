/*
  ==============================================================================

    MIDIController.h
    Created: 20 Feb 2017 2:13:00pm
    Author:  Juan Gil

  ==============================================================================
*/

#ifndef MIDICONTROLLER_H_INCLUDED
#define MIDICONTROLLER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "KarplusStrong.h"

//==============================================================================
/*
*/
class MIDIController :
    public Component,
    public ComboBox::Listener,
    public MidiInputCallback,
    public MidiKeyboardStateListener
{
public:
    MIDIController() :
        lastInputIndex (0),
        midiComesFromOutside (false),
        keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
        addAndMakeVisible (midiInputListLabel);
        midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
        midiInputListLabel.setColour (Label::textColourId, Colours::white);
        midiInputListLabel.attachToComponent (&midiInputList, true);

        const StringArray midiInputs (MidiInput::getDevices());
        addAndMakeVisible (midiInputList);
        midiInputList.addItemList (midiInputs, 1);
        midiInputList.setTextWhenNothingSelected ("No MIDI Inputs Enabled :(");
        midiInputList.addListener (this);

        for (int ii = 0; ii < midiInputs.size(); ++ii) {
            if (deviceManager.isMidiInputEnabled(midiInputs[ii])) {
                setMidiInput (ii);
                break;
            }
        }

        if (midiInputList.getSelectedId() == 0)
            setMidiInput (0);

        addAndMakeVisible (keyboardComponent);
        keyboardComponent.setKeyWidth (30.0);
        keyboardComponent.setLowestVisibleKey (58);
        keyboardComponent.setKeyPressBaseOctave (5);
        keyboardState.addListener (this);

        karplus.freq = &freq;
        karplus.s_coeff = &s_coeff;
        karplus.p_coeff = &p_coeff;
        karplus.c_coeff = &c_coeff;
        karplus.position = &position;
    }

    ~MIDIController()
    {
    }

    //==========================================================================
    void paint (Graphics& g) override
    {
        Rectangle<int> localBounds = getLocalBounds();

        g.setColour (Colours::darkgrey);
        g.fillRoundedRectangle (localBounds.getX(),
                                localBounds.getY(),
                                localBounds.getWidth(),
                                localBounds.getHeight(),
                                5);
    }

    void resized() override
    {
        Rectangle<int> localBounds (getLocalBounds());

        midiInputList.setBounds (localBounds.removeFromTop (36).removeFromRight (getWidth() - 80).reduced(5));
        keyboardComponent.setBounds (localBounds.reduced(5));
        // midiMessagesBox.setBounds (localBounds);
    }

    //==========================================================================
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
        if (&midiInputList == comboBoxThatHasChanged)
            setMidiInput (midiInputList.getSelectedItemIndex());
    }

    void setMidiInput(int newInputIndex)
    {
        const StringArray midiInputs (MidiInput::getDevices());
        const String newMidiInput (midiInputs[newInputIndex]);
        deviceManager.removeMidiInputCallback (midiInputs[lastInputIndex], this);

        if (! deviceManager.isMidiInputEnabled (newMidiInput))
            deviceManager.setMidiInputEnabled (newMidiInput, true);

        deviceManager.addMidiInputCallback (newMidiInput, this);
        midiInputList.setSelectedId (newInputIndex + 1, dontSendNotification);

        lastInputIndex = newInputIndex;
    }

    //==========================================================================
    void sendMessage (const MidiMessage& message)
    {
        if (message.isNoteOn()) {
            freq = message.getMidiNoteInHertz(message.getNoteNumber());
            s_coeff = 0.5;
            p_coeff = 0.999;
            c_coeff = 0.5;
            position = 0.5;
            karplus.pluckString();
        }
    }

    class IncomingMessageCallback : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MIDIController* o, const MidiMessage& m) :
            owner (o), message (m)
        {
        }

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->sendMessage (message);
        }

        Component::SafePointer<MIDIController> owner;
        MidiMessage message;
    };

    void postMessageToList (const MidiMessage& message)
    {
        (new IncomingMessageCallback (this, message))->post();
    }

    //==========================================================================
    void handleIncomingMidiMessage (MidiInput* source,
                                    const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag (midiComesFromOutside, true);
        keyboardState.processNextMidiEvent (message);
        postMessageToList (message);
    }

    void handleNoteOn (MidiKeyboardState* source,
                       int midiChannel,
                       int midiNoteNumber,
                       float velocity) override
    {
        if (midiComesFromOutside) return;

        MidiMessage message (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (message);
    }

    void handleNoteOff (MidiKeyboardState* source,
                        int midiChannel,
                        int midiNoteNumber,
                        float velocity) override
    {
        if (midiComesFromOutside) return;

        MidiMessage message (MidiMessage::noteOff (midiChannel, midiNoteNumber));
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (message);
    }

    //==========================================================================
    float freq;
    float s_coeff;
    float p_coeff;
    float c_coeff;
    float position;

    KarplusStrong karplus;

private:
    AudioDeviceManager deviceManager;

    Label midiInputListLabel;
    ComboBox midiInputList;

    int lastInputIndex;
    bool midiComesFromOutside;

    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIController)
};


#endif  // MIDICONTROLLER_H_INCLUDED
