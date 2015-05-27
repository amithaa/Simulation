/// @file Flock.cpp
///
#include <vector>
#include <math.h>
#include <QtDebug>
#include <ngl/Vec3.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include "Flock.h"
#include "Boid.h"


Flock::Flock()
{
  bool isleader=false;
  for(int i = 0; i < 3; ++i)
  {
    for(int j = 0; j < 3; ++j)
    {
      for(int k = 0; k < 3; ++k)
      {
        if((i==0) && (j==0) && (k==0))
          isleader=true;
        BoidList.push_back(new Boid(i , j , k,isleader));
      }
    }
  }
}


void Flock::drawBoid()
{
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation transform;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Phong");

  for(unsigned int i = 0; i < BoidList.size(); ++i)
  {
    transform.setPosition(ngl::Vec3(BoidList[i]->getposition().m_x, BoidList[i]->getposition().m_y, BoidList[i]->getposition().m_z));
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=transform.getMatrix();
    MV=transform.getMatrix()*getCam()->getViewMatrix();
    MVP=MV*getCam()->getProjectionMatrix() ;
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",M);
    prim->draw("cone");
  }
}

void Flock::findNeighbours(Boid b) //call this method for each boid
{
  //implement after basic method is over
}

ngl::Vec3 Flock::boidCohesion(Boid *b)
{
  ngl::Vec3 sum(0,0,0);
  float neighbourdistance = 5;
  int count = 0;
  ngl::Vec3 newdirection(0,0,0);

  for(unsigned int i = 0; i < BoidList.size(); ++i)
  {
    ngl::Vec3 dist = b->getposition() - BoidList[i]->getposition();
    int d = abs(dist.length());

    if(b != BoidList[i] && d < neighbourdistance)
    {
      sum += BoidList[i]->getposition();
      count++;
    }
  }

  if(count > 0)
  {
    sum /= count;
    //calculate a vector from the COM to ourselves
    newdirection = sum - b->getposition();
    newdirection.normalize(); //this is the first vector offset
    //newdirection /= 100;
  }

  return newdirection;
}

ngl::Vec3 Flock::boidSeparation(Boid *b)
{
  float separationdistance = 5;
  ngl::Vec3 newdirection(0,0,0);
  for(unsigned int i = 0; i < BoidList.size(); ++i)
  {
    if( b != BoidList[i] )
    {
      ngl::Vec3 d = BoidList[i]->getposition() - b->getposition();
      float dist = d.length();
      if( dist < separationdistance )
      {
        //calculate the heading vector
        ngl::Vec3 headingvector = b->getposition() - BoidList[i]->getposition();
        //newdirection.normalize();

        //calculate the scale
        float scale = headingvector.length() / separationdistance;

        //the closer we are the more the force vector will be
        headingvector.normalize();
        newdirection = headingvector / scale;
      }
    }
  }

  return newdirection;
}


ngl::Vec3 Flock::boidAlignment(Boid *b)
{
  ngl::Vec3 newdirection(0,0,0);
  int count = 0;
  float neighbourdistance = 5;

  for(unsigned int i = 0; i < BoidList.size(); ++i)
  {
    ngl::Vec3 dist = b->getposition() - BoidList[i]->getposition();
    int d = abs(dist.length());
    if( b != BoidList[i] && d < neighbourdistance)
    {
      newdirection += BoidList[i]->getvelocity();
      count ++;
    }

    if(count > 0)
    {
      newdirection /= count;
      newdirection.normalize();
      //newdirection /= 8;
    }
  }

  return newdirection;
}


void Flock::boidLimitVelocity(Boid *b)
{
  double vlim = 5;
  ngl::Vec3 v;

  v = b->getvelocity();

  if(v.m_x > vlim || v.m_y > vlim || v.m_z > vlim)
  {
    double scalar = b->getvelocity().length();
    b->setvelocity( (b->getvelocity() / scalar) * vlim);
  }
}


void Flock::boundingBox(Boid *b)
{
  double r=10;
  ngl::Vec3 v(0,0,0);

  if(b->getposition().m_x < -r)
    b->setposition(b->getposition().m_x + r, b->getposition().m_y, b->getposition().m_z);

  else if(b->getposition().m_x > r)
    b->setposition(-r, b->getposition().m_y, b->getposition().m_z);

  if(b->getposition().m_y < -r)
    b->setposition(b->getposition().m_x, b->getposition().m_y + r, b->getposition().m_z);

  else if(b->getposition().m_y > r)
    b->setposition(b->getposition().m_x, -r, b->getposition().m_z);

  if(b->getposition().m_z < -r)
    b->setposition(b->getposition().m_x, b->getposition().m_y, b->getposition().m_z + r);

  else if(b->getposition().m_z > r)
    b->setposition(b->getposition().m_x, b->getposition().m_y, -r);

}

ngl::Vec3 Flock::tendToPlace(Boid *b)
{
  ngl::Vec3 place(0,0,0);

  return (place - b->getposition()) / 90;
}

ngl::Vec3 Flock::dampingForce(Boid *b)
{
  ngl::Vec3 force_direction(0,0,0);
  force_direction = b->getvelocity();

  return - force_direction * 0.2;
}
