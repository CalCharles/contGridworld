
#include "scene.h"

#define PI 3.1415926585323846
using namespace Eigen;

bool checkError(Vector2f location) {
	// checks for the special invalid location
	if (location[0] == -1232f && location[1] == -9992f) ? return true : return false;
}

Scene::Scene() {
	this.createScene(Vector4f(0,10,0,10), .05, 0.1, -10, 1000);
}
Scene::~Scene() {
// need to destroy: pieces, terminals, might be automatic
}

Scene::Scene(Scene* scene) {
	// too lazy to write this one
}

Scene::Scene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego) {
	this.createScene(view, dxy, dt, penalty, limit);
}
void Scene::createScene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego) {
	this.view[0] = view[0];
	this.view[1] = view[1];
	this.view[2] = view[2];
	this.view[3] = view[3];
	this.radii[0] = (this.view[1] - this.view[0])/2;
	this.radii[1] = (this.view[3] - this.view[2])/2;
	this.center[0] = this.view[0] + radii[0];
	this.center[1] = this.view[1] + radii[1];
	this.dxy = dxy;
	this.dt = dt;
	this.penalty = penalty;
	this.limit = limit;
	this.agent = NULL;
}

void Scene::updateScene() {
	float cx = this.view[0];
	float cy = this.view[2];
	this.agent.updateLocation(this.agent.location + this.agent.velocity * this.dt)
	this.agent.updateVelocity(this.agent.velocity + this.agent.acceleration * this.dt)

	Piece* piece;
	for(int i = 0; i < this.pieces.size(); i++) {
		piece = this.pieces[i];
		piece->updateLocation(piece->location + piece->velocity * this.dt);
		piece->update_velocity(piece->velocity + piece->acceleration * this.dt);
		//no current support for angular velocity, notice mobility of 
	}
	for(int j = 0; j < this.pieces.size(); j++) {
		inter = this.agent.compute_intersection(this.pieces[j])
		if (!checkError(inter)) ? this.agent.apply_contact(this.pieces[j], inter);
	}
	for(int i = 0; i < this.terminals.size(); i++) {
		inter = this.agent.compute_intersection(this.terminals[i])
		if (!checkError(inter)) {
			this.agent.apply_contact(this.terminals[i], inter)
			if (this.terminals[i].terminate) {
				this.terminate = true;
				this.agent.reward = this.agent.reward + this.terminals[i].reward;
				return;
			}
		}
	}
	for(int i = 0; i < this.terminals.size(); i++) {
		for(int j = 0; j < this.pieces.size(); j++) {
			inter = this.terminals[i]->compute_intersection(this.pieces[j])
			if (!checkError(inter)) {
				this.terminals[i].apply_contact(this.pieces[j], inter);
				if (this.terminals[i].terminate) {
					this.terminate = true;
					this.agent.reward += this.terminals[i].reward;
					return;
				}
			}
		}
	}
	for(int i = 0; i < this.pieces.size(); i++) {
		for(int j = 0; j < this.pieces.size(); j++) {
			inter = this.pieces[i]->compute_intersection(this.pieces[j]);
			if (!checkError(inter)) ? this.pieces[i]->apply_contact(this.pieces[j], inter);
		}
	}

	this.agent.reward = this.agent.reward + this.penalty * this.dt

}

void Scene::updateView() {
	this.view[0] = this.agent.location - this.radii[0];
	this.view[1] = this.agent.location + this.radii[0];
	this.view[2] = this.agent.location - this.radii[1];
	this.view[3] = this.agent.location + this.radii[1];
	this.center[0] = this.view[0] + radii[0];
	this.center[1] = this.view[1] + radii[1];
}

void Scene::renderView(string name, int ti) {
	// will implement rendering of images later, should produce images for all states in a trajectory, as well as state information list	
}

void Scene::drawRectangle(Piece* circle) {
	
}

void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(r, g, b);
    glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
}

float pixtox(int i, float dxy, Vector4f view) {
	return ((float)i)/(this.radii[0] * 2/this.dxy) + view[0];
}

float pixtoy(int j, float dxy, Vector4f view) {
	return ((float)j)/(this.radii[1] * 2/this.dxy) + view[1];
}

void Scene::display ( GLFWwindow* window ) {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    glPushMatrix();
    GLfloat translation[3] = {this.center[0], this.center[1], 0.1f};
    glTranslatef (translation[0], translation[1], translation[2]);

    // drawCircle(100, Height_global / 2.0 , min(Width_global, Height_global) * 0.4 / 2.0, false); 
	Piece* piece;
	Terminal* terminal;
	glBegin(GL_POINTS);
	for (int i = 0; i < int(this.radii[0] * 2/this.dxy); i++) {
        for (int j = 0; j < int(this.radii[1] * 2/this.dxy); j++) {
        	float x = pixtox(i); 
        	float y = pixtoy(j);
			Vector2f pt;
			pt[0] = x;
			pt[1] = y;
			Vector3f color(0f,0f,0f);
			for(int i = 0; i < this.terminals.size(); i++) {
				terminal = this.pieces[i];
				if (terminal->test_aabb(pt)){
					if terminal.compute_occupancy(pt) ? color = color + terminal->color;
				}
			}
			for(int i = 0; i < this.pieces.size(); i++) {
				piece = this.pieces[i];
				if(piece->test_aabb(pt)) {
					if piece.compute_occupancy(pt) ? color = color + piece->color;
				}
			}
			if self.agent.test_aabb(pt) {
				if self.agent.compute_occupancy(pt) {
					color = color + self.agent.color;
				}
			}
			setPixel(i,j,color[0], color[1], color[2]);
        }
    }
	glEnd();

    glPopMatrix();
    
    glfwSwapBuffers(window);
}

void Scene::addCircleAgent(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector2f color) {
	self.agent = Agent();
	self.agent.setPiece(mass, radii, velocity, location, color, "Ellipse");
}
void Scene::addCircle(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector2f color, string name) {
	Piece piece = new Piece();
	piece.setPiece(mass, radii, velocity, location, color, name);
	self.pieces.push_back(&piece);
}
void Scene::addCircleHazard(Vector2f radii, Vector2f location, Vector2f color, float reward) {
	Hazard hazard = new Hazard();
	hazard.setPiece(0.0f, radii, Vector2f(0,0), location, color, "Ellipse");
	self.pieces.push_back(&hazard);
}
void Scene::addRectangleHazard(Vector2f radii, Vector2f location, Vector2f color, float reward) {
	Hazard hazard = new Hazard();
	hazard.setPiece(0.0f, radii, Vector2f(0,0), location, color, "Rectangle");
	self.pieces.push_back(&hazard);
}

void Scene::addCircleTerminal(Vector2f radii, Vector2f location, Vector2f color, float reward, bool attach, string name) {
	Piece* chosen = NULL;
	for(int i = 0; i < this.pieces.size(); i++) {
		if(pieces[i]->name == name) ? chosen = pieces[i];
	}
	if(chosen == NULL) throw invalid_argument("Not a name of a current piece");
	Terminal term = new Terminal();
	term.setPiece(0.0f, radii, Vector2f(0,0), location, color, "Ellipse");
	if (attach) ? term.setCondition(chosen, &self.agent) : term.setCondition(chosen, NULL);
	self.terminals.push_back(&term);
}
void Scene::addRectangleTerminal(Vector2f radii, Vector2f location, Vector2f color, float reward, bool attach, string name) {
	Piece* chosen = NULL;
	for(int i = 0; i < this.pieces.size(); i++) {
		if(pieces[i]->name == name) ? chosen = pieces[i];
	}
	if(chosen == NULL) throw invalid_argument("Not a name of a current piece");
	Terminal term = new Terminal();
	term.setPiece(0.0f, radii, Vector2f(0,0), location, color, "Rectangle");
	if (attach) ? term.setCondition(chosen, &self.agent) : term.setCondition(chosen, NULL);
	self.terminals.push_back(&term);
}
