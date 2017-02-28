/*
  ==============================================================================

    Editor.h
    Created: 20 Feb 2017 9:18:54am
    Author:  Juan Gil

  ==============================================================================
*/

#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Control.h"
#include "KarplusStrong.h"

//==============================================================================
/*
*/
class Editor :
    public Component,
    public TextButton::Listener
{
public:
    Editor(float frequency) :
        frequencyControl ("F", 10, 1000, frequency, true),
        sCoeffControl ("S", 0, 0.999, 0.5),
        pCoeffControl ("P", 0.9, 1, 0.999),
        cCoeffControl ("C", -1, 1, 0.5),
        positionControl ("D", 0, 1, 0.5)
    {
        addAndMakeVisible (pluckButton);
        pluckButton.setButtonText ("Pluck!");
        pluckButton.addListener (this);

        addAndMakeVisible (frequencyControl);
        addAndMakeVisible (sCoeffControl);
        addAndMakeVisible (pCoeffControl);
        addAndMakeVisible (cCoeffControl);
        addAndMakeVisible (positionControl);

        karplus.freq = frequencyControl.registerValue();
        karplus.s_coeff = sCoeffControl.registerValue();
        karplus.p_coeff = pCoeffControl.registerValue();
        karplus.c_coeff = cCoeffControl.registerValue();
        karplus.position = positionControl.registerValue();
    }

    ~Editor()
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
        Rectangle<int> localBounds = getLocalBounds();
        pluckButton.setBounds (localBounds.removeFromTop (35).reduced (5));

        int heightChild = localBounds.getHeight() / 5;
        frequencyControl.setBounds(localBounds.removeFromTop (heightChild).reduced (5));
        sCoeffControl.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
        pCoeffControl.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
        cCoeffControl.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
        positionControl.setBounds (localBounds.removeFromTop (heightChild).reduced (5));
    }

    void buttonClicked (Button* button) override
    {
        if (&pluckButton == button) {
            karplus.pluckString();
        }
    }

    //==========================================================================
    TextButton pluckButton;
    Control frequencyControl;
    Control sCoeffControl;
    Control pCoeffControl;
    Control cCoeffControl;
    Control positionControl;

    KarplusStrong karplus;

private:

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};


#endif  // EDITOR_H_INCLUDED
