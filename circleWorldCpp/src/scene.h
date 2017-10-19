#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "piece.cpp"

using namespace Eigen;

class Scene {
protected:
	vector<Piece*> pieces;
	Vector4f view;
	float dxy;
	float dt;
	float penalty;
	int limit;
	Agent agent;
	vector<Terminal*> terminals;
	bool terminate;
	bool ego;
	Vector2f radii;
	Vector2f center;


	Scene();
	~Scene(); // need to destroy: pieces
	Scene(Scene* scene);
	Scene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego);
	void createScene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego);

	void updateView();
	void updateScene();
    void renderView(string name, int ti);
    void display ( GLFWwindow* window );
	
	void applyAction(string action);

    void addCircleAgent(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector2f color);
    void addCircle(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector2f color, string name);
    void addCircleHazard(Vector2f radii, Vector2f location, Vector2f color, float reward);
    void addRectangleHazard(Vector2f radii, Vector2f location, Vector2f color, float reward);
    void addCircleTerminal(Vector2f radii, Vector2f location, Vector2f color, float reward, bool attach, string name);
    void addRectangleTerminal(Vector2f radii, Vector2f location, Vector2f color, float reward, bool attach, string name);

};
