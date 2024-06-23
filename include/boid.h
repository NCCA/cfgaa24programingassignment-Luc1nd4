#ifndef BOID_H_
#define BOID_H_

#include <ngl/Vec3.h>

struct boid {
    ngl::Vec3 pos;
    ngl::Vec3 velocity;
    ngl::Vec3 colour;
    ngl::Vec3 acceleration;
    float maxSpeed;
    float maxForce;

    std::string modelID = "fish";

    // Default constructor
    boid() : pos(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), colour(0.0f, 1.0f, 1.0f), acceleration(0.0f, 0.0f, 0.0f),  maxSpeed(20.0f), maxForce(50.0f) {}

    // Custom constructor
    boid(ngl::Vec3 _pos, ngl::Vec3 _velocity, ngl::Vec3 _acceleration, float _maxSpeed, float _maxForce, float _size = 5.0f, ngl::Vec3 _colour = {0, 0, 1}) :
            pos(_pos), velocity(_velocity), acceleration(_acceleration), maxSpeed(_maxSpeed), maxForce(_maxForce), colour(_colour) {}
};


#endif // BOID_H_
