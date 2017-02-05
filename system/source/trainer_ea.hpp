	// prune population
	std::vector <Network::Network> prune(std::vector <Network::Network>& in_pop, std::vector <double>& in_pop_error) {
#ifdef NT_DEBUG
		std::cout << "debug: prune() start" << std::endl;
#endif
		unsigned int try_1;
		unsigned int try_2;
		std::vector <Network::Network> temp_new_pop;
		for (std::size_t i = 0; i < in_pop.size() / 2; ++i)
		{
			try_1 = rand() % in_pop.size();
			try_2 = rand() % in_pop.size();
			while (try_1 == try_2)
				try_2 = rand() % in_pop.size();
			if (in_pop_error.at(try_1) < in_pop_error.at(try_2))
				temp_new_pop.push_back(in_pop.at(try_1));
			else
				temp_new_pop.push_back(in_pop.at(try_2));
		}
		return temp_new_pop;
	}

	// fill population
	std::vector <Network::Network> populate(std::vector <Network::Network>& in_pop, unsigned int& in_pop_size) {
#ifdef NT_DEBUG
		std::cout << "debug: populate() start" << std::endl;
#endif
		int copy_index;
		Network::Network net;
		std::vector <Network::Network> temp_new_pop = in_pop;
		while (temp_new_pop.size() < in_pop_size)
		{
			copy_index = rand() % in_pop.size();
			// generate a new network
			net = generate_network();
			// set to same weight as random
			net.import_weights(in_pop.at(copy_index).export_weights());
			net.mutate();
			temp_new_pop.push_back(net);
		}
		return temp_new_pop;
	}