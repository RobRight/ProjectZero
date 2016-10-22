/*

Rover Class

Copyright C 2016 William.  All rights reserved.

Description:
- rover in a world
- add, move, ...

Todo:
- change to polar quardinates

*/

#ifndef _ROVER_CLASS_
#define _ROVER_CLASS_

#include <vector>

namespace Rover {

	struct rover {
		// position
		unsigned int x;		// position X (-:+)
		unsigned int y;		// position Y (-:+)
		unsigned int dir;	// direction (0:359)
		// properties
		unsigned int speed;	// movement speed (1:10)
		unsigned int type;	// rover type (1:basic, 2:pro)??
	};

	struct world {
		unsigned int x;
		unsigned int y;
		// .at(row).at(col);
		// .at(y).at(x);
		std::vector < std::vector <unsigned int> > w;
	};

	class Rover_world
	{
	private:
		world w;
	public:
		Rover_world();
		unsigned int type_null;
		unsigned int type_rover;
		std::vector <rover> rovers;
		void create_world(unsigned int, unsigned int);
	};

	Rover_world::Rover_world()
	{
		type_null = 0;
		type_rover = 1;
	}

	void Rover_world::create_world(unsigned int in_x, unsigned int in_y)
	{
		w.x = in_x;
		w.y = in_y;
		for (std::size_t j = 0; j < w.y; ++j)
		{
			std::vector <unsigned int> t_row;
			for (std::size_t j = 0; j < w.y; ++j)
			{
				t_row.push_back(type_null);
			}
			w.w.push_back(t_row);
		}
	}

	//////////////////////////////////
	class Rover

	{
	public:
		Rover_world rw;
		Rover();
		bool check_position_clear(unsigned int, unsigned int);
		rover generate_rover(const unsigned int&, const unsigned int&, const unsigned int&);
		bool add_rover(unsigned int, unsigned int, unsigned int);
		bool move_rover(unsigned int, unsigned int);
	};

	Rover::Rover()
	{

	}

	// check if given position contains a rover
	bool Rover::check_position_clear(unsigned int in_x, unsigned int in_y)
	{
		bool t_clear = true;
		for (std::size_t i = 0; i < rw.rovers.size(); ++i)
		{
			if (rw.rovers.at(i).x == in_x)
				if (rw.rovers.at(i).y == in_y)
					t_clear = false;
		}
		return t_clear;
	}

	rover Rover::generate_rover(const unsigned int& in_x, const unsigned int& in_y, const unsigned int& in_dir)
	{
		rover r;
		r.x = in_x;
		r.y = in_y;
		r.dir = in_dir;
		r.speed = 1;
		r.type = 1;
		return r;
	}

	bool Rover::add_rover(unsigned int in_x, unsigned int in_y, unsigned int in_dir)
	{
		bool t_placed = false;
		if (check_position_clear(in_x, in_y))
		{
			rw.rovers.push_back(generate_rover(in_x, in_y, in_dir));
		}
		return t_placed;
	}

	bool Rover::move_rover(unsigned int in_x, unsigned int in_y)
	{
		bool t_moved = true;

		return t_moved;
	}

}

#endif