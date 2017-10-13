# continuous gridworld python implementation (upgrade to c++ if it runs too slow)

import numpy as np
import scipy.misc as misc

class Piece:
	"""
	A piece is a superclass for all kinds of objects. In particular, the agent (a circle), the 
	"""

	def __init__(self):
		self.mass = 1
		self.acceleration = np.array((0.0,0.0))
		self.velocity = np.array((0.0,0.0))
		self.location = np.array((0.0,0.0))
		self.orientation = 0.0
		self.moment = calculate_moment()
		self.rot_acc = 0.0
		self.rot_vel = 0.0

	def calculate_moment(self):
		raise AssertionError("Do no instantiate piece class")

	def compute_occupancy(self, point):
		raise AssertionError("Do no instantiate piece class")

	def compute_intersection(self, other):
		raise AssertionError("Do no instantiate piece class")

	def apply_contact(self, force, point):
		raise AssertionError("Do no instantiate piece class")

class Circle(piece): 
	"""
	A simple circle object
	"""

	def __init__(self, mass, radius, velocity, location):
		self.mass = mass
		self.acceleration = np.array((0.0,0.0))
		self.velocity = velocity
		self.location = location
		self.orientation = 0.0
		self.radius = radius
		self.r2 = radius ** 2
		self.moment = calculate_moment()
		self.rot_acc = 0.0
		self.rot_vel = 0.0

	def calculate_moment(self):
		return self.mass * (self.radius ** 2)/2.0

	def compute_occupancy(self, point):
		return np.linalg.norm(point - center) < self.radius

	def compute_intersection(self, other):
		Pint = None
		if type(other) is Circle:
			d = np.linalg.norm(self.center - other.center)
			if d <= self.radius + other.radius:
				a = (self.r2 + other.r2 + d**2)/(2*d)
				Pint = self.center + a* (other.center + self.center)
		elif type(other) is Polygon:
			pass
		return Pint
		
	def apply_contact(self, velocity, point):
		true_direction = self.center - point
		self.velocity = self.v

class Agent(Circle):
	def __init__(self, mass, radius, velocity, location):
		Circle.__init__(self, mass, radius, velocity, location)
		self.grab= False
		self.attach = None

	def grab():
		self.grab = True

	def release():
		self.grab = False
