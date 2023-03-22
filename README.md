# mesh-optimal-bounding-box
A tool which simplifies the visual description of a link in URDF syntax using optimal bounding boxes.

The meaning of **optimal bounding box** is a bounding box that tightly fits the original mesh of the link - 
as opposed to an **axis-aligned bounding box** that might be sub-optimal.

The following figure shows an example where the two links of the index finger have been replaced with a simplified representation:

![image](https://user-images.githubusercontent.com/6014499/226982292-2656c70d-424c-4a0a-8582-cbb8e574143f.png)


### Requirements
- [CGAL](https://www.cgal.org/)
- [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [tclap](https://tclap.sourceforge.net/)
- [libconfig](https://github.com/hyperrealm/libconfig)

### How to build

```console
git clone https://github.com/xEnVrE/mesh-optimal-bounding-box
cd mesh-optimal-bounding-box
mkdir build
cd build
cmake [-DCMAKE_INSTALL_PREFIX=<path>] ../
make [install]
```

### How to run

```console
mesh-optimal-bounding-box --from <path_to_config_file>.cfg
```

An example output is

```console
Sizes of the box are:
<size_x> <size_y> <size_z>
The origin of the box is:
xyz =  <x> <y> <z>
rpy =  <r> <p> <y>
```

that can be implemented within an URDF as

```
<visual>
<origin xyz="<x> <y> <z>" rpy="<r> <p> <y>"/>
<geometry>
  <box size="<size_x> <size_y> <size_z>"/>
</geometry>
</visual>
```

### Structure of the configuration file

Examples of the configuration file can be found [here](https://github.com/xEnVrE/mesh-optimal-bounding-box/tree/master/config).

The user is request to provide:
- the path to the mesh of the link
- the origin of the visual tag of the link in the original URDF
- the scale used for the mesh of the link in the original URDF
