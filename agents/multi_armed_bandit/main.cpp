
// Run Multi Armed Bandit

#include "mab.h"

int main() {

    // (max_bias(unused), slot_count)
    MAB::Casino mab(0, 10);
    mab.cycle_games();

    return 0;
}