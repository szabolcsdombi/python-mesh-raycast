import numpy as np
import pytest

import mesh_raycast


def test_single_raycast():
    triangles = np.array([
        [0.0, 0.0, 0.0],
        [4.0, 0.0, 0.0],
        [0.0, 4.0, 0.0],
    ], dtype='f4')

    result = mesh_raycast.raycast(source=(0.4, 0.8, 5.0), direction=(0.0, 0.0, -1.0), mesh=triangles)

    expected = {
        'face': 0,
        'point': (0.4, 0.8, 0.0),
        'normal': (0.0, 0.0, 1.0),
        'coeff': (0.1, 0.2),
        'distance': 5.0,
        'dot': 1.0,
    }

    assert len(result) == 1
    assert result[0]['face'] == expected['face']

    for key in ('point', 'normal', 'coeff', 'distance', 'dot'):
        np.testing.assert_almost_equal(result[0][key], expected[key])


if __name__ == '__main__':
    pytest.main([__file__])
