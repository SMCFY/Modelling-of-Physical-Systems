/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_2F325A9F3A0FA422__
#define __JUCE_HEADER_2F325A9F3A0FA422__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GuiComponents  : public Component,
                       public SliderListener,
                       public ButtonListener
{
public:
    //==============================================================================
    GuiComponents ();
    ~GuiComponents();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    class JUCE_API  Listener
    {
    public:
      /** Destructor. */
      virtual ~Listener() {}

      virtual void meshSizeNChanged (GuiComponents* gcomps) = 0;
      //virtual void meshSizeNChanged (GuiComponents*) {}
      virtual void triggerButtonClicked (GuiComponents* gcomps) = 0;
    };

    /** Adds a listener to be called when this slider's value changes. */
    void addListener (Listener* listener);
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
    ListenerList <GuiComponents::Listener> listeners;
    int meshSizeN;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> sliderN;
    ScopedPointer<Label> labelN;
    ScopedPointer<TextButton> textButtonTrigger;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiComponents)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_2F325A9F3A0FA422__
