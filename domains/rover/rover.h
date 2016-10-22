
/*

Rover Domain
Copyright (c) 2016 William Gregory.  All rights reserved.

Notes:
- dir (1:N, 2:E, 3:S, 4:W)
- loc (x,y)
- pos_y (N+,S-); pos_x (E+,W-)

*/

#ifndef _ROVER_DOMAIN_
#define _ROVER_DOMAIN_

#include <vector>
#include <stdlib>
#include <time.h>

#define LYRAND (double)rand()/RAND_MAX


namespace RD {

    // location, grid based
    struct loc {
        //double r_mag;  // distance from origin
        //double theta;  // angle
        int pos_x;
        int pos_y;
    };

    // position of intrest
    //
    // Functions:
    // - return_position
    // - set_position
    //
    class POI {
    private:
        loc position;
        double value;
    public:
        POI(loc in_loc,
            double in_val) {
            // setup
            position = loc;
            value = in_val;
            movable = false;
        }
        // return position
        loc return_position() {
            return position;
        }
        // set position
        bool set_position(loc in) {
            if (movable) position = in;
            else return false;
            return true;
        }
    }


    // rover object
    // 
    // ToDo:
    // - add sensors (state)
    //
    // Functions:
    // - return_position
    // - set_position
    // - generate_pos_given_move
    // - check_state *
    // - return_state *
    // 
    class Rover {
    private:
        /// variables
        loc position;  // rover position
        //unsigned int type;
        double state; // MORE ON THIS
        /// functions
        // check state (sensors)
        void check_state() {
            state = 1;
        }
    public:
        Rover(loc in_loc) {
            position = in_loc;
        }
        // return position
        loc return_position() {
            return position;
        }
        // set position
        bool set_position(loc in) {
            position = in;
            return true;
        }
        // return position given move from current position
        loc generate_pos_given_move(unsigned int dir) {
            loc t_pos = position;
            switch(dir) {
                case 1:  // N
                    t_pos.pos_y += 1;
                    break;
                case 2:  // E
                    t_pos.pos_x += 1;
                    break;
                case 3:  // S
                    t_pos.pos_y -= 1;
                    break;
                case 4:  // W
                    t_pos.pos_x -= 1;
                    break;
                default:
                    // ERROR
            }
            return t_pos;
        }
        // return state (sensors)
        double return_state() {
            check_state();
            return state;
        }
    };


    // rover domain
    //
    // Functions:
    // - generate_random_position
    // - add_random_pois
    // - add_random_rovers
    // - check_pos_clear
    // - add_rover
    // - move_rover
    // - add POI
    // ----------------
    // - Rover
    // - setup
    // - return states
    // - set moves
    //
    class RoverDomain {
    private:
        /// variables
        std::vector <POI> pois;  // all POIs
        std::vector <Rover> rovers;  // all rovers
        // world ranges
        int pos_x_max;
        int pos_x_min;
        int pos_y_max;
        int pos_y_min;
        /// functions
        // generate random position (sub)
        loc generate_random_position() {
            loc l;
            while (true) {
                l.pos_x = rand() / (pos_x_max-pos_x_min) + pos_x_min;
                l.pos_y = rand() / (pos_y_max-pos_x_min) + pos_y_min;
                if (check_pos_clear(l)) break;
            }
            return l;
        }
        // generate random POSs (sub)
        void add_random_pois(unsigned int in_count) {
            for (std::size_t i=0; i<in_count; ++i) {
                double poi_value = 1.0;
                loc l = generate_random_position();
                POI p(l, poi_value);
                pois.push_back(p);
            }
        }
        void add_random_rovers(unsigned int in_count) {
            for (std::size_t i=0; i<in_count; ++i) {
                loc l = generate_random_position();
                Rover r(l);
                rovers.push_back(r);
            }
        }
        // return true if position is clear of rovers and POIs (sub)
        bool check_pos_clear(loc in) {
            // rovers
            for (std::size_t i=0; i<rovers; ++i) {
                if (in.pos_x == rovers.at(i).pos_x) {
                    if (in.pos_y == rovers.at(i).pos_y) {
                        return false;
                    }
                }
            }
            // POIs
            for (std::size_t i=0; i<POIS; ++i) {
                if (in.pos_x == POIS.at(i).pos_x) {
                    if (in.pos_y == POIS.at(i).pos_y) {
                        return false;
                    }
                }
            }
            return true;
        }
        // add rover to world if space is clear (sub)
        bool add_rover(double in_x, in_y) {
            loc l;
            l.pos_x = in_x;
            l.pos_y = in_y;
            if (!check_pos_clear(l)) return false;
            Rover r(l);
            rovers.push_back(r);
            return true;
        }
        // check if move is legal (sub)
        bool move_rover(Rover in_r, unsigned int in_dir) {
            loc t_pos = in_r.generate_pos_given_move(in_dir);
            if (!check_pos_clear(t_pos)) return false;
            in_r.set_position(t_pos);
            return true;
        }
        // move all rovers given all locations (sub)
        void move_single_rover_master(Rover in_r, unsigned int in_d) {
            loc t_pos = in_r.generate_pos_given_move(in_d);
            in_r.set_position(t_pos);
        }
        // move all rovers given all locations (sub)
        void move_all_rovers_master(std::vector <unsigned int> in) {
            for (std::size_t i=0; i<rovers.size(); ++i) {
                loc t_pos = in_r.generate_pos_given_move(in.at(i));
                rovers.at(i).set_position(t_pos);
            }
        }
        // --------------------------------------------------
        public:
        RoverDomain(int in_x_max,
                    int in_x_min,
                    int in_y_max,
                    int in_y_min) {
            // setup
            pos_x_max = in_x_max;
            pos_x_min = in_x_min;
            pos_y_max = in_y_max;
            pos_y_min = in_y_min;
            srand(time(null));
        }
        // setup (main)
        // add randomly placed POIS and rovers to the world
        void setup(unsigned int in_pois, unsigned int in_rovers) {
            add_random_pois(in_pois);
            add_random_rovers(in_rovers);
        }
        // return states (main)
        std::vector <double> return_states() {
            std::vector <double> t_states;
            for (std::size_t i=0; i<rovers.size(); ++i) {
                t_states.push_back(rovers.at(i).return_state());
            }
            return t_states;
        }
        // set moves (main)
        bool set_moves(std::vector <unsigned int> in) {
            if (in.size() != rovers.size()) return false;
            std::vector <loc> t_locs;
            for (std::size_t i=0; i<rovers.size(); ++i) {
                if(!move_rover(rovers.at(i), in.at(i))) return false;
            }
            move_all_rovers_master(in);
            return true;
        }
    };

}

#endif
