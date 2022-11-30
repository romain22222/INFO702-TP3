LD=g++
CXX=g++ -g -c

EXEC_SRC=\
	testImage2DGray.cpp\
	testImage2DColor.cpp\
	testImage2DColorBand.cpp\
	testExportImage2DColor.cpp\
	invert-red-blue.cpp

MODULE_SRC=\

MODULE_OBJ=${MODULE_SRC:.cpp=.o}
EXEC_OBJ=${EXEC_SRC:.cpp=.o}

EXEC_PRG=${EXEC_SRC:.cpp=}

all: ${EXEC_PRG} ${MODULE_OBJ} ${EXEC_OBJ}

testImage2DGray: testImage2DGray.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

testImage2DColor: testImage2DColor.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

testImage2DColorBand: testImage2DColorBand.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

testExportImage2DColor: testExportImage2DColor.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

invert-red-blue: invert-red-blue.o ${MODULE_OBJ}
	${LD} ${MODULE_OBJ} $< -o $@

.cpp.o: %.cpp
	${CXX} $<

clean:
	rm -f ${EXEC_PRG} ${MODULE_OBJ} ${EXEC_OBJ}
