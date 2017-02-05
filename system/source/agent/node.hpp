


namespace Network {

    class Node
	{
	private:
		/// variables
		bool runtime_error;
		// weight for each connection
		std::vector <double> weights;
		// generated weight allowed min and max values
		double weight_gen_min;
		double weight_gen_max;
		// mutate mod and chance
		double mutate_mod;
		double mutate_chance;
		//std::vector <unsigned int> connections;  // for selectively connected
		bool bias;	// true for bias node (output 1 before weight)
		unsigned int layer_type;  // 0:input, 1:hidden, 2:output (redundant with layer_num)
		unsigned int layer_num;	 // layer 1 for input, N for output, else hidden

		/// functions

		// call to report an error
		void error_call(std::string);

		// generate weights randomly at start (sub)
		// input: number of weights to generate
		// output: populates weight vector
		void generate_weights(unsigned int&);

		// sum inputs (sub)
		// input: all inputs to node
		// output: summation of inputs
		double sum_inputs(std::vector <double>&);

		// activation function (sub)
		// input: scaler input
		// output: input after activation function
		double activation_function(double&);

		// generate outputs and apply weights (sub)
		// input: scaler network input after activation function
		// output: vector, scaler times connection weight
		std::vector <double> generate_outputs(double&);

	public:
		/// functions

		// setup (main)
		// input: node settings
		void setup(
			unsigned int&,
			bool&,
			double&,
			double&,
			unsigned int&,
			unsigned int&
			);

		// cycle (main)
		// input: vector of inputs to node
		// output: outputs, one for each connection, with weights applied
		// note: how to deal with cycle of bias node
		std::vector <double> cycle(std::vector <double>&);

		// mutate (main)
		// mutates the node using mod and chance
		void mutate();

		// export weights (side)
		std::vector <double> export_weights();

		// import weights (side)
		void import_weights(std::vector <double>& in);
        
	};
}