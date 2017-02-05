
// neural network - layer

namespace Network {
    
    class Layer
	{
	private:
		/// variables
		// vector of all nodes in this layer
		std::vector <Node> nodes;
		//
		unsigned int layer_type;
		unsigned int layer_num;
		//
		bool runtime_error;

        // call to report an error
        void Layer::error_call(std::string in);

    public:
        // setup layer (main)
        void Layer::setup(
            unsigned int&,
            unsigned int&,
            double&,
            double&,
            unsigned int&,
            unsigned int&
            );

        // cycle layer (main)
        // input: all inputs for nodes in layer
        // output: all outputs from all nodes in layer
        // format note: < <node1>, <node2>, ..., <nodeN> >
        std::vector <std::vector <double> > Layer::cycle(std::vector <std::vector <double> >&);

        // mutate (main)
        // call all nodes mutate function
        void Layer::mutate();

        // return node count(info)
        unsigned int Layer::return_node_count();

        // export weights (side)
        std::vector <std::vector <double> > Layer::export_weights();

        // import weights (side)
        void Layer::import_weights(std::vector <std::vector <double> >&);
    };
}