#include "show.cpp"

using namespace Eigen;

int main(int argc, char *argv[]) {
	Scene* scene = new Scene();
	Vector2f radii(1.0f,1.0f);
	Vector2f velocity(0.5f,0.5f);
	Vector2f location(1.0f,1.0f);
	Vector3f color(0.0f,0.0f,1.0f);
	scene->addEllipseAgent(1.0f, radii, velocity, location, color);
	scene->addEllipse(2.0f, Vector2f(.5,.5), Vector2f(0,0), Vector2f(5,5), Vector3f(0,.5,.5), "obj1");
	scene->addEllipse(3.0f, Vector2f(.5,.5), Vector2f(0,0), Vector2f(5.8,5.8), Vector3f(0,.5,.5), "obj2");
	scene->addEllipse(3.0f, Vector2f(.5,.5), Vector2f(0,0), Vector2f(4.3,5.8), Vector3f(0,.5,.5), "obj2");
	scene->addEllipse(3.0f, Vector2f(.5,.5), Vector2f(0,0), Vector2f(5.8,4.3), Vector3f(0,.5,.5), "obj2");
	scene->addEllipse(3.0f, Vector2f(.5,.5), Vector2f(0,0), Vector2f(6.8,5.8), Vector3f(0,.5,.5), "obj2");
	scene->addRectangleHazard(Vector2f(3,2), Vector2f(5,10), Vector3f(1,0,0), -100.0f);
	scene->addEllipseTerminal(Vector2f(1,1), Vector2f(10,10), Vector3f(0,0,1), 100.0f, true, "obj1");

	run_display("demonstration", scene);
}