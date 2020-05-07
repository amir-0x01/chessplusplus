// NETWORKING
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <limits.h>
#include <arpa/inet.h>
#include <netdb.h>

//STANDARD LIBRARIES
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <sys/time.h>

//THREADS
#include <chrono>
#include <thread>

//GAME
#include "chess++.hpp"

#define output(x) std::cout << x << std::endl
std::string fun_facts [] = {"The word “checkmate” comes from the Arabic word “shah mat” which translates to “The king is dead” in English.",
                            "The new move where the pawn could move two steps instead of one was introduced in Spain in 1280.",
                            "A computer called Deep Thought became the first of its kind to beat an international maestro in November 1988, Long Beach, California.",
                            "Are you aware of the fact that the number of possible ways of playing the first four moves for both sides in a game of chess is 318,979,564,000?",
                            "Initially, the Queen could only move one square at a time, diagonally. Later, she could move two squares at a time, diagonally.\nIt wasn’t until Reconquista Spain, with its powerful queen Isabella, that the Queen became the strongest piece on the board.",
                            "The number of possible unique chess games is much greater than the number of electrons in the universe.\nThe number of electrons is estimated to be about 10^79, while the number of unique chess games is 10^120.",
                            "The longest chess game theoretically possible is 5,949 moves."};
void display_start_screen(void){
    //reading __about__.txt
    system("clear");
    std::string line;
    std::ifstream file("__about__.txt");
    if(file.is_open()){
        while(getline(file, line)){ std::cout << line << std::endl;}
        file.close();
    }
}

struct game_methods {
    void main_menu(){
        std::string input;
        game_methods game;
        while(true){
            display_start_screen();
            std::cout << " [A] Host a game\n";
            std::cout << " [B] Join a game\n";
            std::cout << " [C] Play on the same screen\n\n";
            std::cout << "$ ";

            std::cin >> input;
        
            if(input == "A"){
                std::chrono::seconds duration(1);
                std::this_thread::sleep_for(duration);
                game.host_game();
            }
            else if(input == "B"){
                std::chrono::seconds duration(1);
                std::this_thread::sleep_for(duration);
                game.join_game();
            }
            else if(input == "C"){
                std::chrono::seconds duration(1);
                std::this_thread::sleep_for(duration); //sleeps for one second
                game.start_same_screen();
            }
            else{
                output("error");
                std::chrono::seconds duration(1);
                std::this_thread::sleep_for(duration); //sleeps for one second
            }
        }
    }
    void start_same_screen(void){
        chess_object chess_game;
        chess_game.set_values("", true, false);
        std::string input_move;
        while(true){
            chess_game.display_board();
            chess_game.check();
            if(chess_game.is_check){output("CHECK !");}
            if(chess_game.return_num_white() == 1){
                chess_game.display_game_over("white");
            }
            else if(chess_game.return_num_black() ==  1){
                chess_game.display_game_over("black");
            }
            else if(chess_game.return_white_rounds()){
                std::cout << "[WHITE]: ";
                std::cin >> input_move;
                if(input_move == "surrender"){chess_game.display_game_over("black");}
            }
            else if(chess_game.return_black_rounds()){
                std::cout << "[BLACK]: ";
                std::cin >> input_move;
                if(input_move == "surrender"){chess_game.display_game_over("white");}
            }
            try{
                if(chess_game.legal_move(input_move, false)){chess_game.move_object(input_move);}
                else if(!chess_game.legal_move(input_move, false)){
                    while(true){
                        //std::cout << "\nILLEGAL MOVE (" << e.what() <<  ")"<< std::endl;
                        std::string temp_input;
                        std::cout << ">> ";
                        std::cin >> temp_input;
                        try{
                            if(chess_game.legal_move(temp_input, false)){
                                chess_game.move_object(temp_input);
                                break;
                            }
                        }
                        catch(std::exception e){}

                    }
                }
            }

            catch(std::exception e){
                while(true){
                    std::cout << "\nILLEGAL MOVE (" << e.what() <<  ")"<< std::endl;
                    std::string temp_input;
                    std::cout << ">> ";
                    std::cin >> temp_input;
                    try{
                        if(chess_game.legal_move(temp_input, false)){
                            chess_game.move_object(temp_input);
                            break;
                        }
                    }
                    catch(std::exception e){}

                }
                
            }
        }
    }

    void host_game(void){
        display_start_screen();
        chess_object chess_game;
        chess_game.set_values("", true, false);

        //for the server, we only need to specify a port number
        std::string username;
        std::cout << "USERNAME$ ";
        std::cin >> username;

        unsigned int port;
        std::cout << "PORT$ ";
        std::cin >> port;

        //buffer to send and receive messages with
        char msg[1500];
        char hostname[HOST_NAME_MAX];
        gethostname(hostname, HOST_NAME_MAX);
        
        //setup a socket and connection tools
        sockaddr_in serv_addr;
        bzero((char*)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(port);
    
        //open stream oriented socket with internet address
        //also keep track of the socket descriptor
        int server_sd = socket(AF_INET, SOCK_STREAM, 0);
        if(server_sd < 0){
            std::cerr << "[?] Error establishing the server socket" << std::endl;
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            main_menu();
        }
        //bind the socket to its local address
        int bind_status = bind(server_sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
        if(bind_status < 0){
            std::cerr << "[?] Error binding socket to local address" << std::endl;
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            main_menu();
        }
        std::cout << "[+] Hosting game as " << hostname << " on port " << port << std::endl;
        
        listen(server_sd, 5);
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);
        
        int new_socket = accept(server_sd, (sockaddr *)&newSockAddr, &newSockAddrSize);
        if(new_socket < 0){
            std::cerr << "[?] Error accepting request from client!" << std::endl;
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            main_menu();
        }

        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        int bytes_red, bytes_written = 0;

        std::string client_username;

        //send username
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, username.c_str()); //it is considered dangerous to use strcpy due to bufferoverflow
        bytes_written += send(new_socket, (char*)&msg, strlen(msg), 0);

        memset(&msg, 0, sizeof(msg));
        bytes_red += recv(new_socket, (char*)&msg, sizeof(msg), 0);
        client_username = std::string(msg);

        std::cout << "[*] Connected with " << client_username << std::endl;
        std::cout << "[*] Game will start in ten seconds !" << std::endl;

        srand(time(0));
        int pos= (rand() % 6) + 1;
        std::cout << "\n" << fun_facts[pos] << std::endl;

        std::chrono::seconds duration(10);
        std::this_thread::sleep_for(duration);

        while(true){
            system("clear");
            chess_game.display_board();
            chess_game.check();
            if(chess_game.is_check){output("CHECK !");}
            if(chess_game.return_num_white() == 1){
                chess_game.display_game_over("white");
            }
            else if(chess_game.return_num_black() ==  1){
                chess_game.display_game_over("black");
            }

            else if(chess_game.return_black_rounds() ){
                std::cout <<  username << " (BLACK): ";
                std::string data;
                std::cin >> data;
                memset(&msg, 0, sizeof(msg)); //clear the buffer
                strcpy(msg, data.c_str()); 

                if(data == "surrender"){
                    //send to the client that server has closed the connection
                    send(new_socket, (char*)&msg, strlen(msg), 0);
                    std::cout << "[*] " << "YOU " << " surrendered !" << std::endl;
                    std::cout << "[*] " << client_username << " WINS !" << std::endl;
                    gettimeofday(&end1, NULL);
                    close(new_socket);
                    close(server_sd);
                    
                    std::cout << "\n********END OF GAME********" << std::endl;
                    std::cout << "Bytes written: " << bytes_written << std::endl;
                    std::cout << "Bytes red: " << bytes_red << std::endl;
                    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                    std::cout << "";
                    exit(2);
                }
                //send the message to client
                try{
                    if(chess_game.legal_move(data, false)){
                        bytes_written += send(new_socket, (char*)&msg, strlen(msg), 0);
                        chess_game.swap_object(data, "black");

                    }
                    else if(!chess_game.legal_move(data, false)){
                        while(true){
                            std::cout << "[?] Illegal move !" << std::endl;
                            std::cout <<  username << " (BLACK): ";
                            std::string data;
                            std::cin >> data;
                            memset(&msg, 0, sizeof(msg)); //clear the buffer
                            strcpy(msg, data.c_str());

                            if(data == "surrender"){
                                //send to the client that server has closed the connection
                                send(new_socket, (char*)&msg, strlen(msg), 0);
                                std::cout << "[*] " << "YOU " << " surrendered !" << std::endl;
                                std::cout << "[*] " << client_username << " WINS !" << std::endl;
                                gettimeofday(&end1, NULL);
                                close(new_socket);
                                close(server_sd);
                                
                                std::cout << "\n********END OF GAME********" << std::endl;
                                std::cout << "Bytes written: " << bytes_written << std::endl;
                                std::cout << "Bytes red: " << bytes_red << std::endl;
                                std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                                std::cout << "";
                                exit(10);
                            }
                            else if(chess_game.legal_move(data, false)){
                                bytes_written += send(new_socket, (char*)&msg, strlen(msg), 0);
                                chess_game.swap_object(data, "black");
                                break;
                            }
                        }
                    }
                }

                catch(std::exception){
                    while(true){
                        std::cout << "[?] Illegal move !" << std::endl;
                        std::cout <<  username << " (BLACK): ";
                        std::string data;
                        std::cin >> data;
                        memset(&msg, 0, sizeof(msg)); //clear the buffer
                        strcpy(msg, data.c_str());

                        if(data == "surrender"){
                            //send to the client that server has closed the connection
                            send(new_socket, (char*)&msg, strlen(msg), 0);
                            std::cout << "[*] " << "YOU " << " surrendered !" << std::endl;
                            std::cout << "[*] " << client_username << " WINS !" << std::endl;
                            gettimeofday(&end1, NULL);
                            close(new_socket);
                            close(server_sd);
                            
                            std::cout << "\n********END OF GAME********" << std::endl;
                            std::cout << "Bytes written: " << bytes_written << std::endl;
                            std::cout << "Bytes red: " << bytes_red << std::endl;
                            std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                            std::cout << "";
                            exit(10);
                        }
                        else if(chess_game.legal_move(data, false)){
                            bytes_written += send(new_socket, (char*)&msg, strlen(msg), 0);
                            chess_game.swap_object(data, "black");
                            break;
                        }
                    }
                }


            }

            else if(chess_game.return_white_rounds()){
                //receive a message from the client (listen)
                std::cout << "Awaiting " << client_username << " to make a move..." << std::endl;
                memset(&msg, 0, sizeof(msg));//clear the buffer
                bytes_red += recv(new_socket, (char*)&msg, sizeof(msg), 0);
                if(!strcmp(msg, "surrender")){
                    std::cout << "[*] " << client_username << " SURRENDERED !" << std::endl;
                    std::cout << "[*] YOU WIN !" << std::endl;
                    gettimeofday(&end1, NULL);
                    close(new_socket);
                    close(server_sd);
                    
                    std::cout << "\n********END OF GAME********" << std::endl;
                    std::cout << "Bytes written: " << bytes_written << std::endl;
                    std::cout << "Bytes red: " << bytes_red << std::endl;
                    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                    exit(10);
                }
        
                chess_game.swap_object(msg, "white");
                chess_game.set_black();

            }

        }
        
    }
    
    void join_game(void){
        display_start_screen();
        chess_object chess_game;
        chess_game.set_values("", true, false);

        std::string username;
        std::cout << "USERNAME$ ";
        std::cin >> username;

        std::string hostname;
        std::cout << "HOSTNAME$ ";
        std::cin >> hostname;

        int port;
        std::cout << "PORT$ ";
        std::cin >> port;


        char msg[1500]; 
        
        struct hostent* host = gethostbyname(hostname.c_str()); 
        sockaddr_in send_sock_addr;   
        bzero((char*)&send_sock_addr, sizeof(send_sock_addr)); 
        send_sock_addr.sin_family = AF_INET; 
        send_sock_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        send_sock_addr.sin_port = htons(port);
        int client_sd = socket(AF_INET, SOCK_STREAM, 0);
        //try to connect...
        int status = connect(client_sd, (sockaddr*) &send_sock_addr, sizeof(send_sock_addr));
        if(status < 0){ 
            std::cout << "[?] Error connecting to socket..." << std::endl;
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            main_menu();
        }

        int bytes_red, bytes_written = 0;
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);

        std::string server_username;

        memset(&msg, 0, sizeof(msg));
        bytes_red += recv(client_sd, (char*)&msg, sizeof(msg), 0);
        server_username = std::string(msg);

        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, username.c_str()); //it is considered dangerous to use strcpy due to bufferoverflow
        bytes_written += send(client_sd, (char*)&msg, strlen(msg), 0);

        std::cout << "[*] Connected with " << server_username << std::endl;
        std::cout << "[*] Game will start in ten seconds !" << std::endl;

        srand(time(0));
        int pos= (rand() % 6) + 1;
        std::cout << "\n" << fun_facts[pos] << std::endl;

        std::chrono::seconds duration(10);
        std::this_thread::sleep_for(duration);

        while(true){
            system("clear");
            chess_game.display_board();
            chess_game.check();
            if(chess_game.is_check){output("CHECK !");}
            if(chess_game.return_num_white() == 1){
                chess_game.display_game_over("white");
            }
            else if(chess_game.return_num_black() ==  1){
                chess_game.display_game_over("black");
            }

            else if(chess_game.return_black_rounds() ){
                std::cout << "Awaiting " << server_username << " to make a move..." << std::endl;
                memset(&msg, 0, sizeof(msg));//clear the buffer
                bytes_red += recv(client_sd, (char*)&msg, sizeof(msg), 0);
                if(!strcmp(msg, "surrender")){ 
                    std::cout << "[*] " << server_username << " SURRENDERED !" << std::endl;
                    std::cout << "[*] YOU WIN !" << std::endl;
                    gettimeofday(&end1, NULL);
                    close(client_sd);
        
                    std::cout << "\n********END OF GAME********" << std::endl;
                    std::cout << "Bytes written: " << bytes_written << std::endl;
                    std::cout << "Bytes red: " << bytes_red << std::endl;
                    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                    std::cout << "";
                    exit(2);
                
                }
                
                chess_game.swap_object(msg, "black");
                chess_game.set_white();

            }

            else if(chess_game.return_white_rounds() ){
                std::string data;
                std::cout << username << " (WHITE): ";
                std::cin >> data;

                memset(&msg, 0, sizeof(msg));//clear the buffer 
                strcpy(msg, data.c_str()); //it is considered dangerous to use strcpy due to bufferoverflow
                if(data == "surrender")
                {
                    send(client_sd, (char*)&msg, strlen(msg), 0);
                    std::cout << "[*] " << "YOU " << " SURRENDERED !" << std::endl;
                    std::cout << "[*] " << server_username << " WINS !" << std::endl;
                    gettimeofday(&end1, NULL);
                    close(client_sd);
        
                    std::cout << "\n********END OF GAME********" << std::endl;
                    std::cout << "Bytes written: " << bytes_written << std::endl;
                    std::cout << "Bytes red: " << bytes_red << std::endl;
                    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                    std::cout << "";
                    exit(2);
                }
                try{
                    if(chess_game.legal_move(data, false)){
                        bytes_written += send(client_sd, (char*)&msg, strlen(msg), 0);
                        chess_game.swap_object(data, "white");
                        chess_game.set_black();
                    }
                    else if(!chess_game.legal_move(data, false)){
                        while(true){
                            std::cout << "[?] Illegal move !" << std::endl;
                            std::cout << username << " (WHITE): ";
                            std::cin >> data;
                            memset(&msg, 0, sizeof(msg));//clear the buffer 
                            strcpy(msg, data.c_str());
                            if(data == "surrender"){
                                send(client_sd, (char*)&msg, strlen(msg), 0);
                                std::cout << "[*] " << "YOU " << " SURRENDERED !" << std::endl;
                                std::cout << "[*] " << server_username << " WINS !" << std::endl;
                                gettimeofday(&end1, NULL);
                                close(client_sd);
        
                                std::cout << "\n********END OF GAME********" << std::endl;
                                std::cout << "Bytes written: " << bytes_written << std::endl;
                                std::cout << "Bytes red: " << bytes_red << std::endl;
                                std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
                                std::cout << "";
                                exit(2);
                            }
                            else if(chess_game.legal_move(data, false)){
                                bytes_written += send(client_sd, (char*)&msg, strlen(msg), 0);
                                chess_game.swap_object(data, "white");
                                chess_game.set_black();
                                break;
                            }
                        }
                        
                    }
                }
                catch(std::exception){
                    while(true){
                        std::cout << "[?] Illegal move !" << std::endl;
                        std::cout << username << " (WHITE): ";
                        std::cin >> data;
                        memset(&msg, 0, sizeof(msg));//clear the buffer 
                        strcpy(msg, data.c_str());
                        if(data == "surrender"){
                            send(client_sd, (char*)&msg, strlen(msg), 0);
                            std::cout << "[*] " << "YOU " << " SURRENDERED !" << std::endl;
                            std::cout << "[*] " << server_username << " WINS !" << std::endl;
                            gettimeofday(&end1, NULL);
                            close(client_sd);
                
                            std::cout << "\n********END OF GAME********" << std::endl;
                            std::cout << "Bytes written: " << bytes_written << std::endl;
                            std::cout << "Bytes red: " << bytes_red << std::endl;
                            std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << std::endl;
        
                            exit(10);
                        }
                        else if(chess_game.legal_move(data, false)){
                            bytes_written += send(client_sd, (char*)&msg, strlen(msg), 0);
                            chess_game.swap_object(data, "white");
                            chess_game.set_black();
                            break;
                        }
                    }
                }


            }
            

        }
    
    }
};

int main(void) {
    game_methods start;
    start.main_menu();
    return 0;
}
