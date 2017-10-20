
#include "scene.h"

#define PI 3.1415926585323846
using namespace Eigen;

bool checkError(Vector2f location) {
	// checks for the special invalid location
	return (location[0] == magicError1 && location[1] == magicError2);
}

Scene::Scene() {
	this->createScene(Vector4f(0,20,0,20), .05, 0.1, -1, 1000, false);
}
Scene::~Scene() {
// need to destroy: pieces, terminals-> might be automatic
}

Scene::Scene(Scene* scene) {
	// too lazy to write this one
}

Scene::Scene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego) {
	this->createScene(view, dxy, dt, penalty, limit, ego);
}
void Scene::createScene(Vector4f view, float dxy, float dt, float penalty, float limit, bool ego) {
	this->view[0] = view[0];
	this->view[1] = view[1];
	this->view[2] = view[2];
	this->view[3] = view[3];
	this->radii[0] = (this->view[1] - this->view[0])/2;
	this->radii[1] = (this->view[3] - this->view[2])/2;
	this->center[0] = this->view[0] + radii[0];
	this->center[1] = this->view[1] + radii[1];
	this->dxy = dxy;
	this->dt = dt;
	this->penalty = penalty;
	this->limit = limit;
	this->agent = NULL;
}

void Scene::updateScene() {
	float cx = this->view[0];
	float cy = this->view[2];
	this->agent->updateLocation(this->agent->location + this->agent->velocity * this->dt);
	this->agent->updateVelocity(this->agent->velocity + this->agent->acceleration * this->dt);
	// std::cout << this->agent->velocity << std::endl;
	Piece* piece;
	for(int i = 0; i < this->pieces.size(); i++) {
		piece = this->pieces[i];
		piece->updateLocation(piece->location + piece->velocity * this->dt);
		if (piece->type != "Hazard")
			piece->updateVelocity(piece->velocity + piece->acceleration * this->dt);
		//no current support for angular velocity, notice mobility of 
	}
	Vector2f inter;
	for(int j = 0; j < this->pieces.size(); j++) {
		inter = this->agent->computeIntersection(this->pieces[j]);
		if (!checkError(inter)) this->agent->applyContact(this->pieces[j], inter);
	}
	for(int i = 0; i < this->terminals.size(); i++) {
		inter = this->agent->computeIntersection(this->terminals[i]);
		if (!checkError(inter)) {
			this->agent->applyContact(this->terminals[i], inter);
			if (this->terminals[i]->terminate) {
				this->terminate = true;
				this->agent->reward = this->agent->reward + this->terminals[i]->reward;
				return;
			}
		}
	}
	for(int i = 0; i < this->terminals.size(); i++) {
		for(int j = 0; j < this->pieces.size(); j++) {
			inter = this->terminals[i]->computeIntersection(this->pieces[j]);
			if (!checkError(inter)) {
				this->terminals[i]->applyContact(this->pieces[j], inter);
				if (this->terminals[i]->terminate) {
					this->terminate = true;
					this->agent->reward += this->terminals[i]->reward;
					return;
				}
			}
		}
	}
	for(int i = 0; i < this->pieces.size(); i++) {
		for(int j = i; j < this->pieces.size(); j++) {
			inter = this->pieces[i]->computeIntersection(this->pieces[j]);
			if (!checkError(inter)) this->pieces[i]->applyContact(this->pieces[j], inter);
		}
	}

	this->agent->reward = this->agent->reward + this->penalty * this->dt;

}

void Scene::updateView() {
	this->view[0] = this->agent->location[0] - this->radii[0];
	this->view[1] = this->agent->location[0] + this->radii[0];
	this->view[2] = this->agent->location[1] - this->radii[1];
	this->view[3] = this->agent->location[1] + this->radii[1];
	this->center[0] = this->view[0] + radii[0];
	this->center[1] = this->view[1] + radii[1];
}

void Scene::renderView(std::string name, int ti) {
	// TODO: will implement rendering of images later, should produce images for all states in a trajectory, as well as state information list	
}

void Scene::takeAction(std::string action, Vector2f force) {
	Vector2f horizontal(0.4f, 0.0f);
	Vector2f vertical(0.0f, 0.4f);
	if(action == "left") {
		this->agent->updateVelocity(this->agent->velocity - horizontal);
	} else if (action == "right") {
		this->agent->updateVelocity(this->agent->velocity + horizontal);
	} else if (action == "up") {
		this->agent->updateVelocity(this->agent->velocity + vertical);
	} else if (action == "down") {
		this->agent->updateVelocity(this->agent->velocity - vertical);
	} else if (action == "space") {
		std::cout << "grab\n"; 
		this->agent->toggleGrab();		
	} else if (action == "rotate") {
		this->agent->rotate(this->dt);
	}
}

void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(r, g, b);
    glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
}

float pixtox(int i, float dxy, Vector4f view, float rx) {
	return ((float)i)*(dxy) + view[0];
}

float pixtoy(int j, float dxy, Vector4f view, float ry) {
	return view[2] + ((float)j)*(dxy);// + view[2];
}

void Scene::display ( GLFWwindow* window ) {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    glPushMatrix();
    GLfloat translation[3] = {this->center[0], this->center[1], 0.1f};
    glTranslatef (translation[0], translation[1], translation[2]);

    // drawCircle(100, Height_global / 2.0 , min(Width_global, Height_global) * 0.4 / 2.0, false); 
	Piece* piece;
	Terminal* terminal;
	glBegin(GL_POINTS);
	for (int i = 0; i < int(this->radii[0] * 2.0f/(this->dxy)); i++) {
        for (int j = 0; j < int(this->radii[1] * 2.0f/(this->dxy)); j++) {
        	float x = pixtox(i, this->dxy, this->view, this->radii[0]); 
        	float y = pixtoy(j, this->dxy, this->view, this->radii[1]);
			Vector2f pt;
			pt[0] = x;
			pt[1] = y;
			Vector3f color(0.0f,0.0f,0.0f);
			for(int i = 0; i < this->terminals.size(); i++) {
				terminal = this->terminals[i];
				if (terminal->testAabb(pt)){
					if (terminal->computeOccupancy(pt)) color = color + terminal->color;
				}
			}
			for(int i = 0; i < this->pieces.size(); i++) {
				piece = this->pieces[i];
				if(piece->testAabb(pt)) {
					if (piece->computeOccupancy(pt)) color = color + piece->color;
				}
			}
			if (this->agent->testAabb(pt)) {
				if (this->agent->computeOccupancy(pt)) {
					color = color + this->agent->color;
				}
			}
			// if (color.norm() > 0) std::cout<<i << j << " " << color[0] << color[1] << color[2]<< std::endl;
			setPixel(i,j,color[0], color[1], color[2]);
        }
    }
	glEnd();

    glPopMatrix();
    
    glfwSwapBuffers(window);
}

void Scene::addEllipseAgent(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color) {
	this->agent = new Agent();
	this->agent->setPiece(mass, radii, velocity, location, color, "Ellipse");
}

void Scene::addEllipse(float mass, Vector2f radii, Vector2f velocity, Vector2f location, Vector3f color, std::string name) {
	Ellipse* piece = new Ellipse();
	piece->setPiece(mass, radii, velocity, location, color, name);
	this->pieces.push_back(piece);
}

void Scene::addEllipseHazard(Vector2f radii, Vector2f location, Vector3f color, float reward) {
	Hazard* hazard = new Hazard();
	hazard->setPiece(0.0f, radii, Vector2f(0,0), location, color, "Ellipse");
	hazard->setReward(reward, dt);
	this->pieces.push_back(hazard);
}

void Scene::addRectangleHazard(Vector2f radii, Vector2f location, Vector3f color, float reward) {
	Hazard* hazard = new Hazard();
	hazard->setPiece(0.0f, radii, Vector2f(0,0), location, color, "Rectangle");
	hazard->setReward(reward, dt);
	this->pieces.push_back(hazard);
}

void Scene::addEllipseTerminal(Vector2f radii, Vector2f location, Vector3f color, float reward, bool attach, std::string name) {
	Piece* chosen = NULL;
	for(int i = 0; i < this->pieces.size(); i++) {
		if(pieces[i]->name == name) chosen = pieces[i];
	}
	if(chosen == NULL) throw std::invalid_argument("Not a name of a current piece");
	Terminal* term = new Terminal();
	term->setPiece(0.0f, radii, Vector2f(0,0), location, color, "Ellipse");
	(attach) ? term->setCondition(chosen, this->agent) : term->setCondition(chosen, NULL);
	this->terminals.push_back(term);
}
void Scene::addRectangleTerminal(Vector2f radii, Vector2f location, Vector3f color, float reward, bool attach, std::string name) {
	Piece* chosen = NULL;
	for(int i = 0; i < this->pieces.size(); i++) {
		if(pieces[i]->name == name) chosen = pieces[i];
	}
	if(chosen == NULL) throw std::invalid_argument("Not a name of a current piece");
	Terminal* term = new Terminal();
	term->setPiece(0.0f, radii, Vector2f(0,0), location, color, "Rectangle");
	(attach) ? term->setCondition(chosen, this->agent) : term->setCondition(chosen, NULL);
	this->terminals.push_back(term);
}
