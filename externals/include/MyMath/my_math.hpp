/*************************************************************
 * MyMathLib, a custom mathematics library written by
 * Rémi Serra and Alexandre Perché, students at ISART Digital.
 *************************************************************/


// ------------------- INCLUDES / NAMESPACE -------------------- //

#pragma once
#include <cmath>
#include <cassert>
#include <cstdint>
#include <vector>
#include <ctime>

#include <my_matrix.hpp>

using namespace std;

// ----------------------- DEFINES ------------------------ //

#define PI 3.14159265358979323846f

#define RED     (Color)   { 1, 0, 0, 1 }
#define GREEN   (Color)   { 0, 1, 0, 1 }
#define BLUE    (Color)   { 0, 0, 1, 1 }
#define YELLOW  (Color)   { 1, 1, 0, 1 }
#define CYAN    (Color)   { 0, 1, 1, 1 }
#define MAGENTA (Color)   { 1, 0, 1, 1 }
#define WHITE   (Color)   { 1, 1, 1, 1 }
#define BLACK   (Color)   { 0, 0, 0, 1 }

#define ColorToVec3(c) Vector3{ c.r, c.g, c.b }

// ------------- GLOBAL FORWARD DECLARATION --------------- //

class Color //? NOTE: [0, 1]
{
public:
    float r, g, b, a = 1.f;
    
    Color operator+ (const Color& c) const { return { r + c.r,    g + c.g ,   b + c.b,    a + c.a  }; }
    Color operator+ (const float& v) const { return { r + v  ,    g + v   ,   b + v  ,    a + v    }; }
    Color operator- (const Color& c) const { return { r - c.r,    g - c.g ,   b - c.b,    a - c.a  }; }
    Color operator- (const float& v) const { return { r - v  ,    g - v   ,   b - v  ,    a - v    }; }
    Color operator* (const Color& c) const { return { r * c.r,    g * c.g ,   b * c.b,    a * c.a  }; }
    Color operator* (const float& v) const { return { r * v  ,    g * v   ,   b * v  ,    a * v    }; }
    Color operator/ (const Color& c) const { return { r / c.r,    g / c.g ,   b / c.b,    a / c.a  }; }
    Color operator/ (const float& v) const { return { r / v  ,    g / v   ,   b / v  ,    a / v    }; }
    void  operator+=(const Color& c)       {          r += c.r;   g += c.g;   b += c.b;   a += c.a;   }
    void  operator+=(const float& v)       {          r += v  ;   g += v  ;   b += v  ;   a += v  ;   }
    void  operator-=(const Color& c)       {          r -= c.r;   g -= c.g;   b -= c.b;   a -= c.a;   }
    void  operator-=(const float& v)       {          r -= v  ;   g -= v  ;   b -= v  ;   a -= v  ;   }
    void  operator*=(const Color& c)       {          r *= c.r;   g *= c.g;   b *= c.b;   a *= c.a;   }
    void  operator*=(const float& v)       {          r *= v  ;   g *= v  ;   b *= v  ;   a *= v  ;   }
    void  operator/=(const Color& c)       {          r /= c.r;   g /= c.g;   b /= c.b;   a /= c.a;   }
    void  operator/=(const float& v)       {          r /= v  ;   g /= v  ;   b /= v  ;   a /= v  ;   }
    bool  operator==(const Color& c)       { return   r == c.r && g == c.g && b == c.b && a == c.a;   }
    bool  operator!=(const Color& c)       { return   r != c.r || g != c.g || b != c.b || a != c.a;   }

    // Returns the hue of an RGB color (0 <= rgba <= 1).
    float getHue();
};
typedef struct { float h, s, v; } HSV;

// --------------------- ARITHMECTIC ---------------------- //

namespace arithmetic
{

    // --------- ARITHMETIC FUNCTIONS ---------- //
    
    // Fast inverse square root from Quake III.
    float Q_rsqrt(const float& number);

    // Rounds the given value to the nearest int.
    int roundInt(const float& val);

    // Rounds down the given value.
    int floorInt(const float& val);

    // Rounds up the given value.
    int ceilInt(const float& val);

    // Returns the sqare power of the given value.
    float sqpow(const float& val);

    // Returns 1 if the given value is positive or null, and -1 if it is negative.
    int signof(const float& val);

    // Converts the given angle from degrees to radians.
    float degToRad(const float& deg);

    // Converts the given angle from radians to degrees.
    float radToDeg(const float& rad);

    // Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.
    float clamp(float val, const float& min, const float& max);

    // Clamps the given value to be inferior or equal to the maximum value.
    float clampUnder(float val, const float& max);

    // Clamps the given value to be superior or equal to the minimum value.
    float clampAbove(float val, const float& min);

    // Compute linear interpolation between start and end for the parameter val (if 0 <= val <= 1: start <= return <= end).
    float lerp(const float& val, const float& start, const float& end);

    // Compute the linear interpolation factor that returns val when lerping between start and end.
    float getLerp(const float& val, const float& start, const float& end);

    // Linear interpolation between two given colors.
    Color colorLerp(const float& val, const Color& start, const Color& end);

    // Blend between two HSV colors.
    HSV blendHSV(const HSV& col0, const HSV& col1);

    // Convert an RGB color (0 <= rgba <= 1) to HSV.
    HSV   RGBtoHSV(const Color& color);

    // Convert an HSV color to RGB.
    Color HSVtoRGB(const HSV& hsv, const float& alpha = 1);

    // Shifts the hue of the given color.
    Color colorShift(const Color& color, const float& hue);

    // Remaps the given value from one range to another.
    float remap(const float& val, const float& inputStart, const float& inputEnd, const float& outputStart, const float& outputEnd);

    // Returns true if the given number is a power of 2.
    bool isPowerOf2(const int& val);

    // Returns the closest power of 2 that is inferior or equal to val.
    int getPowerOf2Under(const int& val);

    // Returns the closest power of 2 that is superior or equal to val.
    int getPowerOf2Above(const int& val);
}

// ----------------------- GEOMETRY ----------------------- //

namespace geometry2D
{
    // ------------- FORWARD DECLARATIONS ------------- //

    class Vector2;
    class Segment2;
    class Triangle2;
    class Rectangle;
    class Polygon;
    class Circle;

    // Calculates linear interpolation for a value from a start point to an end point.
    Vector2 point2Lerp(const float& val, const Vector2& start, const Vector2& end);

    // ---------------------- CLASSES --------------------- //

    // Vector class that holds values for x and y (2 dimensions).
    class Vector2
    {
        public :
            // -- Attributes -- //
            float x, y;

            // -- Constructors & Destructor -- //
            Vector2();                                                           // Null vector.
            Vector2(const float& _x, const float& _y);                     // Vector with 2 coordinates.
            Vector2(const Vector2& p1,  const Vector2& p2);                      // Vector from 2 points.
            Vector2(const Segment2& seg);                                        // Vector from semgent.
            Vector2(const float& rad, const float& length, const bool& isAngle); // Vector from angle (useless bool).
            ~Vector2() {}

            // -- Operators -- //

                                  void    operator= (const Vector2& v);
            template <typename T> bool    operator==(const T& val) const;
            template <typename T> bool    operator!=(const T& val) const;
            template <typename T> Vector2 operator+ (const T& val) const;
            template <typename T> Vector2 operator- (const T& val) const;
            template <typename T> Vector2 operator* (const T& val) const;
            template <typename T> Vector2 operator/ (const T &val) const;
            template <typename T> void    operator+=(const T &val);
            template <typename T> void    operator-=(const T &val);
            template <typename T> void    operator*=(const T &val);
            template <typename T> void    operator/=(const T &val);
                                  float   operator& (const Vector2& v) const;
                                  float   operator^ (const Vector2& v) const;
                                  Vector2 operator- ()             const;

            // -- Methods -- //

            // Returns the middle of the given vector.
            Vector2 getMiddle() const;
            
            // Returns the length of the given vector.
            float getLength() const;
            // Modifies the length of the given vector to correspond to the given value.
            void setLength(const float& length);

            // Normalizes the given vector so that its length is 1.
            void normalize();
            // Normalizes the given vector so that its length is 1.
            Vector2 getNormalized() const;

            // Negates both of the coordinates of the given vector.
            void negate();

            // Copies the signs from the source vector to the destination vector.
            void copysign(const Vector2& source);
            // Copies the signs from the source vector to the destination vector.
            Vector2 getCopiedSign(const Vector2& source) const;

            // Returns the normal of a given vector.
            Vector2 getNormal() const;

            // Interprets the vector as a point and returns the distance to another point.
            float getDistanceFromPoint(const Vector2 &p) const;

            // Returns the angle (in radians) of the given vector.
            float getAngle() const;

            // Returns the angle (in radians) between two vectors.
            float getAngleWithVector2(const Vector2& v) const;

            // Rotates the given vector by the given angle.
            void rotate(const float& angle);
    };

    // Segment2 structure that holds values for the starting point and the end point.
    class Segment2
    {
        public :
            // Attributes.
            Vector2 a, b;

            // Constructors.
            Segment2();                                                              // Null Segment2.
            Segment2(const Vector2& _a,  const Vector2& _b);                   // Segment2 from points.
            Segment2(const Vector2& origin, const Vector2& vec, const bool& vector); // Segment2 from point and vector.

            // Destructor.
            ~Segment2() {}

            // Returns the center of mass of the Segment2.
            Vector2 getCenterOfMass() const;

            // Returns the number of sides of the Segment2.
            int getSidesNum() const;

            // Returns the side of the Segment2 that corresponds to the given index.
            Segment2 getSide(const int& index) const;

            // Returns the number of vertices of the Segment2.
            int getVerticesNum() const;

            // Returns the vertex of the Segment2 that corresponds to the given index.
            Vector2 getVertex(const int& index) const;

            // Moves the Segment2 by the given vector.
            void move(const Vector2& vec);
    };

    // Triangle2 structure that holds values for 3 points.
    class Triangle2 
    {
        public:
            // Attributes.
            Vector2 a, b, c;

            // Constructor.
            Triangle2();                                                                 // Null triangle.
            Triangle2(const Vector2& _a, const Vector2& _b, const Vector2& _c); // Triangle2 from points.

            // Destructor.
            ~Triangle2() {}
            
            // Returns the center of mass of the triangle.
            Vector2 getCenterOfMass() const;

            // Returns the number of sides of the triangle.
            int getSidesNum() const;

            // Returns the side of the triangle that corresponds to the given index.
            Segment2 getSide(const int& index) const;

            // Returns the number of vertices of the triangle.
            int getVerticesNum() const;

            // Returns the vertex of the triangle that corresponds to the given index.
            Vector2 getVertex(const int& index) const;

            // Moves the triangle by the given vector.
            void move(const Vector2& vec);
    };

    // Rectangle structure that holds values for the origin point, width and height.
    class Rectangle
    {        
        public:            
            // Attributes.
            Vector2 origin;
            double  width, height;

            // Constructor.
            Rectangle();                                                                             // Null rectangle.
            Rectangle(const Vector2& _origin, const double& _width, const double& _height); // Rectangle from origin, width and height.

            // Destructor.
            ~Rectangle() {}

            // Returns the center of mass of the rectangle.
            Vector2 getCenterOfMass() const;

            // Returns the number of sides of the rectangle.
            int getSidesNum() const;

            // Returns the side of the rectangle that corresponds to the given index.
            Segment2 getSide(const int& index) const;

            // Returns the number of vertices of the rectangle.
            int getVerticesNum() const;

            // Returns the vertex of the rectangle that corresponds to the given index.
            Vector2 getVertex(const int& index) const;

            // Moves the rectangle by the given vector.
            void move(const Vector2& vec);
    };

    // Polygon structure that holds values for the origin point, the radius and the number of sides.
    class Polygon
    {
        public:
            // Attributes.
            Vector2 origin;
            double  radius, rotation;
            int     sides;

            // Constructor.
            Polygon();                                                                                          // Null polygon.
            Polygon(const Vector2& _origin, const double& _radius, const double& _rotation, const int& _sides); // Polygon from origin, radius, rotation and side amount.

            // Destructor.
            ~Polygon() {}

            // Returns the center of mass of the polygon.
            Vector2 getCenterOfMass() const;

            // Returns the number of sides of the polygon.
            int getSidesNum() const;

            // Returns the side of the polygon that corresponds to the given index.
            Segment2 getSide(const int& index) const;

            // Returns the number of vertices of the polygon.
            int getVerticesNum() const;

            // Returns the vertex of the polygon that corresponds to the given index.
            Vector2 getVertex(const int& index) const;

            // Moves the polygon by the given vector.
            void move(const Vector2& vec);
    };

    // Circle structure that holds values for the origin point and radius.
    class Circle 
    {
        public :
            // Attributes.
            Vector2 origin;
            double  radius;

            // Constructor.
            Circle();                                        // Null circle.
            Circle(const Vector2& _origin, const double& _radius); // Circle from origin and radius.

            // Destructor.
            ~Circle() {}

            // Returns the center of mass of the circle.
            Vector2 getCenterOfMass() const;

            // Returns the number of sides of the circle.
            int getSidesNum() const;

            // Does nothing and returns a null Segment2.
            Segment2 getSide(const int& index) const;

            // Returns the number of vertices of the circle.
            int getVerticesNum() const;

            // Does nothing and returns a null vector.
            Vector2 getVertex(const int& index) const;

            // Moves the circle by the given vector.
            void move(const Vector2& vec);
    };
}


namespace geometry3D
{
    class Vector3;
    class Vector4;
    class Segment3;
    class Triangle3;
    class Plane3;
    struct Vertex;
    struct Plane;

    // Calculates linear interpolation for a value from a start point to an end point.
    Vector3 point3Lerp(const float& val, const Vector3& start, const Vector3& end);

    // Returns the coordinates of a point on a sphere of radius r, using the given angles.
    Vector3 getSphericalCoords(const float& r, const float& theta, const float& phi);

    matrix::Matrix<4, 4> getTranslationMatrix(const geometry3D::Vector3& translation);
    matrix::Matrix<4, 4> getScaleMatrix      (const geometry3D::Vector3& scale);
    matrix::Matrix<4, 4> getXRotationMatrix  (float angle);
    matrix::Matrix<4, 4> getYRotationMatrix  (float angle);
    matrix::Matrix<4, 4> getZRotationMatrix  (float angle);
    matrix::Matrix<4, 4> getTransformMatrix  (const geometry3D::Vector3& position, const geometry3D::Vector3& rotation, const geometry3D::Vector3& scale, const bool& reverse = false);

    // Vector class that holds values for x, y and z (3 dimensions).
    class Vector3
    {
        public :
            // -- Attributes -- //
            float x, y, z;

            // -- Constructors & Destructor -- //
            Vector3();                                                                               // Null vector.
            Vector3(const float& _x, const float& _y, const float& _z);                              // Vector with 3 coordinates.
            Vector3(const Vector3& p1,  const Vector3& p2);                                          // Vector from 2 points.
            Vector3(const Segment3& seg);                                                            // Vector from semgent.
            Vector3(const float& theta, const float& phi, const float& length, const bool& isAngle); // Vector from angles (useless bool).
            ~Vector3() {}

            // -- Operators -- //

                                  void    operator= (const Vector3& other);
            template <typename T> bool    operator==(const T& val) const;
            template <typename T> bool    operator!=(const T& val) const;
            template <typename T> Vector3 operator+ (const T& val) const;
            template <typename T> Vector3 operator- (const T& val) const;
            template <typename T> Vector3 operator* (const T& val) const;
            template <typename T> Vector3 operator/ (const T &val) const;
            template <typename T> void    operator+=(const T &val);
            template <typename T> void    operator-=(const T &val);
            template <typename T> void    operator*=(const T &val);
            template <typename T> void    operator/=(const T &val);
                                  float   operator& (const Vector3& val) const;
                                  Vector3 operator^ (const Vector3& val) const;
                                  Vector3 operator- ()                   const;

            // -- Methods -- //

            // Returns the middle of the given vector.
            Vector3 getMiddle() const;
            
            // Returns the length of the given vector.
            float getLength() const;
            // Modifies the length of the given vector to correspond to the given value.
            void setLength(const float& length);

            // Normalizes the given vector so that its length is 1.
            void normalize();
            // Normalizes the given vector so that its length is 1.
            Vector3 getNormalized() const;

            // Negates both of the coordinates of the given vector.
            void negate();

            // Copies the signs from the source vector to the destination vector.
            void copysign(const Vector3& source);
            // Copies the signs from the source vector to the destination vector.
            Vector3 getCopiedSign(const Vector3& source) const;

            // Interprets the vector as a point and returns the distance to another point.
            float getDistanceFromPoint(const Vector3 &p) const;

            // Returns the angle (in radians) of the given vector.
            float getAngleTheta() const;
            float getAnglePhi()   const;

            // Returns the angle (in radians) between two vectors.
            float getAngleThetaWithVector3(const Vector3& v) const;
            float getAnglePhiWithVector3  (const Vector3& v) const;

            // Rotates the given vector by the given angle.
            void rotate(const float& theta, const float& phi);

            // Creates a Vector4 from this vector.
            Vector4 toVector4();
    };

    // Point in 3D space with homogeneous coordinates.
    class Vector4
    {
        public:
            // Attributes.
            float x, y, z, w;

            // -- Constructors & Destructor -- //
            Vector4();                                                                                                // Null vector.
            Vector4(const float& _x, const float& _y, const float& _z, const float& _w);                              // Vector with 3 coordinates.
            Vector4(const Vector3& vec, const float& _w);                                                             // Vector4 from vector3.
            Vector4(const Vector4& p1, const Vector4& p2, const float& _w);                                           // Vector4 from 2 points.
            Vector4(const Segment3& seg, const float& _w);                                                            // Vector4 from semgent.
            Vector4(const float& theta, const float& phi, const float& length, const float& _w, const bool& isAngle); // Vector4 from angles (useless bool).
            ~Vector4() {}

            // -- Operators -- //
            
                                  void    operator= (const Vector4& other);
            template <typename T> bool    operator==(const T& val) const;
            template <typename T> bool    operator!=(const T& val) const;
            template <typename T> Vector4 operator+ (const T& val) const;
            template <typename T> Vector4 operator- (const T& val) const;
            template <typename T> Vector4 operator* (const T& val) const;
            template <typename T> Vector4 operator/ (const T &val) const;
            template <typename T> void    operator+=(const T &val);
            template <typename T> void    operator-=(const T &val);
            template <typename T> void    operator*=(const T &val);
            template <typename T> void    operator/=(const T &val);
                                  float   operator& (const Vector4& val) const;
                                  Vector3 operator^ (const Vector4& val) const;
                                  Vector4 operator- ()                   const;

            // -- Methods -- //

            // Returns the middle of this vector.
            Vector4 getMiddle() const;

            // Homogenizes the vector4 by dividing it by w.
            void homogenize();
            // Homogenizes the vector4 by dividing it by w.
            Vector4 getHomogenized() const;
            
            // Returns the length of this vector.
            float getLength() const;
            // Modifies the length of this vector to correspond to the given value.
            void setLength(const float& length);

            // Normalizes the given vector so that its length is 1.
            void normalize();
            // Normalizes the given vector so that its length is 1.
            Vector4 getNormalized() const;

            // Negates both of the coordinates of this vector.
            void negate();

            // Copies the signs from the source vector to the destination vector.
            void copysign(Vector4 source);
            // Copies the signs from the source vector to the destination vector.
            Vector4 getCopiedSign(Vector4 source) const;

            // Interprets the vector as a point and returns the distance to another point.
            float getDistanceFromPoint(const Vector4 &p) const;

            // Returns the angle (in radians) of this vector.
            float getAngleTheta() const;
            // Returns the angle (in radians) of this vector.
            float getAnglePhi()   const;

            // Returns the angle (in radians) between two vectors.
            float getAngleThetaWithVector4(Vector4 v) const;
            // Returns the angle (in radians) between two vectors.
            float getAnglePhiWithVector4  (Vector4 v) const;

            // Rotates the given vector by the given angle.
            void rotate(const float& theta, const float& phi);

            // Creates a Vector3 from this vector.
            Vector3 toVector3(bool homogenizeVec = false) const;
    };

    class Plane3
    {
        public:
            // Attributes.
            Vector3 normal; // Normalized normal vector to the plane.
            float distance; // The distance from the origin.

            // Constructor.
            Plane3();
            Plane3(const Vector3& _normal, const float& _distance);

            // Destructor.
            ~Plane3() {}

            // Returns 0 if the given segment doesn't clip against this plane.
            // Else, returns 1 if point A clips, 2 if point B clips and 3 if both clip.
            int doesSegmentClip(const Segment3& seg) const;

            // Clips the given segment against this plane.
            Segment3 clipSegment(Segment3 seg) const;

            // Returns true if the given triangle clips against theis plane, false if not.
            bool doesTriangleClip(const Triangle3& triangle) const;

            // Clips the given triangle against this plane (returns 0-4 points).
            std::vector<Vertex> clipTriangle(const Triangle3& triangle) const;
    };

    struct Vertex
    {
        Vector3 pos;            // Pos
        Vector3 normal;         // Normal
        Color   color;          // Color
        geometry2D::Vector2 uv; // Texture coordinates
    };

    struct Frustum
    {
        geometry3D::Plane3 up;
        geometry3D::Plane3 down;
        geometry3D::Plane3 left;
        geometry3D::Plane3 right;
        geometry3D::Plane3 near;
        geometry3D::Plane3 far;
    };

    // Returns an array of vertices that result of the clipping of the given triangle against the given frustum.
    std::vector<geometry3D::Triangle3> clipHomogeneousTriangle(const geometry3D::Triangle3& triangle, float pointAbsW[3]);

    // Segment3 structure that holds values for the starting point and the end point.
    class Segment3
    {
        public :
            // Attributes.
            Vertex a, b;

            // Constructors.
            Segment3();                                                              // Null Segment3.
            Segment3(const Vertex& _a,  const Vertex& _b);                   // Segment3 from points.
            Segment3(const Vertex& origin, const Vector3& vec, const bool& vector); // Segment3 from point and vector.

            // Destructor.
            ~Segment3() {}

            // Returns the center of mass of the Segment3.
            Vertex getCenterOfMass() const;

            // Returns the vertex of the Segment3 that corresponds to the given index.
            Vertex getVertex(const int& index) const;

            // Moves the Segment3 by the given vector.
            void move(const Vector3& vec);
    };

    // Triangle3 structure that holds values for 3 vertices.
    class Triangle3
    {
        public:
            // Attributes.
            Vertex a, b, c;

            // Constructor.
            Triangle3();                                                     // Null triangle.
            Triangle3(const Vertex& _a, const Vertex& _b, const Vertex& _c); // Triangle3 from vertices.

            // Destructor.
            ~Triangle3() {}
            
            // Returns the center of mass of the triangle.
            Vertex getCenterOfMass() const;

            // Returns the side of the triangle that corresponds to the given index.
            Segment3 getSide(const int& index) const;

            // Returns the vertex of the triangle that corresponds to the given index.
            Vertex getVertex(const int& index) const;

            // Moves the triangle by the given vector.
            void move(const Vector3& vec);
    };
}


// ------------------- COLLISIONS 2D -------------------- //

namespace collisions2D
{
    // Returns the smallest rectangle that contanins the given shape.
    template <typename T> geometry2D::Rectangle getBoundingBox(T shape);

    // Returns an axis that passes through the center of the given circle and the center of the given shape.
    template <typename T> geometry2D::Segment2 CircleGetAxis(geometry2D::Circle circle, T shape);

    // Returns the axis of the given shapes that corresponds to the given index.
    template <typename T1, typename T2>
    geometry2D::Segment2 ShapesGetAxis(T1 shape1, T2 shape2, int index);

    // Returns true if the given point is colliding with the given circle.
    bool collisionCirclePoint(geometry2D::Circle c, geometry2D::Vector2 p);

    // Returns true if the given circles are in collision.
    bool collisionCircles(geometry2D::Circle c1, geometry2D::Circle c2);

    // Checks for collision between two rectangles.
    bool collisionAABB(geometry2D::Rectangle rec1, geometry2D::Rectangle rec2);

    // Project a shape onto a given axis.
    template <typename T> geometry2D::Segment2 projectShapeOnAxis(geometry2D::Segment2 axis, T shape);

    // Returns true if the given point is colliding with the given Segment2.
    bool collisionSegment2Point(geometry2D::Segment2 Segment2, geometry2D::Vector2 point);

    // Returns true if the given projections are colliding each others
    bool collisionProjections(geometry2D::Segment2 projection1, geometry2D::Segment2 projection2);

    // Checks for collision between two given shapes.
    template <typename T1, typename T2> bool collisionSAT(T1 shape1, T2 shape2);
}

#include "my_math.inl"

// When all programming gods have truned their backs,
// praise the blessed meowing duck.
/*     _
   .__(.)< (MEOW)
    \___)
*/
