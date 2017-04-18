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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GuiComponents.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GuiComponents::GuiComponents (CriticalSection& incs)
    : csGuiMeshSize(incs)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (sliderN = new Slider ("slider for N"));
    sliderN->setTooltip (TRANS("changes N"));
    sliderN->setRange (2, 64, 1);
    sliderN->setSliderStyle (Slider::LinearHorizontal);
    sliderN->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderN->addListener (this);

    addAndMakeVisible (labelN = new Label ("label for N",
                                           TRANS("NJ")));
    labelN->setFont (Font ("Arial", 15.00f, Font::bold));
    labelN->setJustificationType (Justification::centredRight);
    labelN->setEditable (false, false, false);
    labelN->setColour (TextEditor::textColourId, Colours::black);
    labelN->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textButtonTrigger = new TextButton ("button trigger"));
    textButtonTrigger->setTooltip (TRANS("Trigger the sound!\n"));
    textButtonTrigger->setButtonText (TRANS("Trigger!"));
    textButtonTrigger->addListener (this);

    addAndMakeVisible (labelAmp = new Label ("label for Amp",
                                             TRANS("Amp\n")));
    labelAmp->setFont (Font ("Arial", 15.00f, Font::bold));
    labelAmp->setJustificationType (Justification::centredRight);
    labelAmp->setEditable (false, false, false);
    labelAmp->setColour (TextEditor::textColourId, Colours::black);
    labelAmp->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderAmp = new Slider ("slider for Amp"));
    sliderAmp->setTooltip (TRANS("changes Amp\n"));
    sliderAmp->setRange (1, 500, 0);
    sliderAmp->setSliderStyle (Slider::LinearHorizontal);
    sliderAmp->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderAmp->addListener (this);

    addAndMakeVisible (labelMeshPosX = new Label ("label for MeshPosX",
                                                  TRANS("MeshPosX")));
    labelMeshPosX->setFont (Font ("Arial", 15.00f, Font::bold));
    labelMeshPosX->setJustificationType (Justification::centredRight);
    labelMeshPosX->setEditable (false, false, false);
    labelMeshPosX->setColour (TextEditor::textColourId, Colours::black);
    labelMeshPosX->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderMeshPosX = new Slider ("slider for MeshPosX"));
    sliderMeshPosX->setTooltip (TRANS("changes MeshPosX"));
    sliderMeshPosX->setRange (0, 63, 1);
    sliderMeshPosX->setSliderStyle (Slider::LinearHorizontal);
    sliderMeshPosX->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderMeshPosX->addListener (this);

    addAndMakeVisible (labelMeshPosY = new Label ("label for MeshPosY",
                                                  TRANS("MeshPosY")));
    labelMeshPosY->setFont (Font ("Arial", 15.00f, Font::bold));
    labelMeshPosY->setJustificationType (Justification::centredRight);
    labelMeshPosY->setEditable (false, false, false);
    labelMeshPosY->setColour (TextEditor::textColourId, Colours::black);
    labelMeshPosY->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderMeshPosY = new Slider ("slider for MeshPosY"));
    sliderMeshPosY->setTooltip (TRANS("changes MeshPosY"));
    sliderMeshPosY->setRange (0, 63, 1);
    sliderMeshPosY->setSliderStyle (Slider::LinearHorizontal);
    sliderMeshPosY->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderMeshPosY->addListener (this);


    //[UserPreSize]
    sliderN->setValue (32);
    //[/UserPreSize]

    setSize (800, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GuiComponents::~GuiComponents()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sliderN = nullptr;
    labelN = nullptr;
    textButtonTrigger = nullptr;
    labelAmp = nullptr;
    sliderAmp = nullptr;
    labelMeshPosX = nullptr;
    sliderMeshPosX = nullptr;
    labelMeshPosY = nullptr;
    sliderMeshPosY = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GuiComponents::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GuiComponents::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    sliderN->setBounds (48, 8, proportionOfWidth (0.5000f), 24);
    labelN->setBounds (8, 8, 31, 24);
    textButtonTrigger->setBounds (getWidth() - 170, 8, 150, 24);
    labelAmp->setBounds (0, 32, 40, 24);
    sliderAmp->setBounds (48, 32, proportionOfWidth (0.5000f), 24);
    labelMeshPosX->setBounds (0, 56, 80, 24);
    sliderMeshPosX->setBounds (80, 56, proportionOfWidth (0.5000f), 24);
    labelMeshPosY->setBounds (0, 80, 80, 24);
    sliderMeshPosY->setBounds (80, 80, proportionOfWidth (0.5000f), 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GuiComponents::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderN)
    {
        const ScopedLock sl(csGuiMeshSize);
        //[UserSliderCode_sliderN] -- add your slider handling code here..
        meshSizeN = (int)sliderN->getValue();
        // reset meshpos sliders:
        sliderMeshPosX->setRange (0, meshSizeN-1, 1);
        sliderMeshPosY->setRange (0, meshSizeN-1, 1);
        // send event:
        Component::BailOutChecker checker (this);
        GuiComponents* gcomps = this; // (must use an intermediate variable here to avoid a VS2005 compiler bug)
        listeners.callChecked (checker, &GuiComponents::Listener::meshSizeNChanged, gcomps);
        //[/UserSliderCode_sliderN]
    }
    else if (sliderThatWasMoved == sliderAmp)
    {
        //[UserSliderCode_sliderAmp] -- add your slider handling code here..
        amp = (float)sliderAmp->getValue();
        Component::BailOutChecker checker (this);
        GuiComponents* gcomps = this; // (must use an intermediate variable here to avoid a VS2005 compiler bug)
        listeners.callChecked (checker, &GuiComponents::Listener::ampChanged, gcomps);
        //[/UserSliderCode_sliderAmp]
    }
    else if (sliderThatWasMoved == sliderMeshPosX)
    {
        //[UserSliderCode_sliderMeshPosX] -- add your slider handling code here..
        meshPosX = (int)sliderMeshPosX->getValue();
        Component::BailOutChecker checker (this);
        GuiComponents* gcomps = this; // (must use an intermediate variable here to avoid a VS2005 compiler bug)
        listeners.callChecked (checker, &GuiComponents::Listener::meshPosXChanged, gcomps);
        //[/UserSliderCode_sliderMeshPosX]
    }
    else if (sliderThatWasMoved == sliderMeshPosY)
    {
        //[UserSliderCode_sliderMeshPosY] -- add your slider handling code here..
        meshPosY = (int)sliderMeshPosY->getValue();
        Component::BailOutChecker checker (this);
        GuiComponents* gcomps = this; // (must use an intermediate variable here to avoid a VS2005 compiler bug)
        listeners.callChecked (checker, &GuiComponents::Listener::meshPosYChanged, gcomps);
        //[/UserSliderCode_sliderMeshPosY]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GuiComponents::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButtonTrigger)
    {
        //[UserButtonCode_textButtonTrigger] -- add your button handler code here..
        Component::BailOutChecker checker (this);
        GuiComponents* gcomps = this; // (must use an intermediate variable here to avoid a VS2005 compiler bug)
        listeners.callChecked (checker, &GuiComponents::Listener::triggerButtonClicked, gcomps);
        //[/UserButtonCode_textButtonTrigger]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void GuiComponents::addListener (GuiComponents::Listener* const listener)       { listeners.add (listener); }
void GuiComponents::removeListener (GuiComponents::Listener* const listener)    { listeners.remove (listener); }
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GuiComponents" componentName=""
                 parentClasses="public Component" constructorParams="CriticalSection&amp; incs"
                 variableInitialisers="csGuiMeshSize(incs)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="800"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="slider for N" id="437c6ee0435feb1f" memberName="sliderN"
          virtualName="" explicitFocusOrder="0" pos="48 8 50% 24" tooltip="changes N"
          min="2" max="64" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="label for N" id="4a63e31803c56bd0" memberName="labelN"
         virtualName="" explicitFocusOrder="0" pos="8 8 31 24" edTextCol="ff000000"
         edBkgCol="0" labelText="NJ" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Arial" fontsize="15" bold="1"
         italic="0" justification="34"/>
  <TEXTBUTTON name="button trigger" id="eefe4fcf9a2137a9" memberName="textButtonTrigger"
              virtualName="" explicitFocusOrder="0" pos="170R 8 150 24" tooltip="Trigger the sound!&#10;"
              buttonText="Trigger!" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="label for Amp" id="3fc986231a8d318c" memberName="labelAmp"
         virtualName="" explicitFocusOrder="0" pos="0 32 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Amp&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Arial" fontsize="15" bold="1"
         italic="0" justification="34"/>
  <SLIDER name="slider for Amp" id="4ed88ac0173b5d93" memberName="sliderAmp"
          virtualName="" explicitFocusOrder="0" pos="48 32 50% 24" tooltip="changes Amp&#10;"
          min="1" max="500" int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="label for MeshPosX" id="117a0c9888829080" memberName="labelMeshPosX"
         virtualName="" explicitFocusOrder="0" pos="0 56 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MeshPosX" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Arial" fontsize="15" bold="1"
         italic="0" justification="34"/>
  <SLIDER name="slider for MeshPosX" id="39ba5a9015d4debd" memberName="sliderMeshPosX"
          virtualName="" explicitFocusOrder="0" pos="80 56 50% 24" tooltip="changes MeshPosX"
          min="0" max="63" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="label for MeshPosY" id="85fbedf9df706fdc" memberName="labelMeshPosY"
         virtualName="" explicitFocusOrder="0" pos="0 80 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MeshPosY" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Arial" fontsize="15" bold="1"
         italic="0" justification="34"/>
  <SLIDER name="slider for MeshPosY" id="57a2f05cae68f580" memberName="sliderMeshPosY"
          virtualName="" explicitFocusOrder="0" pos="80 80 50% 24" tooltip="changes MeshPosY"
          min="0" max="63" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
