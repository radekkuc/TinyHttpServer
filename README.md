# TinyHttpServer
A minimal HTTP server written in C++ with support for handling basic HTTP requests and serving static files. This server demonstrates the core functionalities needed to respond to GET and POST requests, making it a lightweight option for simple web hosting and testing purposes.

Features:
- Static File Serving: Hosts files (e.g., HTML, CSS, JS) from a designated directory (public), allowing it to act as a static file server.
- Dynamic JSON Handling: Accepts and stores JSON data via POST requests.
- Threaded Connections: Uses threading to handle multiple client connections simultaneously.
- Basic MIME Type Support: Determines MIME type based on file extension to support common web files.

Getting Started
1. Clone the repository:
git clone https://github.com/your-username/SimpleWebServer.git
cd SimpleWebServer

2. Compile the server:
g++ server.cpp -o server -lpthread -lstdc++fs

4. Create a directory named public and add any files you want the server to serve (e.g., index.html).
