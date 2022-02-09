#include "my_math.hpp"

struct Transform
{
    Vector3 pos;
    Vector3 rot;
    Vector3 scale;
};

class Object
{
private:
    Transform t;

public:
    Object() {}
    Object(const Transform& _t);
    virtual ~Object() {}

    virtual Transform getTransform() const;
    virtual void setTransform(const Transform& _t);
    virtual void draw();
};

class Cube : public Object
{
private:
    float size;
public:
    Cube() {}
    Cube(const Transform& _t, const float& _size);
    ~Cube() {}

    float getSize() const;
};