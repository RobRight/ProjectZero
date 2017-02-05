
namespace Network {

    void Node::error_call(std::string in) {
        std::cout << "ERROR:Node: " << in << std::endl;
        runtime_error = true;
        exit(0);
    }

    void Node::generate_weights(unsigned int& in_size) {
        weights.clear();
        // not output node - randomly generate weights
        for (std::size_t i = 0; i<in_size; ++i) {
            // generate weights within weight_gen_min:weight_gen_max
            if(LYRAND > 0.5) weights.push_back(LYRAND);
            else weights.push_back(-LYRAND);
        }
    }

    double Node::sum_inputs(std::vector <double>& in) {
        double t_avg = 0.0;
        for (std::size_t i=0; i<in.size(); ++i) {
            t_avg += in.at(i);
        }
        return t_avg;
        //return t_avg / (double) in.size();
    }

    double Node::activation_function(double& in) {
        //return 1/(1+exp(-in));  // sigmoid
        return tanh(in);  // hyperbolic tangent
    }

    std::vector <double> Node::generate_outputs(double& in) {
        std::vector <double> t_out;
        switch (layer_type) {
            case 0:
            case 1:
                // output = out * weight(i)
                for (std::size_t i=0; i<weights.size(); ++i) {
                    t_out.push_back(in*weights.at(i));
                }
                break;
            default: // output layer - no weights
                t_out.push_back(in);
        }
        return t_out;
    }

    void Node::setup(unsigned int& in_w_count,
            bool& in_bias,
            double& in_mod,
            double& in_chance,
            unsigned int& in_type,
            unsigned int& in_num ) {
#ifdef NN_DEBUG
        std::cout << "DEBUG:Node: setup start" << std::endl;
#endif
        // settings
        weight_gen_min = (-1);
        weight_gen_max = ( 1);
        // settings end
        // set input options
        runtime_error = false;
        bias = in_bias;
        mutate_mod = in_mod;
        mutate_chance = in_chance;
        layer_type = in_type;
        layer_num = in_num;
        // generate weights except for output layer
        if (layer_type != 2) generate_weights(in_w_count);
#ifdef NN_DEBUG
        std::string tm = "creating node type: " + std::to_string(in_type) +
            "; bias: " + std::to_string(bias) + "; weights: " + std::to_string(in_w_count);
        std::cout << "DEBUG:Node: " << tm << std::endl;
#endif
    }

    std::vector <double> Node::cycle(std::vector <double>& in) {
#ifdef NN_DEBUG
        std::cout << "DEBUG:Node: cycle start" << std::endl;
#endif
        // test inputs to node (info not available)
        // cycle
        double t_out;  // node output scaler
        if (!bias) {
            switch (layer_type) {
                case 0:
                    t_out = in.at(0); // input layer
                    break;
                default:
                    double t_summed = sum_inputs(in);
                    t_out = activation_function(t_summed);	// not input layer
            }
        }
        else t_out = 1.0;  // bias node (output 1.0)
        std::vector <double> t_final_out = generate_outputs(t_out);
        return t_final_out;
    }

    void Node::mutate() {
        for (std::size_t i=0; i<weights.size(); ++i) {
            if (LYRAND < mutate_chance) {
                // weight = weight + LYRAND*mod - LYRAND*mod
                weights.at(i) = weights.at(i) + LYRAND*mutate_mod \
                                - LYRAND*mutate_mod;
            }
        }
    }

    std::vector <double> Node::export_weights() {
        return weights;
    }

    void Node::import_weights(std::vector <double>& in) {
        weights = in;
    }
}