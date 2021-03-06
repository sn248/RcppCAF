#include <dist_node.hpp>

using namespace caf;
dist_node::dist_node(Eigen::MatrixXd locs, actor pr) : 
                                            locations(locs),
                                            parent(pr)
{
    alive.assign(
        [=](euclidean_atom, unsigned int row1, unsigned int row2)
        {
            double result = calculateDistance(row1, row2);
            unsigned int idx = row1 + row2*(locations.rows() - 1) - 
                               (row2*(row2 + 1))/2 - 1;
            send(parent, idx, result);
        },
        [=](exit_atom)
        {
            quit();
        }
    );
}
dist_node::~dist_node()
{
    // default destructor
}

behavior dist_node::make_behavior(){
    send(this, wakeup_atom::value);
    return([=](wakeup_atom){become(alive);});
}

double dist_node::calculateDistance(size_t row1, size_t row2)
{
    return((locations.row(row1) - locations.row(row2)).norm()); 
}
