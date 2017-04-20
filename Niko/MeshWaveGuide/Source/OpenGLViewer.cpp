/*
  ==============================================================================

    OpenGLViewer.cpp
    Created: 3 Apr 2017 7:12:36am
    Author:  sd

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenGLViewer.h"
#include <sstream>

// note, only static members can be initialized like this! Else in constructor...
const unsigned char OpenGLViewer::temp_binary_data_7[] =
//~ {
  //~ 0x76, 0x20, 0x30, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x20, 0x30, 0x2e,
  //~ 0x30, 0x0a, 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x20,
  //~ 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x31, 0x2e,
  //~ 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x30, 0x2e, 0x30, 0x20,
  //~ 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x30, 0x2e,
  //~ 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20,
  //~ 0x31, 0x2e, 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a,
  //~ 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x32, 0x2e, 0x30, 0x20, 0x30, 0x2e,
  //~ 0x30, 0x0a, 0x76, 0x20, 0x30, 0x2e, 0x30, 0x20, 0x32, 0x2e, 0x30, 0x20,
  //~ 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e,
  //~ 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x32, 0x2e, 0x30, 0x20,
  //~ 0x30, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x32, 0x2e,
  //~ 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20,
  //~ 0x31, 0x2e, 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a,
  //~ 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x30, 0x2e,
  //~ 0x30, 0x0a, 0x76, 0x20, 0x32, 0x2e, 0x30, 0x20, 0x31, 0x2e, 0x30, 0x20,
  //~ 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x32, 0x2e, 0x30, 0x20, 0x32, 0x2e,
  //~ 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x76, 0x20, 0x31, 0x2e, 0x30, 0x20,
  //~ 0x32, 0x2e, 0x30, 0x20, 0x30, 0x2e, 0x30, 0x0a, 0x67, 0x20, 0x6d, 0x79,
  //~ 0x50, 0x6c, 0x61, 0x6e, 0x65, 0x0a, 0x66, 0x20, 0x31, 0x20, 0x32, 0x20,
  //~ 0x33, 0x20, 0x34, 0x0a, 0x66, 0x20, 0x35, 0x20, 0x36, 0x20, 0x37, 0x20,
  //~ 0x38, 0x0a, 0x66, 0x20, 0x39, 0x20, 0x31, 0x30, 0x20, 0x31, 0x31, 0x20,
  //~ 0x31, 0x32, 0x0a, 0x66, 0x20, 0x31, 0x33, 0x20, 0x31, 0x34, 0x20, 0x31,
  //~ 0x35, 0x20, 0x31, 0x36, 0x0a, 0x67, 0x0a
//~ };
R"(
v 0.0 0.0
v 1.0 0.0
v 1.0 1.0
v 0.0 1.0
g myPlane
f 1 2 3 4

)";
// error: ‘static’ may not be used when defining (as opposed to declaring) a static data member [-fpermissive]
const char* OpenGLViewer::teapot_obj = (const char*) OpenGLViewer::temp_binary_data_7;


struct OpenGLViewer::Shape
{
    bool skipDraw = false;
    int currentBuffer = -1;
    Shape (OpenGLContext& openGLContext)
    {
        if (shapeFile.load (teapot_obj).wasOk()) {
            //~ Logger::outputDebugString ("Here"); // output to stderr
            DBG(" shapeFile.shapes.size: " + String(shapeFile.shapes.size())); // does not output in non-debug builds
            for (int i = 0; i < shapeFile.shapes.size(); ++i) {
                WavefrontObjFile::Shape& tshape = *shapeFile.shapes.getUnchecked(i);
                vertexBuffers.add (new VertexBuffer (openGLContext, tshape));
                DBG( "_vertexBuffers size: " + String(vertexBuffers.size()) );
            }
        }
    }
    // different constructor function signature, just to overload:
    Shape (OpenGLContext& openGLContext, WavefrontObjFile::Shape& inshape)
    {
      // vertexBuffers.clear(); // should be already, we're in ctor
      vertexBuffers.add (new VertexBuffer (openGLContext, inshape));
      currentBuffer = 0;
    }

    void reload(OpenGLContext& openGLContext, String& objstr) {
        skipDraw = true;
        vertexBuffers.clear();
        if (shapeFile.load (objstr).wasOk()) {
            DBG(" shapeFile.shapes.size: " + String(shapeFile.shapes.size())); // does not output in non-debug builds
            for (int i = 0; i < shapeFile.shapes.size(); ++i) {
                WavefrontObjFile::Shape& tshape = *shapeFile.shapes.getUnchecked(i);
                vertexBuffers.add (new VertexBuffer (openGLContext, tshape));
            }
        }
        skipDraw = false;
    }

    void draw (OpenGLContext& openGLContext, Attributes& attributes)
    {
        //~ DBG( "vertexBuffers size: " + String(vertexBuffers.size()) );
        if (skipDraw) return;
        for (int i = 0; i < vertexBuffers.size(); ++i)
        {
            VertexBuffer& vertexBuffer = *vertexBuffers.getUnchecked (i);
            vertexBuffer.bind();

            attributes.enable (openGLContext);
            //~ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP); // this forces wireframe; can be below, too
            glDrawElements (GL_LINE_LOOP, vertexBuffer.numIndices, GL_UNSIGNED_INT, 0); //GL_TRIANGLES, GL_LINE_LOOP, GL_QUADS
            //~ DBG( "vertexBuffer nI: " + String(i) + " " + String(vertexBuffer.numIndices) );
            attributes.disable (openGLContext);
        }
    }

private:
    struct VertexBuffer
    {
        VertexBuffer (OpenGLContext& context, WavefrontObjFile::Shape& shape) : openGLContext (context)
        {
            numIndices = shape.mesh.indices.size();

            openGLContext.extensions.glGenBuffers (1, &vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);

            Array<Vertex> vertices;
            createVertexListFromMesh (shape.mesh, vertices, Colours::green);

            openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, vertices.size() * (int) sizeof (Vertex),
                                                   vertices.getRawDataPointer(), GL_STATIC_DRAW);

            openGLContext.extensions.glGenBuffers (1, &indexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, numIndices * (int) sizeof (juce::uint32),
                                                   shape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
        }

        ~VertexBuffer()
        {
            openGLContext.extensions.glDeleteBuffers (1, &vertexBuffer);
            openGLContext.extensions.glDeleteBuffers (1, &indexBuffer);
        }

        void bind()
        {
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        }

        GLuint vertexBuffer, indexBuffer;
        int numIndices;
        OpenGLContext& openGLContext;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VertexBuffer)
    };

    WavefrontObjFile shapeFile;
    OwnedArray<VertexBuffer> vertexBuffers;

    static void addVertexToList (Array<Vertex>& list, WavefrontObjFile::Vertex inv, WavefrontObjFile::Vertex inn = { 0.5f, 0.5f, 0.5f }, WavefrontObjFile::TextureCoord intc = { 0.5f, 0.5f }, Colour colour=Colours::green)
    {
        const float scale = 0.2f;
        // WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
        // WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };
        Vertex vert =
        {
            { scale * inv.x, scale * inv.y, scale * inv.z, },
            { scale * inn.x, scale * inn.y, scale * inn.z, },
            { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
            { intc.x, intc.y }
        };

        list.add (vert);
    }
    static void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh, Array<Vertex>& list, Colour colour)
    {
        const float scale = 0.2f;
        WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
        WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };

        for (int i = 0; i < mesh.vertices.size(); ++i)
        {
            const WavefrontObjFile::Vertex& v = mesh.vertices.getReference (i);

            const WavefrontObjFile::Vertex& n
            = i < mesh.normals.size() ? mesh.normals.getReference (i) : defaultNormal;

            const WavefrontObjFile::TextureCoord& tc
            = i < mesh.textureCoords.size() ? mesh.textureCoords.getReference (i) : defaultTexCoord;

            Vertex vert =
            {
                { scale * v.x, scale * v.y, scale * v.z, },
                { scale * n.x, scale * n.y, scale * n.z, },
                { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                { tc.x, tc.y }
            };

            list.add (vert);
        }
    }
};

//==============================================================================
OpenGLViewer::OpenGLViewer()
                  : doBackgroundDrawing (false),
                    scale (0.5f)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // (the vertex shader also ultimately takes care of rotation via mouse drag)
    newVertexShader =
      "attribute vec4 position;\n"
      "attribute vec4 normal;\n"
      "varying vec3 norm;\n"
      "uniform mat4 projectionMatrix;\n"
      "uniform mat4 viewMatrix;\n"
      "uniform vec2 meshPos;\n"
      "void main()\n"
      "{\n"
      "    norm = normal.xyz;\n"
      "    if ((position.x != meshPos.x) && (position.y != meshPos.y)) { \n"
      "      gl_Position = projectionMatrix * viewMatrix * position;\n"
      "    }\n"
      "}\n";
    newFragmentShader =
      "void main()\n"
      "{\n"
      "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}\n";

    setOpaque (true);

    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);

    //~ if (!shape) {
      //~ updateShader(); //cannot call updateShader() here: -> addVertexShader -> addShader segfaults here!
    //~ }
}

OpenGLViewer::~OpenGLViewer()
{
    openGLContext.detach();
}

void OpenGLViewer::newOpenGLContextCreated() //override
{
    // nothing to do in this case - we'll initialise our shaders + textures
    // on demand, during the render callback.
    freeAllContextObjects();

    //~ if (controlsOverlay != nullptr)
        //~ controlsOverlay->updateShader();
    if (!shape) {
      updateShader(); //shape = new Shape (openGLContext);
    }
}

void OpenGLViewer::openGLContextClosing() //override
{
    // When the context is about to close, you must use this callback to delete
    // any GPU resources while the context is still current.
    freeAllContextObjects();

    //~ if (lastTexture != nullptr)
        //~ setTexture (lastTexture);
}

void OpenGLViewer::updateShader()
{
  if (newVertexShader.isNotEmpty() || newFragmentShader.isNotEmpty())
  {
    ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
    String statusText;

    if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (newVertexShader))
        && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (newFragmentShader))
        && newShader->link())
    {
      shape = nullptr;
      attributes = nullptr;
      uniforms = nullptr;

      shader = newShader;
      shader->use();

      //~ shape    = new Shape (openGLContext);
      shape    = new Shape (openGLContext, curshape);
      attributes = new Attributes (openGLContext, *shader);
      uniforms   = new Uniforms (openGLContext, *shader);

      //~ statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
      //~ statusText = newShader->getLastError();
    }

    //~ controlsOverlay->statusLabel.setText (statusText, dontSendNotification);

    //~ newVertexShader = String();
    //~ newFragmentShader = String();
  }
}

// cannot call from here, likelihood all will disappear is too great
// simply set a signal variable, and have the real function execute at the end of the renderOpenGL
void OpenGLViewer::updateMeshSizeN(int newsize) {
  doMeshSizeN = newsize;
}

void OpenGLViewer::updateMeshSizeNreal(int newsize)
{
    meshSizeN = newsize;
    // if mesh size is 2 (2x2 verts) then there is 1 cell
    // if mesh size is 3 (3x3 verts) then there are 4 cells
    // if mesh size is 4 (4x4 verts) then there are 9 cells
    // if mesh size is 5 (5x5 verts) then there are 16 cells
    // (meshSizeN-1)^2
    int nrc = meshSizeN-1; // num rows/columns
    int numCells = int(pow(nrc, 2));
    DBG( "meshSizeN " + String(meshSizeN) + " numCells " + String( numCells ));
    std::stringstream ssv, ssf;
    ssv.precision(1);
    // ss << "Hello, world, " << myInt << niceToSeeYouString; std::string s = ss.str();
    //~ for (int tx=0; tx<meshSizeN-1; tx++) {
      //~ for (int ty=0; ty<meshSizeN-1; ty++) {

      //~ }
    //~ }
    // easier calc - but some vertices will be repeated
    // wavefront obj strings (ssv, ssf) not used anymore, but keeping for reference:
    WavefrontObjFile::Mesh tmesh;
    for (int tx=0; tx<nrc; tx++) {
      for (int ty=0; ty<nrc; ty++) {
        ssv << "v " << std::fixed << (float)tx << " " << std::fixed << (float)ty << " " << "0.0\n";
        ssv << "v " << std::fixed << (float)(tx+1) << " " << std::fixed << (float)ty << " " << "0.0\n";
        ssv << "v " << std::fixed << (float)(tx+1) << " " << std::fixed << (float)(ty+1) << " " << "0.0\n";
        ssv << "v " << std::fixed << (float)tx << " " << std::fixed << (float)(ty+1) << " " << "0.0\n";
        WavefrontObjFile::Vertex tv0 = { (float)tx, (float)ty, 0.0f }; // aggregate initializer
        WavefrontObjFile::Vertex tv1 = { (float)(tx+0), (float)(ty+1), 0.0f };
        WavefrontObjFile::Vertex tv2 = { (float)(tx+1), (float)(ty+1), 0.0f };
        WavefrontObjFile::Vertex tv3 = { (float)(tx+1), (float)(ty+0), 0.0f };
        tmesh.vertices.add(tv0); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
        tmesh.vertices.add(tv1); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
        tmesh.vertices.add(tv2); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
        tmesh.vertices.add(tv3); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
        // add return to original point, since its a GL_LINE_STRIP, otherwise it will also draw diagonals when it moves to next elem
        WavefrontObjFile::Vertex tv4 = { (float)tx, (float)ty, 0.0f };
        tmesh.vertices.add(tv4); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
      }
      // add return to original point, since its a GL_LINE_STRIP, otherwise it will also draw diagonals when it moves to next row
      WavefrontObjFile::Vertex tvx = { (float)tx, 0.0f, 0.0f };
      tmesh.vertices.add(tvx); tmesh.indices.add((WavefrontObjFile::Index)(tmesh.vertices.size()-1));
    }
    for(int tx=0; tx<numCells; tx++) {
      ssf << "f " << (4*tx+1) << " " << (4*tx+2) << " " << (4*tx+3) << " " << (4*tx+4) << "\n";
      //~ WavefrontObjFile::Index ti0 = (5*tx+0), ti1 = (5*tx+1), ti2 = (5*tx+2), ti3 = (5*tx+3), ti4 = (5*tx+4);
      //~ tmesh.indices.add(ti0); tmesh.indices.add(ti1); tmesh.indices.add(ti2); tmesh.indices.add(ti3); tmesh.indices.add(ti4);
    }
    String objstr( ssv.str() + "g myPlane\n" + ssf.str() + "g\n" );
    // dump Wavefront obj string to stdout - note, it gets quite large for NJ>32, and might hog stdout, so disabling it for now:
    //~ DBG( objstr );
    //
    //~ const ScopedLock sl (updateLock);
    //~ shape = nullptr;
    //shape    = new Shape (openGLContext);
    //shape->reload(openGLContext, objstr);
    //~ WavefrontObjFile::Shape tshape;
    curshape.name = "myshape";
    curshape.mesh = tmesh;
    curshape.material = WavefrontObjFile::Material();

    //Shape (OpenGLContext& openGLContext, WavefrontObjFile::Shape& inshape)
    shape    = new Shape (openGLContext, curshape);
}

// This is a virtual method in OpenGLRenderer, and is called when it's time
// to do your GL rendering.
void OpenGLViewer::renderOpenGL() //override
{
    //~ const ScopedLock sl (updateLock);
    jassert (OpenGLHelpers::isContextActive());

    const float desktopScale = (float) openGLContext.getRenderingScale();
    OpenGLHelpers::clear (Colours::lightblue);

    //~ if (textureToUse != nullptr)
        //~ if (! textureToUse->applyTo (texture))
            //~ textureToUse = nullptr;

    // Having used the juce 2D renderer, it will have messed-up a whole load of GL state, so
    // we need to initialise some important settings before doing our normal GL 3D drawing..
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    openGLContext.extensions.glActiveTexture (GL_TEXTURE0);
    glEnable (GL_TEXTURE_2D);

    glViewport (0, 0, roundToInt (desktopScale * getWidth()), roundToInt (desktopScale * getHeight()));

    //~ texture.bind();

    shader->use();

    if (uniforms->projectionMatrix != nullptr)
      uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);

    if (uniforms->viewMatrix != nullptr)
      uniforms->viewMatrix->setMatrix4 (getViewMatrix().mat, 1, false);

    if (uniforms->texture != nullptr)
      uniforms->texture->set ((GLint) 0);

    if (uniforms->lightPosition != nullptr)
      uniforms->lightPosition->set (-15.0f, 10.0f, 15.0f, 0.0f);

    if (uniforms->meshPos != nullptr)
      uniforms->meshPos->set (meshPosX, meshPosY);

    //~ if (uniforms->bouncingNumber != nullptr)
      //~ uniforms->bouncingNumber->set (bouncingNumber.getValue());

    shape->draw (openGLContext, *attributes);

    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    if (doMeshSizeN > 0) { // finally, this trick fixes
      updateMeshSizeNreal(doMeshSizeN);
      doMeshSizeN = -1;
    }
}

void OpenGLViewer::freeAllContextObjects()
{
    shape = nullptr;
    shader = nullptr;
    attributes = nullptr;
    uniforms = nullptr;
    //~ texture.release();
}

Matrix3D<float> OpenGLViewer::getProjectionMatrix() const
{
  float w = 1.0f / (scale + 0.1f);
  float h = w * getLocalBounds().toFloat().getAspectRatio (false);
  return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
}

Matrix3D<float> OpenGLViewer::getViewMatrix() const
{
  float rotation = 0.0f;
  Matrix3D<float> viewMatrix = draggableOrientation.getRotationMatrix()
                  * Vector3D<float> (0.0f, 1.0f, -10.0f);

  Matrix3D<float> rotationMatrix = viewMatrix.rotated (Vector3D<float> (rotation, rotation, -0.3f));

  return rotationMatrix * viewMatrix;
}


void OpenGLViewer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //~ g.fillAll (Colours::white);   // clear the background

    //~ g.setColour (Colours::grey);
    //~ g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //~ g.setColour (Colours::lightblue);
    //~ g.setFont (14.0f);
    //~ g.drawText ("OpenGLViewer", getLocalBounds(),
                //~ Justification::centred, true);   // draw some placeholder text
}

void OpenGLViewer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    draggableOrientation.setViewport (getLocalBounds());
}
