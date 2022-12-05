import ctypes

# 该 DLL/.so 必须要和 m2d_interface.py 在同一个目录下。
lib = ctypes.CDLL(".\\libmesh2depth.dll")

"""
用法：
process_folder("./obj")
目录必须存在。
"""
def process_folder(folder):
    fd_bytes = bytes(folder, "utf-8")
    return lib.process_folder(fd_bytes)

"""
mode 可以是 jpeg 或 csv。
用法：
process_model("jpeg", v, indices)
"""
def process_face(mode, vertices, indices):
    num_verts = len(vertices)
    num_indices = len(indices)

    if mode != "jpeg" and mode != "csv":
        raise ValueError("Unknown process mode.")

    mode_rep = 0 if mode == "jpeg" else 1
    
    return lib.process_face(mode_rep,
        (ctypes.c_float * num_verts)(*vertices),
        num_verts,
        (ctypes.c_uint * num_indices)(*indices),
        num_indices)

"""
models 应该是一个数组，每一个数组里面都有相应的一个 tuple
mode 可以是 jpeg 或者 csv。
[([顶点], [索引]), ([顶点], [索引]), ([顶点], [索引]), ...]
用法：
顶点集 0 = [
    60.0, 65.0, 0.0,
    1.0, 66.0, 3.0,
    80.0, 100.0, 6.0,
    12, 34.0, 12.0,
    1.0, 50.0, 77.0
]
索引集 0 = [0, 1, 2, 1, 2, 3]
顶点集 1 = v0
索引集 1 = [0, 1, 3, 2, 3, 4]
process_faces("jpeg", [(顶点集 0, 索引集 0), (顶点集 1, 索引集 1)])
"""
def process_faces(mode, models):
    if mode != "jpeg" and mode != "csv":
        raise ValueError("Unknown process mode.")

    lib.processor_init()

    for i in range(0, len(models)):
        name = "model " + str((i + 1))
        vertices, indices = models[i]
        num_verts = len(vertices)
        num_indices = len(indices)
        lib.processor_add_model(bytes(name, "utf-8"),
            (ctypes.c_float * num_verts)(*vertices),
            num_verts,
            (ctypes.c_uint * num_indices)(*indices),
            num_indices)

    mode_rep = 0 if mode == "jpeg" else 1
    lib.processor_process(mode_rep)

    lib.processor_free()
