#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <thread>



using json = nlohmann::json;
using namespace std;

struct UserData{
    string name;
    int age;
};

vector<UserData> userDataStorage; 

string getMimeType(const string& extension){
    if(extension == ".html") return "text/html";
    if(extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".json") return "application/json";
    if (extension == ".png") return "image/png";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    return "text/plain";
}

string loadStaticFile(const string& path){
    ifstream file(path, ios::binary);
    if(!file.is_open()) return "";

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

void handleClient(int clientSocket){
       // Display message
    char buf[4096];
    memset(buf, 0, 4096);
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    if(bytesReceived == -1){
        cerr << "Error in recv()" << endl;
        return;
    }
    
    string request(buf,bytesReceived);
    cout << "Received request:\n" << request << endl; 

    string response;
    if(request.find("GET /") != string::npos){
        size_t pos = request.find("GET /") + 5;
        size_t endPos = request.find(" ", pos);
        string filePath = request.substr(pos, endPos - pos);
        if(filePath == "") filePath = "index.html";

        string fullPath = "public/" + filePath;
        string fileContent = loadStaticFile(fullPath);
        if (!fileContent.empty()) {
            string fileExtension = filePath.substr(filePath.find_last_of('.'));
            string mimeType = getMimeType(fileExtension);
            response = "HTTP/1.1 200 OK\r\nContent-Type: " + mimeType +
                       "\r\nContent-Length: " + to_string(fileContent.size()) + "\r\n\r\n" + fileContent;
         
        } else if(request.find("GET /hello") != string::npos){
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!";
        }
        else if (request.find("GET /status") != string::npos) {
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nServer is running";
        } else if (request.find("GET /data") != string::npos) {
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 10\r\n\r\nSome data";
        } else if(request.find("GET /users") != string::npos){
            json jsonResponse;
            for(const auto& user : userDataStorage){
                jsonResponse.push_back({{"name", user.name}, {"age", user.age}});
            }

            string jsonStr = jsonResponse.dump();
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + 
                       to_string(jsonStr.size()) + "\r\n\r\n" + jsonStr;
        } else {
            response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\n404 Not Found";
        }
    }
    else if(request.find("POST /json") != string::npos){
        size_t pos = request.find("\r\n\r\n");
        string postData;
        if(pos != string::npos){
            postData = request.substr(pos + 4);
        } else{
            postData = "";
        }

        try{
            json jsonData = json::parse(postData);
            UserData newUser;
            newUser.name = jsonData["name"].get<string>();
            newUser.age = jsonData["age"].get<int>();
            userDataStorage.push_back(newUser);
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 19\r\n\r\nJSON data received!";
        } catch(json::parse_error& e){
            response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 23\r\n\r\nInvalid JSON format";
        }
    }
    else if(request.find("POST /") != string::npos){
        size_t pos = request.find("\r\n\r\n");
        string postData;
        if(pos != string::npos){
            postData = request.substr(pos + 4);
        } else{
            postData = "";
        }

        cout << "Received POST data:\n" << postData << endl;
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 19\r\n\r\nPOST data received!";
    } else{
        response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\nContent-Length: 23\r\n\r\n405 Method Not Allowed";
        }

    send(clientSocket, response.c_str(), response.size(), 0);
}

int main(){
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1){
        cerr << "Cant create a socket" << endl;
        return -1;
    }

    // Bind address/port to socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);


    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
        cerr << "Cant bind to address/port" << endl;
        return -2;
    }

    // Mark socket to listening
    if(listen(listening, SOMAXCONN) == -1){
        cerr << "Cant mark the socket to listen" << endl;
        return -3;
    }

    cout << "Server is running on port 54000" << endl;

    // Accept a call from a client
    while(true){
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if(clientSocket == -1){
            cerr << "Cant accept" << endl;
            return -4;
        }
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }
    close(listening);
    return 0;
}
