#include "Engine/Math/Convex.hpp"
#include "Engine/Math/MathUtils.hpp"
#define UNUSED(x) (void)(x)

ConvexPoly2::ConvexPoly2(std::vector<Vec2> const& vertexPositions)
    :m_vertexPositions(vertexPositions)
{
}

ConvexPoly2::ConvexPoly2(ConvexHull2 const& convexHull)
{
    UNUSED(convexHull);
}

std::vector<Vec2> const ConvexPoly2::GetVertexPositions() const
{
    return m_vertexPositions;
}

unsigned char ConvexPoly2::GetVertexNumber() const
{
    return static_cast<unsigned char>(m_vertexPositions.size());
}

void ConvexPoly2::Translate(Vec2 const& translation)
{
    for (size_t i = 0; i < m_vertexPositions.size(); ++i)
    {
        m_vertexPositions[i] += translation;
    }
}

void ConvexPoly2::ScaleAroundPosition(Vec2 const& relativePos, float scale)
{
    for (size_t i = 0; i < m_vertexPositions.size(); ++i)
    {
        Vec2 displacement = m_vertexPositions[i] - relativePos;
        displacement *= scale;
        m_vertexPositions[i] = relativePos + displacement;
    }
}


void ConvexPoly2::RotateAroundPosition(Vec2 const& relativePos, float rotateDegree)
{
    Mat44 rotationMatrix = Mat44::CreateZRotationDegrees(rotateDegree);
    for (size_t i = 0; i < m_vertexPositions.size(); ++i)
    {
        Vec2 displacement = m_vertexPositions[i] - relativePos;
        displacement = rotationMatrix.TransformVectorQuantity2D(displacement);
        m_vertexPositions[i] = relativePos + displacement;
    }
}

void ConvexPoly2::RotateWithDiscAroundPosition(Vec2 const& relativePos, float rotateDegree, Vec2& disCenter)
{
    Mat44 rotationMatrix = Mat44::CreateZRotationDegrees(rotateDegree);
    for (size_t i = 0; i < m_vertexPositions.size(); ++i)
    {
        Vec2 displacement = m_vertexPositions[i] - relativePos;
        displacement = rotationMatrix.TransformVectorQuantity2D(displacement);
        m_vertexPositions[i] = relativePos + displacement;
    }
    Vec2 relativeToDisCenter = disCenter - relativePos;
    relativeToDisCenter = rotationMatrix.TransformVectorQuantity2D(relativeToDisCenter);
    disCenter = relativePos + relativeToDisCenter;
}

ConvexHull2::ConvexHull2(std::vector<Plane2> const& planes)
    :m_boundingPlanes(planes)
{
}

ConvexHull2::ConvexHull2(ConvexPoly2 const& convexPoly)
{
    std::vector<Vec2> const& vertexPositions = convexPoly.GetVertexPositions();
    size_t numVerts = vertexPositions.size();
    m_boundingPlanes.reserve(numVerts);
    for (size_t i = 0; i < vertexPositions.size()-1; ++i)
    {
        Vec2 normal = (vertexPositions[i + 1] - vertexPositions[i]).GetNormalized();
        normal.RotateMinus90Degrees();
        float distanceFromOrigin = DotProduct2D(normal, vertexPositions[i]);
        Plane2 plane;
        plane.m_normal = normal;
        plane.m_distFromOriginAlongNormal = distanceFromOrigin;
        m_boundingPlanes.push_back(plane);
    }
    Vec2 normal = (vertexPositions[0] - vertexPositions[numVerts - 1]).GetNormalized();
    normal.RotateMinus90Degrees();
    float distanceFromOrigin = DotProduct2D(normal, vertexPositions[numVerts - 1]);
    Plane2 plane;
    plane.m_normal = normal;
    plane.m_distFromOriginAlongNormal = distanceFromOrigin;
    m_boundingPlanes.push_back(plane);
}
