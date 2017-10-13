# test.py
import numpy as np
import scene as Scene

scene = Scene.Scene()
scene.add_agent(mass=1, radii=[.5,.5], velocity=np.array([10.0,10.0]), location=np.array([0.0,0.0]), color=np.array([0,1,0]))
scene.add_circle(mass=.5, radii=[.3,.3], velocity=np.array([0.0,0.0]), location=np.array([5.0,5.0]), color=np.array([1,0,0]))
scene.add_circle(mass=1.5, radii=[1.5,1.5], velocity=np.array([0.0,0.0]), location=np.array([8.0,8.0]), color=np.array([1,0,0]))
scene.add_circle_hazard(radii=[1,1],location=np.array([3.0,3.0]), color=np.array([0,0,1]), reward = -10)
scene.run_demonstration()