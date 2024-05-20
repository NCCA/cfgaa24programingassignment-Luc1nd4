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

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);


  ngl::Image textureImage("/home/s5524683/Desktop/CFGAA/labcode-Luc1nd4/BoidsNGL/cmake-build-debug/carp9_carp_BaseColor.1001.png");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, textureImage.format(), GL_UNSIGNED_BYTE, textureImage.getPixels());
  glGenerateMipmap(GL_TEXTURE_2D);


  glBindTexture(GL_TEXTURE_2D, 0);

  m_textureID = textureID;
  ngl::ShaderLib::createShaderProgram("ParticleShader");
  ngl::ShaderLib::attachShader("ParticleVertex", ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader("ParticleFragment", ngl::ShaderType::FRAGMENT);
  ngl::ShaderLib::loadShaderSource("ParticleVertex", "shaders/ParticleVertex.glsl");
  ngl::ShaderLib::loadShaderSource("ParticleFragment", "shaders/ParticleFragment.glsl");

  ngl::ShaderLib::compileShader("ParticleVertex");
  ngl::ShaderLib::compileShader("ParticleFragment");
  ngl::ShaderLib::attachShaderToProgram("ParticleShader", "ParticleVertex");
  ngl::ShaderLib::attachShaderToProgram("ParticleShader", "ParticleFragment");

    // link the shader no attributes are bound
  ngl::ShaderLib::linkProgramObject("ParticleShader");
  ngl::ShaderLib::use("ParticleShader");

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);


  ngl::Obj fish("fish.obj", "carp9_carp_BaseColor.1001.png");
  fish.createVAO();
  ngl::VAOPrimitives::loadObj("fish", "fish.obj");

    //Shaders and perspective

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
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  ngl::ShaderLib::setUniform("MVP", m_project * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("tex", 0); // Texture unit 0

    // Drawing code
  m_flock->render(m_view, m_project, m_textureID);

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

