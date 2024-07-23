/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    EmitterX::Box::Box() {

    }

    EmitterX::Cylinder::Cylinder() : radius(), height(),
        start_angle(), end_angle(), on_edge(), direction() {

    }

    EmitterX::Polygon::Polygon() : size(), count(), direction() {

    }

    EmitterX::Sphere::Sphere() : radius(), latitude(), longitude(), on_edge(), direction() {

    }

    EmitterX::Data::Data() : start_time(), life_time(),
        flags(), timer(), emission_interval(), type(), seed() {
        loop_start_time = 0;
        loop_end_time = -1;
        particles_per_emission = 1.0f;
        direction = DIRECTION_EFFECT_ROTATION;
    }

    EmitterX::EmitterX() : buffer_init() {
        version = 0x04 ;
    }

    EmitterX::~EmitterX() {
        for (ParticleX*& i : particles)
            delete i;
    }

    EmitterX& EmitterX::operator=(const EmitterX& emit) {
        data = emit.data;
        version = emit.version;

        for (ParticleX*& i : particles)
            delete i;
        particles.clear();

        particles.reserve(emit.particles.size());
        for (ParticleX* i : emit.particles) {
            ParticleX* ptcl = new ParticleX;
            *ptcl = *i;
            particles.push_back(ptcl);
        }
        return *this;
    }
}
