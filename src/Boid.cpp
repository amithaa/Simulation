/// @file Boid.cpp
///

#include <vector>
#include <QtDebug>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <time.h>
#include <stdlib.h>
#include "Boid.h"

#define size 2


Boid::Boid()
{

}

Boid::Boid(float _positionx, float _positiony, float _positionz,bool isleader)
{
  setposition(_positionx, _positiony, _positionz);
  srand(time(NULL));
  setvelocity(rand()%size,rand()%size,rand()%size);
  leader=isleader;
}

Boid::~Boid()
{

}




