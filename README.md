# python-mesh-raycast

Ray-casting on meshes for python.

## Install

This module is **not available through pip**, it was designed to be forked and extended. Simplicity is a key goal in design. This module is using [glm](https://glm.g-truc.net/) and the Python's [c-api](https://docs.python.org/3/c-api/). The implementation can be found in the [mesh_raycast.cpp](mesh_raycast.cpp).

```
git clone https://github.com/cprogrammer1994/python-mesh-raycast
cd python-mesh-raycast
python setup.py develop
```

## Cheat Sheet

```py
import mesh_raycast

triangles = np.array([
    [0.0, 0.0, 0.0],
    [4.0, 0.0, 0.0],
    [0.0, 4.0, 0.0],
], dtype='f4')

result = mesh_raycast.raycast(source=(0.4, 0.8, 5.0), direction=(0.0, 0.0, -1.0), mesh=triangles)
```

The `result` is a list of objects with the following keys:

```py
{
    'face': 0,
    'point': (0.4, 0.8, 0.0),
    'normal': (0.0, 0.0, 1.0),
    'coeff': (0.1, 0.2),
    'distance': 5.0,
    'dot': 1.0,
}
```

- `face` is the index of the triangle from mesh
- `point` is the point in world coordinates where the ray and the triangle intersects
- `normal` is the normal of the triangle
- `coeff` is a pair of coefficients from the internal calculations
- `distance` is the distance between point and source
- `dot` is the dot product of -direction and normal

### sorting the result

```py
sorted(result, key=lambda x: x['distance'])
```

### filtering the result

```py
first_matching_face = min(result, key=lambda x: x['distance'])['face']
```

```py
non_backfacing = filter(lambda x: x['dot'] > 0.0, result)
```
