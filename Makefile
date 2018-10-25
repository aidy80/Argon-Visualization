CXX		 = clang++
CXXFLAGS = -Wall -Wextra -Wreturn-type -std=c++11 -g -O2
LDFLAGS  = -g -framework OpenGL -framework GLUT -lglui

display: working.cpp VertexList.cpp tinyxmlerror.cpp tinyxmlparser.cpp tinyxml.cpp tinystr.cpp Camera.cpp SceneParser.cpp phaseReader.cpp 
	${CXX} ${CXXFLAGS} ${LDFLAGS} working.cpp VertexList.cpp tinyxmlerror.cpp tinyxmlparser.cpp tinyxml.cpp tinystr.cpp Camera.cpp SceneParser.cpp phaseReader.cpp -o display 

nveSim: Ft_NVE.f90
	gfortran -o nveSim Ft_NVE.f90

clean: 
	rm *.o
