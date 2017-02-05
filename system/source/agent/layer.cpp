
// neural network - layer

namespace Network {

    // call to report an error
    void Layer::error_call(std::string in) {
        std::cout << "ERROR:Layer: " << in << std::endl;
        runtime_error = true;
        exit(0);
    }
    
    // setup layer (main)
    void Layer::setup( unsigned int& in_n_count,  // nodes in layer
                unsigned int& in_next_n_count,	// nodes in next layer
                double& in_mod,
                double& in_chance,
                unsigned int& in_type,	// layer type
                unsigned int& in_num ) {
        // setup
#ifdef NN_DEBUG
        std::cout << "DEBUG:Layer: setup start" << std::endl;
#endif
        runtime_error = false;
        nodes.clear();
        layer_type = in_type;
        layer_num = in_num;
        // fill node vector with in_n_count nodes
        for (std::size_t i=0; i<in_n_count; ++i) {
            // note:
            // - weight size = nodes in next layer
            // - in_n_count includes bias node
            // - in_next_n_count is 0 when output layer

            // last node bias if not an output layer
            bool t_bias = true;
            if (i == (in_n_count-1) && in_type != 2) {	// bias
                Node n;
                n.setup(in_next_n_count, t_bias, in_mod, in_chance, layer_type, layer_num);
                nodes.push_back(n);
            } else {  // normal
                t_bias = false;
                Node n;
                n.setup(in_next_n_count, t_bias, in_mod, in_chance, layer_type, layer_num);
                nodes.push_back(n);
            }
        }
    }

    // cycle layer (main)
    // input: all inputs for nodes in layer
    // output: all outputs from all nodes in layer
    // format note: < <node1>, <node2>, ..., <nodeN> >
    std::vector <std::vector <double> > Layer::cycle(std::vector <std::vector <double> >& in) {
#ifdef NN_DEBUG
        std::cout << "DEBUG:Layer: cycle start" << std::endl;
#endif
#ifdef NN_TEST
        if (in.at(0).size() != nodes.size()) {
            std::string tm = "cycle - input size not equal to nodes size.  nodes: " +
                std::to_string(nodes.size()) + "; inputs: " +
                std::to_string(in.size()) + ";";
            error_call(tm);
        }
#endif
        // cycle
        std::vector <std::vector <double> > t_in;  // formatted input vector
        // format input FIX
        for (std::size_t i=0; i<nodes.size(); ++i) {
            std::vector <double> tt;
            for (std::size_t j=0; j<in.size(); ++j) {
                tt.push_back(in.at(j).at(i));
            }
            t_in.push_back(tt);
        }
        // input and cycle
        std::vector <std::vector <double> > t_out;	// outputs from all nodes
        for (std::size_t i=0; i<t_in.size(); ++i) {
#ifdef NN_DEBUG
            std::string tm = "input node:" + std::to_string(i) + "; ";
            for (std::size_t d=0; d<t_in.at(i).size(); ++d) {
                tm = tm + std::to_string(t_in.at(i).at(d)) + "; ";
            }
            std::cout << "DEBUG:Layer: " << tm << std::endl;
#endif
            t_out.push_back(nodes.at(i).cycle(t_in.at(i)));
        }
        return t_out;
    }

    // mutate (main)
    // call all nodes mutate function
    void Layer::mutate() {
        for (std::size_t i=0; i<nodes.size(); ++i) {
            nodes.at(i).mutate();
        }
    }

    // return node count(info)
    unsigned int Layer::return_node_count() {
        return nodes.size();
    }

    // export weights (side)
    std::vector <std::vector <double> > Layer::export_weights() {
        std::vector <std::vector <double> > t;
        for (std::size_t i=0; i<nodes.size(); ++i) {
            t.push_back(nodes.at(i).export_weights());
        }
        return t;
    }

    // import weights (side)
    void Layer::import_weights(std::vector <std::vector <double> >& in) {
        if (in.size() != nodes.size()) error_call("import weights size mismatch");
        for (std::size_t i=0; i<in.size(); ++i) {
            nodes.at(i).import_weights(in.at(i));
        }
    }
}