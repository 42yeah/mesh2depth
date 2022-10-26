## mesh2depth

```sh
mkdir build
cmake ..
cmake --build .
```

GLFW is required.

### Usage

```sh
./mesh2depth <.obj dir> [export mode]
```

Export mode can be:

- 1: CSV. Export the depthmap as high-precision float CSV.
- 2: JPEG. Export the depthmap as, you know, JPEG.

Camera will always center at `(64, 64, bounding box max z + 1)` and stare at negative *z* direction, with a near plane of 0.1 and a far plane of 300. Please change the source code accordingly if this doesn't suit what you need.
