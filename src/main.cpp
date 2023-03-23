#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>

#include <ConfigParser.h>
#include <OptimalBoundingBox.h>

#include <Eigen/Dense>

using namespace Eigen;


int main(int argc, char** argv)
{
    const std::string exec_name = "mesh-optimal-bounding-box";

    ConfigParser conf(argc, argv);

    std::string mesh_prefix_path; conf("package-path", mesh_prefix_path);
    std::string mesh_relative_path; conf("visual::mesh::filename", mesh_relative_path);
    std::string mesh_path = mesh_prefix_path + "/" + mesh_relative_path;
    double scaling; conf("scaling", scaling);

    /**
     * Evaluate optimal bounding box.
     */
    bool use_convex_hull = true;
    OptimalBoundingBox obb(mesh_path, use_convex_hull);
    obb.evaluate();

    /**
     * Get box size.
     */
    Vector3d sizes = obb.get_size() * scaling;

    /**
     * Get box transform.
     */
    Transform<double, 3, Affine> obb_transform = obb.get_frame_transform();
    obb_transform.translation() *= scaling;

    /**
     * Evaluate the current visual origin transformation.
     */
    Transform<double, 3, Affine> visual_origin;
    {
        VectorXd origin_xyz; conf("visual::origin::xyz", origin_xyz);
        VectorXd origin_rpy; conf("visual::origin::rpy", origin_rpy);

        /**
         * Assuming the RPY convention indicated in https://github.com/robotology/blender-robotics-utils/issues/3#issuecomment-906262419 for the rotation.
         */
        AngleAxisd rot_y(origin_rpy(2), Vector3d::UnitZ());
        AngleAxisd rot_p(origin_rpy(1), Vector3d::UnitY());
        AngleAxisd rot_r(origin_rpy(0), Vector3d::UnitX());

        visual_origin = Translation<double, 3>(origin_xyz);
        visual_origin.rotate(rot_y * rot_p * rot_r);
    }

    /**
     * Evaluate the overall visual origin of the optimal bounding box.
     */
    Transform<double, 3, Affine> overall_origin = visual_origin * obb_transform;

    /**
     * Evaluate the RPY angles.
     */
    Vector3d overall_origin_rpy = overall_origin.rotation().eulerAngles(2, 1, 0).reverse();

    /**
     * Print the outcome.
     */
    std::cout << "Sizes of the box are:" << std::endl;
    std::cout << std::fixed << std::setprecision(10) << sizes.transpose() << std::endl;

    std::cout << "Sizes of the cylinder are:" << std::endl;
    std::cout << "radius = " << std::fixed << std::setprecision(10) << (sizes(0) + sizes(1)) / 2.0 / 2.0 << std::endl;
    std::cout << "length = " << std::fixed << std::setprecision(10) << sizes(2) << std::endl;

    std::cout << "The origin of the box/cylinder is:" << std::endl;
    std::cout << "xyz = " << std::fixed << std::setprecision(10) << overall_origin.translation().transpose() << std::endl;
    std::cout << "rpy = " << std::fixed << std::setprecision(10) << overall_origin_rpy.transpose() << std::endl;


    return EXIT_SUCCESS;
}
