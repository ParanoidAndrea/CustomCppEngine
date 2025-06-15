#pragma once
#include "ThirdParty/Squirrel/SmoothNoise.hpp"

float Get2dRidgeNoise(int indexX, int indexY, unsigned int seed );
float Compute2dRidgeNoise( float posX, float posY, float scale=1.f, unsigned int numOctaves=1, float octavePersistence=0.5f, float octaveScale=2.f, bool renormalize=true, unsigned int seed=0 );