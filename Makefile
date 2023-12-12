CXX="g++"
CXX_FLAGS= -Wall -Wzero-as-null-pointer-constant
OPENSSL_FLAGS= -lssl -lcrypto

OUT_SERVER="server_run.exe"
OUT_CLIENT="client_run.exe"

all: server.o client.o s_socket.o c_socket.o m_socket.o base_message.o crypto.o
	${CXX} ${CXX_FLAGS} server.o s_socket.o m_socket.o base_message.o crypto.o -o ${OUT_SERVER} ${OPENSSL_FLAGS}
	${CXX} ${CXX_FLAGS} client.o c_socket.o m_socket.o base_message.o crypto.o -o ${OUT_CLIENT} ${OPENSSL_FLAGS}

server.o: src/server/server.cpp src/server/server.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/server/server.cpp

client.o: src/client/client.cpp src/client/client.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/client/client.cpp

s_socket.o: src/sockets/s_socket.cpp src/sockets/s_socket.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/sockets/s_socket.cpp

c_socket.o: src/sockets/c_socket.cpp src/sockets/c_socket.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/sockets/c_socket.cpp

m_socket.o: src/sockets/m_socket.cpp src/sockets/m_socket.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/sockets/m_socket.cpp

base_message.o: src/messages/base_message.cpp src/messages/base_message.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/messages/base_message.cpp

crypto.o: src/crypto/crypto.cpp src/crypto/crypto.hpp
	${CXX} ${CXX_FLAGS} -I . -c src/crypto/crypto.cpp

clean:
	rm -rf *.o
	rm ${OUT_SERVER} ${OUT_CLIENT}