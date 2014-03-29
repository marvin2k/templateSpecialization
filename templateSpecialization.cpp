#include <iostream>
#include <vector>

#include <Eigen/Geometry>

#include "Minimal.h"

int main()
{

    Minimal<int>A;
    Minimal<short>B;
    Minimal<float>E(1.4f);
    Minimal<std::vector<int> >X;
    // calling setZero for a vector...?
    X.setZero();

    Minimal<Eigen::Vector2f>C;
    Minimal<Eigen::Vector2f>D(Eigen::Vector2f::Random());
    Minimal<Eigen::Array2i>F(Eigen::Array2i(2,8));
    C = D;

    std::cerr << " E " << E << " C " << C << " A " << A.val << " B " << B.val << "\n";

    A.increase();
    E.decreace();
    A.increase();
    C.setZero();
    C.increase();
    B.increase();
    B.increase();
    B.increase();

    std::cerr << " E " << E << " C " << C << " A " << A.val << " B " << B.val << "\n";

    return 0;
}
