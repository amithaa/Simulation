/// @file GLWindow.cpp
/// @brief Defines the methods to create the window

#include <iostream>
#include <QColorDialog>
#include <QMouseEvent>
#include <QGuiApplication>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/Mat4.h>
#include <ngl/Material.h>
#include <ngl/Obj.h>
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Vec3.h>
#include "GLWindow.h"
#include "Boid.h"
#include "Flock.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;


//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ) : QGLWidget( _format, _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;

  //m_clothTimer = startTimer(0);
}


// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::initializeGL()
{

  ngl::NGLInit::instance();

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  //ngl::Vec3 from(0,5,2);
  ngl::Vec3 from(0,0,20);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(120,(float)720.0/576.0,0.5,50);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  shader->setShaderParam1i("Normalize",1);

  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::BLACKPLASTIC);
  m.loadToShader("material");
  ngl::Light light(ngl::Vec3(2,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  light.setTransform(iv);
  light.setAttenuation(1,0,0);
  light.enable();
  light.loadToShader("light");

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createCone("cone",0.06,0.1,10,10);

  m_flock=new Flock();
  m_flock->setCam(m_cam);
  m_flock->drawBoid();
  m_flockTimer=startTimer(0);
 // m_cohesion = 7;
 // m_separation = 1;
 // m_alignment = 5;

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL( int _w, int _h )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,float(_w/_h),0.1,300);

}


void GLWindow::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix();
  MV=m_transform.getMatrix()*m_cam->getViewMatrix();
  MVP=MV*m_cam->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ngl::Transformation trans;
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  m_transform.reset();

  // multiply the rotations

  // create the rotation matrices

  m_flock->drawBoid();

  //move boids to new positions
  ngl::Vec3 v1, v2, v3, p1, p2, p3;

  //for every boid, calculate cohesion, alignment and separation
  for( unsigned int i = 0; i < m_flock->BoidList.size(); ++i)
  {

    v1 = m_flock->boidCohesion(m_flock->BoidList[i]);
    v2 = m_flock->boidSeparation(m_flock->BoidList[i]);
    v3 = m_flock->boidAlignment(m_flock->BoidList[i]);
    //p1 = m_flock->boundingBox(m_flock->BoidList[i]);
    p2 = m_flock->tendToPlace(m_flock->BoidList[i]);
    p3 = m_flock->dampingForce(m_flock->BoidList[i]);


    m_flock->BoidList[i]->setacceleration(2* v1 + 0.8 * v2 +2* v3 + p3);
    //m_flock->BoidList[i]->getvelocity().normalize();


    //euler integration
    m_flock->BoidList[i]->setvelocity(m_flock->BoidList[i]->getvelocity() + m_flock->BoidList[i]->getacceleration() * 0.008);

    m_flock->BoidList[i]->setposition(m_flock->BoidList[i]->getposition() + m_flock->BoidList[i]->getvelocity() * 0.008);

    //limit the velocity
    m_flock->boidLimitVelocity(m_flock->BoidList[i]);

    //bounding box
    m_flock->boundingBox(m_flock->BoidList[i]);

    //ngl::Vec3 temp2 = m_flock->BoidList[i]->getposition() + m_flock->BoidList[i]->getvelocity();
    //temp2.normalize();
   // m_flock->BoidList[i]->setposition(temp2);

  }
}




//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent ( QMouseEvent * _event )
{
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    updateGL();

  }
        // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    updateGL();

   }
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (QMouseEvent * _event  )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (  QMouseEvent * _event  )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

	// check the diff of the wheel position (0 means no change)
	if(_event->delta() > 0)
	{
		m_modelPos.m_z+=ZOOM;
	}
	else if(_event->delta() <0 )
	{
		m_modelPos.m_z-=ZOOM;
	}
	updateGL();
}
//----------------------------------------------------------------------------------------------------------------------

void GLWindow::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
    updateGL();
}



GLWindow::~GLWindow()
{
	ngl::NGLInit *init = ngl::NGLInit::instance();
	init->NGLQuit();
}


void GLWindow::timerEvent(QTimerEvent *_event)
{
	if(_event->timerId() ==  m_flockTimer)
	{
		updateGL();
	}
}

