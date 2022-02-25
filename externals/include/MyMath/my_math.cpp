#include "my_math.hpp"


// Returns the hue of an RGB color (0 <= rgba <= 1) (output in radians).
float Color::getHue()
{
    float minV = min(min(r, g), b);
    float maxV = max(max(r, g), b);
    float diff = maxV - minV;

    float hue;

    // If red is the maximum value.
    if (r > g && r > b)
        hue = (g - b) / diff;

    // If green is the maximum value.
    else if (g > r && g > b)
        hue = 2 + (b - r) / diff;

    // If blue is the maximum value.
    else
        hue = 4 + (r - g) / diff;
    
    // Keep the hue between 0 and 2pi;
    if (hue < 0)
        hue += 2*PI;
    else if (hue > 2*PI)
        hue -= 2*PI;
    
    return hue;
}

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
int arithmetic::signof(const float& val) { if (val == 0) return 1; return val / abs((int)val); }

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

// Compute the linear interpolation factor that returns val when lerping between start and end.
float arithmetic::getLerp(const float& val, const float& start, const float& end)
{
    if (end - start != 0)
        return (val - start) / (end - start);
    return 0;
}

// Linear interpolation between two given colors.
Color arithmetic::colorLerp(const float& val, const Color& start, const Color& end)
{
    return (Color){
        start.r + val * (end.r - start.r),
        start.g + val * (end.g - start.g),
        start.b + val * (end.b - start.b),
        start.a + val * (end.a - start.a)
    };
}

// Blend between two HSV colors.
HSV arithmetic::blendHSV(const HSV& col0, const HSV& col1)
{
    Vector2 totalVec = Vector2(col0.h, 1, true) + Vector2(col1.h, 1, true);
    float avgHue = totalVec.getAngle();
    float avgSat = (col0.s + col1.s) / 2;
    float avgVal = (col0.v + col1.v) / 2;
    return HSV{ avgHue, avgSat, avgVal };
}

// Convert an RGB color (0 <= rgba <= 1) to HSV.
HSV arithmetic::RGBtoHSV(const Color& color)
{
    HSV hsv;

    float minV = min(min(color.r, color.g), color.b);
    float maxV = max(max(color.r, color.g), color.b);
    float diff = maxV - minV;

    float r = color.r;
    float g = color.g;
    float b = color.b;
    
    // Set Value.
    hsv.v = maxV;

    // If max and min are the same, return.
    if (diff  < 0.00001f)
        return { 0, 0, hsv.v };

    // Set Saturation.
    if (maxV > 0)
        hsv.s = diff/maxV;
    else
        return { 0, 0, hsv.v };

    // Set Hue.
    if (r >= maxV)
        hsv.h = (g - b) / diff;
    else if (g >= maxV)
        hsv.h = 2.0f + (b - r) / diff;
    else if (b >= maxV)
        hsv.h = 4.0f + (r - g) / diff;
    
    // Keep Hue above 0.
    if (hsv.h < 0)
        hsv.h += 2*PI;

    return hsv;
}

// Convert an HSV color to RGB.
Color arithmetic::HSVtoRGB(const HSV& hsv, const float& alpha)
{
    Color color = { 0, 0, 0, alpha };

    // Red channel
    float k = fmodf((5.0f + hsv.h), 6);
    float t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.r = hsv.v - hsv.v*hsv.s*k;

    // Green channel
    k = fmodf((3.0f + hsv.h), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.g = hsv.v - hsv.v*hsv.s*k;

    // Blue channel
    k = fmodf((1.0f + hsv.h), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    color.b = hsv.v - hsv.v*hsv.s*k;

    return color;
}

// Shifts the hue of the given color.
Color arithmetic::colorShift(const Color& color, const float& hue)
{
    HSV hsv = RGBtoHSV(color);
    hsv.h += hue;
    if (hsv.h >= 2*PI)
        hsv.h -= 2*PI;
    else if (hsv.h < 0)
        hsv.h += 2*PI;
    Color out = HSVtoRGB(hsv);

    return out;
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

// Vector2 negation.
Vector2 Vector2::operator-()                 const { return { -x, -y }; }

// ------------ VECTOR2 METHODS ----------- //

// Returns the middle of the given vector.
Vector2 Vector2::getMiddle()                          const { return Vector2(x / 2, y / 2); }

// Returns the length of the given vector.
float Vector2::getLength()                            const { return sqrt(arithmetic::sqpow(x) + arithmetic::sqpow(y)); }

// Modifies the length of the given vector to correspond to the given value.
void Vector2::setLength(const float& length)                { *(this) = Vector2(getAngle(), length, true); }

// Normalizes the given vector so that its length is 1.
void Vector2::normalize()                                   { x /= getLength(); y /= getLength(); }

// Normalizes the given vector so that its length is 1.
Vector2 Vector2::getNormalized()                      const { return Vector2(x / getLength(), y / getLength()); }

// Negates both of the coordinates of the given vector.
void Vector2::negate()                                      { *(this) = Vector2(-x, -y); }

// Copies the signs from the source vector to the destination vector.
void Vector2::copysign(const Vector2& source)               { *(this) = Vector2(std::copysign(x, source.x), std::copysign(y, source.y)); }

// Copies the signs from the source vector to the destination vector.
Vector2 Vector2::getCopiedSign(const Vector2& source) const { return Vector2(std::copysign(x, source.x), std::copysign(y, source.y)); }

// Returns the normal of a given vector.
Vector2 Vector2::getNormal()                          const { return Vector2(-y, x); }

// Interprets the vector as a point and returns the distance to another point.
float Vector2::getDistanceFromPoint(const Vector2& p) const { return Vector2(*this, p).getLength(); }

// Returns the angle (in radians) of the given vector.
float Vector2::getAngle()                             const { return std::copysign(std::acos(getNormalized().x), std::asin(getNormalized().y)); }

// Returns the angle (in radians) between two vectors.
float Vector2::getAngleWithVector2(const Vector2& v)  const
{
    float this_angle = getAngle();
    float v_angle    = v.getAngle();
    return (this_angle >= v_angle ? (this_angle - v_angle) : (v_angle - this_angle));
}

// Rotates the given vector by the given angle.
void Vector2::rotate(const float& angle)
{
    float this_length = getLength();
    float this_angle  = getAngle();
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
    return getSide(index).a;
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

// Returns the coordinates of a point on a sphere of radius r, using the given angles.
Vector3 geometry3D::getSphericalCoords(const float& r, const float& theta, const float& phi)
{
    float sinTheta = sinf(theta);

    return { r * sinTheta * cosf(phi),
             r * cosf(theta),
             r * sinTheta * sinf(phi)
    };
}

matrix::Matrix<4, 4> geometry3D::getTranslationMatrix(const Vector3& translation)
{
    return matrix::Matrix<4,4>(1,             0,             0,             0,
                               0,             1,             0,             0,
                               0,             0,             1,             0,
                               translation.x, translation.y, translation.z, 1);
}

matrix::Matrix<4, 4> geometry3D::getScaleMatrix(const Vector3& scale)
{
    return matrix::Matrix<4,4>(scale.x, 0,       0,       0,
                               0,       scale.y, 0,       0,
                               0,       0,       scale.z, 0,
                               0,       0,       0,       1);
}

matrix::Matrix<4, 4> geometry3D::getXRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(1, 0,            0,           0,
                               0, cosf(angle), -sinf(angle), 0,
                               0, sinf(angle),  cosf(angle), 0,
                               0, 0,            0,           1);
}

matrix::Matrix<4, 4> geometry3D::getYRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(cosf(angle),  0, sinf(angle), 0,
                               0,            1, 0,           0,
                               -sinf(angle), 0, cosf(angle), 0,
                               0,            0, 0,           1);
}

matrix::Matrix<4, 4> geometry3D::getZRotationMatrix(float angle)
{
    return matrix::Matrix<4,4>(cosf(angle), -sinf(angle), 0, 0,
                               sinf(angle), cosf(angle),  0, 0,
                               0,           0,            1, 0,
                               0,           0,            0, 1 );
}

matrix::Matrix<4, 4> geometry3D::getTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& reverse)
{
    // For cameras.
    if (reverse)
    {
        return getTranslationMatrix(position)   *
               getYRotationMatrix  (rotation.y) *
               getXRotationMatrix  (rotation.x) * 
               getZRotationMatrix  (rotation.z) *
               getScaleMatrix      (scale);
    }
    // For objects in world space.
    else
    {
        return getScaleMatrix      (scale)      *
               getZRotationMatrix  (rotation.z) *
               getXRotationMatrix  (rotation.x) * 
               getYRotationMatrix  (rotation.y) *
               getTranslationMatrix(position);
    }
}

// Returns an array of vertices that result of the clipping of the given triangle against the given frustum.
std::vector<geometry3D::Triangle3> geometry3D::clipHomogeneousTriangle(const geometry3D::Triangle3& triangle, float vertexAbsW[3])
{
    // TODO: fix this.
    //* https://www.youtube.com/watch?v=VMD7fsCYO9o&t=432s&ab_channel=thebennybox
    //* https://www.youtube.com/watch?v=og7hOFypKpQ&ab_channel=UCDavisAcademics
    //* https://www.cubic.org/docs/3dclip.htm#ma4

    std::vector<Vertex> vertices;
    for (int i = 0; i < 3; i++)
        vertices.push_back(*((&triangle.a) + i));
    
    std::vector<Vertex> clippedVertices;

    // Loop over the vertices.
    for (int i = 0; i < 3; i++)
    {
        // If the current vertex is between -w and w.
        if (-vertexAbsW[i] <= vertices[i].pos.x && vertices[i].pos.x <= vertexAbsW[i] &&
            -vertexAbsW[i] <= vertices[i].pos.y && vertices[i].pos.y <= vertexAbsW[i] &&
            -vertexAbsW[i] <= vertices[i].pos.z && vertices[i].pos.z <= vertexAbsW[i])
        {
            // Add the vertex to the list.
            clippedVertices.push_back(vertices[i]);
        }

        // If it is not between -w and w.
        else
        {
            // Get the current, previous and next position and w.
            float   curW    = vertexAbsW[i];
            Vector3 curPos  = vertices[i].pos;
            float   prevW   = vertexAbsW[(i == 0 ? 2 : i-1)];
            Vector3 prevPos = vertices[(i == 0 ? 2 : i-1)].pos;
            float   nextW   = vertexAbsW[(i+1)%3];
            Vector3 nextPos = vertices[(i+1)%3].pos;

            {
                // Compute the lerping factors.
                float xFactor = (prevW - prevPos.x) / ((prevW - prevPos.x) - (curW - curPos.x));
                // float yFactor = (prevW - prevPos.y) / ((prevW - prevPos.y) - (curW - curPos.y));
                // float zFactor = (prevW - prevPos.z) / ((prevW - prevPos.z) - (curW - curPos.z));

                printf("Lerp factors: %.2f\n", xFactor);

                // Lerp the vertice's coords.
                float xLerp = arithmetic::lerp(xFactor, prevPos.x, curPos.x);
                // float yLerp = arithmetic::lerp(yFactor, prevPos.y, curPos.y);
                // float zLerp = arithmetic::lerp(zFactor, prevPos.z, curPos.z);

                // Create a new vertex.
                Vertex lerpVertex = {
                    { xLerp, curPos.y, curPos.z },
                    vertices[i].normal,
                    vertices[i].color,
                    vertices[i].uv
                };

                // Add the vertex to the list.
                clippedVertices.push_back(lerpVertex);
            }

            {
                // Compute the lerping factors.
                float xFactor = (nextW - nextPos.x) / ((nextW - nextPos.x) - (curW - curPos.x));
                // float yFactor = (nextW - nextPos.y) / ((nextW - nextPos.y) - (curW - curPos.y));
                // float zFactor = (nextW - nextPos.z) / ((nextW - nextPos.z) - (curW - curPos.z));

                // Lerp the vertice's coords.
                float xLerp = arithmetic::lerp(xFactor, nextPos.x, curPos.x);
                // float yLerp = arithmetic::lerp(yFactor, nextPos.y, curPos.y);
                // float zLerp = arithmetic::lerp(zFactor, nextPos.z, curPos.z);

                // Create a new vertex.
                Vertex lerpVertex = {
                    { xLerp, curPos.y, curPos.z },
                    vertices[i].normal,
                    vertices[i].color,
                    vertices[i].uv
                };

                // Add the vertex to the list.
                clippedVertices.push_back(lerpVertex);
            }
        }
    }

    // Create the output triangles.
    std::vector<Triangle3> clippedTriangles;
    for (int i = 1; i < (int)clippedVertices.size() - 1; i++)
    {
        clippedTriangles.push_back({ clippedVertices[0], clippedVertices[i], clippedVertices[i+1] });
    }

    return clippedTriangles;
}



// -------------------- VECTOR3 -------------------- //

// Constructors.
Vector3::Vector3()                                                  : x(0),                         y(0),                         z(0)                         {}; // Null vector.
Vector3::Vector3(const float& _x, const float& _y, const float& _z) : x(_x),                        y(_y),                        z(_z)                        {}; // Vector with 3 coordinates.
Vector3::Vector3(const Vector3& p1, const Vector3& p2)              : x(p2.x - p1.x),               y(p2.y - p1.y),               z(p2.z - p1.z)               {}; // Vector from 2 points.
Vector3::Vector3(const Segment3& seg)                               : x(seg.b.pos.x - seg.a.pos.x), y(seg.b.pos.y - seg.a.pos.y), z(seg.b.pos.z - seg.a.pos.z) {}; // Vector from Segment3.
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

// Vector3 negation.
Vector3 Vector3::operator-()                 const { return { -x, -y, -z }; }

// ------------ VECTOR3 METHODS ----------- //

// Returns the middle of the given vector.
Vector3 Vector3::getMiddle()   const { return Vector3(x / 2, y / 2, z / 2); }

// Returns the length of the given vector.
float Vector3::getLength()     const { return sqrt(arithmetic::sqpow(x) + arithmetic::sqpow(y) + arithmetic::sqpow(z)); }

// Modifies the length of the given vector to correspond to the given value.
void Vector3::setLength(const float& length) { normalize(); (*this) *= length; }

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

// Copies the signs from the source vector to the destination vector.
void Vector3::copysign(const Vector3& source)               { *(this) = Vector3(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z)); }

// Copies the signs from the source vector to the destination vector.
Vector3 Vector3::getCopiedSign(const Vector3& source) const { return Vector3(std::copysign(x, source.x), std::copysign(y, source.y), std::copysign(z, source.z)); }

// Interprets the vector as a point and returns the distance to another point.
float Vector3::getDistanceFromPoint(const Vector3& p) const { return Vector3(*this, p).getLength(); }

// Returns the angle (in radians) of the given vector.
float Vector3::getAngleTheta() const { return fmod(2*PI - Vector2(x, z).getAngle() + PI/2, 2*PI); }
float Vector3::getAnglePhi()   const { return (2*PI - Vector2(Vector2(x, z).getLength(), y).getAngle()) - 2*PI; }

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
Vector4::Vector4()                                                                   : x(0),                         y(0),                         z(0),                         w(1)  {}; // Null vector.
Vector4::Vector4(const float& _x, const float& _y, const float& _z, const float& _w) : x(_x),                        y(_y),                        z(_z),                        w(_w) {}; // Vector with 3 coordinates.
Vector4::Vector4(const Vector4& p1, const Vector4& p2, const float& _w)              : x(p2.x - p1.x),               y(p2.y - p1.y),               z(p2.z - p1.z),               w(_w) {}; // Vector from 2 points.
Vector4::Vector4(const Vector3&  vec, const float& _w)                               : x(vec.x),                     y(vec.y),                     z(vec.z),                     w(_w) {}; // Vector from Segment3.
Vector4::Vector4(const Segment3& seg, const float& _w)                               : x(seg.b.pos.x - seg.a.pos.x), y(seg.b.pos.y - seg.a.pos.y), z(seg.b.pos.z - seg.a.pos.z), w(_w) {}; // Vector from Segment3.
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

// Vector4 negation.
Vector4 Vector4::operator-()                 const { return { -x, -y, -z, w }; }

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
Vector3 Vector4::toVector3(bool homogenizeVec) const
{
    if (homogenizeVec)
        return Vector3(x/w, y/w, z/w);
    else
        return Vector3(x, y, z);
}



// -------------------- PLANE3 -------------------- //

// Constructor.
Plane3::Plane3()                                               : normal({ 0, 0, 1 }),             distance(0)         {}
Plane3::Plane3(const Vector3& _normal, const float& _distance) : normal(_normal.getNormalized()), distance(_distance) {}

// ---------- PLANE3 METHODS ---------- //

// Returns 0 if the given segment doesn't clip against this plane.
// Else, returns 1 if point A clips, 2 if point B clips and 3 if both clip.
int Plane3::doesSegmentClip(const Segment3& seg) const
{
    // Get the origin point of the plane.
    Vector3 origin = -normal * distance;

    // Get the distance from each point to the plane.
    // float distA = (seg.a.pos & normal) - distance;
    // float distB = (seg.b.pos & normal) - distance;
    float distA = normal & (seg.a.pos - origin);
    float distB = normal & (seg.b.pos - origin);

    printf("Point distances: %.2f, %.2f\n", distA, distB);

    // If both distances are positive, the segment doesn't clip.
    if (distA > 0 && distB > 0)
        return 0;

    // If both distances are negative, the whole segment clips.
    else if (distA <= 0 && distB <= 0)
        return 3;

    // If the 1st distance is negative, point A clips.
    else if (distA <= 0)
        return 1;

    // If the 1st distance is negative, point A clips.
    else
        return 2;
}

// Clips the given 3D segment against this plane.
Segment3 Plane3::clipSegment(Segment3 seg) const
{
    // Get the origin point of the plane.
    Vector3 origin = -normal * distance;

    // Get the distance from each point to the plane.
    // float distA = (seg.a.pos & normal) - distance;
    // float distB = (seg.b.pos & normal) - distance;
    float distA = normal & (seg.a.pos - origin);
    float distB = normal & (seg.b.pos - origin);

    // If both distances are positive, the segment is on the good side of the plane.
    if (distA > 0 && distB > 0)
        return seg;

    // If both distances are negative, the segment is on the wrong side of the plane.
    if (distA <= 0 && distB <= 0)
        return Segment3();

    // Compute the intersection factor (between 0 and 1).
    float factor = distA / (distA - distB);

    // Compute the intersection point.
    Vertex intersectVertex = {
                    point3Lerp(factor, seg.a.pos,    seg.b.pos),
                    point3Lerp(factor, seg.a.normal, seg.b.normal),
        arithmetic::colorLerp (factor, seg.a.color,  seg.b.color),
                    point2Lerp(factor, seg.a.uv,     seg.b.uv),
    };
    
    // Override the point that is clipping with the intersection point.
    if      (distA <= 0) seg.a = intersectVertex;
    else if (distB <= 0) seg.b = intersectVertex;
    return seg;
}

// Returns true if the given triangle clips against theis plane, false if not.
bool Plane3::doesTriangleClip(const Triangle3& triangle) const
{
    for (int i = 0; i < 3; i++)
        if (doesSegmentClip(triangle.getSide(0)))
            return true;
    return false;
}

// Clips the given triangle against this plane.
std::vector<Vertex> Plane3::clipTriangle(const Triangle3& triangle) const
{
    // Output array of vertices.
    std::vector<Vertex> vertices;

    // Clip the triangle's segments against the plane.
    for (int i = 0; i < 3; i++)
    {
        Segment3 clipSeg = clipSegment(triangle.getSide(i));
        int clipPoint = doesSegmentClip(triangle.getSide(i));

        // Add the clipped segment vertices to the output array.
        switch (clipPoint)
        {
        case 0:
        case 2:
            vertices.push_back(clipSeg.a);
            break;
        default:
            break;
        }
    }

    return vertices;
}


// -------------------- SEGMENT3 -------------------- //

// Constructors.
Segment3::Segment3()                                                              : a(Vertex{}), b(Vertex{}) {}; // Null Segment3.
Segment3::Segment3(const Vertex& _a,  const Vertex& _b)                           : a(_a),        b(_b)      {}; // Segment3 from points.
Segment3::Segment3(const Vertex& origin, const Vector3& vec, const bool& vector)  : a(origin),    b({ origin.pos + vec, origin.normal, origin.color, origin.uv }) {}; // Segment3 from point and vector.

// ---------------- SEGMENT3 METHODS --------------- //

// Returns the center of mass of the Segment3.
Vertex Segment3::getCenterOfMass() const
{
    return Vertex{ 
        { (a.pos.x + b.pos.x) / 2, (a.pos.y + b.pos.y) / 2, (a.pos.z + b.pos.z) / 2 }, 
        geometry3D::point3Lerp(0.5, a.normal, b.normal),
        arithmetic::colorLerp(0.5, a.color, b.color),
        geometry2D::point2Lerp(0.5, a.uv, b.uv)
    };
}

// Returns the vertex of the Segment3 that corresponds to the given index.
Vertex Segment3::getVertex(const int& index) const
{
    assert (0 <= index && index < 2);

    switch (index)
    {
        case 0:  return a;
        case 1:  return b;
        default: return Vertex{};
    }
}

// Moves the Segment3 by the given vector.
void Segment3::move(const Vector3& vec)
{
    a.pos += vec;
    b.pos += vec;
}

// -------------------- TRIANGLE3 -------------------- //

// Constructor.
Triangle3::Triangle3()                                                     : a(Vertex{}), b(Vertex{}), c(Vertex{}) {}; // Null triangle.
Triangle3::Triangle3(const Vertex& _a, const Vertex& _b, const Vertex& _c) : a(_a),       b(_b),       c(_c)       {}; // Triangle3 from points.

// ---------- TRIANGLE3 METHODS ---------- //

// Returns the center of mass of the triangle.
Vertex Triangle3::getCenterOfMass() const 
{ 
    return Vertex{
        { (a.pos.x + b.pos.x + c.pos.x) / 3, (a.pos.y + b.pos.y + c.pos.y) / 3, (a.pos.z + b.pos.z + c.pos.z) / 3 },
        { (a.normal.x + b.normal.x + c.normal.x) / 3, (a.normal.y + b.normal.y + c.normal.y) / 3, (a.normal.z + b.normal.z + c.normal.z) / 3 },
        { (a.color.r + b.color.r + c.color.r) / 3, (a.color.g + b.color.g + c.color.g) / 3, (a.color.b + b.color.b + c.color.b) / 3, (a.color.a + b.color.a + c.color.a) / 3 },
        { (a.uv.x + b.uv.x + c.uv.x) / 3, (a.uv.y + b.uv.y + c.uv.y) / 3 }
    }; 
}

// Returns the side of the triangle that corresponds to the given index.
Segment3 Triangle3::getSide(const int& index) const
{
    assert (0 <= index && index < 3);

    switch (index)
    {
        case 0:   return Segment3(a, b);
        case 1:   return Segment3(b, c);
        case 2:   return Segment3(c, a);
        default:  return Segment3(Vertex{}, Vertex{});
    }
}

// Returns the vertex of the triangle that corresponds to the given index.
Vertex Triangle3::getVertex(const int& index) const
{
    assert (0 <= index && index < 3);

    switch (index)
    {
    case 0:  return a;
    case 1:  return b;
    case 2:  return c;
    default: return Vertex{};
    }
}

// Moves the triangle by the given vector.
void Triangle3::move(const Vector3& vec)
{
    a.pos += vec;
    b.pos += vec;
    c.pos += vec;
}







//* ------------------------------ COLLISIONS 2D ------------------------------ *//

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
