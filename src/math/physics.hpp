#pragma once
#include "Vector2.hpp"

bool aabb(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
bool circleCollision(Vector2f a, float ar, Vector2f b, float br);

