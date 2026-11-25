#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using Vec3 = Eigen::Vector3d;
using Mat3 = Eigen::Matrix3d;
using Quat = Eigen::Quaterniond;

// IMU 测量数据
struct ImuData {
    double timestamp = 0.0;
    Vec3   gyro      = Vec3::Zero();
    Vec3   acc       = Vec3::Zero();
};

// 带相对时间戳的雷达点
struct PointXYZT {
    double offset_time = 0.0;
    Vec3   xyz         = Vec3::Zero();
};

// IMU 状态
struct ImuState {
    double timestamp = 0.0;
    Quat   q         = Quat::Identity();
    Vec3   p         = Vec3::Zero();
    Vec3   v         = Vec3::Zero();
    Vec3   bg        = Vec3::Zero();
    Vec3   ba        = Vec3::Zero();
    Vec3   g{0.0, 0.0, -9.81};
};

// LiDAR 到 IMU 的外参
struct Extrinsic {
    Quat q_IL = Quat::Identity();
    Vec3 t_IL = Vec3::Zero();
};

using PointCloud    = std::vector<PointXYZT>;
using ImuTrajectory = std::vector<ImuState>;

// 一帧雷达及对应的 IMU 数据
struct MeasureGroup {
    double lidar_beg_time = 0.0;
    double lidar_end_time = 0.0;

    PointCloud           cloud;
    std::vector<ImuData> imu;
};

Quat exp_so3(const Vec3& phi) {
    const double theta = phi.norm();
    if (theta < 1e-8) {
        return Quat(1.0, 0.5 * phi.x(), 0.5 * phi.y(), 0.5 * phi.z()).normalized();
    }

    return Quat(Eigen::AngleAxisd(theta, phi / theta));
}

int main(int, char**) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "点云运动去畸变\n";
}