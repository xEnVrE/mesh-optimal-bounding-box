#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <Eigen/Dense>

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef K::Point_3                                             Point;
typedef CGAL::Surface_mesh<Point>                              Surface_mesh;


class OptimalBoundingBox
{
public:
    OptimalBoundingBox(const std::string& mesh_path, const bool use_convex_hull_);

    void evaluate();

    Eigen::Vector3d get_size();

    Eigen::Transform<double, 3, Eigen::Affine> get_frame_transform();

private:
    Eigen::Vector3d to_eigen(const Point& point);

    Surface_mesh mesh_;

    Eigen::Transform<double, 3, Eigen::Affine> frame_transform_;

    Eigen::Vector3d sizes_;

    bool use_convex_hull_;
};
