#include <OptimalBoundingBox.h>

#include <CGAL/optimal_bounding_box.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

namespace PMP = CGAL::Polygon_mesh_processing;
using namespace Eigen;


OptimalBoundingBox::OptimalBoundingBox(const std::string& mesh_path, const bool use_convex_hull) :
    use_convex_hull_(use_convex_hull)
{
    if(!PMP::IO::read_polygon_mesh(mesh_path, mesh_) || mesh_.is_empty())
        throw(std::runtime_error("Invalid input mesh file " + mesh_path));
}


void OptimalBoundingBox::evaluate()
{
    /**
     * Evaluate the points of the oriented bounding box.
     */
    std::array<Point, 8> bounding_box_points;
    CGAL::oriented_bounding_box(mesh_, bounding_box_points, CGAL::parameters::use_convex_hull(use_convex_hull_));

    /**
     * Taking into consideration the relative displacement of points as per:
     * https://doc.cgal.org/latest/BGL/hexahedron.png
     *
     * We assume that:
     * - the x axis is identified by P1 - P0;
     * - the y axis is identified by P3 - P0;
     * - the z axis is identified by P5 - P0;
     */

    /**
     * Evaluate sizes.
     */
    sizes_ = Vector3d
    (
        (to_eigen(bounding_box_points[1]) - to_eigen(bounding_box_points[0])).norm(),
        (to_eigen(bounding_box_points[3]) - to_eigen(bounding_box_points[0])).norm(),
        (to_eigen(bounding_box_points[5]) - to_eigen(bounding_box_points[0])).norm()
    );

    /**
     * Evaluate the reference frame attached to the bounding box.
     */
    Vector3d center = Vector3d::Zero();
    for (const auto& point : bounding_box_points)
        center += to_eigen(point);
    center /= bounding_box_points.size();

    Matrix3d rotation;
    rotation.col(0) = (to_eigen(bounding_box_points[1]) - to_eigen(bounding_box_points[0])).normalized();
    rotation.col(1) = (to_eigen(bounding_box_points[3]) - to_eigen(bounding_box_points[0])).normalized();
    rotation.col(2) = rotation.col(0).cross(rotation.col(1));

    frame_transform_ = Translation<double, 3>(center);
    frame_transform_.rotate(rotation);
}


Vector3d OptimalBoundingBox::get_size()
{
    return sizes_;
}


Transform<double, 3, Eigen::Affine> OptimalBoundingBox::get_frame_transform()
{
    return frame_transform_;
}


Vector3d OptimalBoundingBox::to_eigen(const Point& point)
{
    return Vector3d(point.x(), point.y(), point.z());
}
