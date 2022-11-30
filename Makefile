LD=g++
CXX=g++ -Wall -std=c++11 -g -c

EXEC_SRC=\
	testGrayLevelImage2D.cpp\
	bruit-impulsionnel.cpp\
	double-brightness.cpp\
	filtrage-median.cpp\
	histogram.cpp\
	egalise.cpp\
	netlify.cpp\
	bruit-gaussian.cpp

MODULE_SRC=\
	GrayLevelImage2D.cpp

MODULE_OBJ=${MODULE_SRC:.cpp=.o}
EXEC_OBJ=${EXEC_SRC:.cpp=.o}

EXEC_PRG=${EXEC_SRC:.cpp=}

all: ${EXEC_PRG} ${MODULE_OBJ} ${EXEC_OBJ}

testGrayLevelImage2D: testGrayLevelImage2D.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

bruit-impulsionnel: bruit-impulsionnel.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

double-brightness: double-brightness.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

filtrage-median: filtrage-median.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

histogram: histogram.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

egalise: egalise.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

netlify: netlify.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

bruit-gaussian: bruit-gaussian.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

.cpp.o: %.cpp %.hpp
	${CXX} $<

clean:
	rm -f ${EXEC_PRG} ${MODULE_OBJ} ${EXEC_OBJ}
