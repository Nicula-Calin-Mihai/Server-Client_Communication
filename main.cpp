#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include "helpers.h"
#include "requests.h"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main() {
	string buffer, username, password, cookie, token;
	int PORT = 8080;
	char HOST[] = "34.241.4.235";
	while (1) {
		cin >> buffer;
		// 4.1
		if (buffer == "register") {
			cout << "username=";
			cin >> username;
			cout << "password=";
			cin >> password;
			json gionson;
			gionson["username"] = username;
			gionson["password"] = password;
			char **gionii = (char **) malloc(sizeof(char*) * 1);
			gionii[0] = (char *) malloc (sizeof(char) * 300);
			strcpy(gionii[0], gionson.dump(4).c_str());
			char *message = compute_post_request(HOST, "/api/v1/tema/auth/register", "application/json", gionii, 1, NULL, 0, false, NULL);
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			// AL 9-lea caracter este cifra sutelor si asa mai departe
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399)
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			else
				cout << "Registered\n";
			delete[] message;
			close_connection(sockfd);
		}
		// 4.2
		if (buffer == "login") {
			cout << "username=";
			cin >> username;
			cout << "password=";
			cin >> password;
			json gionson;
			gionson["username"] = username;
			gionson["password"] = password;
			char **gionii = (char **) malloc(sizeof(char*) * 1);
			gionii[0] = (char *) malloc (sizeof(char) * 300);
			strcpy(gionii[0], gionson.dump(4).c_str());
			char *message = compute_post_request(HOST, "/api/v1/tema/auth/login", "application/json", gionii, 1, NULL, 0, false, NULL);
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399)
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			else {
				string str;
				str.append(message);
				auto cookieStart = str.find("Set-Cookie");
				auto cookieEnd = str.find(';', cookieStart);
				// Sar peste Set-Cookie si ce mai e dupa, si iau pana la primul ';'
				cookie = str.substr(cookieStart + 12, cookieEnd - (cookieStart + 12));
				cout << "Logged In\n";
			}
			delete[] message;
			close_connection(sockfd);
		}
		// 4.3
		if (buffer == "enter_library") {
			char **gionii = (char **) malloc(sizeof(char*) * 1);
			gionii[0] = (char *) malloc (sizeof(char) * 300);
			strcpy(gionii[0], cookie.c_str());
			char *message = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, gionii, 1, false, NULL);
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399)
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			else {
				token = json::parse(basic_extract_json_response(message))["token"];
				cout << "Accessed" << "\n";
			}
			delete[] message;
			close_connection(sockfd);
		}
		// 4.4
		if (buffer == "get_books") {
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			char *message = compute_get_request(HOST, "/api/v1/tema/library/books", NULL, NULL, 0, true, token.c_str());
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399) {
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			}
			else {
				json gionson = json::parse(basic_extract_json_responsev2(message));
				if (gionson.empty()) {
					cout << "Empty" << "\n";
				}
				else {
					for (const auto &el: gionson.items()) {
						auto x = el.value();
						cout << x["id"] << " " << x["title"] << "\n";
					}
				}
			}
			delete[] message;
			close_connection(sockfd);
		}
		// 4.5
		if (buffer == "get_book") {
			char id[100];
			cout << "id="; cin >> id;
			char URL[100] = "/api/v1/tema/library/books/";
			strcat(URL, id);
			char *message = compute_get_request(HOST, URL, NULL, NULL, 0, true, token.c_str());
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399) {
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			}
			else {
				json gionson = json::parse(basic_extract_json_responsev2(message));
				cout << gionson.dump(4) << "\n";
			}
			delete[] message;
			close_connection(sockfd);
		}
		// 4.6
		if (buffer == "add_book") {
			getchar();
			cout << "title=";
			string title;
			getline(cin, title, '\n');
			cout << "author=";
			string author;
			getline(cin, author, '\n');
			cout << "genre=";
			string genre;
			getline(cin, genre, '\n');
			cout << "page_count=";
			string page_count;
			getline(cin, page_count, '\n');
			cout << "publisher=";
			string publisher;
			getline(cin, publisher, '\n');
			json gionson;
			gionson["title"] = title;
			gionson["author"] = author;
			gionson["genre"] = genre;
			gionson["page_count"] = page_count;
			gionson["publisher"] = publisher;
			char **gionii = (char **) malloc(sizeof(char*) * 1);
			gionii[0] = (char *) malloc (sizeof(char) * 300);
			strcpy(gionii[0], gionson.dump(4).c_str());
			char *message = compute_post_request(HOST, "/api/v1/tema/library/books", "application/json", gionii, 1, NULL, 0, true, token.c_str());
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399)
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			else
				cout << "Added" << "\n";
			delete[] message;
			close_connection(sockfd);
		}
		// 4,7
		if (buffer == "delete_book") {
			char id[100];
			cout << "id="; cin >> id;
			char URL[100] = "/api/v1/tema/library/books/";
			strcat(URL, id);
			char *message = compute_delete_request(HOST, URL, NULL, NULL, 0, true, token.c_str());
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399)
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			else
				cout << "Deleted" << "\n";
			delete[] message;
			close_connection(sockfd);
		}
		// 4.8
		if (buffer == "logout") {
			char **gionii = (char **) malloc(sizeof(char*) * 1);
			gionii[0] = (char *) malloc (sizeof(char) * 300);
			strcpy(gionii[0], cookie.c_str());
			char *message = compute_get_request(HOST, "/api/v1/tema/auth/logout", NULL, gionii, 1, false, NULL);
			int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
			send_to_server(sockfd, message);
			delete[] message;
			message = receive_from_server(sockfd);
			int errorMessage = (message[9] - '0') * 100 + (message[10] - '0') * 10 + (message[11] - '0');
			if (errorMessage > 399) {
				cout << "EROARE " << json::parse(basic_extract_json_response(message))["error"] << "\n";
			}
			else {
				cout << "Logged Out" << "\n";
				cookie.clear();
				token.clear();
			}
			delete[] message;
			close_connection(sockfd);
		}
		// Exit
		if (buffer == "exit") {
			break;
		}
	}
	return 0;
}
