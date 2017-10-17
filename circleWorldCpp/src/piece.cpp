using namespace std;

#include <cstring>
#include "piece.h"

// class NotImplementedException : public std::logic_error
// {
// public:
//     virtual char const * what() const { return "Function not yet implemented."; }
// };

Vector2f qsolver(float a, float b, float c, )
{
	float d, x1,x2;
	d=(b*b)-(4*a*c);
	if(d>=0){
		x1=(-b+sqrt(d))/(2*a);
		x2=(-b-sqrt(d))/(2*a);
		return Vector2f(x1,x2);
	}else{
		return Vector2f(-1232f, -9992f); // assuming this is an invalid value, if not, this should be switched to using NaN
	}
	getch();

}

Piece::Piece() {
	// does nothing
}

Piece::~Piece() {
	// if Vectors manage their own memory, does nothing
}

bool Piece::testAabb(Piece* b) {
    if abs(this.location[0] - b[0]) > (self.radii[0]) return false;
    if abs(this.location[1] - b[1]) > (self.radii[1]) return false;
    return true;

}

bool Piece::compareAabb(Piece b) {
    if (abs(self.location[0] - b.location[0]) > (self.radii[0] + b.radii[0])) return false;
    if (abs(self.location[1] - b.location[1]) > (self.radii[1] + b.radii[1])) return false;
    return true;
}

Rectangle::Rectangle() {
	// does nothing
}

Rectangle::~Rectangle() {
	// if Vectors manage their own memory, does nothing
}

Rectangle::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name) {
	/* Must call setPiece in order to construct, and I am too lazy to write
	a constructor that calls this, even though it is only one line and
	writing this comment takes longer than that
	*/
	this.type = "Rectangle";
	this.shape = "Rectangle";
	this.mass = mass;
	this.radii = radii;
	this.velocity - velocity;
	this.location = location;
	this.orientation = 0f;
	this.moment = this.calculateMoment();
	this.rotAcc = 0f; // not yet implemented
	this.rotVel = 0f; // not yet implemented
	this.attach = NULL;
	this.color = color
	this.name = name;

	Vector2f t1(radii[0], radii[1]);
	Vector2f t2(radii[0], -radii[1]);
	this.corners[0] = this.location[0] - t1[0];
	this.corners[1] = this.location[1] - t1[1];
	this.corners[2] = this.location[0] + t2[0];
	this.corners[3] = this.location[1] + t2[1];
	this.corners[4] = this.location[0] + t1[0];
	this.corners[5] = this.location[1] + t1[1];
	this.corners[6] = this.location[0] - t2[0];
	this.corners[7] = this.location[1] - t2[1];
}

float Rectangle::calculateMoment() {
	return pow(this.radii[0] * 2, 3)/12 + pow(self.radii[1] * 2, 3)/12;
}

float Rectangle::computeOccupancy(Vector2f point) {
	// Orientation not yet implemented
	return this.testAabb(Vector2f point)
}

Vector2f computeIntersection(Piece* other) {
	if(other.shape == "Circle") {
		if (self.compareAabb(other)) {
			if self.testAabb(other->location) {
				return Vector2f(this.corners[0], this.corners[1]);
			}
			for(int i = 0; i < 8; i += 2) {
				Vector2f edge1 = Vector2f(this.corners[i], this.corners[i+1]);
				Vector2f edge2;
				if (i != 6){
					edge2 = Vector2f(this.corners[i+2], this.corners[i+3]);
				} else {
					edge2 = Vector2f(this.corners[0], this.corners[1]);					
				}
		        csum = edge1 + edge2 + other->location
		        float c1 = csum.squaredNorm();
		        float c2 = -2*edge1[0]*csum[0] - 2*edge1[1]*csum[1]
		        float c3 = edge1[0] ** 2 + edge1[1] ** 2 - ((Circle*)other)->r2;
		        Vector2f roots = np.roots(coeffs);
		        if (roots[0] == -1232f && roots[1] = -9992f) return Vector2f(-1232, -9992);
		        else if (roots[0] == roots[1]) {
		        	return Vector2f((1-roots[0]) * edge1[0] + roots[0] * edge2[0], (1-roots[0]) * edge1[1] + roots[0] * edge2[1]);
		        } else {
		        	float avg = (roots[0] + roots[1])/2
		        	return Vector2f((1-avg) * edge1[0] + avg * edge2[0], (1-avg) * edge1[1] + avg * edge2[1]);
		        }
		    }
		}
	}
	if (other.shape == "Rectangle") {
		// I haven't implemented collisions with rectangles (though this should be fairly easy, just line-segment, line-segments collisions )
		throw invalid_argument("Not implemented yet");
	}
	// If you get a shape that isn't actually a shape
	throw invalid_argument("Unknown Shape");
}

void Rectangle::applyContact(Piece* other, Vector2f point) {
	// I haven't written contacts with rectangles because I'm not sure how the dynamics work yet
	throw invalid_argument("Not implemented yet")
}

void Rectangle::updateVelocity(Vector2f newVelocity) {
	// I haven't written motion with rectangles because I'm not sure how the dynamics work yet
	throw invalid_argument("Not implemented yet")
}

void Ellipse::setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name) {
	/* Must call setPiece in order to construct, and I am too lazy to write
	a constructor that calls this, even though it is only one line and
	writing this comment takes longer than that
	*/
	this.type = "Ellipse";
	this.shape = "Ellipse";
	this.mass = mass;
	this.radii = radii;
	this.velocity - velocity;
	this.location = location;
	this.orientation = 0f;
	this.r2 = radii[0] * radii[0]; 
	this.moment = this.calculateMoment();
	this.rotAcc = 0f; // not yet implemented
	this.rotVel = 0f; // not yet implemented
	this.attach = NULL;
	this.color = color
	this.name = name;
}

float Ellipse::calculateMoment() {
	return self.mass * (self.r2)/2f;
}

bool Ellipse::computeOccupancy(Vector2f point) {
	return (point - this.location).squaredNorm() < self.r2;
}

Vector2f Ellipse::computeIntersection(Piece* other) {
	if (other->shape == "Circle") {
		if(this.compareAabb(other)) {
			d = (this.location - other->location).norm();
			if (d <= this.radii[0] + other->radii[0] + .05) {
				a = (this.r2 + other->r2 + pow(d,2))/(2*d);
				return this.location + a * (other->location + this.location);
			}
		}
		else if (other.shape == "Rectangle") {
			if(this.compareAabb(other)) {
				return other->computeIntersection(&this);
			}
		}
	}
	throw invalid_argument("Unknown shape");
}

void Ellipse::applyContact(Piece* other, Vector2f point) {
	// it would be nice to add some inelasticity to the values
	if(other->type == "Hazard") {
		if (this.attach != NULL && this.attach->type == "Agent") {
			self.attach.reward += other.reward;
		}
	} else if (other.type == "Terminal") {
		if (other->satisfy_condition(&this)) {
			return;
		}
	} else {
			if (other->shape == "Circle") {
			weighting = 2f/(this.mass + other->mass)*(this.velocity-other.velocity).dot(this.location-other.location)/(this.location-other.location).squaredNorm();
			self.velocity = this.velocity -other->mass * weighting * this.location - other->location
			other.velocity = other->velocity - self.mass * weighting * this.location - self.location
		} else if (other->shape == "Rectangle") {
			throw invalid_argument("Not implemented yet");
		} 
	}
}

void Ellipse::updateVelocity(Vector2f newVelocity) { 
	this.velocity = newVelocity;
	// Doesn't support application of torques
	if(this.attach != NULL) {
		this.attach->velocity = newVelocity;
	}
}

