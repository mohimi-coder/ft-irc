# ft-irc

<img src="https://capsule-render.vercel.app/api?type=waving&color=timeGradient&height=180&text=Hi,%20welcome%20to%20our%20cub3D!&fontSize=24&fontAlignY=32&animation=twinkling" alt="header" width="100%" />
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



## Responding to Messages

Now that you’re familiar with the project's structure, let’s go over how to handle incoming messages effectively.
Instead of relying on the outdated RFC1459, RFC2812, or RFC7194 specifications, I recommend referring to the IRCv3 specification.
While the older RFCs are still functional, they may cause unnecessary complications when interpreting messages from modern clients.
To better understand real-world message handling, try connecting to Libera Chat—one of the most popular public IRC networks—using netcat. Observing how it operates in practice can be more insightful than strictly following 
the RFC documentation. This approach allows you to replicate real client behavior and streamline your implementation.

