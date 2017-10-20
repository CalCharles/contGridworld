#include <stdio.h>
// #include <stdlib.h>
#include <math.h>
#include "../eigen/Eigen/Dense"
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>


using namespace Eigen;

class Piece {
public:
    float mass;
    Vector2f acceleration;
    Vector2f velocity;
    Vector2f location;
    float orientation;
    float moment;    
    float rotAcc;
    float rotVel;
    Vector3f color;
    Vector2f radii;
    std::string name;
    std::string geometry;
    std::string type; // Not exactly the safe way of doing this
    Piece* attach;


    Piece();
	~Piece();
	Piece(Piece* obj);

	virtual void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name)=0;

    virtual float calculateMoment()=0;

    virtual bool computeOccupancy(Vector2f point)=0;

    bool testAabb(Vector2f b);

    bool compareAabb(Piece* b);

    virtual Vector2f computeIntersection(Piece* other)=0;

    virtual void applyContact(Piece* other, Vector2f point)=0;

    virtual void updateVelocity(Vector2f newVelocity)=0;

    virtual void updateLocation(Vector2f location)=0;

};

class Ellipse : public Piece{
	/*
	Even though these are called Ellipses, I haven't actually implemented ellipse functionality, since the dynamics for these can be somewhat troublesome.
	*/
public:
    float r2;

    Ellipse();
    ~Ellipse();
    Ellipse(Ellipse* obj);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);

	float calculateMoment();

	bool computeOccupancy(Vector2f point);

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Rectangle : public Piece{
public:
    float corners[8];

	Rectangle();
	~Rectangle();
	Rectangle(Rectangle* rect);
	
	virtual void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);

	float calculateMoment();

	bool computeOccupancy(Vector2f point);

	Vector2f computeIntersection(Piece* other);

	virtual void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Agent : public Piece {
public:
	Piece* shape;
	bool grabbing;
	float reward;

	Agent();
	~Agent();
	Agent(Agent* agent);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);

	float calculateMoment();

	void toggleGrab();

	void rotate(float dt);

	bool computeOccupancy(Vector2f point);

	void updateShape();

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Hazard : public Piece{
public:
	Piece* shape;
	float reward;

	Hazard();
	~Hazard();
	Hazard(Hazard* hazard);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);

	void setReward(float reward, float dt);

	float calculateMoment();

	bool computeOccupancy(Vector2f point);
	
	void updateShape();

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

class Terminal : public Piece{
public:
	Piece* shape;
	float reward;
	bool terminate;
	Piece* obj;

	Terminal();
	~Terminal();
	Terminal(Terminal* terminal);

	void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);
	
	void setCondition(Piece* obj, Piece* attach);

	void setReward(float reward, float dt);

	bool satisfyCondition(Piece* obj);

	float calculateMoment();
	
	void updateShape();

	bool computeOccupancy(Vector2f point);

	Vector2f computeIntersection(Piece* other);

	void applyContact(Piece* other, Vector2f point);

	void updateVelocity(Vector2f newVelocity);

    void updateLocation(Vector2f location);

};

// class Wall : public Rectangle {
// public:

// 	Wall();
// 	~Wall();
// 	Wall(Wall* rect);
	
// 	virtual void setPiece(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);

// 	virtual void applyContact(Piece* other, Vector2f point);
// };