#ifndef FLOCK_H_
#define FLOCK_H_

#include <vector>
#include <string_view>
#include <ngl/AbstractVAO.h>
#include <ngl/Mat4.h>
#include "boid.h"

class Flock {
public:
    Flock(size_t numBoids);
    void update();
    void render(const ngl::Mat4& _view, const ngl::Mat4& _project, GLuint _texture) const;;

    void setMaxSpeed(int _newBoidMaxSpeed);
    void setMaxForce(int _newBoidMaxForce);
    void setDesiredSeparation(int _newBoidDesiredSeparation);
    void setAlignmentDist(int _newAlignmentDist);
    void setCohesionDist(int _newCohesionDist);

private:
    std::vector<boid> boids;
    ngl::Vec3 randomVectorOnSphere();
    void initializeBoids();
    ngl::Vec3 separation(const boid& boid);
    float desiredSeparation = 4.0f;
    ngl::Vec3 alignment(const boid& boid);
    float alignmentDist = 5.0f;
    ngl::Vec3 cohesion(const boid& boid);
    float cohesionDist = 3.0f;
    ngl::Vec3 seek(const boid& boid, const ngl::Vec3& target);
    ngl::Vec3 m_bounds = {100, 100, 50};
    void checkBounds(boid& boid);
    float maxSpeed = 20.0f;
    float maxForce = 0.1f;
    ngl::Vec3 m_position = {0, 0, 0};
    float m_spread = 20.0f;
    ngl::Vec3 m_centre = {20, 0, 0};
    float m_radius;
    float boidSize = 5;
    ngl::Vec3 circleTarget(const boid& boid);
    std::unique_ptr<ngl::AbstractVAO> m_vao;
};


#endif // FLOCK_H_
