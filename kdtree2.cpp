#include <Eigen/Core>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using Point = Eigen::Vector2d;

struct KDNode {
    Point                   point;
    int                     axis;
    std::unique_ptr<KDNode> left  = nullptr;
    std::unique_ptr<KDNode> right = nullptr;
};

class KDTree {
public:
    explicit KDTree(std::vector<Point>& points) { root_ = build(points, 0, static_cast<int>(points.size())); }

    // 前序遍历打印节点信息
    void print() { traverse_print(root_.get()); }

private:
    std::unique_ptr<KDNode> root_ = nullptr;

    // points 按值传参，不改变原有点序列的顺序
    std::unique_ptr<KDNode> build(std::vector<Point> points, int left, int right, int depth = 0) {
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
    void traverse_print(KDNode* node, int depth = 0) {
        if (node == nullptr) {
            return;
        }
        std::cout << "axis" << node->axis << "    (" << node->point.x() << ", " << node->point.y() << ")"
                  << "    depth" << depth << std::endl;

        traverse_print(node->left.get(), depth + 1);
        traverse_print(node->right.get(), depth + 1);
    }
};

int main(int, char**) {
    std::vector<Point> points{
        Point(2, 3),
        Point(5, 4),
        Point(9, 6),
        Point(4, 7),
        Point(8, 1),
        Point(7, 2),
        Point(3, 6),
    };

    KDTree tree(points);
    tree.print();
}