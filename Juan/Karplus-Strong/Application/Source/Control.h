/*
  ==============================================================================

    Control.h
    Created: 19 Feb 2017 9:40:57pm
    Author:  Juan Gil

  ==============================================================================
*/

#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Control :
    public Component,
    private Slider::Listener
{
public:
    Control (String name, float min, float max, float def, bool log = false)
    {
        addAndMakeVisible (myLabel);
        myLabel.setText (String(name), dontSendNotification);
        myLabel.setColour (Label::textColourId, Colours::white);

        addAndMakeVisible (mySlider);
        mySlider.addListener (this);
        mySlider.setRange (min, max);
        mySlider.setValue (def);
        mySlider.setTextBoxStyle (Slider::TextBoxLeft,
                                  false,
                                  100,
                                  mySlider.getTextBoxHeight());
        if (log) {
            mySlider.setSkewFactorFromMidPoint (sqrt (min * max));
        }
    }

    ~Control()
    {
    }

    //==========================================================================
    void resized() override
    {
        Rectangle<int> localBounds = getLocalBounds();
        myLabel.setBounds (localBounds.removeFromLeft (20));
        mySlider.setBounds (localBounds);
    }

    //==========================================================================
    float* registerValue()
    {
        return &myValue;
    }

    //==========================================================================
    Label myLabel;
    Slider mySlider;

private:
    float myValue;

    void sliderValueChanged (Slider* slider) override
    {
        if (&mySlider == slider) {
            myValue = mySlider.getValue();
        }
    }

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Control)
};


#endif  // CONTROL_H_INCLUDED
