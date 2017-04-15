#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "GuiComponents.h"
#include "OpenGLViewer.h"
#include "MeshSynthesizer.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                               public GuiComponents::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {

        // Create an instance of our GUIComponents content component, and add it to our window...
        //addChildComponent(guiComps);
        //guiComps.setVisible(true);
        addAndMakeVisible(guiComps);
        addAndMakeVisible(myViewport);
        myViewport.setViewedComponent(&guiComps, false);
        guiComps.addListener(this);
        addAndMakeVisible(myOpenGLViewer);
        setSize (800, 600); // if setSize is after the viewport setup, then no need for epxlicit resize()

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        guiComps.removeListener(this);
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        generateMeshSynths (sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();

        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
          float* const channelData = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

          if (channel == 0)
          {
            // we have only one synth atm; but still, keep the loop:
            for (auto synth : meshSynths)
              synth->generateAndAddData (channelData, bufferToFill.numSamples);
          }
          else
          {
            memcpy (channelData,
                bufferToFill.buffer->getReadPointer (0),
                bufferToFill.numSamples * sizeof (float));
          }
        }
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
        meshSynths.clear();
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colour (0xff888888));


        // You can add your drawing code here!
        g.drawImageAt (meshSynths.getUnchecked(0)->meshStateImage, 650, 200);
        g.drawImageAt (meshSynths.getUnchecked(0)->meshStateImage2, 650, 300);
        repaint(); // must have this to have the image update...
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        //~ guiComps.setBounds(0, 0, getWidth(), getHeight());
        //~ guiComps.setBounds(getLocalBounds());
        guiComps.setSize(getWidth()-0, guiComps.getHeight()); // needs some space for v.scrollbar
        myViewport.setBounds(0, 0, proportionOfWidth(1.0f), 76); // but first size the view
        guiComps.setSize(myViewport.getMaximumVisibleWidth(), guiComps.getHeight()); // then resize component again, leaving space for scrollbar
        myOpenGLViewer.setBounds(0, 80, proportionOfWidth(0.8f), proportionOfHeight(0.75f));
    }

    void ampChanged (GuiComponents* gcomps) override
    {
      //~ DBG( "Received ampChanged: " + String(gcomps->amp));
      meshSynths.getUnchecked(0)->amp = gcomps->amp;
    }
    void meshSizeNChanged (GuiComponents* gcomps) override
    {
      //DBG( "Received meshSizeNChanged: " + String(gcomps->meshSizeN));
      myOpenGLViewer.updateMeshSizeN(gcomps->meshSizeN);
      meshSynths.getUnchecked(0)->updateMeshSizeNJ(gcomps->meshSizeN);
      repaint(); // must have this to have image redrawn with new size
    }

    void generateMeshSynths (double sampleRate)
    {
      meshSynths.clear();
      // create just one synth for now
      double frequencyInHz = 400;
      meshSynths.add (new MeshSynthesizer (sampleRate, frequencyInHz));
    }

    void triggerButtonClicked (GuiComponents* gcomps) override
    {
      DBG( "Received triggerButtonClicked: " );
      float position = 0.5;
      meshSynths.getUnchecked(0)->stringPlucked (position);
    }


private:
    //==============================================================================

    // Your private member variables go here...
    GuiComponents guiComps;
    Viewport myViewport;
    OpenGLViewer myOpenGLViewer;
    //~ MeshSynthesizer meshSynth; // this instantiates instantly with MeshSynthesizer(), however, we don't have a constructor without arguments, so we get: error: no matching function for call to ‘MeshSynthesizer::MeshSynthesizer()’; note:   candidate expects 2 arguments, 0 provided
    // so either should use pointers (and then we take care of destruction), or as in StringSynthesiser.h -> use OwnedArray, and instantiate synths inside.. here we'll use only one synth though:
    OwnedArray<MeshSynthesizer> meshSynths;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
