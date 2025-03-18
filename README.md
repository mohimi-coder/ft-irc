# ft-irc

<img src="https://capsule-render.vercel.app/api?type=waving&color=timeGradient&height=180&text=Hi,%20welcome%20to%20our%20FT-IRC!&fontSize=24&fontAlignY=32&animation=twinkling" alt="header" width="100%" />
<samp>

This project is designed to provide an in-depth study of socket operations and their interactions, demonstrated through an IRC chat server. 
The project is built using a Makefile and can be executed with the following command:

``./ircserv [port] [pass]`` 

## ft_irc Project Overview

ft_irc is a project you may encounter during your coursework. If you're reading this, chances are you're working on it now. This project involves developing an IRC (Internet Relay Chat) server, providing a great opportunity to deepen your understanding of bi-directional file descriptor communication (also known as sockets).

While the project is manageable, it will challenge you and help you build a strong foundation in network programming.

## What is IRC?

IRC is a simple yet powerful protocol designed for online communities. 
Think of it as an early version of modern chat platforms like Discord or Slack, but with a key difference: IRC is open-source. 
Anyone can create a server or build a client, allowing full control over the chat environment without relying on third-party services.

## Understanding the Project Scope

This project can be broken down into four fundamental pillars:

## The Server 
    – Manages client connections and overall network communication.
## The Command Parser 
    – Interprets and processes user commands.
## The Channels 
    – Handles group communication and message routing.
## The Client 
    – Represents a connection to the server (not the actual IRC client used to connect).
    
Each component plays a vital role in building a functional IRC server, enabling clients to connect, communicate, and interact in real time.

Below is a diagram illustrating how I structured the project, with a focus on object-oriented programming (OOP):

![irc-global-overview](https://github.com/user-attachments/assets/e8937157-47e6-45af-9019-6d972b01c232)


## 🌐The Server
    The server is the core of the IRC system, responsible for managing socket connections and maintaining key variables like the server password. It facilitates real-time communication between clients and the IRC network through efficient socket management.

## 🤔How It Works

    1️⃣ Creating & Binding a Listening Socket – The server initializes a socket and binds it to a specific port to listen for incoming client connections.
    
    2️⃣ Accepting Connections – When a client requests to connect, the server accepts the connection and assigns it a new socket for communication.
    
    3️⃣Handling Multiple Clients – Each connected client is managed using separate sockets, enabling multiple users to interact with the server simultaneously.
    

## ⁴⁰⁴Parsing the Input
    Once the server manages socket connections, the next step is processing incoming commands and directing them to the appropriate service. 
    This typically involves basic conditional checks (e.g., if-else statements) to identify and execute commands correctly.

## Authentication Handling

    This stage is also where you might implement a global authentication wall, ensuring that only registered users can access certain commands. 
    A simple three-tier authentication system is commonly used:

## ❶ UNAUTHENTICATED 
    – The client has connected but has not provided valid credentials.
## ❷ AUTHENTICATED 
    – The client has passed basic authentication but is not yet fully registered.
## ❸ REGISTERED 
    – The client is fully verified and can access all permitted commands.




## Responding to Messages

Now that you’re familiar with the project's structure, let’s go over how to handle incoming messages effectively.
Instead of relying on the outdated RFC1459, RFC2812, or RFC7194 specifications, I recommend referring to the IRCv3 specification.
While the older RFCs are still functional, they may cause unnecessary complications when interpreting messages from modern clients.
To better understand real-world message handling, try connecting to Libera Chat—one of the most popular public IRC networks—using netcat. Observing how it operates in practice can be more insightful than strictly following 
the RFC documentation. This approach allows you to replicate real client behavior and streamline your implementation.

