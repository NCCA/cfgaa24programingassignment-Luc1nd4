#include <QMouseEvent>
#include <QGuiApplication>
#include <QMainWindow>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <QApplication>
#include <ngl/Util.h>
#include <ngl/ShaderLib.h>
#include <iostream>
#include <ngl/Obj.h>
#include <ngl/Image.h>
NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Boids Flocking System");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f,float(_w)/_h,0.1f,200);
}


void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.2f, 0.5f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  //Initial settings
  m_flock=std::make_unique<Flock>(300);

  //Shaders and perspective

  ngl::ShaderLib::loadShader("ParticleShader","shaders/ParticleVertex.glsl","shaders/ParticleFragment.glsl");
  ngl::ShaderLib::use("ParticleShader");

  ngl::Texture t("carp.png");
  m_textureID = t.setTextureGL();

  ngl::Obj fish("fish.obj");
  fish.setTexture("carp.png");
  fish.loadTexture("carp.png");
  fish.createVAO();
  ngl::VAOPrimitives::loadObj("fish", "fish.obj");


  m_view = ngl::lookAt({0,100,100},{0,0,0},{0,1,0});
  startTimer(10);



}


void NGLScene::timerEvent(QTimerEvent *_event)
{
    m_flock->update();
    update();
}


void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_win.width,m_win.height);
    auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
    auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
    auto mouseRotation = rotX * rotY;
    mouseRotation.m_m[3][0] = m_modelPos.m_x;
    mouseRotation.m_m[3][0] = m_modelPos.m_x;
    mouseRotation.m_m[3][0] = m_modelPos.m_x;
    ngl::ShaderLib::use("ParticleShader");


    glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, m_textureID); // Bind the texture to unit 0

    ngl::ShaderLib::use("ParticleShader");

    // Set the MVP uniform as before
    ngl::ShaderLib::setUniform("MVP", m_project * m_view * mouseRotation);

    // Set the "tex" uniform to texture unit 0
    int texLoc = glGetUniformLocation(ngl::ShaderLib::getProgramID("ParticleShader"), "tex");
    glUniform1i(texLoc, 0); // Set the location to texture unit 0 (where the texture is bound)

    // Drawing code
    m_flock->render(m_view, m_project, m_textureID, mouseRotation);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}

//Update sliders

void NGLScene::setBoidSpeed(int _newBoidSpeed)
{
    m_flock->setMaxSpeed(_newBoidSpeed);
}

void NGLScene::setBoidForce(int _newBoidForce)
{
    m_flock->setMaxForce(_newBoidForce);
}

void NGLScene::setFlockDesiredSeparaion(int _newBoidDesiredSeparation)
{
    m_flock->setDesiredSeparation(_newBoidDesiredSeparation);
}

void NGLScene::setFlockAlignmentDist(int _newBoidAlignmentDist)
{
    m_flock->setAlignmentDist(_newBoidAlignmentDist);
}

void NGLScene::setFlockCohesionDist(int _newBoidCohesionDist)
{
    m_flock->setCohesionDist(_newBoidCohesionDist);
}

