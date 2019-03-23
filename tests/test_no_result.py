import numpy as np
import pytest

import mesh_raycast


def test_no_result():
    triangles = np.array([
        [0.0, 0.0, 0.0],
        [4.0, 0.0, 0.0],
        [0.0, 4.0, 0.0],
    ], dtype='f4')

    result = mesh_raycast.raycast(source=(4.0, 4.0, 4.0), direction=(1.0, 1.0, -1.0), mesh=triangles)

    assert len(result) == 0


if __name__ == '__main__':
    pytest.main([__file__])
