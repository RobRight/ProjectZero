
# Makefile
# Ststem with Agent and Domain named "run"
# Optional test file named "test"

# compile option
#COP	=	system
#COP	=	test
COP	=	system test

# system files
SMAIN	=	main.cpp
SAGENT	=	network_trainer.hpp neural_network.hpp
SDOMAIN	=	pendulum.hpp
SSYSTEM	=	utilities.hpp system.hpp
SOUT	=	-o run
# test files
TMAIN	=	_testing/test_code.cpp
TOUT	=	-o _testing/run_test

# flags
CXX 	=	g++
CVER	=	-std=c++11

# compile
all: $(COP)

system: 
	$(CXX) $(CVER) $(SAGENT) $(SDOMAIN) $(SSYSTEM) $(SMAIN) $(SOUT)

#test:
#	$(CXX) $(CVER) $(SAGENT) $(SDOMAIN) $(SSYSTEM) $(TMAIN) $(TOUT)
