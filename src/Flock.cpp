#include "Flock.h"
#include <ngl/Random.h>
#include <fmt/format.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
#include <ngl/VAOPrimitives.h>


Flock::Flock(size_t _numBoids)
{
    boids.resize(_numBoids);
    initializeBoids();
}

void Flock::initializeBoids()
{
    for (auto& boid : boids)
    {
        boid.pos = m_position + randomVectorOnSphere() * m_spread;
        boid.velocity = randomVectorOnSphere();
        boid.acceleration = ngl::Vec3::zero();
        boid.maxSpeed = 0.5f;
        boid.maxForce = 0.01f;
    }
}



// Calculates a force vector that keeps boids separated to avoid crowding neighbors
ngl::Vec3 Flock::separation(const boid& boid)
{
    ngl::Vec3 steer(0, 0, 0);
    int count = 0;
    for (const auto& other : boids)
    {
        float d = (boid.pos - other.pos).length();
        if (d > 0 && d < desiredSeparation)
        {
            // Vector pointing away from nearby boid
            ngl::Vec3 diff = boid.pos - other.pos;
            diff.normalize(); // Normalize to get direction
            diff /= d; // Inverse weighting by distance
            steer += diff; // Add direction to steer away
            count++; // Increment count of too-close boids
        }
    }
    if (count > 0)
    {
        steer /= static_cast<float>(count); // Average the steering over all too-close boids
    }
    if (steer.length() > 0)
    {
        steer.normalize(); // Normalize steering vector
        steer *= boid.maxSpeed; // Scale to maximum speed
        steer -= boid.velocity; // Adjust by current velocity
        steer.clamp(boid.maxForce); // Limit the force by max force
    }
    return steer; // Return the steering force
}


// Aligns boid velocity with the average velocity of nearby boids
ngl::Vec3 Flock::alignment(const boid& boid)
{
    ngl::Vec3 sum(0, 0, 0);
    int count = 0;
    for (const auto& other : boids)
    {
        float d = (boid.pos - other.pos).length();
        if (d > 0 && d < alignmentDist)
        {
            sum += other.velocity; // Sum up all velocities
            count++; // Count number of boids within alignment distance
        }
    }
    if (count > 0)
    {
        sum /= static_cast<float>(count); // Average the velocity
        sum.normalize(); // Normalize to get only the direction
        sum *= boid.maxSpeed; // Scale to maximum speed
        ngl::Vec3 steer = sum - boid.velocity; // Steering is the difference from current velocity
        steer.clamp(boid.maxForce); // Limit the steering force
        return steer; // Return the alignment force
    }
    return ngl::Vec3(0, 0, 0); // If no nearby boids, no alignment force
}

// Steers a boid to move towards the average position of nearby boids for cohesion
ngl::Vec3 Flock::cohesion(const boid& boid)
{
    ngl::Vec3 sum(0, 0, 0);
    int count = 0;
    for (const auto& other : boids)
    {
        float d = (boid.pos - other.pos).length();
        if (d > 0 && d < cohesionDist)
        {
            sum += other.pos; // Sum up the positions of all nearby boids
            count++; // Count the number of boids within cohesion distance
        }
    }
    if (count > 0)
    {
        sum /= static_cast<float>(count); // Compute the average position
        return seek(boid, sum); // Steer towards the average position
    }
    return ngl::Vec3(0, 0, 0); // If no nearby boids, no cohesion force
}


void Flock::checkBounds(boid& boid)
{
    if (std::abs(boid.pos.m_x) > m_bounds.m_x)
    {
        boid.velocity.m_x = -boid.velocity.m_x;
        boid.pos.m_x = std::copysign(m_bounds.m_x, boid.pos.m_x);
    }
    if (std::abs(boid.pos.m_y) > m_bounds.m_y)
    {
        boid.velocity.m_y = -boid.velocity.m_y;
        boid.pos.m_y = std::copysign(m_bounds.m_y, boid.pos.m_y);
    }
    if (std::abs(boid.pos.m_z) > m_bounds.m_z)
    {
        boid.velocity.m_z = -boid.velocity.m_z;
        boid.pos.m_z = std::copysign(m_bounds.m_z, boid.pos.m_z);
    }
}



void Flock::update()
{
    for (auto& boid : boids)
    {
        ngl::Vec3 sep = separation(boid);
        ngl::Vec3 ali = alignment(boid);
        ngl::Vec3 coh = cohesion(boid);
        ngl::Vec3 circle = seek(boid, circleTarget(boid));

        // Arbitrarily weighting these forces
        sep *= 2.0f; // Weight for separation might be higher to avoid crowding
        ali *= 2.0f; // Weight for alignment
        coh *= 1.0f; // Weight for cohesion
        circle *= 0.3f;

        boid.velocity += sep + ali + coh + circle;
        boid.velocity.clamp(boid.maxSpeed);
        boid.pos += boid.velocity;
        checkBounds(boid);
    }
}

ngl::Vec3 Flock::seek(const boid& boid, const ngl::Vec3& target)
{
    // Desired velocity is the vector pointing from the boid's current position to the target
    ngl::Vec3 desired = target - boid.pos;
    // Scale to maximum speed
    desired.normalize();
    desired *= boid.maxSpeed;

    // The steering force is the difference between the desired velocity and the boid's current velocity
    ngl::Vec3 steer = desired - boid.velocity;
    // Clamp the magnitude of the steering force to the maximum force that this boid can exert
    steer.clamp(boid.maxForce);  // Assuming clamp modifies the vector to limit its maximum length

    return steer;
}


void Flock::render(const ngl::Mat4& _view, const ngl::Mat4& _project, GLuint m_textureID, const ngl::Mat4& _mouse) const
{
    ngl::ShaderLib::use("ParticleShader");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    for (const auto& boid : boids) {
        ngl::Vec3 up(0, 1, 0);
        ngl::Vec3 forward = boid.velocity;
        forward.normalize();

        // Calculate right and orthogonal up vectors
        ngl::Vec3 right = up.cross(forward);
        right.normalize();
        up = forward.cross(right);
        up.normalize();

        // Create rotation matrix from axis vectors
        ngl::Mat4 rotation;
        rotation.m_m[0][0] = right.m_x; rotation.m_m[0][1] = right.m_y; rotation.m_m[0][2] = right.m_z; rotation.m_m[0][3] = 0.0f;
        rotation.m_m[1][0] = up.m_x;    rotation.m_m[1][1] = up.m_y;    rotation.m_m[1][2] = up.m_z;    rotation.m_m[1][3] = 0.0f;
        rotation.m_m[2][0] = forward.m_x; rotation.m_m[2][1] = forward.m_y; rotation.m_m[2][2] = forward.m_z; rotation.m_m[2][3] = 0.0f;
        rotation.m_m[3][0] = 0.0f;     rotation.m_m[3][1] = 0.0f;     rotation.m_m[3][2] = 0.0f;     rotation.m_m[3][3] = 1.0f;

        // Build the model matrix
        ngl::Mat4 model;
        model = model.translate(boid.pos.m_x, boid.pos.m_y, boid.pos.m_z);
        model *= rotation;


        // Combine with other transformations
        ngl::Mat4 MVP = _project * _view * model;
        ngl::ShaderLib::setUniform("MVP", MVP);

        // Draw the model
        ngl::VAOPrimitives::draw(boid.modelID);
    }
}


ngl::Vec3 Flock::randomVectorOnSphere()
{
    auto phi = ngl::Random::randomPositiveNumber(M_PI * 2.0f);
    auto costheta = ngl::Random::randomNumber();
    auto theta = acosf(costheta);
    auto u = ngl::Random::randomPositiveNumber();
    auto r = 1.0f *std::cbrt(u);
    return ngl::Vec3(r*sinf(theta) *cosf(phi),
                     r* sinf(theta) * sinf(phi),
                     r*cosf(theta));
}

ngl::Vec3 Flock::circleTarget(const boid& boid)
{
    ngl::Vec3 toCenter = m_centre - boid.pos;
    float longitude = atan2(toCenter.m_y, toCenter.m_x) + 0.1; // Longitude change
    float latitude = asin(toCenter.m_z / toCenter.length()) + 0.1; // Latitude change

    ngl::Vec3 target;
    target.m_x = m_centre.m_x + m_radius * cos(latitude) * cos(longitude);
    target.m_y = m_centre.m_y + m_radius * cos(latitude) * sin(longitude);
    target.m_z = m_centre.m_z + m_radius * sin(latitude);

    return target;
}

void Flock::setMaxSpeed(int speed)
{
    for(auto &boid : boids)
    {
        boid.maxSpeed = (float)speed/100.0f;
    }
}

void Flock::setMaxForce(int force)
{
    for(auto &boid : boids)
    {
        boid.maxForce = (float)force/1000.0f;
    }
}

void Flock::setDesiredSeparation(int _desiredSeparation)
{
    desiredSeparation = (float)_desiredSeparation/10.0f;
}

void Flock::setAlignmentDist(int _newAlignmentDist)
{
    alignmentDist = (float)_newAlignmentDist/10.0f;
}

void Flock::setCohesionDist(int _newCohesionDist)
{
    cohesionDist = (float)_newCohesionDist/10.0f;
}




