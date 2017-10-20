#include <vector>
#include <GLFW/glfw3.h>
#include "piece.cpp"

using namespace Eigen;

class Scene {
public:
	std::vector<Piece*> pieces;
	Vector4f view;
	float dxy;
	float dt;
	float penalty;
	int limit;
	Agent* agent;
	std::vector<Terminal*> terminals;
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
    void renderView(std::string name, int ti);
    void display ( GLFWwindow* window );
	
	void takeAction(std::string action, Vector2f force);

    void addEllipseAgent(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color);
    void addEllipse(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name);
    void addEllipseHazard(Vector2f radii, Vector2f location, Vector3f color, float reward);
    void addRectangleHazard(Vector2f radii, Vector2f location, Vector3f color, float reward);
    void addEllipseTerminal(Vector2f radii, Vector2f location, Vector3f color, float reward, bool attach, std::string name);
    void addRectangleTerminal(Vector2f radii, Vector2f location, Vector3f color, float reward, bool attach, std::string name);

};
