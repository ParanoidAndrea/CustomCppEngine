#pragma once
struct Vec2;
struct EulerAngles;
struct IntVec3;
struct Vec3
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	static const Vec3 ZERO;

public:
	// Construction/Destruction
	~Vec3() {}												// destructor (do nothing)
	Vec3() {}												// default constructor (do nothing)
	explicit Vec3(float initialX, float initialY, float initialZ);		// explicit constructor
	explicit Vec3(Vec2 const& vect2);
	explicit Vec3(Vec2 const& vect2, float initialZ);
	explicit Vec3(IntVec3 const& copyFrom);
	// Accessors
	static Vec3 const Min(const Vec3& a, const Vec3& b) ;
	static Vec3 const Max(const Vec3& a, const Vec3& b) ;
	float GetLength() const;
	float GetLengthXY() const;
	float GetLengthSquared() const;
	float GetLengthXYSquared() const;
	float GetAngleAboutZRadians() const;
	float GetAngleAboutZDegrees() const;
	Vec3 const GetRotatedAboutZRadians(float deltaRadians) const;
	Vec3 const GetRotatedAboutZDegrees(float deltaDegrees) const;
	Vec3 const GetClamped(float maxLength) const;
	Vec3 const GetNormalized() const;
	void Normalize();
	static Vec3 const MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length = 1.f);
	static Vec3 const MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length = 1.f);
	bool SetFromText(const char* text);
	EulerAngles const GetEulerAngle() const;

	// Operators
	bool operator==(Vec3 const& compare) const;
	bool operator!=(Vec3 const& compare) const;
	Vec3 const operator+(Vec3 const& VecToAdd) const;
	Vec3 const operator-(Vec3 const& vecToSubtract) const;
	Vec3 const operator-() const;
	Vec3 const operator*(float uniformScale) const;
	Vec3 const operator*(const Vec3& vecToMultiply) const;
	Vec3 const operator/(float inverseScale) const;
	Vec3 const operator/(Vec3 inverseVector) const;

	// Operators(self-mutating)
	void operator+=(Vec3 const& vecToAdd);
	void operator-=(Vec3 const& vecToSubtract);
	void operator*=(float uniformScale);
	void operator/=(float uniformDivisor);
	void operator=(Vec3 const& copyfrom);
	// friend functions
	friend Vec3 const operator*(float uniformScale, Vec3 const& vecToScale);
};