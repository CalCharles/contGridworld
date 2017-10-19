#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <Eigen/LU>


using namespace Eigen;

class Piece {
protected:
    float mass;
    Vector2f acceleration;
    Vector2f velocity;
    Vector2f location;
    float orientation;
    float moment;    
    float rotAcc;
    float rotVel;
    Vector3f color;
    float attach;
    Vector2f radii;
    string name;
    string geometry;
    string type; // Not exactly the safe way of doing this
    Piece* attach


    Piece();
	~Piece();
	Piece(Piece* obj);

	virtual void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);

    virtual float calculateMoment();

    virtual bool computeOccupancy(point);

    bool testAabb(Piece* b);

    bool compareAabb(Piece* b);

    virtual Vector2f computeIntersection(Piece* other);

    virtual void applyContact(Piece* other, Vector2f point);

    virtual void updateVelocity(Vector2f newVelocity);

    virtual void updateLocation(Vector2f location);

};

class Ellipse : public Piece{
	/*
	Even though these are called Ellipses, I haven't actually implemented ellipse functionality, since the dynamics for these can be somewhat troublesome.
	*/
protected:
    float r2

    Ellipse();
    ~Ellipse();
    Ellipse(Ellipse* obj);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);

	float calculateMoment();

	bool computeOccupancy(point);

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Rectangle : public Piece{
protected:
    float corners[8];

	Rectangle();
	~Rectangle();
	Rectangle(Rectangle* rect);
	
	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);

	float calculateMoment();

	bool computeOccupancy(point);

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Agent : public Piece {
protected:
	Piece shape;
	bool grabbing;
	float reward;

	Agent();
	~Agent();
	Agent(Agent* agent);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);

	float calculateMoment();

	void toggleGrab();

	void rotate(float dt);

	bool computeOccupancy(point);

	void updateShape();

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Hazard : public Piece{
protected:
	Piece shape;
	float reward;

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);

	float calculateMoment();

	bool computeOccupancy(point);
	
	void updateShape();

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Terminal : public Piece{
protected:
	Piece shape;
	float reward;
	bool terminate;
	Piece* obj;

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, string name);
	
	void set_condition(Piece* obj, bool attach);

	void satisfyCondition(Piece* obj);

	float calculateMoment();
	
	void updateShape();

	bool computeOccupancy(point);

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};