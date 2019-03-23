from setuptools import Extension, setup

ext = Extension(
    name='mesh_raycast',
    sources=['mesh_raycast.cpp'],
)

setup(
    name='mesh_raycast',
    version='0.1.0',
    ext_modules=[ext],
)
