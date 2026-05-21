CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = osfault

SRCS = main.cpp display.cpp debugger.cpp userprofile.cpp gamestate.cpp \
       case_engine.cpp leaderboard.cpp inventory.cpp hud.cpp \
       algorithms.cpp history.cpp caselab.cpp case_runner.cpp \
       case1.cpp case2.cpp case3.cpp case4.cpp \
       case5.cpp case6.cpp case7.cpp case8.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
