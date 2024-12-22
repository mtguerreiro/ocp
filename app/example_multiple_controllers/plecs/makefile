OPIL := /home/marco/projects/opil

ifeq ($(OS),Windows_NT)
	RM := del /q
	PLAT := win
	BUILD_DIR := build
	DIR_GUARD := if not exist ${BUILD_DIR} mkdir ${BUILD_DIR}
	SOCK_LIB  := ws2_32
	EXT_LIB   := .dll
	COMM_DIR  := win
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		RM := rm -rf
		PLAT := linux
		BUILD_DIR := build
		DIR_GUARD := mkdir -p ${BUILD_DIR}
		EXT_LIB   := .so
		COMM_DIR  := linux
	else
		$(error Unsupported platform: $(UNAME_S))
	endif
endif

all: ${PLAT}

$(PLAT): ${OPIL}/sw/plecs/opil_plecs.c
	${DIR_GUARD}
	gcc -fPIC -c ${OPIL}/comm/$(COMM_DIR)/hostCommSock.c -I${OPIL}/comm/ -o ${BUILD_DIR}/hostCommSock.o
	gcc -fPIC -c ${OPIL}/simif/simif.c -I. -I../ -I${OPIL}/ -o ${BUILD_DIR}/simif.o
	gcc -fPIC -c ${OPIL}/opilhost.c -o ${BUILD_DIR}/opilhost.o
	gcc -fPIC -c ${OPIL}/sw/plecs/opil_plecs.c -I. -I../ -I${OPIL}/ -o ${BUILD_DIR}/opil_plecs.o
ifeq ($(PLAT),win)
	gcc -L. -Wl,-rpath=. -shared -o ${BUILD_DIR}/libopil_plecs${EXT_LIB} ${BUILD_DIR}/opil_plecs.o ${BUILD_DIR}/hostCommSock.o ${BUILD_DIR}/simif.o ${BUILD_DIR}/opilhost.o -l${SOCK_LIB}
else
	gcc -L. -Wl,-rpath=. -shared -o ${BUILD_DIR}/libopil_plecs${EXT_LIB} ${BUILD_DIR}/opil_plecs.o ${BUILD_DIR}/hostCommSock.o ${BUILD_DIR}/simif.o ${BUILD_DIR}/opilhost.o
endif

clean:
	${RM} ${BUILD_DIR}
