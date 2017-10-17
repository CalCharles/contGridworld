# scene.py
import cv2
import numpy as np
import pieces as Pieces


class Scene():
	"""
	a gridworld scene
	"""

	def __init__(self, penalty, limit):
		self.pieces = []
		self.boundaries = [0,10,0,10]
		self.granularity = .05
		self.dt = .01
		self.penalty = penalty
		self.limit = limit
		self.agent = None
		self.map = np.zeros((int((self.boundaries[1] - self.boundaries[0])/self.granularity), int((self.boundaries[3] - self.boundaries[2])/self.granularity), 3))
		self.terminals = []
		self.terminate = False

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
			if inter is not None:
				self.agent.apply_contact(self.pieces[j], inter)
		for i in range(0,len(self.terminals)):
			inter = self.agent.compute_intersection(self.terminals[i])
			if inter is not None:
				self.agent.apply_contact(self.terminals[i], inter)
				if self.terminals[i].terminate:
					self.terminate = True
					self.agent.reward += self.terminals[i].reward
					return
		for i in range(len(self.terminals)):
			for j in range(0,len(self.pieces)):
				inter = self.terminals[i].compute_intersection(self.pieces[j])
				if inter is not None:
					self.terminals[i].apply_contact(self.pieces[j], inter)
					if self.terminals[i].terminate:
						self.terminate = True
						self.agent.reward += self.terminals[i].reward
						return
		for i in range(len(self.pieces)):
			for j in range(i+1,len(self.pieces)):
				inter = self.pieces[i].compute_intersection(self.pieces[j])
				if inter is not None:
					self.pieces[i].apply_contact(self.pieces[j], inter)

		self.agent.reward += self.penalty * self.dt


	def render_scene(self):
		self.map = np.zeros((int((self.boundaries[1] - self.boundaries[0])/self.granularity), int((self.boundaries[3] - self.boundaries[2])/self.granularity), 3))
		cx, cy = self.boundaries[0], self.boundaries[2]
		print "rendering"
		x = self.boundaries[0]
		y = self.boundaries[1]
		for j in range(int((self.boundaries[1] - self.boundaries[0]) / self.granularity)):
			x += self.granularity
			y = 0
			for i in range(int((self.boundaries[3] - self.boundaries[2]) / self.granularity)):
				y += self.granularity
				pt = np.array([x,y])
				if self.agent.test_aabb(pt):
					if self.agent.compute_occupancy(pt):
						self.map[i,j] = self.map[i,j] + self.agent.color
				for piece in self.pieces:
					if piece.test_aabb(pt):
						if piece.compute_occupancy(pt):
							self.map[i,j] = self.map[i,j] + piece.color
				for piece in self.terminals:
					if piece.test_aabb(pt):
						if piece.compute_occupancy(pt):
							self.map[i,j] = self.map[i,j] + piece.color

		self.map = np.flipud(self.map)


	def run_demonstration(self):
		"""
		Runs a demonstration on the current scene. Takes in keyboard commands for movement, and applies corresponding action
		"""
		T = 0
		while True: 
			if self.agent != None:
				print self.agent.velocity, self.agent.grabbing, self.agent.reward, T, self.terminate
				self.render_scene()
				T += self.dt
				self.update_scene()
				if T == self.limit:
					cv2.destroyAllWindows()
					return
				a = cv2.waitKey(30)
				cv2.imshow("sphere scene",self.map)
				if a == ord("d"):
					self.agent.velocity[0] += 1 # should add actions to the agent rather than hard coding
				elif a == ord("a"):
					self.agent.velocity[0] -= 1
				elif a == ord("w"):
					self.agent.velocity[1] += 1
				elif a == ord("s"):
					print "input"
					self.agent.velocity[1] -= 1
				elif a == ord("q"):
					print "input"
					self.agent.rotate(self.dt)
				elif a == ord(" "):
					self.agent.toggleGrab()
				elif a == 27:
					cv2.destroyAllWindows()
					return
				if self.terminate:
					cv2.destroyAllWindows()
					return

	def run_rollout(self, policy):
		"""
		Runs a demonstration on the current scene. Takes in keyboard commands for movement, and 
		"""
		T = 0
		while True: 
			if self.agent != None:
				print self.agent.velocity, self.agent.grabbing, self.agent.reward, T
				# self.render_scene()
				T += self.dt
				self.update_scene()
				if T == self.limit:
					cv2.destroyAllWindows()
					return
				a = cv2.waitKey(30)
				cv2.imshow("sphere scene",self.map)
				if a == ord("d"):
					self.agent.velocity[0] += 1 # should add actions to the agent rather than hard coding
				elif a == ord("a"):
					self.agent.velocity[0] -= 1
				elif a == ord("w"):
					self.agent.velocity[1] += 1
				elif a == ord("s"):
					print "input"
					self.agent.velocity[1] -= 1
				elif a == ord("q"):
					print "input"
					self.agent.velocity[1] -= 1
				elif a == ord(" "):
					self.agent.toggleGrab()
				elif a == 27:
					cv2.destroyAllWindows()
					return
				if self.terminate:
					cv2.destroyAllWindows()
					return

	def add_agent(self, mass, radii, velocity, location, color):
		self.agent = Pieces.Agent(mass, radii, velocity, location, color)

	def add_circle(self, mass, radii, velocity, location, color, name= ""):
		self.pieces.append(Pieces.Circle(mass, radii, velocity, location, color, name=name))

	def add_circle_hazard(self, radii, location, color, reward):
		self.pieces.append(Pieces.Hazard(radii, location, color, Pieces.Circle, reward))

	def add_rectangle_hazard(self, radii, location, color, reward):
		self.pieces.append(Pieces.Hazard(radii, location, color, Pieces.Rectangle, reward))

	def add_circle_terminal_agent(self, radii, location, color, reward, attach):
		self.terminals.append(Pieces.Terminal(radii, location, color, Pieces.Circle, reward, self.agent, attach))

	def add_rectangle_terminal_agent(self, radii, location, color, reward, attach):
		self.terminals.append(Pieces.Terminal(radii, location, color, Pieces.Rectangle, reward, self.agent, attach))

	def add_circle_terminal_object(self, radii, location, color, reward, attach, name):
		for piece in self.pieces:
			if piece.name == name:
				chosen = piece
		if chosen is None:
			raise AssertionError("Not a name of a current piece")
		self.terminals.append(Pieces.Terminal(radii, location, color, Pieces.Circle, reward, chosen, attach))

	def add_rectangle_terminal_object(self, radii, location, color, reward, attach, name):
		for piece in self.pieces:
			if piece.tpe != "Hazard" and piece.name == name:
				chosen = piece
		if chosen is None:
			raise AssertionError("Not a name of a current piece")
		self.terminals.append(Pieces.Terminal(radii, location, color, Pieces.Rectangle, reward, chosen, attach))