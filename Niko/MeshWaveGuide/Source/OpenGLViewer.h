/*
  ==============================================================================

    OpenGLViewer.h
    Created: 3 Apr 2017 7:12:36am
    Author:  sd

  ==============================================================================
*/

#ifndef OPENGLVIEWER_H_INCLUDED
#define OPENGLVIEWER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "WavefrontObjParser.h"

//==============================================================================
/*
*/
class OpenGLViewer    : public Component,
                        private OpenGLRenderer,
                        private Button::Listener
{
public:
    OpenGLViewer();
    ~OpenGLViewer();

    void paint (Graphics&) override;
    void resized() override;

    Draggable3DOrientation draggableOrientation;
    bool doBackgroundDrawing;
    float scale;

    void newOpenGLContextCreated() override;
    void openGLContextClosing() override;
    void renderOpenGL() override;
    void freeAllContextObjects();

    //~ CriticalSection updateLock;

    void buttonClicked (Button*) override
    {
    }
    void mouseDown (const MouseEvent& e) override
    {
      draggableOrientation.mouseDown (e.getPosition());
    }
    void mouseDrag (const MouseEvent& e) override
    {
      draggableOrientation.mouseDrag (e.getPosition());
    }
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& d) override
    {
        //sizeSlider.setValue (sizeSlider.getValue() + d.deltaY);
        // demo.scale = (float) sizeSlider.getValue();
        scale = scale + d.deltaY;
    }
    void mouseMagnify (const MouseEvent&, float magnifyAmmount) override // for mobile: pinch/zoom
    {
        //sizeSlider.setValue (sizeSlider.getValue() + magnifyAmmount - 1.0f);
        scale = scale + magnifyAmmount - 1.0f;
    }

    int meshSizeN = 2; // minimum two, initial model is also 2x2 vertices
    int doMeshSizeN = -1; // signal
    void updateMeshSizeN(int newsize);
    void updateMeshSizeNreal(int newsize);

    // v.x, v.y, v.z, n.x, n.y, n.z, tc.x, tc.y -> 8
    // cannot initialize static const member in header file; just declare it here
    static const unsigned char temp_binary_data_7[];
    //~ const char* teapot_obj = (const char*) temp_binary_data_7; // SO: 43179796
    //static constexpr const char* teapot_obj = (const char*) temp_binary_data_7;
    static const char* teapot_obj; // = (const char*) temp_binary_data_7;


    struct Vertex
    {
      float position[3];
      float normal[3];
      float colour[4];
      float texCoord[2];
    };

    //==============================================================================
    // This class just manages the attributes that the demo shaders use.
    struct Attributes
    {
      Attributes (OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
      {
        position     = createAttribute (openGLContext, shader, "position");
        normal     = createAttribute (openGLContext, shader, "normal");
        sourceColour   = createAttribute (openGLContext, shader, "sourceColour");
        textureCoordIn = createAttribute (openGLContext, shader, "textureCoordIn");
      }

      void enable (OpenGLContext& openGLContext)
      {
        if (position != nullptr)
        {
          openGLContext.extensions.glVertexAttribPointer (position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), 0);
          openGLContext.extensions.glEnableVertexAttribArray (position->attributeID);
        }

        if (normal != nullptr)
        {
          openGLContext.extensions.glVertexAttribPointer (normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 3));
          openGLContext.extensions.glEnableVertexAttribArray (normal->attributeID);
        }

        if (sourceColour != nullptr)
        {
          openGLContext.extensions.glVertexAttribPointer (sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 6));
          openGLContext.extensions.glEnableVertexAttribArray (sourceColour->attributeID);
        }

        if (textureCoordIn != nullptr)
        {
          openGLContext.extensions.glVertexAttribPointer (textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 10));
          openGLContext.extensions.glEnableVertexAttribArray (textureCoordIn->attributeID);
        }
      }

      void disable (OpenGLContext& openGLContext)
      {
        if (position != nullptr)     openGLContext.extensions.glDisableVertexAttribArray (position->attributeID);
        if (normal != nullptr)     openGLContext.extensions.glDisableVertexAttribArray (normal->attributeID);
        if (sourceColour != nullptr)   openGLContext.extensions.glDisableVertexAttribArray (sourceColour->attributeID);
        if (textureCoordIn != nullptr)  openGLContext.extensions.glDisableVertexAttribArray (textureCoordIn->attributeID);
      }

      ScopedPointer<OpenGLShaderProgram::Attribute> position, normal, sourceColour, textureCoordIn;

    private:
      static OpenGLShaderProgram::Attribute* createAttribute (OpenGLContext& openGLContext,
                                  OpenGLShaderProgram& shader,
                                  const char* attributeName)
      {
        if (openGLContext.extensions.glGetAttribLocation (shader.getProgramID(), attributeName) < 0)
          return nullptr;

        return new OpenGLShaderProgram::Attribute (shader, attributeName);
      }
    };

    //==============================================================================
    // This class just manages the uniform values that the demo shaders use.
    struct Uniforms
    {
      Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
      {
        projectionMatrix = createUniform (openGLContext, shader, "projectionMatrix");
        viewMatrix     = createUniform (openGLContext, shader, "viewMatrix");
        texture      = createUniform (openGLContext, shader, "demoTexture");
        lightPosition  = createUniform (openGLContext, shader, "lightPosition");
        //~ bouncingNumber   = createUniform (openGLContext, shader, "bouncingNumber");
      }

      ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, texture, lightPosition; //, bouncingNumber;

    private:
      static OpenGLShaderProgram::Uniform* createUniform (OpenGLContext& openGLContext,
                                OpenGLShaderProgram& shader,
                                const char* uniformName)
      {
        if (openGLContext.extensions.glGetUniformLocation (shader.getProgramID(), uniformName) < 0)
          return nullptr;

        return new OpenGLShaderProgram::Uniform (shader, uniformName);
      }
    };

    //==============================================================================

    struct Shape;
    //==============================================================================

    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;

    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Shape> shape;
    ScopedPointer<Attributes> attributes;
    ScopedPointer<Uniforms> uniforms;
    WavefrontObjFile::Shape curshape;

private:
    OpenGLContext openGLContext;
    OpenGLTexture texture;
    String newVertexShader, newFragmentShader;

    void updateShader();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLViewer)
};


#endif  // OPENGLVIEWER_H_INCLUDED
