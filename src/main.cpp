/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QApplication>
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QSlider>


int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(48);
  // now we are going to create our scene window
  NGLScene window;
  // and set the OpenGL format
  window.setFormat(format);
  // we can now query the version to see if it worked
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
  // set the window size
  window.resize(1024, 720);

  //UI Window

  QApplication app2(argc, argv);
  QWidget window2;
  window2.setWindowTitle("Adjust Parameters");
  window2.resize(200,720);
  window2.move(1025,25);
  QVBoxLayout* layout = new QVBoxLayout(&window2);

  //QT Sliders

  QSlider* boidForceSlider = new QSlider(Qt::Horizontal, &window2);
  boidForceSlider->setRange(10, 100);
  boidForceSlider->setValue(10);

  QSlider* boidSpeedSlider = new QSlider(Qt::Horizontal, &window2);
  boidSpeedSlider->setRange(10, 100);
  boidSpeedSlider->setValue(50);

  QSlider* flockDesiredSeparationSlider = new QSlider(Qt::Horizontal, &window2);
  flockDesiredSeparationSlider->setRange(10, 100);
  flockDesiredSeparationSlider->setValue(40);

  QSlider* flockAlignmentDistSlider = new QSlider(Qt::Horizontal, &window2);
  flockAlignmentDistSlider->setRange(10, 100);
  flockAlignmentDistSlider->setValue(10);

  QSlider* flockCohesionDistSlider = new QSlider(Qt::Horizontal, &window2);
  flockCohesionDistSlider->setRange(10, 100);
  flockCohesionDistSlider->setValue(10);


  QObject::connect(boidSpeedSlider, &QSlider::valueChanged, &window, &NGLScene::setBoidSpeed);
  QObject::connect(boidForceSlider, &QSlider::valueChanged, &window, &NGLScene::setBoidForce);
  QObject::connect(flockDesiredSeparationSlider, &QSlider::valueChanged, &window, &NGLScene::setFlockDesiredSeparaion);
  QObject::connect(flockAlignmentDistSlider, &QSlider::valueChanged, &window, &NGLScene::setFlockAlignmentDist);
  QObject::connect(flockCohesionDistSlider, &QSlider::valueChanged, &window, &NGLScene::setFlockCohesionDist);


  //QT Labels
  QLabel* boidSpeedLabel = new QLabel("Boid Max Speed");
  QLabel* boidForceLabel = new QLabel("Boid Max Force");
  QLabel* FlockSeparationLabel = new QLabel("Desired Separation");
  QLabel* FlockAlignmentLabel = new QLabel("Alignment Distance");
  QLabel* FlockCohesionLabel = new QLabel("Cohesion Distance");

  layout->addWidget(boidSpeedLabel, 0, Qt::AlignTop);
  layout->addWidget(boidSpeedSlider, 0, Qt::AlignTop);

  layout->addWidget(boidForceLabel, 0, Qt::AlignTop);
  layout->addWidget(boidForceSlider, 0, Qt::AlignTop);

  layout->addWidget(FlockSeparationLabel, 0, Qt::AlignTop);
  layout->addWidget(flockDesiredSeparationSlider, 0, Qt::AlignTop);

  layout->addWidget(FlockAlignmentLabel, 0, Qt::AlignTop);
  layout->addWidget(flockAlignmentDistSlider, 0, Qt::AlignTop);

  layout->addWidget(FlockCohesionLabel, 0, Qt::AlignTop);
  layout->addWidget(flockCohesionDistSlider, 0, Qt::AlignTop);

  // and finally show
  window2.show();
  window.show();


  return app.exec() && app2.exec();
}



