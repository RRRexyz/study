#include <Eigen/Core>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <random>
#include "timer.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

using Point = Eigen::Vector2d;

struct KDNode {
    Point                   point;
    int                     axis;
    std::unique_ptr<KDNode> left  = nullptr;
    std::unique_ptr<KDNode> right = nullptr;
};

struct NearestResult {
    const Point* point;
    double       distance_squared;
};

class KDTree {
public:
    explicit KDTree(std::vector<Point> points) { root_ = build(points, 0, static_cast<int>(points.size())); }

    // 前序遍历打印节点信息
    void print() const { traverse_print(root_.get()); }

    NearestResult nearest_search(const Point& query) {
        node_visit_count = 0;

        const KDNode* best_node = nullptr;
        double        best_d2   = std::numeric_limits<double>::infinity();

        nearest_search_recursive(query, root_.get(), best_node, best_d2);

        const Point* point = best_node ? &best_node->point : nullptr;
        return NearestResult{point, best_d2};
    }

    std::size_t get_node_visit_count() const { return node_visit_count; }

private:
    std::unique_ptr<KDNode> root_ = nullptr;

    int node_visit_count = 0;

    // points 按值传参，不改变原有点序列的顺序
    std::unique_ptr<KDNode> build(std::vector<Point>& points, int left, int right, int depth = 0) {
        // 递归终止条件
        if (left >= right) {
            return nullptr;
        }

        int axis = depth % 2;
        int mid  = left + (right - left) / 2;
        std::nth_element(points.begin() + left,
                         points.begin() + mid,
                         points.begin() + right,
                         [axis](const Point& a, const Point& b) { return a[axis] < b[axis]; });

        auto node   = std::make_unique<KDNode>();
        node->axis  = axis;
        node->point = points[mid];
        node->left  = build(points, left, mid, depth + 1);
        // mid 已经被当前节点占用，不能再进入子节点
        node->right = build(points, mid + 1, right, depth + 1);
        return node;
    }

    // 前序遍历打印节点信息
    void traverse_print(const KDNode* node, int depth = 0) const {
        if (node == nullptr) {
            return;
        }
        std::cout << "(" << node->point.transpose() << ")" << "    axis" << node->axis << "    depth" << depth
                  << std::endl;

        traverse_print(node->left.get(), depth + 1);
        traverse_print(node->right.get(), depth + 1);
    }

    void nearest_search_recursive(const Point&   query,
                                  const KDNode*  current,
                                  const KDNode*& best_node,
                                  double&        best_d2) {
        if (current == nullptr) {
            return;
        }
        ++node_visit_count;

        double d2 = (current->point - query).squaredNorm();
        if (d2 < best_d2) {
            best_d2   = d2;
            best_node = current;
        }

        const KDNode* near_child;
        const KDNode* far_child;
        double        delta = query[current->axis] - current->point[current->axis];
        if (delta <= 0) {
            near_child = current->left.get();
            far_child  = current->right.get();
        } else {
            near_child = current->right.get();
            far_child  = current->left.get();
        }

        nearest_search_recursive(query, near_child, best_node, best_d2);

        if (delta * delta <= best_d2) {
            nearest_search_recursive(query, far_child, best_node, best_d2);
        }
    }
};

NearestResult brute_force_nearest_search(const std::vector<Point>& points, const Point& query) {
    if (points.empty()) {
        return NearestResult{nullptr, std::numeric_limits<double>::infinity()};
    }

    const Point* best_point = nullptr;
    double       best_d2    = std::numeric_limits<double>::infinity();
    for (const Point& point : points) {
        double d2 = (query - point).squaredNorm();
        if (d2 < best_d2) {
            best_d2    = d2;
            best_point = &point;
        }
    }
    return NearestResult{best_point, best_d2};
}

std::vector<Point> generate_points(std::size_t N, double min = -100.0, double max = 100.0, unsigned int seed = 24) {
    std::mt19937                           gen(seed);
    std::uniform_real_distribution<double> dist(min, max);
    std::vector<Point>                     points;
    points.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        points.emplace_back(dist(gen), dist(gen));
    }
    return points;
}

int main(int, char**) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    constexpr std::size_t N      = 10000;
    std::vector<Point>    points = generate_points(N, -100, 100, 24);
    ScopedTimer           kdtree_build("KD树构建");
    KDTree                tree(points);
    kdtree_build.stop();

    Point         query(15.278, -34.2352);
    ScopedTimer   kdtree_search("KD树单点最近邻搜索");
    NearestResult tree_result = tree.nearest_search(query);
    kdtree_search.stop();
    if (tree_result.point) {
        std::cout << "p = (" << tree_result.point->transpose() << ")    d = " << std::sqrt(tree_result.distance_squared)
                  << std::endl;
        std::cout << "节点访问率 = " << tree.get_node_visit_count() << "/" << N << " = "
                  << tree.get_node_visit_count() / static_cast<double>(N) * 100.0 << "%" << std::endl;
    }

    ScopedTimer   brute_search("暴力单点最近邻搜索");
    NearestResult brute_result = brute_force_nearest_search(points, query);
    brute_search.stop();
    if (brute_result.point) {
        std::cout << "p = (" << brute_result.point->transpose()
                  << ")    d = " << std::sqrt(brute_result.distance_squared) << std::endl;
    }
}