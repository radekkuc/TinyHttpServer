# TinyHttpServer
A minimal HTTP server written in C++ with support for handling basic HTTP requests and serving static files. This server demonstrates the core functionalities needed to respond to GET and POST requests, making it a lightweight option for simple web hosting and testing purposes.

Features:
- Static File Serving: Hosts files (e.g., HTML, CSS, JS) from a designated directory (public), allowing it to act as a static file server.
- Dynamic JSON Handling: Accepts and stores JSON data via POST requests.
- Threaded Connections: Uses threading to handle multiple client connections simultaneously.
- Basic MIME Type Support: Determines MIME type based on file extension to support common web files.

Getting Started
1. Clone the repository:

![image](https://github.com/user-attachments/assets/068989ee-98af-4f13-af10-fdbf72c5de98)

![image](https://github.com/user-attachments/assets/7357f899-1a02-4775-ac0c-daabbb5fa327)

2. Compile the server:

![image](https://github.com/user-attachments/assets/8a394a0d-b093-4fed-a2fe-2073009e3605)

3. Create a directory named public and add any files you want the server to serve (e.g., index.html).

4. Run the server:

![image](https://github.com/user-attachments/assets/fbaf73b9-db06-45a8-9e04-f02eb1c4fb55)

