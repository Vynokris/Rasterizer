#include "my_math.hpp"

//* ------------------------------ ARITHMECTIC ------------------------------ *//

// --------- ARITHMETIC FUNCTIONS -------- //

// Fast inverse square root from Quake III.
float arithmetic::Q_rsqrt(const float& number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = * ( long * ) &y; // evil floating point bit level hacking
    i = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y = * ( float * ) &i;
    y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
//	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

    return y;
}

// Rounds the given value to the nearest int.
int arithmetic::roundInt(const float& val) { return (int)round(val); }

// Rounds down the given value.
int arithmetic::floorInt(const float& val) { return (int)floor(val); }

// Rounds up the given value.
int arithmetic::ceilInt(const float& val) { return (int)ceil(val); }

// Returns the sqare power of the given value.
float arithmetic::sqpow(const float& val) { return val * val; }

// Returns 1 if the given value is positive or null, and -1 if it is negative.
int arithmetic::signOf(const float& val) { if (val == 0) return 1; return val / abs((int)val); }

// Converts the given angle from degrees to radians.
float arithmetic::degToRad(const float& deg) { return deg * (PI / 180.0f); }

// Converts the given angle from radians to degrees.
float arithmetic::radToDeg(const float& rad) { return rad * (180.0f / PI); }

// Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.
float arithmetic::clamp(float val, const float& min, const float& max)
{
    assert (min <= max); 
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

// Clamps the given value to be inferior or equal to the maximum value.
float arithmetic::clampUnder(float val, const float& max) { if (val > max) val = max; return val; }

// Clamps the given value to be superior or equal to the minimum value.
float arithmetic::clampAbove(float val, const float& min) { if (val < min) val = min; return val; }

// Compute linear interpolation between start and end for the parameter val (if 0 <= val <= 1: start <= return <= end).
float arithmetic::lerp(const float& val, const float& start, const float& end)
{
    return start + val * (end - start);
}

// Remaps the given value from one range to another.
float arithmetic::remap(const float& val, const float& inputStart, const float& inputEnd, const float& outputStart, const float& outputEnd)
{
    return outputStart + (val - inputStart) * (outputEnd - outputStart) / (inputEnd - inputStart);
}

// Returns true if the given number is a power of 2.
bool arithmetic::isPowerOf2(const int& val)
{
    return val == (int)pow(2, (int)(log(val)/log(2)));
}

// Returns the closest power of 2 that is inferior or equal to val.
int arithmetic::getPowerOf2Under(const int& val)
{
    return (int)pow(2, (int)(log(val)/log(2)));
}

// Returns the closest power of 2 that is superior or equal to val.
int arithmetic::getPowerOf2Above(const int& val)
{
    if (isPowerOf2(val)) return (int)pow(2, (int)(log(val)/log(2)));
    else                 return (int)pow(2, (int)(log(val)/log(2)) + 1);
} 






//* ------------------------------ GEOMETRY 2D ------------------------------ *//

// --------------- VECTOR2 -------------- //

using namespace geometry2D;


// Calculates linear interpolation for a value from a start point to an end point.
Vector2 geometry2D::point2Lerp(const float& val, const Vector2& start, const Vector2& end)
{
    return Vector2(arithmetic::lerp(val, start.x, end.x), 
                   arithmetic::lerp(val, start.y, end.y));
}

// Constructors.
Vector2::Vector2()                                                           : x(0),                 y(0)                 {}; // Null vector.
Vector2::Vector2(const float& _x, const float& _y)                           : x(_x),                y(_y)                {}; // Vector with 2 coordinates.
Vector2::Vector2(const Vector2& p1, const Vector2& p2)                       : x(p2.x - p1.x),       y(p2.y - p1.y)       {}; // Vector from 2 points.
Vector2::Vector2(const Segment2& seg)                                        : x(seg.b.x - seg.a.x), y(seg.b.y - seg.a.y) {}; // Vector from Segment2.
Vector2::Vector2(const float& rad, const float& length, const bool& isAngle) : x(cos(rad) * length), y(sin(rad) * length) {}; // Vector from angle (useless bool).

// ---------- VECTOR2 OPERATORS ---------- //

// Copy constructor.
void Vector2::operator=(const Vector2& other)      { x = other.x; y = other.y; }

// Vector2 dot product.
float Vector2::operator&(const Vector2& val) const { return (x * val.x) + (y * val.y); }

// Vector2 cross product.
float Vector2::operator^(const Vector2& val) const { return (x * val.y) - (y * val.x); }

// ------------ VECTOR2 METHODS ----------- //

// Returns the middle of the given vector.
Vector2 Vector2::getMiddle()                          const { return Vector2(x / 2, y / 2); }

// Returns the length of the given vector.
float Vector2::getLength()                            const { return sqrt(arithmetic::sqpow(x) + arithmetic::sqpow(y)); }

// Modifies the length of the given vector to correspond to the given value.
void Vector2::setLength(const float& length)                { *(this) = Vector2(this->getAngle(), length, true); }

// Normalizes the given vector so that its length is 1.
void Vector2::normalize()                                   { x /= this->getLength(); y /= this->getLength(); }

// Normalizes the given vector so that its length is 1.
Vector2 Vector2::getNormalized()                      const { return Vector2(x / this->getLength(), y / this->getLength()); }

// Negates both of the coordinates of the given vector.
void Vector2::negate()                                      { *(this) = Vector2(-x, -y); }

// Negates both of the coordinates of the given vector.
Vector2 Vector2::getNegated()                         const { return Vector2(-x, -y); }

// Copies the signs from the source vector to the destination vector.
void Vector2::copysign(const Vector2& source)               { *(this) = Vector2(std::copysign(x, source.x), std::copysign(y, source.y)); }

// Copies the signs from the source vector to the destination vector.
Vector2 Vector2::getCopiedSign(const Vector2& source) const { return Vector2(std::copysign(x, source.x), std::copysign(y, source.y)); }

// Returns the normal of a given vector.
Vector2 Vector2::getNormal()                          const { return Vector2(-y, x); }

// Interprets the vector as a point and returns the distance to another point.
float Vector2::getDistanceFromPoint(const Vector2& p) const { return Vector2(*this, p).getLength(); }

// Returns the angle (in radians) of the given vector.
float Vector2::getAngle()                             const { return std::copysign(std::acos(this->getNormalized().x), std::asin(this->getNormalized().y)); }

// Returns the angle (in radians) between two vectors.
float Vector2::getAngleWithVector2(const Vector2& v)  const
{
    float this_angle = this->getAngle();
    float v_angle    = v.getAngle();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}

// Rotates the given vector by the given angle.
void Vector2::rotate(const float& angle)
{
    float this_length = this->getLength();
    float this_angle  = this->getAngle();
    *(this) = Vector2(this_angle + angle, this_length, true);
}



// -------------------- SEGMENT2 -------------------- //

// Constructors.
Segment2::Segment2()                                                              : a(Vector2()), b(Vector2())      {}; // Null Segment2.
Segment2::Segment2(const Vector2& _a, const Vector2& _b)                          : a(_a),        b(_b)             {}; // Segment2 from points.
Segment2::Segment2(const Vector2& origin, const Vector2& vec, const bool& vector) : a(origin),      b(origin + vec) {}; // Segment2 from point and vector.

// ---------------- SEGMENT2 METHODS --------------- //

// Returns the center of mass of the Segment2.
Vector2 Segment2::getCenterOfMass() const
{
    return Vector2((a.x + b.x) / 2, (a.y +b.y) / 2);
}

// Returns the number of sides of the Segment2.
int Segment2::getSidesNum() const { return 1; }

// Returns the side of the Segment2 that corresponds to the given index.
Segment2 Segment2::getSide(const int& index) const
{
    assert (0 <= index && index < 1);
    return *this;
}

// Returns the number of vertices of the Segment2.
int Segment2::getVerticesNum() const { return 2; }

// Returns the vertex of the Segment2 that corresponds to the given index.
Vector2 Segment2::getVertex(const int& index) const
{
    assert (0 <= index && index < 2);

    switch (index)
    {
        case 0:  return a;
        case 1:  return b;
        default: return Vector2();
    }
}

// Moves the Segment2 by the given vector.
void Segment2::move(const Vector2& vec)
{
    a += vec;
    b += vec;
}

// -------------------- TRIANGLE -------------------- //

// Constructor.
Triangle2::Triangle2()                                                        : a(Vector2()), b(Vector2()), c(Vector2()) {}; // Null triangle.
Triangle2::Triangle2(const Vector2& _a, const Vector2& _b, const Vector2& _c) : a(_a),        b(_b),        c(_c)        {}; // Triangle2 from points.

// ---------- TRIANGLE METHODS ---------- //

// Returns the center of mass of the triangle.
Vector2 Triangle2::getCenterOfMass() const { return Vector2((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3); }

// Returns the number of sides of the triangle.
int Triangle2::getSidesNum() const { return 3; }

// Returns the side of the triangle that corresponds to the given index.
Segment2 Triangle2::getSide(const int& index) const
{
    assert (0<= index && index < 3);

    switch (index)
    {
        case 0:   return Segment2(a, b);
        case 1:   return Segment2(b, c);
        case 2:   return Segment2(c, a);
        default:  return Segment2(Vector2(), Vector2());
    }
}

// Returns the number of vertices of the triangle.
int Triangle2::getVerticesNum() const { return 3; }

// Returns the vertex of the triangle that corresponds to the given index.
Vector2 Triangle2::getVertex(const int& index) const
{
    assert (0 <= index && index < 3);

    switch (index)
    {
    case 0:  return a;
    case 1:  return b;
    case 2:  return c;
    default: return Vector2();
    }
}

// Moves the triangle by the given vector.
void Triangle2::move(const Vector2& vec)
{
    a += vec;
    b += vec;
    c += vec;
}



// -------------------- RECTANGLE -------------------- //

// Constructor.
Rectangle::Rectangle()                                                                    : origin(Vector2()), width(0),      height(0)       {}; // Null rectangle.
Rectangle::Rectangle(const Vector2& _origin, const double& _width, const double& _height) : origin(_origin),   width(_width), height(_height) {}; // Rectangle from origin, width and height.

// ---------- RECTANGLE METHODS ---------- //

// Returns the center of mass of the rectangle.
Vector2 Rectangle::getCenterOfMass() const { return Vector2(origin.x + width / 2, origin.y + height / 2); };

// Returns the number of sides of the rectangle.
int Rectangle::getSidesNum() const { return 4; }

// Returns the side of the rectangle that corresponds to the given index.
Segment2 Rectangle::getSide(const int& index) const
{
    assert (0 <= index && index < 4);

    switch (index)
    {
        case 0:  return Segment2(Vector2(origin.x + width, origin.y)         , origin);
        case 1:  return Segment2(origin                                        , Vector2(origin.x, origin.y + height));
        case 2:  return Segment2(Vector2(origin.x, origin.y + height)        , Vector2(origin.x + width, origin.y + height));
        case 3:  return Segment2(Vector2(origin.x + width, origin.y + height), Vector2(origin.x + width, origin.y));
        default: return Segment2(Vector2(), Vector2());
    }
}

// Returns the number of vertices of the rectangle.
int Rectangle::getVerticesNum() const { return 4; }

// Returns the vertex of the rectangle that corresponds to the given index.
Vector2 Rectangle::getVertex(const int& index) const
{
    assert (0 <= index && index < 4);

    switch (index)
    {
        case 0:  return Vector2(origin.x + width, origin.y);
        case 1:  return origin;
        case 2:  return Vector2(origin.x, origin.y + height);
        case 3:  return Vector2(origin.x + width, origin.y + height);
        default: return Vector2();
    }
}

// Moves the rectangle by the given vector.
void Rectangle::move(const Vector2& vec) { origin += vec; }

// -------------------- POLYGON -------------------- //

// Constructor.
Polygon::Polygon()                                                                                          : origin(Vector2()), radius(0),       rotation(0),         sides(3)      {}; // Null polygon.
Polygon::Polygon(const Vector2& _origin, const double& _radius, const double& _rotation, const int& _sides) : origin(_origin),   radius(_radius), rotation(_rotation), sides(_sides) {}; // Polygon from origin, radius, rotation and side amount.

// ---------- POLYGON METHODS ---------- //

// Returns the center of mass of the polygon.
Vector2 Polygon::getCenterOfMass() const { return origin; }

// Returns the number of sides of the polygon.
int Polygon::getSidesNum() const { return sides; }

// Returns the side of the polygon that corresponds to the given index.
Segment2 Polygon::getSide(const int& index) const
{
    assert(0 <= index && index < sides);

    double corner_angle    = arithmetic::degToRad(360 / sides);
    double angle_offset    = PI / 2 + (index * corner_angle);
    Vector2 poly_point_a = origin + Vector2(angle_offset + rotation, radius, true);
    Vector2 poly_point_b = origin + Vector2(angle_offset + corner_angle + rotation, radius, true);

    return Segment2(poly_point_a, poly_point_b);
}

// Returns the number of vertices of the polygon.
int Polygon::getVerticesNum() const { return sides; }

// Returns the vertex of the polygon that corresponds to the given index.
Vector2 Polygon::getVertex(const int& index) const
{
    assert (0 <= index && index < sides);
    return this->getSide(index).a;
}

// Moves the polygon by the given vector.
void Polygon::move(const Vector2& vec) { origin += vec; }



// -------------------- CIRCLE -------------------- //

// Constructor.
Circle::Circle()                                              : origin(Vector2()),  radius(0)       {}; // Null circle.
Circle::Circle(const Vector2& _origin, const double& _radius) : origin(_origin),    radius(_radius) {}; // Circle from origin and radius.

// ---------- CIRCLE METHODS ---------- //

// Returns the center of mass of the circle.
Vector2 Circle::getCenterOfMass() const { return origin; }

// Returns the number of sides of the circle.
int Circle::getSidesNum() const { return 1; }

// Does nothing and returns a null Segment2.
Segment2 getSide(const int& index) { return Segment2(); }

// Returns the number of vertices of the circle.
int Circle::getVerticesNum() const { return 0; }

// Does nothing and returns a null vector.
Vector2 Circle::getVertex(const int& index) const { return Vector2(); }

// Moves the circle by the given vector.
void Circle::move(const Vector2& vec) { origin += vec; }







//* ------------------------------- GEOMETRY 3D ----------------------------- *//


using namespace geometry3D;

// Calculates linear interpolation for a value from a start point to an end point.
Vector3 geometry3D::point3Lerp(const float& val, const Vector3& start, const Vector3& end)
{
    return Vector3(arithmetic::lerp(val, start.x, end.x), 
                   arithmetic::lerp(val, start.y, end.y), 
                   arithmetic::lerp(val, start.z, end.z));
}



// -------------------- VECTOR3 -------------------- //

// Constructors.
Vector3::Vector3()                                                  : x(0),                 y(0),                 z(0)                 {}; // Null vector.
Vector3::Vector3(const float& _x, const float& _y, const float& _z) : x(_x),                y(_y),                z(_z)                {}; // Vector with 3 coordinates.
Vector3::Vector3(const Vector3& p1, const Vector3& p2)              : x(p2.x - p1.x),       y(p2.y - p1.y),       z(p2.z - p1.z)       {}; // Vector from 2 points.
Vector3::Vector3(const Segment3& seg)                               : x(seg.b.x - seg.a.x), y(seg.b.y - seg.a.y), z(seg.b.z - seg.a.z) {}; // Vector from Segment3.
Vector3::Vector3(const float& theta, const float& phi, const float& length, const bool& isAngle) // Vector from angle (useless bool).
{
    x = length * sinf(theta) * cosf(phi);
    y = length * cosf(theta);
    z = length * sinf(theta) * sinf(phi);
}

// ---------- VECTOR3 OPERATORS ---------- //

// Copy constructor.
void Vector3::operator=(const Vector3& v)          { x = v.x; y = v.y; z = v.z; }

// Vector3 dot product.
float Vector3::operator&(const Vector3& v)   const { return (x * v.x) + (y * v.y) + (z * v.z); }

// Vector3 cross product.
Vector3 Vector3::operator^(const Vector3& v) const { return Vector3((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x)); }

// ------------ VECTOR3 METHODS ----------- //

// Returns the middle of the given vector.
Vector3 Vector3::getMiddle()   const { return Vector3(x / 2, y / 2, z / 2); }

// Returns the length of the given vector.
float Vector3::getLength()     const { return sqrt(arithmetic::sqpow(x) + arithmetic::sqpow(y) + arithmetic::sqpow(z)); }

// Modifies the length of the given vector to correspond to the given value.
void Vector3::setLength(const float& length)                { *(this) = Vector3(getAngleTheta(), getAnglePhi(), length, true); }

// Normalizes the given vector so that its length is 1.
void Vector3::normalize() 
{
    float length = getLength();
    x /= length; 
    y /= length; 
    z /= length; 
}

// Normalizes the given vector so that its length is 1.
Vector3 Vector3::getNormalized() const 
{ 
    float length = getLength();
    return Vector3(x / length, y / length, z / length); 
}

// Negates both of the coordinates of the given vector.
void Vector3::negate()                                      { *(this) = Vector3(-x, -y, -z); }

// Negates both of the coordinates of the given vector.
Vector3 Vector3::getNegated()                         const { return Vector3(-x, -y, -z); }

// Copies the signs from the source vector to the destination vector.
void Vector3::copysign(const Vector3& source)               { *(this) = Vector3(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z)); }

// Copies the signs from the source vector to the destination vector.
Vector3 Vector3::getCopiedSign(const Vector3& source) const { return Vector3(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z)); }

// Interprets the vector as a point and returns the distance to another point.
float Vector3::getDistanceFromPoint(const Vector3& p) const { return Vector3(*this, p).getLength(); }

// Returns the angle (in radians) of the given vector.
float Vector3::getAngleTheta() const  {  return acosf(z / getLength()); }
float Vector3::getAnglePhi()   const 
{ 
    if (x > 0)
        return atanf(y / x);
    if (x < 0)
        return atanf(y / x) + PI;
    return PI / 2;
}

// Returns the angle (in radians) between two vectors.
float Vector3::getAngleThetaWithVector3(const Vector3& v) const
{
    float this_angle =   getAngleTheta();
    float v_angle    = v.getAngleTheta();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}
float Vector3::getAnglePhiWithVector3(const Vector3& v) const
{
    float this_angle =   getAnglePhi();
    float v_angle    = v.getAnglePhi();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}

// Rotates the given vector by the given angle.
void Vector3::rotate(const float& theta, const float& phi)  { *(this) = Vector3(getAngleTheta() + theta, getAnglePhi() + phi, getLength(), true); }

// Creates a Vector4 from this vector.
Vector4 Vector3::toVector4() { return Vector4(x, y, z, 1); }



// -------------------- VECTOR4 -------------------- //

// Constructors.
Vector4::Vector4()                                                                   : x(0),                 y(0),                 z(0),                 w(1)  {}; // Null vector.
Vector4::Vector4(const float& _x, const float& _y, const float& _z, const float& _w) : x(_x),                y(_y),                z(_z),                w(_w) {}; // Vector with 3 coordinates.
Vector4::Vector4(const Vector4& p1, const Vector4& p2, const float& _w)              : x(p2.x - p1.x),       y(p2.y - p1.y),       z(p2.z - p1.z),       w(_w) {}; // Vector from 2 points.
Vector4::Vector4(const Vector3&  vec, const float& _w)                               : x(vec.x - vec.x),     y(vec.y - vec.y),     z(vec.z - vec.z),     w(_w) {}; // Vector from Segment3.
Vector4::Vector4(const Segment3& seg, const float& _w)                               : x(seg.b.x - seg.a.x), y(seg.b.y - seg.a.y), z(seg.b.z - seg.a.z), w(_w) {}; // Vector from Segment3.
Vector4::Vector4(const float& theta, const float& phi, const float& length, const float& _w, const bool& isAngle) // Vector from angle (useless bool).
{
    x = length * sinf(theta) * cosf(phi);
    y = length * cosf(theta);
    z = length * sinf(theta) * sinf(phi);
    w = _w;
}

// ---------- VECTOR4 OPERATORS ---------- //

// Copy constructor.
void Vector4::operator=(const Vector4& v)          { x = v.x; y = v.y; z = v.z; w = v.w; }

// Vector4 dot product.
float Vector4::operator&(const Vector4& v)   const { return (x * v.x) + (y * v.y) + (z * v.z); }

// Vector4 cross product.
Vector3 Vector4::operator^(const Vector4& v) const { return Vector3((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x)); }

// ------------ VECTOR4 METHODS ----------- //

// Returns the middle of the given vector.
Vector4 Vector4::getMiddle() const { return Vector4(x / 2, y / 2, z / 2, w); }

// Homogenizes the vector4 by dividing it by w.
void Vector4::homogenize() { *this = Vector4(x/w, y/w, z/w, w/w); }

// Homogenizes the vector4 by dividing it by w.
Vector4 Vector4::getHomogenized() const { return Vector4(x/w, y/w, z/w, w/w); }

// Returns the length of the given vector.
float Vector4::getLength()        const { return sqrt(arithmetic::sqpow(x) + arithmetic::sqpow(y) + arithmetic::sqpow(z)); }

// Modifies the length of the given vector to correspond to the given value.
void Vector4::setLength(const float& length) { *(this) = Vector4(getAngleTheta(), getAnglePhi(), length, true); }

// Normalizes the given vector so that its length is 1.
void Vector4::normalize() 
{
    float length = getLength();
    x /= length; 
    y /= length; 
    z /= length; 
}

// Normalizes the given vector so that its length is 1.
Vector4 Vector4::getNormalized() const 
{ 
    float length = getLength();
    return Vector4(x / length, y / length, z / length, w); 
}

// Negates both of the coordinates of the given vector.
void Vector4::negate() { *(this) = Vector4(-x, -y, -z, w); }

// Negates both of the coordinates of the given vector.
Vector4 Vector4::getNegated() const { return Vector4(-x, -y, -z, w); }

// Copies the signs from the source vector to the destination vector.
void Vector4::copysign(Vector4 source) { *(this) = Vector4(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z), w); }

// Copies the signs from the source vector to the destination vector.
Vector4 Vector4::getCopiedSign(Vector4 source) const { return Vector4(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z), w); }

// Interprets the vector as a point and returns the distance to another point.
float Vector4::getDistanceFromPoint(const Vector4& p) const { return Vector4(*this, p, w).getLength(); }

// Returns the angle (in radians) of the given vector.
float Vector4::getAngleTheta() const  {  return acosf(z / getLength()); }

float Vector4::getAnglePhi()   const 
{ 
    if (x > 0) return atanf(y / x);
    if (x < 0) return atanf(y / x) + PI;
    return PI / 2;
}

// Returns the angle (in radians) between two vectors.
float Vector4::getAngleThetaWithVector4(Vector4 v) const
{
    float this_angle =   getAngleTheta();
    float v_angle    = v.getAngleTheta();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}
float Vector4::getAnglePhiWithVector4(Vector4 v) const
{
    float this_angle =   getAnglePhi();
    float v_angle    = v.getAnglePhi();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}

// Rotates the given vector by the given angle.
void Vector4::rotate(const float& theta, const float& phi)  { *(this) = Vector4(getAngleTheta() + theta, getAnglePhi() + phi, getLength(), w, true); }

// Creates a Vector3 from this vector.
Vector3 Vector4::toVector3(bool homogenizeVec)
{
    if (homogenizeVec)
        homogenize();
    return Vector3(x, y, z);
}


// -------------------- SEGMENT3 -------------------- //

// Constructors.
Segment3::Segment3()                                                              : a(Vector3()), b(Vector3())    {}; // Null Segment3.
Segment3::Segment3(const Vector3& _a,  const Vector3& _b)                         : a(_a),        b(_b)           {}; // Segment3 from points.
Segment3::Segment3(const Vector3& origin, const Vector3& vec, const bool& vector) : a(origin),    b(origin + vec) {}; // Segment3 from point and vector.

// ---------------- SEGMENT3 METHODS --------------- //

// Returns the center of mass of the Segment3.
Vector3 Segment3::getCenterOfMass() const
{
    return Vector3((a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2);
}

// Returns the vertex of the Segment3 that corresponds to the given index.
Vector3 Segment3::getVertex(const int& index) const
{
    assert (0 <= index && index < 2);

    switch (index)
    {
        case 0:  return a;
        case 1:  return b;
        default: return Vector3();
    }
}

// Moves the Segment3 by the given vector.
void Segment3::move(const Vector3& vec)
{
    a += vec;
    b += vec;
}

// -------------------- TRIANGLE3 -------------------- //

// Constructor.
Triangle3::Triangle3()                                                        : a(Vector3()), b(Vector3()), c(Vector3()) {}; // Null triangle.
Triangle3::Triangle3(const Vector3& _a, const Vector3& _b, const Vector3& _c) : a(_a),        b(_b),        c(_c)        {}; // Triangle3 from points.

// ---------- TRIANGLE3 METHODS ---------- //

// Returns the center of mass of the triangle.
Vector3 Triangle3::getCenterOfMass() const { return Vector3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3); }

// Returns the side of the triangle that corresponds to the given index.
Segment3 Triangle3::getSide(const int& index) const
{
    assert (0<= index && index < 3);

    switch (index)
    {
        case 0:   return Segment3(a, b);
        case 1:   return Segment3(b, c);
        case 2:   return Segment3(c, a);
        default:  return Segment3(Vector3(), Vector3());
    }
}

// Returns the vertex of the triangle that corresponds to the given index.
Vector3 Triangle3::getVertex(const int& index) const
{
    assert (0 <= index && index < 3);

    switch (index)
    {
    case 0:  return a;
    case 1:  return b;
    case 2:  return c;
    default: return Vector3();
    }
}

// Moves the triangle by the given vector.
void Triangle3::move(const Vector3& vec)
{
    a += vec;
    b += vec;
    c += vec;
}

// ------------------------------ COLLISIONS ------------------------------ //

// Returns true if the given point is colliding with the given circle.
bool collisions2D::collisionCirclePoint(geometry2D::Circle c, geometry2D::Vector2 p)
{
    return (c.origin.getDistanceFromPoint(p) <= c.radius ? true : false);
}

// Returns true if the given circles are in collision.
bool collisions2D::collisionCircles(geometry2D::Circle c1, geometry2D::Circle c2)
{
    return (c1.origin.getDistanceFromPoint(c2.origin) <= c1.radius + c2.radius ? true : false);
}

// Checks for collision between two rectangles.
bool collisions2D::collisionAABB(geometry2D::Rectangle rec1, geometry2D::Rectangle rec2)
{
    return (rec1.origin.x + rec1.width  >= rec2.origin.x              &&
            rec1.origin.x               <= rec2.origin.x + rec2.width &&
            rec1.origin.y + rec1.height >= rec2.origin.y              &&
            rec1.origin.y               <= rec2.origin.y + rec2.height);
}

// Returns true if the given point is colliding with the given Segment2.
bool collisions2D::collisionSegment2Point(geometry2D::Segment2 Segment2, geometry2D::Vector2 point)
{
    if (arithmetic::roundInt(Vector2(Segment2) ^ Vector2(Segment2.a, point)) == 0)
    {
        if ((point.x >= Segment2.a.x && point.x <= Segment2.b.x) || 
            (point.y >= Segment2.a.y && point.y <= Segment2.b.y) ||
            (point.x <= Segment2.a.x && point.x >= Segment2.b.x) || 
            (point.y <= Segment2.a.y && point.y >= Segment2.b.y))
        {
            return true;
        }
    }
    return false;
}

// Returns true if the given projections are colliding each others
bool collisions2D::collisionProjections(geometry2D::Segment2 projection1, geometry2D::Segment2 projection2)
{
    return (collisionSegment2Point(projection1, projection2.a) ||
            collisionSegment2Point(projection1, projection2.b) ||
            collisionSegment2Point(projection2, projection1.a) ||
            collisionSegment2Point(projection2, projection1.b));
}







// ------------------------------ RENDER 3D ------------------------------ //

matrix::Matrix<4, 4> render3D::getTranslationMatrix(const Vector3& translation)
{
    return matrix::Matrix<4,4>(1, 0, 0, translation.x,
                                   0, 1, 0, translation.y,
                                   0, 0, 1, translation.z,
                                   0, 0, 0, 1);
}

matrix::Matrix<4, 4> render3D::getScaleMatrix(const Vector3& scale)
{
    return matrix::Matrix<4,4>(scale.x, 0, 0, 0,
                                   0, scale.y, 0, 0,
                                   0, 0, scale.z, 0,
                                   0, 0, 0, 1);
}

matrix::Matrix<4, 4> render3D::getXRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(1, 0, 0, 0,
                                   0, cosf(angle), -sinf(angle), 0,
                                   0, sinf(angle),  cosf(angle), 0,
                                   0, 0, 0, 1);
}

matrix::Matrix<4, 4> render3D::getYRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(cosf(angle), 0, sinf(angle), 0,
                                   0, 1, 0, 0,
                                   -sinf(angle), 0, cosf(angle), 0,
                                   0, 0, 0, 1);
}

matrix::Matrix<4, 4> render3D::getZRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(cosf(angle), -sinf(angle), 0, 0,
                                   sinf(angle), cosf(angle), 0, 0,
                                   0, 0, 1, 0 ,
                                   0, 0, 0, 1 );
}

matrix::Matrix<4, 4> render3D::getTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
    return getTranslationMatrix(position)   *
           getYRotationMatrix  (rotation.y) *
           getXRotationMatrix  (rotation.x) * 
           getZRotationMatrix  (rotation.z) *
           getScaleMatrix      (scale);
}