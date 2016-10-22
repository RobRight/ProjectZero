
/*

Multi-Armed Bandit
Copyright (c) 2016 William Gregory.  All rights reserved.

Player in a casino playing slots, tasked with finding the best paying slot.

*/

#ifndef _MULTI_ARMED_BANDIT_
#define _MULTI_ARMED_BANDIT_

#include <vector>
#include <stdlib>
#include <time.h>

#define LYRAND (double)rand()/RAND_MAX

namespace MAB {

    //
    // Functions:
    // - move_greedy
    // - move_random
    //
    // - Player
    // - choose_slot
    // - add_reward
    //
    class Player {
    private:
        unsigned int slot_count;
        std::vector <double> current_standings;  // one per slot, learned reward
        unsigned int round;
        unsigned int move_greedy() {
            unsigned int t_move = 0;
            double t_best = current_standings.at(0);
            for (std::size_t i=1; i<slot_count; ++i) {
                if (current_standings.at(i) > t_best) {
                    t_move = i;
                    t_best = current_standings.at(i);
                }
            }
            return t_move;
        }
        unsigned int move_random() {
            unsigned int t_move = rand() % slot_count;
            return t_move;
        }
    public:
        Player(unsigned int in_c) {
            slot_count = in_c;
            round = 0;
            for (std::size_t i=0; i<slot_count; ++i) {
                current_standings.push_back(1.0);
            }
        }
        unsigned int choose_slot() {
            unsigned int t_slot;
            if (round < 10) move_random();
            else {
                if (LYRAND > 0.6) move_random();
                else move_greedy();
            }
            return t_slot;
        }
        void add_reward(unsigned int in_s, unsigned int in_r) {
            current_standings.at(in_s) = (current_standings.at(in_s) + in_r) / 2;
        }
    };

    //
    // Functions:
    // - Slot
    // - generate_payout
    //
    class Slot {
    // currently a constant reward.  add a negitive reward and random reward with a bias.
    private:
        double reward;
    public:
        Slot(unsigned int in_max) {
            reward = rand() % in_max;
        }
        double generate_payout() {
            return reward; //* LYRAND;
        }
    };

    //
    // Functions:
    // - setup_player
    // - setup_slots
    // - pull_slot
    // 
    // - Casino
    // - cycle_games
    //
    class Casino {
    private:
        Player player;
        std::vector <double> slots;
        unsigned int reward_max;
        unsigned int slot_count;
        unsigned int round;
        unsigned int max_round;
        void setup_player() {
            Player p(slot_count);
            Player player = p;
        }
        void setup_slots() {
            for (std::size_t i=0; i<slot_count; ++i) {
                Slot s(reward_max);
                slots.push_back(s);
            }
        }
        double pull_slot(unsigned int in) {
            if (in > (slots.size()-1)) return 0.0;  // unknown slot
            double payout = slots.at(in).generate_payout();
        }
    public:
        Casino(unsigned int in_r, unsigned int in_c) {
            reward_max = in_r;
            slot_count = in_c;
            round = 0;
            max_round = 100;
            srand(time(NULL));
            setup_player();
            setup_slots();
        }
        void cycle_games() {
            while (round < max_round) {
                unsigned int t_slot = player.choose_slot();
                double t_reward = pull_slot(t_slot);
                player.add_reward(t_slot, t_reward);
            }
        }
    };

}

#endif