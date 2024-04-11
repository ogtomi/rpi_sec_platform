# Server/ Client secure communication platform

## Aim

The platform aims to provide a secured communication channel between a client and a server using TCP/IP. After establishing the communication, client can access the server's database which contains user credentials. The client can create new users and also log in with the already existing user credentials.

## E2E

### Establishing communication

In the first step of creating a communication channel server and client sockets are created. TCP sockets are used for communication between server and client processes.</br>

The server opens up a port, binds a socket to it, and listens for incoming connections from clients. Once this is done, the client can connect to it and the server can accept the connection.</br>

If the socket connection is valid, a handshake is performed. The aim of that operation is for the two communicating sides to acknowledge each other, verify each other, and establish the cryptographic session keys. A detailed description of the handshake can be found [here](#handshake). If the handshake is successful the session keys are derived and the communication channel is established.

### Database operations

Once the communication channel is established server sends the UI to the client.

> Note: The UI contains a list of commands that can be executed on the database.

After receiving the UI, client can create a new user in the database and also log in with the already existing user credentials.

#### Creating a new user

Using this option, client creates a new entry in the database table. The entry contains user name and password. The password itself is not stored in the database. After submitting the entry, a cryptographic hash is calculated from the password input and this hash is stored in the database.

#### Log in with already existing credentials

Using this option, client can verify itself against already existing credentials. This consists of two suboperations. Firstly the submitted user name is looked up in the database table to check if it exists. If so, in the next step the hash from the submitted password is compared with the hash from the database. If those two are the same log in is successful.

## Methods used

### Handshake

A handshake is a series of steps that allows client and a server to authenticate each other, agree on encryption standards, and establish a secure channel for transferring data.

#### Step-by-step procedure

1. Client and server generate ECC keys.
2. Client and server send their public key to the other party.
3. A shared secret is obtained using [ECDH](#ecdh) algorithm.
4. A cryptographic hash is calculated by each party using [SHA-256](#sha-256) algorithm.
5. A session key and IV are derived from the previously calculated hash.
    > Note: IV - Initialization Vector

    > Note: The first 128 bits of hash are used as AES-128 key and the second 128 bits as IV.

### TCP IP

TCP/IP stands for Transmission Control Protocol/ Internet Protocol and is a suite of communication protocols used to interconnect network devices on the internet. It specifies how data is exchanged over the internet by providing end-to-end communications that identify how it should be broken into packets, addressed, transmitted, routed, and received at the destination.

### ECDH

Elliptic-curve Diffie-Hellman is a key agreement protocol that allows two parties, each having an elliptic-curve public-private key pair, to establish a shared secret over an insecure channel. This shared secret is used to calculate a cryptographic hash from which encryption parameters are derived.</br>

ECDH is based on the following property of EC points:

$(a * G) * b = (b * G) * a$

where:</br>
$a, b$ - secret numbers</br>
$G$ - generator point</br>

If there are two secret numbers $a$ and $b$ (private keys) and an ECC elliptic curve with generator point $G$, values $(a * G)$ and $(b * G)$ (public keys) can be exchangese over an insecure channel. Then a shared secret can be derived from the aforementioned equation.

### AES-128

The algorithm's documentation can be found [here](https://github.com/ogtomi/crypto_lib/blob/master/docs/aes.md).

### SHA-256

The algorithm's documentation can be found [here](https://github.com/ogtomi/crypto_lib/blob/master/docs/sha256.md).

## Libraries used
### OPENSSL

The library's documentation can be found [here](https://www.openssl.org/).

### PQXX

The library's documentation can be found [here](https://github.com/jtv/libpqxx).