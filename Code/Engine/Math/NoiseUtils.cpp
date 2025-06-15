#include "Engine/Math/NoiseUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Easing.hpp"
#include "ThirdParty/Squirrel/RawNoise.hpp"
float Get2dRidgeNoise(int indexX, int indexY, unsigned int seed)
{
    float rawNoise = Get2dNoiseZeroToOne(indexX, indexY, seed);
    return 2.f * (0.5f - abs(0.5f - rawNoise));
}

float Compute2dRidgeNoise(float posX, float posY, float scale/*=1.f*/, unsigned int numOctaves/*=1*/, float octavePersistence/*=0.5f*/, float octaveScale/*=2.f*/, bool renormalize/*=true*/, unsigned int seed/*=0 */)
{
    const float OCTAVE_OFFSET = 0.636764989593174f; // Translation/bias to add to each octave

    float totalNoise = 0.f;
    float totalAmplitude = 0.f;
    float currentAmplitude = 1.f;
    float invScale = (1.f / scale);
    Vec2 currentPos(posX * invScale, posY * invScale);
    float weight = 1.0f;  // Weight for ridge noise modification

    for (unsigned int octaveNum = 0; octaveNum < numOctaves; ++octaveNum)
    {
        // Determine noise values at nearby integer "grid point" positions
        Vec2 cellMins(floorf(currentPos.x), floorf(currentPos.y));
        int indexWestX = (int)cellMins.x;
        int indexSouthY = (int)cellMins.y;
        int indexEastX = indexWestX + 1;
        int indexNorthY = indexSouthY + 1;
        float valueSouthWest = Get2dRidgeNoise(indexWestX, indexSouthY, seed);
        float valueSouthEast = Get2dRidgeNoise(indexEastX, indexSouthY, seed);
        float valueNorthWest = Get2dRidgeNoise(indexWestX, indexNorthY, seed);
        float valueNorthEast = Get2dRidgeNoise(indexEastX, indexNorthY, seed);

        // Do a smoothed (nonlinear) weighted average of nearby grid point values
        Vec2 displacementFromMins = currentPos - cellMins;
        float weightEast = SmoothStep3(displacementFromMins.x);
        float weightNorth = SmoothStep3(displacementFromMins.y);
        float weightWest = 1.f - weightEast;
        float weightSouth = 1.f - weightNorth;

        float blendSouth = (weightEast * valueSouthEast) + (weightWest * valueSouthWest);
        float blendNorth = (weightEast * valueNorthEast) + (weightWest * valueNorthWest);
        float blendTotal = (weightSouth * blendSouth) + (weightNorth * blendNorth);
        float noiseThisOctave = 2.f * (blendTotal - 0.5f); // Map from [0,1] to [-1,1]

        // Apply weight for ridged noise (emphasizes ridges)
        noiseThisOctave *= weight;

        // Update weight based on current noise value for next octave
        weight = GetClamped(noiseThisOctave * 0.5f + 0.5f, 0.0f, 1.0f);

        // Accumulate results and prepare for next octave (if any)
        totalNoise += noiseThisOctave * currentAmplitude;
        totalAmplitude += currentAmplitude;
        currentAmplitude *= octavePersistence;
        currentPos *= octaveScale;
        currentPos.x += OCTAVE_OFFSET; // Add "irrational" offsets to noise position components
        currentPos.y += OCTAVE_OFFSET; //	at each octave to break up their grid alignment
        ++seed; // Eliminates octaves "echoing" each other (since each octave is uniquely seeded)
    }

    if (renormalize && totalAmplitude > 0.f)
    {
        totalNoise /= totalAmplitude;				// Amplitude exceeds 1.0 if octaves are used!
        totalNoise = (totalNoise * 0.5f) + 0.5f;	// Map to [0,1]
        totalNoise = SmoothStep3(totalNoise);		// Push towards extents (octaves pull us away)
        totalNoise = (totalNoise * 2.0f) - 1.f;		// Map back to [-1,1]
    }
    return totalNoise;
}
