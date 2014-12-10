EXEC := buscaminas3D

DIRSRC := ./src/
DIROBJ := ./obj/
DIRHEA := ./include/
CXXFLAGS := -I $(DIRHEA) -Wall `pkg-config --cflags CEGUI` -std=c++11 `pkg-config --cflags OGRE` `pkg-config --cflags OIS`
LDFLAGS := `pkg-config --libs-only-L OGRE`
LDLIBS := `pkg-config --libs CEGUI` `pkg-config --libs CEGUI-OGRE` `pkg-config --libs-only-l OGRE` -lOIS -lGL -lstdc++ -lboost_system
CXX := g++
OBJS := $(subst $(DIRSRC), $(DIROBJ), $(patsubst %.cpp, %.o,$(wildcard $(DIRSRC)*.cpp)))

ifeq ($(mode), release) 
	CXXFLAGS := $(CXXFLAGS) -O2
else
	CXXFLAGS := $(CXXFLAGS) -ggdb
endif





all:	$(EXEC)

$(EXEC):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(DIROBJ)%.o:	$(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	$(RM) $(DIROBJ)* ./$(EXEC)
