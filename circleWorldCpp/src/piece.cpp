
#include "piece.h"

using namespace Eigen;
// class NotImplementedException : public std::logic_error
// {
// public:
//     virtual char const * what() const { return "Function not yet implemented."; }
// };

#define magicError1 -1232.0
#define magicError2 -9992.0

Vector2f qsolver(float a, float b, float c)
{
	float d, x1,x2;
	d=(b*b)-(4*a*c);
	if(d>=0){
		x1=(-b+sqrt(d))/(2*a);
		x2=(-b-sqrt(d))/(2*a);
		return Vector2f(x1,x2);
	}else{
		return Vector2f(magicError1, magicError2); // assuming this is an invalid value, if not, this should be switched to using NaN
	}

}

Piece::Piece() {
	// does nothing
}

Piece::~Piece() {
	// if Vectors manage their own memory, does nothing
}

bool Piece::testAabb(Vector2f b) {
    if (fabs(this->location[0] - b[0]) > (this->radii[0])) return false;
    if (fabs(this->location[1] - b[1]) > (this->radii[1])) return false;
    return true;

}

bool Piece::compareAabb(Piece* b) {
    if (fabs(this->location[0] - b->location[0]) > (this->radii[0] + b->radii[0])) return false;
    if (fabs(this->location[1] - b->location[1]) > (this->radii[1] + b->radii[1])) return false;
    return true;
}

Rectangle::Rectangle() {
	// does nothing
}

Rectangle::~Rectangle() {
	// if Vectors manage their own memory, does nothing
}

void Rectangle::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	/* Must call setPiece in order to construct, and I am too lazy to write
	a constructor that calls this, even though it is only one line and
	writing this comment takes longer than that
	*/
	this->type = "Rectangle";
	this->geometry = "Rectangle";
	this->mass = mass;
	this->radii = radii;
	this->velocity = velocity;
	this->acceleration = Vector2f(0,0);
	this->location = location;
	this->orientation = 0.0f;
	this->moment = this->calculateMoment();
	this->rotAcc = 0.0f; // not yet implemented
	this->rotVel = 0.0f;// not yet implemented
	this->attach = NULL;
	this->color = color;
	this->name = name;

	Vector2f t1(radii[0], radii[1]);
	Vector2f t2(radii[0], -radii[1]);
	this->corners[0] = this->location[0] - t1[0];
	this->corners[1] = this->location[1] - t1[1];
	this->corners[2] = this->location[0] + t2[0];
	this->corners[3] = this->location[1] + t2[1];
	this->corners[4] = this->location[0] + t1[0];
	this->corners[5] = this->location[1] + t1[1];
	this->corners[6] = this->location[0] - t2[0];
	this->corners[7] = this->location[1] - t2[1];
}

float Rectangle::calculateMoment() {
	return pow(this->radii[0] * 2, 3)/12 + pow(this->radii[1] * 2, 3)/12;
}

bool Rectangle::computeOccupancy(Vector2f point) {
	// Orientation not yet implemented
	return this->testAabb(point);
}

inline float square(float x) {
	return x * x;
}

Vector2f Rectangle::computeIntersection(Piece* other) {
	if(other->geometry == "Ellipse") {
		if (this->compareAabb(other)) {
			if (this->testAabb(other->location)) {
				return Vector2f(this->corners[0], this->corners[1]);
			}
			for(int i = 0; i < 8; i += 2) {
				Vector2f edge1 = Vector2f(this->corners[i], this->corners[i+1]);
				Vector2f edge2;
				if (i != 6){
					edge2 = Vector2f(this->corners[i+2], this->corners[i+3]);
				} else {
					edge2 = Vector2f(this->corners[0], this->corners[1]);					
				}
		        // std::cout << "edge1 " << edge1 << "\n" "edge2" << edge2 << " \n";
		        Vector2f csum = edge1 + edge2 + other->location;
		        float c1 = csum.squaredNorm();
		        float c2 = -2*edge1[0]*csum[0] - 2*edge1[1]*csum[1];
		        float c3 = square(edge1[0]) + square(edge1[1]) - ((Ellipse*)other)->r2;
		        Vector2f roots = qsolver(c1, c2, c3);
		        if (roots[0] == -1232.0f && roots[1] == -9992.0f) {
		        	continue;
		        } else if (roots[0] == roots[1]) {
		        	return Vector2f((1-roots[0]) * edge1[0] + roots[0] * edge2[0], (1-roots[0]) * edge1[1] + roots[0] * edge2[1]);
		        } else {
		        	float avg = (roots[0] + roots[1])/2.0f;
		        	return Vector2f((1-avg) * edge1[0] + avg * edge2[0], (1-avg) * edge1[1] + avg * edge2[1]);
		        }
		    }
		}
		return Vector2f(magicError1, magicError2);
	}
	if (other->geometry == "Rectangle") {
		// I haven't implemented collisions with rectangles (though this should be fairly easy, just line-segment, line-segments collisions )
		return Vector2f(magicError1, magicError2);
		// throw std::invalid_argument("Not implemented yet");
	}
	// If you get a shape that isn't actually a shape
	throw std::invalid_argument("Unknown Shape");
}

void Rectangle::applyContact(Piece* other, Vector2f point) {
	// I haven't written contacts with rectangles because I'm not sure how the dynamics work yet
	throw std::invalid_argument("Not implemented yet");
}

void Rectangle::updateVelocity(Vector2f newVelocity) {
	// I haven't written motion with rectangles because I'm not sure how the dynamics work yet
	throw std::invalid_argument("Not implemented yet");
}

void Rectangle::updateLocation(Vector2f location) {
	throw std::invalid_argument("Not implemented yet");
}

Ellipse::Ellipse() {
	// does nothing
}

Ellipse::~Ellipse() {
	// does nothing
}

void Ellipse::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	/* Must call setPiece in order to construct, and I am too lazy to write
	a constructor that calls this, even though it is only one line and
	writing this comment takes longer than that
	*/
	this->type = "Ellipse";
	this->geometry = "Ellipse";
	this->mass = mass;
	this->radii = radii;
	this->velocity = velocity;
	this->acceleration = Vector2f(0,0);
	this->location = location;
	this->orientation = 0.0f;
	this->r2 = radii[0] * radii[1]; 
	this->moment = this->calculateMoment();
	this->rotAcc = 0.0f; // not yet implemented
	this->rotVel = 0.0f; // not yet implemented
	this->attach = NULL;
	this->color = color;
	this->name = name;
}

float Ellipse::calculateMoment() {
	return this->mass * (this->r2)/2.0f;
}

bool Ellipse::computeOccupancy(Vector2f point) {
	return (point - this->location).squaredNorm() < this->r2;
}

Vector2f Ellipse::computeIntersection(Piece* other) {
	if (other->geometry == "Ellipse") {
		if(this->compareAabb(other)) {
			float d = (this->location - other->location).norm();
			if (d <= this->radii[0] + other->radii[0] + .05 && d != 0) {
				float a = (this->r2 + (other->radii[0] * other->radii[1]) + pow(d,2))/(2*d);
				// std::cout << a << "\n" << this->location << "\n" << other->location << "\n\n";
				return this->location + a * (other->location + this->location);
			}
		}
		return Vector2f(magicError1, magicError2);
	}
	else if (other->geometry == "Rectangle") {
		if(this->compareAabb(other)) {
			return other->computeIntersection(this);
		}
		return Vector2f(magicError1, magicError2);
	}
	throw std::invalid_argument("Unknown shape");
}

void Ellipse::applyContact(Piece* other, Vector2f point) {
	// it would be nice to add some inelasticity to the values
	if(other->type == "Hazard") {
		if (this->attach != NULL && this->attach->type == "Agent") {
			((Agent*)this->attach)->reward += ((Hazard*)other)->reward;
		}
	} else if (other->type == "Terminal") {
		if (((Terminal*)other)->satisfyCondition(this)) {
			return;
		}
	} else {
			if (other->geometry == "Ellipse") {
                float weighting = 2.0f/(this->mass + other->mass)*((this->velocity-other->velocity).dot(this->location-other->location))/((this->location - other->location).squaredNorm());
				std::cout<< "contact: " << weighting << "\n" << other->velocity - this->mass *weighting* (other->location-this->location) << "\n";
                this->updateVelocity(this->velocity - other->mass *weighting* (this->location-other->location));
                other->updateVelocity(other->velocity - this->mass *weighting* (other->location-this->location));
		} else if (other->geometry == "Rectangle") {
			throw std::invalid_argument("Not implemented yet");
		} 
	}
}

void Ellipse::updateVelocity(Vector2f newVelocity) { 
	this->velocity = newVelocity;
	// Doesn't support application of torques
	if(this->attach != NULL) {
		this->attach->velocity = newVelocity;
	}
}

void Ellipse::updateLocation(Vector2f location) {
	this->location = location;
}


Agent::Agent() {
	// must call setPiece
}

Agent::~Agent() {
	// Nothing to delete
}

void Agent::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	if (name == "Ellipse") {
		this->shape = new Ellipse();
		this->shape->setPiece(mass, radii, velocity, location, color, name);
		this->geometry = "Ellipse";
	} // other shapes are not yet implemented
    this->grabbing= false;
    this->color = color;
    this->reward = 0;
    this->name = "Agent";
    this->attach = NULL;
    this->mass = mass;
    this->radii = radii;
    this->type = "Agent"; 
    this->location = this->shape->location;
	this->velocity = this->shape->velocity;
	this->acceleration = this->shape->acceleration;
	this->orientation = this->shape->orientation;
	this->rotAcc = this->shape->rotAcc;
	this->rotVel = this->shape->rotVel;
	this->moment = this->shape->calculateMoment();
}

float Agent::calculateMoment() {
	return this->shape->calculateMoment();
}

bool Agent::computeOccupancy(Vector2f point) {
	return this->shape->computeOccupancy(point);
}

Vector2f Agent::computeIntersection(Piece* other) {
	return this->shape->computeIntersection(other);
}

void Agent::toggleGrab() {
	if (this->attach != NULL) {
		// this->updateShape();
		Vector2f launchvec = this->attach->location - this->shape->location;
		this->attach->location = this->attach->location+ 0.2 * launchvec;
        this->attach->attach = NULL;
		this->updateVelocity(this->attach->velocity + launchvec / launchvec.norm() * (5 * this->mass/(this->attach->mass + this->mass)));
        float oldmass = this->attach->mass;
        this->attach = NULL;
        this->updateVelocity(this->velocity - launchvec / launchvec.norm() * (5 * oldmass/(oldmass + this->mass)));
	} else {
		this->grabbing = !this->grabbing;
	}
}

void Agent::updateShape() {
	// when the shape and the agent get out of sync, perform this procedure (whenever the agent performs any operation)
	this->shape->location = this->location;
	this->shape->velocity = this->velocity;
	this->shape->acceleration = this->acceleration;
	this->shape->orientation = this->orientation;
	this->shape->rotAcc = this->rotAcc;
	this->shape->rotVel = this->rotVel;
}

void Agent::rotate(float dt) {
	/*  rotates a fixed number of radians based on the total mass and the size of the time step, such that for unit agent mass and no other mass,
        the rotation is pi/4 radians per second (does nothing if nothing is attached, however, since orientation and rotation have not been written yet
        furthermore, the orientatin of the attached object is unchanged by this operation)
    */
    if (this->attach != NULL) {
	    float angle = 10*dt/((this->location-this->attach->location).norm() * (this->attach->mass +this->mass));
	    Matrix2f rotation;
	    rotation << cos(angle), -sin(angle),
	    			sin(angle), cos(angle);
	    this->attach->location = rotation * (this->attach->location-this->location) + this->location;
	}
}

void Agent::applyContact(Piece* other, Vector2f point) {
	// std::cout<< "contact\n" << other->type << "\n";
	if ( other->type == "Hazard") {
        this->reward += ((Hazard*)other)->reward;
    } else if ( other->type == "Terminal") {
        if (((Terminal*)other)->satisfyCondition(this));
    } else {
		if (other->geometry == "Ellipse") {
	        if (this->grabbing) {
	            this->updateVelocity((this->mass * this->velocity + other->mass * other->velocity)/ (this->mass + other->mass));
	            other->velocity = this->velocity;
	            this->attach = other;
	            other->attach = this;
	            this->grabbing = false;
	        } else {
	            if (this->attach == NULL) {
	                // only use the object mass
	                float weighting = 2/(this->mass + other->mass)*((this->velocity-other->velocity).dot(this->location-other->location))/((this->location - other->location).squaredNorm());
	                this->updateVelocity(this->velocity - other->mass *weighting* (this->location-other->location));
	                other->updateVelocity(other->velocity - this->mass *weighting* (other->location-this->location));
	            } else{
	                // use the total mass, but ignore rotational effects
	                float totalMass = this->mass + this->attach->mass;
	        		// std::cout<< ((this->velocity-other->velocity).dot(this->location-other->location))/((this->location-other->location).squaredNorm()) << "\n"; 
	                float weighting = 2/(totalMass + other->mass)*((this->velocity-other->velocity).dot(this->location-other->location))/((this->location-other->location).squaredNorm());
	                this->updateVelocity(this->velocity - other->mass *weighting* (this->location-other->location));
	                other->updateVelocity(other->velocity - totalMass *weighting* (other->location-this->location)); 
	            }
	        }
	    } else if ( other->geometry == "Rectangle") {
	        throw std::invalid_argument("Rectangle not yet implemented");
	    }
	}
}

void Agent::updateVelocity(Vector2f newVelocity) {
	this->shape->velocity = newVelocity;
	this->velocity = newVelocity;
	// Doesn't support application of torques
	if(this->attach != NULL) {
		this->attach->velocity = newVelocity;
	}
}

void Agent::updateLocation(Vector2f location) {
	this->location = location;
	this->shape->location = location;
}




Hazard::Hazard() {
	// does nothing
}

Hazard::~Hazard() {
	// does nothing
}

void Hazard::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	if (name == "Ellipse") {
		this->shape = new Ellipse();
		this->shape->setPiece(mass, radii, velocity, location, color, name);
		this->geometry = "Ellipse";
	} // other shapes are not yet implemented
	if (name == "Rectangle") {
		this->shape = new Rectangle();
		this->shape->setPiece(mass, radii, velocity, location, color, name);
		this->geometry = "Rectangle";
	} // other shapes are not yet implemented
    this->reward = 0;
    this->name = name;
    this->color = color;
    this->attach = NULL;
    this->mass = mass;
    this->radii = radii;
    this->type = "Hazard"; 
    this->location = this->shape->location;
	this->velocity = this->shape->velocity;
	this->acceleration = this->shape->acceleration;
	this->orientation = this->shape->orientation;
	this->rotAcc = this->shape->rotAcc;
	this->rotVel = this->shape->rotVel;
	this->moment = this->calculateMoment();
}

void Hazard::setReward(float reward, float dt) {
	this->reward = reward * dt;
}

void Hazard::updateShape() {
	// when the shape and the hazard get out of sync, update the hazard parameters. In general, hazards are not affected by dynamics, so this might not be too important
	this->shape->location = this->location;
	this->shape->velocity = this->velocity;
	this->shape->acceleration = this->acceleration;
	this->shape->orientation = this->orientation;
	this->shape->rotAcc = this->rotAcc;
	this->shape->rotVel = this->rotVel;
}

float Hazard::calculateMoment() {
	return this->shape->calculateMoment();
}

bool Hazard::computeOccupancy(Vector2f point) {
	return this->shape->computeOccupancy(point);
}

Vector2f Hazard::computeIntersection(Piece* other) {
	return this->shape->computeIntersection(other);
}

void Hazard::applyContact(Piece* other, Vector2f point) {
	return other->applyContact(this, point);
}

void Hazard::updateVelocity(Vector2f newVelocity) {
	throw std::invalid_argument("Velocities not implemented for hazards");
}

void Hazard::updateLocation(Vector2f location) {
	// you can change positions with time, so technically you can have velocities
	this->location = location;
	this->shape->location = location;
}

Terminal::Terminal() {
	// does nothing
}

Terminal::~Terminal() {
	// does nothing
}

void Terminal::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	if (name == "Ellipse") {
		this->shape = new Ellipse();
		this->shape->setPiece(mass, radii, velocity, location, color, name);
		this->geometry = "Ellipse";
	} // other shapes are not yet implemented
	if (name == "Rectangle") {
		this->shape = new Rectangle();
		this->shape->setPiece(mass, radii, velocity, location, color, name);
		this->geometry = "Rectangle";
	} // other shapes are not yet implemented

    this->reward = 0;
    this->color = color;
    this->name = name;
    this->attach = NULL;
    this->mass = mass;
    this->radii = radii;
    this->type = "Terminal"; 
    this->location = this->shape->location;
	this->velocity = this->shape->velocity;
	this->acceleration = this->shape->acceleration;
	this->orientation = this->shape->orientation;
	this->rotAcc = this->shape->rotAcc;
	this->rotVel = this->shape->rotVel;
	this->moment = this->calculateMoment();
}

void Terminal::setReward(float reward, float dt) {
	this->reward = reward * dt;
}

void Terminal::updateShape() {
	// when the shape and the hazard get out of sync, update the hazard parameters. In general, hazards are not affected by dynamics, so this might not be too important
	this->shape->location = this->location;
	this->shape->velocity = this->velocity;
	this->shape->acceleration = this->acceleration;
	this->shape->orientation = this->orientation;
	this->shape->rotAcc = this->rotAcc;
	this->shape->rotVel = this->rotVel;
}

float Terminal::calculateMoment() {
	return this->shape->calculateMoment();
}

bool Terminal::computeOccupancy(Vector2f point) {
	return this->shape->computeOccupancy(point);
}

Vector2f Terminal::computeIntersection(Piece* other) {
	return this->shape->computeIntersection(other);
}

void Terminal::applyContact(Piece* other, Vector2f point) {
	return other->applyContact(this, point);
}

void Terminal::updateVelocity(Vector2f newVelocity) {
	throw std::invalid_argument("Velocities not implemented for hazards");
}

void Terminal::updateLocation(Vector2f location) {
	// you can change positions with time, so technically you can have velocities
	this->location = location;
	this->shape->location = location;
}

void Terminal::setCondition(Piece* obj, Piece* attach) {
	this->obj = obj;
	this->attach = attach;
}

bool Terminal::satisfyCondition(Piece* obj) {
	if(obj->name == this->obj->name) {
        if(this->attach != NULL) {
            this->terminate = true;
            return true;
        } else {
            if (obj->attach != NULL && obj->attach->type == "Agent") {
                this->terminate = true;
                return true;
        	}
            return false;
        }
    } else {
    	return false;
    }
}



// Wall::Wall() {
// 	// no initialization
// }
// Wall::~Wall(); {
// 	// no initialization
// }
// Wall::Wall(Wall* rect) {
// 	// no initialization
// }
	
// virtual void Wall::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
// 	/*
// 	location and radii are the endpoints of the wall. 
// 	*/
// 	this->type = "Wall";
// 	this->geometry = "Wall";
// 	this->mass = mass;
// 	this->radii = radii;
// 	this->velocity = velocity;
// 	this->acceleration = Vector2f(0,0);
// 	this->location = location;
// 	this->orientation = 0.0f;
// 	this->moment = this->calculateMoment();
// 	this->rotAcc = 0.0f; // not yet implemented
// 	this->rotVel = 0.0f;// not yet implemented
// 	this->attach = NULL;
// 	this->color = color;
// 	this->name = name;

// 	Vector2f t1(radii[0], radii[1]);
// 	Vector2f t2(radii[0], -radii[1]);
// 	this->corners[0] = this->location[0] - t1[0];
// 	this->corners[1] = this->location[1] - t1[1];
// 	this->corners[2] = this->location[0] + t2[0];
// 	this->corners[3] = this->location[1] + t2[1];
// 	this->corners[4] = this->location[0] + t1[0];
// 	this->corners[5] = this->location[1] + t1[1];
// 	this->corners[6] = this->location[0] - t2[0];
// 	this->corners[7] = this->location[1] - t2[1];


// }

// 	virtual void applyContact(Piece* other, Vector2f point);
