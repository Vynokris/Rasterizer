#include "Object.hpp"

// ---------- Object class ---------- //

Object::Object(const Transform& _t) : t(_t) {}

Transform Object::getTransform() const {}

void Object::setTransform(const Transform& _t) { t = _t; }

void Object::draw() {}

// -------- Cube child class -------- //

Cube::Cube(const Transform& _t, const float& _size) : Object(_t), size(_size) {}

float Cube::getSize() const { return size; };