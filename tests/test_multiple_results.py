import numpy as np
import pytest

import mesh_raycast


def test_multiple_results():
    triangles = np.array([
        [0.0, 0.0, 0.0],
        [4.0, 0.0, 0.0],
        [0.0, 4.0, 0.0],

        [0.0, 0.0, 2.0],
        [2.0, 0.0, 2.0],
        [0.0, 2.0, 2.0],
    ], dtype='f4')

    source = (1.0, 1.0, 5.0)
    direction = mesh_raycast.direction(source, (0.5, 0.5, -0.5))
    result = mesh_raycast.raycast(source=source, direction=direction, mesh=triangles)

    assert len(result) == 2
    assert {result[0]['face'], result[1]['face']} == {0, 1}


if __name__ == '__main__':
    pytest.main([__file__])
