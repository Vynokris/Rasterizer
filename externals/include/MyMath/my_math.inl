#pragma once
#include "my_math.hpp"

using namespace matrix;
using namespace geometry2D;
using namespace geometry3D;

// ---------- VECTOR2 ---------- //

// Vector2 addition.
template <typename T>
inline Vector2 Vector2::operator+(const T& val) const
{
    return Vector2(x + val, y + val);
}
template<>
inline Vector2 Vector2::operator+<Vector2>(const Vector2& val) const
{
    return Vector2(x + val.x, y + val.y);
}

// Vector2 substraction.
template <typename T>
inline Vector2 Vector2::operator-(const T& val) const
{
    return Vector2(x - val,   y - val  );
}
template <>
inline Vector2 Vector2::operator-<Vector2>(const Vector2& val) const
{
    return Vector2(x - val.x, y - val.y);
}

// Vector2 multiplication.
template <typename T>
inline Vector2 Vector2::operator*(const T& val) const
{
    return Vector2(x * val,   y * val  );
}
template <>
inline Vector2 Vector2::operator*<Vector2>(const Vector2& val) const
{
    return Vector2(x * val.x, y * val.y);
}

// Vector2 division.
template <typename T>
inline Vector2 Vector2::operator/(const T& val) const
{
    return Vector2(x / val,   y / val  );
}
template <>
inline Vector2 Vector2::operator/<Vector2>(const Vector2& val) const
{
    return Vector2(x / val.x, y / val.y);
}

// Vector2 addition assignement.
template <typename T>
inline void Vector2::operator+=(const T& val)
{
    x += val;
    y += val;
}
template <>
inline void Vector2::operator+=<Vector2>(const Vector2& val)
{
    x += val.x; 
    y += val.y;
}

// Vector2 substraction assignement.
template <typename T>
inline void Vector2::operator-=(const T& val)
{
    x -= val;
    y -= val;
}
template <>
inline void Vector2::operator-=<Vector2>(const Vector2& val)
{
    x -= val.x; 
    y -= val.y;
}

// Vector2 multiplication assignement.
template <typename T>
inline void Vector2::operator*=(const T& val)
{
    x *= val;
    y *= val;
}
template <>
inline void Vector2::operator*=<Vector2>(const Vector2& val)
{
    x *= val.x; 
    y *= val.y;
}

// Vector2 division assignement.
template <typename T>
inline void Vector2::operator/=(const T& val)
{
    x /= val;
    y /= val;
}
template <>
inline void Vector2::operator/=<Vector2>(const Vector2& val)
{
    x /= val.x; 
    y /= val.y;
}

// ---------- VECTOR3 ---------- //

// Vector3 addition.
template <typename T>
inline Vector3 Vector3::operator+(const T& val) const
{
    return Vector3(x + val, y + val, z + val);
}
template<>
inline Vector3 Vector3::operator+<Vector3>(const Vector3& val) const
{
    return Vector3(x + val.x, y + val.y, z + val.z);
}

// Vector3 substraction.
template <typename T>
inline Vector3 Vector3::operator-(const T& val) const
{
    return Vector3(x - val, y - val, z - val);
}
template <>
inline Vector3 Vector3::operator-<Vector3>(const Vector3& val) const
{
    return Vector3(x - val.x, y - val.y, z - val.z);
}

// Vector3 multiplication.
template <typename T>
inline Vector3 Vector3::operator*(const T& val) const
{
    return Vector3(x * val, y * val, z * val);
}
template <>
inline Vector3 Vector3::operator*<Vector3>(const Vector3& val) const
{
    return Vector3(x * val.x, y * val.y, z * val.z);
}

// Vector3 division.
template <typename T>
inline Vector3 Vector3::operator/(const T& val) const
{
    return Vector3(x / val, y / val, z / val);
}
template <>
inline Vector3 Vector3::operator/<Vector3>(const Vector3& val) const
{
    return Vector3(x / val.x, y / val.y, z / val.z);
}

// Vector3 addition assignement.
template <typename T>
inline void Vector3::operator+=(const T& val)
{
    x += val;
    y += val;
    z += val;
}
template <>
inline void Vector3::operator+=<Vector3>(const Vector3& val)
{
    x += val.x; 
    y += val.y;
    z += val.z;
}

// Vector3 substraction assignement.
template <typename T>
inline void Vector3::operator-=(const T& val)
{
    x -= val;
    y -= val;
    z -= val;
}
template <>
inline void Vector3::operator-=<Vector3>(const Vector3& val)
{
    x -= val.x; 
    y -= val.y;
    z -= val.z;
}

// Vector3 multiplication assignement.
template <typename T>
inline void Vector3::operator*=(const T& val)
{
    x *= val;
    y *= val;
    z *= val;
}
template <>
inline void Vector3::operator*=<Vector3>(const Vector3& val)
{
    x *= val.x; 
    y *= val.y;
    z *= val.z;
}

// Vector3 division assignement.
template <typename T>
inline void Vector3::operator/=(const T& val)
{
    x /= val;
    y /= val;
    z /= val;
}
template <>
inline void Vector3::operator/=<Vector3>(const Vector3& val)
{
    x /= val.x; 
    y /= val.y;
    z /= val.z;
}

// ---------- VECTOR4 ---------- //

// Vector4 addition.
template <typename T>
inline Vector4 Vector4::operator+(const T& val) const
{
    return Vector4(x + val, y + val, z + val, w + val);
}
template<>
inline Vector4 Vector4::operator+<Vector4>(const Vector4& val) const
{
    return Vector4(x + val.x, y + val.y, z + val.z, w + val.w);
}

// Vector4 substraction.
template <typename T>
inline Vector4 Vector4::operator-(const T& val) const
{
    return Vector4(x - val, y - val, z - val, w - val);
}
template <>
inline Vector4 Vector4::operator-<Vector4>(const Vector4& val) const
{
    return Vector4(x - val.x, y - val.y, z - val.z, w - val.w);
}

// Vector4 multiplication.
template <typename T>
inline Vector4 Vector4::operator*(const T& val) const
{
    return Vector4(x * val, y * val, z * val, w * val);
}

template <>
inline Vector4 Vector4::operator*<Vector4>(const Vector4& val) const
{
    return Vector4(x * val.x, y * val.y, z * val.z, w * val.w);
}

// Vector4 division.
template <typename T>
inline Vector4 Vector4::operator/(const T& val) const
{
    return Vector4(x / val, y / val, z / val, w / val);
}
template <>
inline Vector4 Vector4::operator/<Vector4>(const Vector4& val) const
{
    return Vector4(x / val.x, y / val.y, z / val.z, w / val.w);
}

// Vector4 addition assignement.
template <typename T>
inline void Vector4::operator+=(const T& val)
{
    x += val;
    y += val;
    z += val;
    w += val;
}
template <>
inline void Vector4::operator+=<Vector4>(const Vector4& val)
{
    x += val.x; 
    y += val.y;
    z += val.z;
    w += val.w;
}

// Vector4 substraction assignement.
template <typename T>
inline void Vector4::operator-=(const T& val)
{
    x -= val;
    y -= val;
    z -= val;
    w -= val;
}
template <>
inline void Vector4::operator-=<Vector4>(const Vector4& val)
{
    x -= val.x; 
    y -= val.y;
    z -= val.z;
    w -= val.w;
}

// Vector4 multiplication assignement.
template <typename T>
inline void Vector4::operator*=(const T& val)
{
    x *= val;
    y *= val;
    z *= val;
    w *= val;
}
template <>
inline void Vector4::operator*=<Vector4>(const Vector4& val)
{
    x *= val.x; 
    y *= val.y;
    z *= val.z;
    w *= val.w;
}

// Vector4 division assignement.
template <typename T>
inline void Vector4::operator/=(const T& val)
{
    x /= val;
    y /= val;
    z /= val;
    w /= val;
}

template <>
inline void Vector4::operator/=<Vector4>(const Vector4& val)
{
    x /= val.x; 
    y /= val.y;
    z /= val.z;
    w /= val.w;
}

// ----------- MATRIX 4 ----------- //

template <>
inline Vector4 Vector4::operator*<Matrix<4,4>>(const Matrix<4,4>& val) const
{
    return Vector4
    (
        x * val[0][0] + x * val[0][1] + x * val[0][2] + x * val[0][3],
        y * val[1][0] + y * val[1][1] + y * val[1][2] + y * val[1][3],
        z * val[2][0] + z * val[2][1] + z * val[2][2] + z * val[2][3],
        w * val[3][0] + w * val[3][1] + w * val[3][2] + w * val[3][3]
    );
}

// ---------- COLLISIONS 2D ---------- //

// Returns the smallest rectangle that contanins the given shape.
template <typename T>
inline Rectangle collisions2D::getBoundingBox(T shape)
{
    // Get the shape's vertices information.
    int vertices_num = shape.getVerticesNum();

    // Create the min and max values for x and y.
    Vector2 vertex = shape.getVertex(0);
    double xmin      = vertex.x;
    double xmax      = vertex.x;
    double ymin      = vertex.y;
    double ymax      = vertex.y;

    // Loop though the vertices and find the min and max values for x and y.
    for (int i = 1; i < vertices_num; i++)
    {
        vertex = shape.getVertex(i);
        if (vertex.x <= xmin) xmin = vertex.x;
        if (vertex.x >= xmax) xmax = vertex.x;
        if (vertex.y <= ymin) ymin = vertex.y;
        if (vertex.y >= ymax) ymax = vertex.y;
    }

    // Create the shape's bounding box.
    Rectangle bounding_box = Rectangle(Vector2(xmin, ymin), xmax - xmin, ymax - ymin);

    return bounding_box;
}

// Bounding box specialization for a circle.
template <>
inline Rectangle collisions2D::getBoundingBox<geometry2D::Circle>(geometry2D::Circle shape)
{
    Rectangle bounding_box = Rectangle(shape.origin - shape.radius, 
                                           shape.radius * 2, 
                                           shape.radius * 2);

    return bounding_box;
}

// Returns an axis that passes through the center of the given circle and the center of the given shape.
template <typename T>
inline geometry2D::Segment2 collisions2D::CircleGetAxis(Circle circle, T shape)
{
    // Make a Segment2 that starts at the center of the circle, goes in the direction of the center of the shape and is of length 1.
    return Segment2(circle.origin, Vector2(circle.origin, shape.getCenterOfMass()).getNormalized(), true);
}

// Returns the axis of the given shapes that corresponds to the given index.
template <typename T1, typename T2>
inline Segment2 collisions2D::ShapesGetAxis(T1 shape1, T2 shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape1.getSide(index);
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape2.getSide(index - shape1.getSidesNum());
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }

    return axis;
}

// Shapes axis specialization for a circle and a shape.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Circle, Segment2>(Circle shape1, Segment2 shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is a circle, get its axis.
        axis = CircleGetAxis(shape1, shape2);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape2.getSide(index - shape1.getSidesNum());
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }

    return axis;
}

// Shapes axis specialization for a circle and a shape.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Circle, Triangle2>(Circle shape1, Triangle2 shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is a circle, get its axis.
        axis = CircleGetAxis(shape1, shape2);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape2.getSide(index - shape1.getSidesNum());
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }

    return axis;
}

// Shapes axis specialization for a circle and a shape.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Circle, Rectangle>(Circle shape1, Rectangle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is a circle, get its axis.
        axis = CircleGetAxis(shape1, shape2);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape2.getSide(index - shape1.getSidesNum());
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }

    return axis;
}

// Shapes axis specialization for a circle and a shape.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Circle, Polygon>(Circle shape1, Polygon shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is a circle, get its axis.
        axis = CircleGetAxis(shape1, shape2);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape2.getSide(index - shape1.getSidesNum());
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }

    return axis;
}

// Shapes axis specialization for a shape and a circle.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Segment2, Circle>(Segment2 shape1, Circle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape1.getSide(index);
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is a circle, get its axis.
        axis = CircleGetAxis(shape2, shape1);
    }

    return axis;
}

template <>
inline Segment2 collisions2D::ShapesGetAxis<Triangle2, Circle>(Triangle2 shape1, Circle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape1.getSide(index);
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is a circle, get its axis.
        axis = CircleGetAxis(shape2, shape1);
    }

    return axis;
}

template <>
inline Segment2 collisions2D::ShapesGetAxis<Rectangle, Circle>(Rectangle shape1, Circle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape1.getSide(index);
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is a circle, get its axis.
        axis = CircleGetAxis(shape2, shape1);
    }

    return axis;
}


template <>
inline Segment2 collisions2D::ShapesGetAxis<Polygon, Circle>(Polygon shape1, Circle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is not a circle, get the side pointed to by the index and calculate its normal.
        side = shape1.getSide(index);
        axis = Segment2(side.getCenterOfMass(),
                        Vector2(side).getNormalized().getNormal(),
                        true);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is a circle, get its axis.
        axis = CircleGetAxis(shape2, shape1);
    }

    return axis;
}

// Shapes axis specialization for 2 circles.
template <>
inline Segment2 collisions2D::ShapesGetAxis<Circle, Circle>(Circle shape1, Circle shape2, int index)
{
    assert (0 <= index && index < shape1.getSidesNum() + shape2.getSidesNum());

    Segment2 side;
    Segment2 axis;

    // If the given index refers to an axis of the first shape...
    if (index < shape1.getSidesNum())
    {
        // If the first shape is a circle, get its axis.
        axis = CircleGetAxis(shape1, shape2);
    }
    // If the given index refers to an axis of the second shape...
    else
    {
        // If the second shape is a circle, get its axis.
        axis = CircleGetAxis(shape2, shape1);
    }

    return axis;
}

// Project a shape onto a given axis.
template <typename T>
inline Segment2 collisions2D::projectShapeOnAxis(Segment2 axis, T shape)
{
    // Get the axis' vector.
    Vector2 axis_vec = Vector2(axis);

    //* https://fr.wikipedia.org/wiki/Projection_orthogonale#Projet%C3%A9_orthogonal_sur_une_droite,_distance

    // Get all the vertices of the shape.
    int vertices_num = shape.getVerticesNum();
    Vector2 vertex;
    Vector2 projected_points[vertices_num];

    // Loop over the vertices of the shape and get their projections onto the axis.
    for (int i = 0; i < vertices_num; i++)
    {
        vertex = shape.getVertex(i);
        projected_points[i] = axis.a + axis_vec * (Vector2(axis.a, vertex) & axis_vec);
    }

    // Find the closest and farthest points from the axis origin.
    Vector2 min_point = projected_points[0];
    Vector2 max_point = min_point;

    for (int i = 0; i < vertices_num; i++)
    {
        if (projected_points[i].getCopiedSign(axis_vec).x > max_point.getCopiedSign(axis_vec).x ||
            projected_points[i].getCopiedSign(axis_vec).y > max_point.getCopiedSign(axis_vec).y)
        {
            max_point = projected_points[i];
        }

        if (projected_points[i].getCopiedSign(axis_vec).x < min_point.getCopiedSign(axis_vec).x ||
            projected_points[i].getCopiedSign(axis_vec).y < min_point.getCopiedSign(axis_vec).y)
        {
            min_point = projected_points[i];
        }
    }

    Vector2 axis_orig_to_min_point = Vector2(axis.a, min_point);
    Segment2 projection = Segment2(axis.a + axis_orig_to_min_point, Vector2(min_point, max_point), true);

    return projection;
}

// Projection specialization for a circle.
template <>
inline Segment2 collisions2D::projectShapeOnAxis<Circle>(Segment2 axis, Circle shape)
{
    // Get the axis' vector.
    Vector2 axis_vec = Vector2(axis);

    // Project the circle's origin onto the axis.
    Vector2 origin_projection = axis.a + axis_vec * (Vector2(axis.a, shape.origin) & axis_vec);

    // Create a Segment2 of the circle's projection.
    Segment2 circle_projection = Segment2(origin_projection - axis_vec * shape.radius,
                                            origin_projection + axis_vec * shape.radius);
    
    return circle_projection;
}

// Checks for collision between two given shapes.
template<typename T1, typename T2>
inline bool collisions2D::collisionSAT(T1 shape1, T2 shape2)
{
    // Check for collisions2D on the shapes' bounding boxes to not have to check if they are not in collision.
    if (collisionAABB(getBoundingBox(shape1), getBoundingBox(shape2)))
    {
        // Get the number of sides of both shapes.
        int sides = shape1.getSidesNum() + shape2.getSidesNum();

        // Loop over all of the axes.
        for (int i = 0; i < sides; i++)
        {
            // Project both shapes onto the axis.
            Segment2 projection1 = projectShapeOnAxis(ShapesGetAxis(shape1, shape2, i), shape1);
            Segment2 projection2 = projectShapeOnAxis(ShapesGetAxis(shape1, shape2, i), shape2);

            // If the projections don't overlap, the shapes are not in collision.
            if (!collisionProjections(projection1, projection2))
                return false;
        }
        return true;
    }
    return false;
}

// SAT specialization for circles.
template<>
inline bool collisions2D::collisionSAT<Circle, Circle>(Circle shape1, Circle shape2)
{
    return collisionCircles(shape1, shape2);
}

// SAT specialization for rectangles.
template<>
inline bool collisions2D::collisionSAT<Rectangle, Rectangle>(Rectangle shape1, Rectangle shape2)
{
    return collisionAABB(shape1, shape2);
}
