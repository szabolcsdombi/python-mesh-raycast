import numpy as np

import mesh_raycast

triangles = np.array([
    [0.0, 0.0, 0.0],
    [1.0, 0.0, 0.0],
    [0.0, 1.0, 0.0],
], dtype='f4')

print(mesh_raycast.raycast((0.0, 0.0, 2.0), mesh_raycast.normalize((0.1, 0.2, -1.0)), triangles))
