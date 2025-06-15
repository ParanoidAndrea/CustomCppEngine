#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Plane2.hpp"
#include <vector>
class ConvexHull2;

class ConvexPoly2
{
public:
    ConvexPoly2() = default;
    ConvexPoly2(std::vector<Vec2> const& vertexPositions);
    explicit ConvexPoly2(ConvexHull2 const& convexHull);
    std::vector<Vec2> const GetVertexPositions() const;
    unsigned char GetVertexNumber() const;
    void Translate(Vec2 const& translation);
    void ScaleAroundPosition(Vec2 const& relativePos, float scale);
    void RotateAroundPosition(Vec2 const& relativePos, float rotateDegree);
    void RotateWithDiscAroundPosition(Vec2 const& relativePos, float rotateDegree, Vec2& disCenter);

private:
    std::vector<Vec2> m_vertexPositions; //They are in specific order like counter-wise and in positive or negative theta
};

class ConvexHull2
{
public:
    ConvexHull2() = default;
    ConvexHull2(std::vector <Plane2> const& planes);
    explicit ConvexHull2(ConvexPoly2 const& convexPoly);
public:
    std::vector<Plane2> m_boundingPlanes;

};