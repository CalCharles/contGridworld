# continuous gridworld python implementation (upgrade to c++ if it runs too slow)

import numpy as np
import scipy.misc as misc

class Piece:
    """
    A piece is a superclass for all kinds of objects. In particular, the agent (a circle), the 
    """

    tpe = "Piece"

    def __init__(self):
        self.mass = 1
        self.acceleration = np.array((0.0,0.0))
        self.velocity = np.array((0.0,0.0))
        self.location = np.array((0.0,0.0))
        self.orientation = 0.0
        self.moment = self.calculate_moment()
        self.rot_acc = 0.0
        self.rot_vel = 0.0
        self.color = np.array([0.0, 0.0,0.0])
        self.attach = None
        self.radii = np.array([0.0, 0.0,0.0])
        self.name = piece

    def calculate_moment(self):
        raise AssertionError("Do no instantiate piece class")

    def compute_occupancy(self, point):
        raise AssertionError("Do no instantiate piece class")

    def test_aabb(self, b):
        if abs(self.location[0] - b[0]) > (self.radii[0]):
            return False
        if abs(self.location[1] - b[1]) > (self.radii[1]):
            return False
        return True

    def compare_aabb(self, b):
        if abs(self.location[0] - b.location[0]) > (self.radii[0] + b.radii[0]):
            return False
        if abs(self.location[1] - b.location[1]) > (self.radii[1] + b.radii[1]):
            return False
        return True

    def compute_intersection(self, other):
        raise AssertionError("Do no instantiate piece class")

    def apply_contact(self, other, point):
        raise AssertionError("Do no instantiate piece class")

    def update_velocity(self, newVelocity):
        raise AssertionError("Do no instantiate piece class")

class Rectangle(Piece):
    """
    A piece is a superclass for all kinds of objects. In particular, the agent (a circle), the 
    """

    tpe = "Rectangle"

    def __init__(self, mass, radii, velocity, location, color, name="Rect"):
        self.name = name
        self.mass = mass
        self.acceleration = np.array((0.0,0.0))
        self.velocity = velocity
        self.location = location
        self.orientation = 0.0
        self.radii = radii
        self.moment = self.calculate_moment()
        self.rot_acc = 0.0
        self.rot_vel = 0.0
        self.color = color
        self.attach = None
        tmp1 = np.array([radii[0],radii[1]])
        tmp2 = np.array([radii[0],-radii[1]])
        self.corners = [self.location - tmp1 , self.location+tmp2, self.location+tmp1, self.location - tmp2]
        self.edges = [[self.corners[0], self.corners[1]], [self.corners[0], self.corners[1]], [self.corners[0], self.corners[1]], [self.corners[0], self.corners[1]]]

    def calculate_moment(self):
        return pow(self.radii[0] * 2, 3)/12 + pow(self.radii[1] * 2, 3)/12

    def compute_occupancy(self, point):
        return self.test_aabb(point)

    def compute_intersection(self, other):
        if other.tpe == "Circle" or other.tpe == "Agent" or ((other.tpe == "Hazard" or other.tpe == "Terminal") and other.type == "Rectangle"):
            if self.compare_aabb(other):
                if self.test_aabb(other.location):
                    return self.corners[0] # return a random corner if the sphere is contained inside
                for edge in self.edges:
                    print "contact"
                    csum = edge[0] + edge[1] + other.location
                    coeffs = []
                    coeffs.append(np.linalg.norm(csum)**2)
                    coeffs.append(-2*edge[0][0]*csum[0] - 2*edge[0][1]*csum[1])
                    coeffs.append(edge[0][0] ** 2 + edge[0][1] ** 2 - other.r2)
                    roots = np.roots(coeffs)
                    if len(roots) == 0:
                        return None
                    elif len(roots) == 1:
                        return roots[0]
                    else:
                        return (roots[0] + roots[1]) /2
        if other.tpe == "Rectangle" or (other.tpe == "Hazard" or other.tpe == "Terminal" and other.type == "Rectangle"):
            # this isn't implemented, so just ignore rectangle intersections
            return None
            raise AssertionError("Not yet implemented")
            

    def apply_contact(self, other, point):
        raise AssertionError("Rectangle contacts (momentum) not yet implemented")

    def update_velocity(self, newVelocity):
        raise AssertionError("Momentum for squares not yet implemented")



class Circle(Piece): 
    """
    A simple circle object
    """

    tpe = "Circle"

    def __init__(self, mass, radii, velocity, location, color, name = "circ"):
        self.name = name
        self.mass = mass
        self.acceleration = np.array((0.0,0.0))
        self.velocity = velocity
        self.location = location
        self.orientation = 0.0
        self.radii = radii
        self.r2 = radii[0] ** 2
        self.moment = self.calculate_moment()
        self.rot_acc = 0.0
        self.rot_vel = 0.0
        self.color = color
        self.attach = None

    def calculate_moment(self):
        return self.mass * (self.radii[0] ** 2)/2.0

    def compute_occupancy(self, point):
        # print point
        return np.linalg.norm(point - self.location) < self.radii[0]

    def test_aabb(self, b):
        if abs(self.location[0] - b[0]) > (self.radii[0]):
            return False
        if abs(self.location[1] - b[1]) > (self.radii[1]):
            return False
        return True

    def compare_aabb(self, b):
        if abs(self.location[0] - b.location[0]) > (self.radii[0] + b.radii[0]):
            return False
        if abs(self.location[1] - b.location[1]) > (self.radii[1] + b.radii[1]):
            return False
        return True



    def compute_intersection(self, other):
        Pint = None
        if other.tpe == "Circle" or other.tpe == "Agent" or ((other.tpe == "Hazard" or other.tpe == "Terminal") and other.type == "Circle"):
            if self.compare_aabb(other):
                d = np.linalg.norm(self.location - other.location)
                if d <= self.radii[0] + other.radii[0] + .05:
                    print "contact " + other.tpe
                    a = (self.r2 + other.r2 + d**2)/(2*d)
                    Pint = self.location + a* (other.location + self.location)
        elif other.tpe == "Rectangle" or (other.tpe == "Hazard" or other.tpe == "Terminal" and other.type == "Rectangle"):
            if self.compare_aabb(other):
                Pint = other.compute_intersection(self)
        # elif.tpe(other) is Polygon:
        #     pass
        return Pint
        
    def apply_contact(self, other, point):
        if other.tpe == "Circle" or other.tpe == "Agent":
            print self.location, other.location
            weighting = 2/(self.mass + other.mass)*np.dot(self.velocity-other.velocity, self.location-other.location)/(np.square(np.linalg.norm(self.location-other.location)))
            self.velocity = self.velocity - other.mass *weighting* (self.location-other.location)
            other.velocity = other.velocity - self.mass *weighting* (other.location-self.location)
        elif other.tpe == "Rectangle":
            raise AssertionError("Rectangle not yet implemented")
        elif other.tpe == "Hazard":
            if self.attach is not None and self.attach.tpe == "Agent":
                self.attach.reward += other.reward
        elif other.tpe == "Terminal":
            if other.satisfy_condition(self):
                return
            # no collision dynamics yet implemented for hazards


    def update_velocity(self, newVelocity):
        self.velocity = newVelocity
        if self.attach is not None:
            self.attach.velocity = newVelocity

class Agent(Circle):

    tpe = "Agent"

    def __init__(self, mass, radii, velocity, location, color):
        Circle.__init__(self, mass, radii, velocity, location, color)
        self.grabbing= False
        self.attach = None
        self.reward = 0
        self.name = "Agent"

    def toggleGrab(self):    
        if self.attach is not None:
            launchvec = (self.attach.location- self.location)
            self.attach.location = self.attach.location + 0.2 * launchvec
            self.attach.velocity = self.attach.velocity + launchvec / np.linalg.norm(launchvec) * (100 * self.mass/(self.attach.mass + self.mass))
            self.velocity = self.velocity - launchvec / np.linalg.norm(launchvec) * (100 * self.attach.mass/(self.attach.mass + self.mass))
            self.attach.attach = None
            self.attach = None
        else:
            self.grabbing = not self.grabbing

    def rotate(self, dt):
        """
        rotates a fixed number of radians based on the total mass and the size of the time step, such that for unit agent mass and no other mass,
        the rotation is pi/4 radians per second (does nothing if nothing is attached, however)
        """
        if self.attach is not None:
            # angle = 0.25*np.pi*dt/(0.5*self.mass*np.pi + np.linalg.norm(self.location-self.attach.location)**2 *other.mass) # what the equation should be
            angle = 10*dt/(np.sum(np.abs(self.location-self.attach.location)) * (self.attach.mass +self.mass))
            rotation = np.array([[np.cos(angle), -np.sin(angle)], [np.sin(angle), np.cos(angle)]])
            self.attach.location = np.dot(rotation, self.attach.location-self.location) + self.location

    def apply_contact(self, other, point):
        # if grabbing perform a perfectly inelastic collision
        # otherwise perform a perfectly elastic collision (like with normal circles)
        if other.tpe == "Circle":
            if self.grabbing == True:
                self.velocity = (self.mass *self.velocity + other.mass * other.velocity)/ (self.mass + other.mass)
                other.velocity = self.velocity
                self.attach = other
                other.attach = self
                self.grabbing = False
            else:
                if self.attach is None:
                    # only use the object mass
                    weighting = 2/(self.mass + other.mass)*np.dot(self.velocity-other.velocity, self.location-other.location)/(np.square(np.linalg.norm(self.location-other.location)))
                    self.velocity = self.velocity - other.mass *weighting* (self.location-other.location)
                    other.velocity = other.velocity - self.mass *weighting* (other.location-self.location)
                else: 
                    # use the total mass, but ignore rotational effects
                    totalMass = self.mass + self.attach.mass
                    weighting = 2/(totalMass + other.mass)*np.dot(self.velocity-other.velocity, self.location-other.location)/(np.square(np.linalg.norm(self.location-other.location)))
                    self.velocity = self.velocity - other.mass *weighting* (self.location-other.location)
                    other.velocity = other.velocity - totalMass *weighting* (other.location-self.location)
        elif other.tpe == "Rectangle":
            raise AssertionError("Rectangle not yet implemented")
        elif other.tpe == "Hazard":
            self.reward += other.reward
        elif other.tpe == "Terminal":
            if other.satisfy_condition(self):
                return

class Hazard(Piece):

    tpe = "Hazard"
    def __init__(self, radii, location, color, shape, reward):
        self.shape = shape(0, radii, np.array((0.0,0.0)), location, color)
        self.attach = None
        self.color = color
        self.location = location
        self.velocity = self.shape.velocity
        self.acceleration = self.shape.acceleration
        self.type = shape.tpe
        if shape.tpe == "Circle":
            self.r2 = self.shape.r2 
        self.radii = radii
        self.reward = reward

    def calculate_moment(self):
        self.shape.calculate_moment()

    def compute_occupancy(self, point):
        return self.shape.compute_occupancy(point)

    def test_aabb(self, b):
        return self.shape.test_aabb(b)

    def compare_aabb(self, b):
        return self.shape.compare_aabb(b)

    def compute_intersection(self, other):
        return self.shape.compute_intersection(other)

    def apply_contact(self, other, point):
        other.apply_contact(self, point)

    def update_velocity(self, newVelocity):
        return
        raise AssertionError("Momentum not yet applied to Hazards")

class Terminal(Piece):

    tpe = "Terminal"
    def __init__(self, radii, location, color, shape, reward, obj, attach):
        self.shape = shape(0, radii, np.array((0.0,0.0)), location, color)
        self.attach = None
        self.color = color
        self.location = location
        self.velocity = self.shape.velocity
        self.acceleration = self.shape.acceleration
        self.type = shape.tpe
        if shape.tpe == "Circle":
            self.r2 = self.shape.r2 
        self.radii = radii
        self.reward = reward
        self.set_condition(obj, attach)
        self.terminate = False

    def calculate_moment(self):
        self.shape.calculate_moment()

    def compute_occupancy(self, point):
        return self.shape.compute_occupancy(point)

    def test_aabb(self, b):
        return self.shape.test_aabb(b)

    def compare_aabb(self, b):
        return self.shape.compare_aabb(b)

    def compute_intersection(self, other):
        return self.shape.compute_intersection(other)

    def apply_contact(self, other, point):
        print "terminal contact"
        other.apply_contact(self, point)

    def update_velocity(self, newVelocity):
        return
        raise AssertionError("Momentum not yet applied to Terminal states")

    def set_condition(self, obj, attach):
        self.obj = obj
        self.attach = attach

    def satisfy_condition(self, obj):
        print "names", obj.name, self.obj.name
        if obj.name == self.obj.name:
            if not self.attach:
                self.terminate = True
                print "terminate"
                return True
            if self.attach:
                if obj.attach is not None and obj.attach.tpe == "Agent":
                    self.terminate = True
                    return True
                return False
        else:
            return False


