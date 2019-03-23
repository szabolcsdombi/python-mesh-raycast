import numpy as np
import pytest

import mesh_raycast


def test_non_bidirectional():
    triangles = np.array([
        [0.0, 0.0, 0.0],
        [4.0, 0.0, 0.0],
        [0.0, 4.0, 0.0],

        [0.0, 0.0, 2.0],
        [2.0, 0.0, 2.0],
        [0.0, 2.0, 2.0],
    ], dtype='f4')

    source = (1.0, 1.0, 1.0)
    direction = mesh_raycast.direction(source, (0.5, 0.5, -0.5))
    result = mesh_raycast.raycast(source=source, direction=direction, mesh=triangles)

    assert len(result) == 1
    assert result[0]['face'] == 0


if __name__ == '__main__':
    pytest.main([__file__])
