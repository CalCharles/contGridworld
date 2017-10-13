# scene.py
import cv2
import numpy as np
import pieces as Pieces


class Scene():
	"""
	a gridworld scene
	"""

	def __init__(self):
		self.pieces = []
		self.boundaries = [0,10,0,10]
		self.granularity = .05
		self.dt = .01
		self.agent = None
		self.map = np.zeros((int((self.boundaries[1] - self.boundaries[0])/self.granularity), int((self.boundaries[3] - self.boundaries[2])/self.granularity), 3))

	def update_scene(self):
		cx, cy = self.boundaries[0], self.boundaries[2]
		self.agent.location = self.agent.location + self.agent.velocity * self.dt
		self.agent.update_velocity(self.agent.velocity + self.agent.acceleration * self.dt)

		for piece in self.pieces:
			piece.location = piece.location + piece.velocity * self.dt
			piece.update_velocity(piece.velocity + piece.acceleration * self.dt)
			#no current support for angular velocity
		for j in range(0,len(self.pieces)):
			inter = self.agent.compute_intersection(self.pieces[j])
			if inter != None:
				self.agent.apply_contact(self.pieces[j], inter)
		for i in range(len(self.pieces)):
			for j in range(i+1,len(self.pieces)):
				inter = self.pieces[i].compute_intersection(self.pieces[j])
				if inter != None:
					self.pieces[i].apply_contact(self.pieces[j], inter)


	def render_scene(self):
		self.map = np.zeros((int((self.boundaries[1] - self.boundaries[0])/self.granularity), int((self.boundaries[3] - self.boundaries[2])/self.granularity), 3))
		cx, cy = self.boundaries[0], self.boundaries[2]
		print "rendering"
		x = self.boundaries[0]
		y = self.boundaries[1]
		for i in range(int((self.boundaries[1] - self.boundaries[0]) / self.granularity)):
			x += self.granularity
			y = 0
			for j in range(int((self.boundaries[3] - self.boundaries[2]) / self.granularity)):
				y += self.granularity
				pt = np.array([x,y])
				if self.agent.test_aabb(pt):
					if self.agent.compute_occupancy(pt):
						self.map[i,j] = self.map[i,j] + self.agent.color
				for piece in self.pieces:
					if piece.test_aabb(pt):
						if piece.compute_occupancy(pt):
							self.map[i,j] = self.map[i,j] + piece.color


	def run_demonstration(self):
		"""
		Runs a demonstration on the current scene. Takes in keyboard commands for movement, and 
		"""
		while True: 
			if self.agent != None:
				print self.agent.velocity, self.agent.grabbing, self.agent.reward
				self.render_scene()
				self.update_scene()
				a = cv2.waitKey(30)
				cv2.imshow("sphere scene",self.map)
				if a == ord("d"):
					self.agent.velocity[0] += 1
				elif a == ord("a"):
					self.agent.velocity[0] -= 1
				elif a == ord("w"):
					self.agent.velocity[1] += 1
				elif a == ord("s"):
					print "input"
					self.agent.velocity[1] -= 1
				elif a == ord(" "):
					self.agent.toggleGrab()
				elif a == 27:
					cv2.destroyAllWindows()
					return

	def add_agent(self, mass, radii, velocity, location, color):
		self.agent = Pieces.Agent(mass, radii, velocity, location, color)

	def add_circle(self, mass, radii, velocity, location, color):
		self.pieces.append(Pieces.Circle(mass, radii, velocity, location, color))

	def add_circle_hazard(self, radii, location, color, reward):
		self.pieces.append(Pieces.Hazard(radii, location, color, Pieces.Circle, reward))

	def add_rectangle_hazard(self, radii, location, color, reward):
		self.pieces.append(Pieces.Hazard(radii, location, color, Pieces.Rectangle, reward))