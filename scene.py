# scene.py


def collision_velocities(piece1, piece2, intersection):
	total_mass = piece1.mass + piece2.mass
	piece1.apply_contact( (piece2.velocity-piece1.velocity) *  (piece1.mass/total_mass), intersection)
	piece2.apply_contact( (piece2.velocity-piece1.velocity) *  (piece2.mass/total_mass), intersection)


class Scene():
	"""
	a gridworld scene
	"""

	def __init__(self):
		self.pieces = []
		self.boundaries = [0,10,0,10]
		self.granularity = .01
		self.dt = .01
		self.agent = None


	def render_scene(self):
		cx, cy = self.boundaries[0], self.boundaries[2]
		for x in np.linspace(self.boundaries[0], self.boundaries[1], self.granularity):
			for y in np.linspace(self.boundaries[2], self.boundaries[3], self.granularity):
				for piece in pieces:
					piece.location += piece.velocity * self.dt
					piece.velocity += piece.acceleration * self.dt
					#no current support for angular velocity
				for i in range(len(pieces)):
					for j in range(i+1,len(pieces)):
						inter = pieces[i].compute_intersection(pieces[j])
						if inter != None:
							if pieces[i] is Agent: 
								if pieces[i]
