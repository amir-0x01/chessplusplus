#ifndef CHESSPP_H
#define CHESSPP_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <cstdlib>

#define output(x) std::cout << x << std::endl

class chess_object{
    private:
        const std::string SPACE_GAP = "   ";
        std::string board [10][9] = {
        {"0"," A "," B "," C "," D "," E "," F "," G "," H "},
        {"1"," ♖ "," ♘ "," ♗ "," ♔ "," ♕ "," ♗ "," ♘ "," ♖ "},
        {"2"," ♙ "," ♙ "," ♙ "," ♙ "," ♙ "," ♙ "," ♙ "," ♙ "},
        {"3","   ","   ","   ","   ","   ","   ","   ","   "},
        {"4","   ","   ","   ","   ","   ","   ","   ","   "},
        {"5","   ","   ","   ","   ","   ","   ","   ","   "},
        {"6","   ","   ","   ","   ","   ","   ","   ","   "},
        {"7"," ♟ "," ♟ "," ♟ "," ♟ "," ♟ "," ♟ "," ♟ "," ♟ "},
        {"8"," ♜ "," ♞ "," ♝ "," ♚ "," ♛ "," ♝ "," ♞ "," ♜ "},
        };
        std::string previous_move;

        std::vector<std::string> whites_captured;
        std::vector<std::string> blacks_captured;
        std::vector<std::string> total_moves;
    
        bool white_rounds;
        bool black_rounds;

        unsigned int number_white = 16;
        unsigned int number_black = 16;

    public:
        unsigned static int moves;
        bool is_check = false;

        std::vector<std::string> legal_possible_moves_white;
        std::vector<std::string> legal_possible_moves_black;

        std::string* return_board(){return *board;}
        std::string return_previous_move(){return previous_move;}
        
        std::string return_board_position(int pos);
        bool return_white_rounds(){return white_rounds;}
        bool return_black_rounds(){return black_rounds;}

        int return_num_white(){return number_white;}
        int return_num_black(){return number_black;}
        
        int return_moves(){return moves;}
        int return_position(std::string c);

        bool is_white(std::string object);
        bool is_black(std::string object);

        bool check();
        bool checkmate();

        std::string where_king(std::string object);
        std::vector<std::string> white_king_moves;
        std::vector<std::string> black_king_moves;
        std::vector<std::string> counter_check;
  
        void set_values(std::string previous_move, bool white_rounds, bool black_rounds);
        void set_black();
        void set_white();
        void move_prediction(std::string object, std::string pos, std::string color, bool is_king); //if is_king append to king_mvoves
    

        bool legal_move(std::string move, bool checking);
        bool legal_move_rook(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string move_type, bool color, bool checking);
        bool legal_move_bishop(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string color, bool checking);
        bool legal_move_knight(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num);
        bool legal_move_king(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num);
        bool legal_moves_queen(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, bool color, bool checking);

        void promotion(std::string arr_pos, std::string arr_num, std::string color);
        bool en_passant(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string move_type, std::vector<std::vector<std::string>> en_passant, std::string color, bool checking);
        void move_object(std::string object);
        void swap_object(std::string move, std::string color);

        void display_board();
        void display_game_over(std::string color);
};
unsigned int chess_object::moves = 0;

std::string chess_object::where_king(std::string object){
    for(int arr_num = 1; arr_num < 9; arr_num++){
        for(int arr_pos = 1; arr_pos < 9; arr_pos++){if(board[arr_num][arr_pos] == object){return return_board_position(arr_pos)+std::to_string(arr_num);}}
    }
}

bool chess_object::is_white(std::string object){
    std::string white_objects [6] = {" ♙ "," ♘ "," ♕ "," ♔ "," ♗ "," ♖ "};
    for(int i = 0; i < 6; i++){if(white_objects[i] == object){return true;}}
    return false;
}
bool chess_object::is_black(std::string object){
    std::string black_objects [6] = {" ♟ "," ♞ "," ♛ "," ♚ "," ♝ "," ♜ "};
    for(int i = 0; i < 6; i++){if(black_objects[i] == object){return true;}}
    return false;
}

void chess_object::move_prediction(std::string object, std::string pos, std::string color, bool is_king){
    //reset the values
    white_king_moves.empty();
    black_king_moves.empty();
    legal_possible_moves_black.empty();
    legal_possible_moves_white.empty();
    
    for(unsigned int next_num = 1; next_num < 9; next_num++){
        for(unsigned int next_pos = 1; next_pos < 9; next_pos++){
            std::string object_next_p = board[next_num][next_pos]; 
            std::string str_next_p = return_board_position(next_pos) + std::to_string(next_num); //board position
            std::string move = (pos) + str_next_p;
            bool already_in = false;

            if(chess_object::legal_move(move, true) && color == "white" && is_king){ //predicts every moves for the king and append it
                for(unsigned int i = 0; i < white_king_moves.size(); i++){
                    if(str_next_p == white_king_moves[i]){already_in = true;}
                }
                if(!already_in){
                    //std::cout << "KING (WHITE)" << " --> " <<  str_next_p << " (" << move << ")\n";
                    white_king_moves.push_back(str_next_p);
                }
            }
            else if(chess_object::legal_move(move, true) && color == "black" && is_king){
                for(unsigned int i = 0; i < black_king_moves.size(); i++){
                    if(str_next_p == black_king_moves[i]){already_in = true;}
                }
                if(!already_in){
                    //std::cout << "KING (BLACK)" << " --> " <<  str_next_p << " (" << move << ")\n";
                    black_king_moves.push_back(str_next_p);
                }
            }
            else if(chess_object::legal_move(move, true) && color == "white"){
                //std::cout << object << " " << str_next_p << " (" << move << ")\n";
                legal_possible_moves_white.push_back(str_next_p);
                legal_possible_moves_white.push_back(object);
            }
            else if(chess_object::legal_move(move, true) && color == "black"){
                //std::cout << object << " " << str_next_p << " (" << move << ")\n";
                legal_possible_moves_black.push_back(str_next_p);
                legal_possible_moves_black.push_back(object);
            }
        }
    }
}

bool chess_object::check(){
    std::string king_position;
    //predict possible moves for every piece
    for(unsigned int arr_num = 1; arr_num < 9; arr_num++){
        for(unsigned int arr_pos = 1; arr_pos < 9; arr_pos++){
            std::string object = board[arr_num][arr_pos];
            std::string pos = return_board_position(arr_pos) + std::to_string(arr_num); //position of object
            if(is_black(object) && object == " ♚ "){move_prediction(object, pos, "black", true);}
            else if(is_white(object) && object == " ♔ "){move_prediction(object, pos, "white", true);}
            else if(is_black(object) && object != " ♚ "){move_prediction(object, pos, "black", false);}
            else if(is_white(object) && object != " ♔ "){move_prediction(object, pos, "white", false);}
        }
    }
    if(chess_object::return_white_rounds()){
        king_position = where_king(" ♔ ");
        for(unsigned int p = 0; p < legal_possible_moves_black.size(); p++){
            if(king_position == legal_possible_moves_black[p]){
                chess_object::is_check = true;
                return true;
            }
        }
    }
    else if(chess_object::return_black_rounds()){
        king_position = where_king(" ♚ ");
        for(unsigned int p = 0; p < legal_possible_moves_white.size(); p++){
            if(king_position == legal_possible_moves_white[p]){
                chess_object::is_check = true;
                return true;
            }
        }
    }
    return false;
}

// SETTING VALUES
void chess_object::set_black(){
    black_rounds = true;
    white_rounds = false;
}
void chess_object::set_white(){
    white_rounds = true;
    black_rounds = false;
}
void chess_object::set_values(std::string previous_move, bool white_rounds, bool black_rounds){
    this-> previous_move = previous_move;
    this-> white_rounds = white_rounds;
    this-> black_rounds = black_rounds;
}

// DISPLAY BOARD / GAME OVER SCREEN
void chess_object::display_game_over(std::string color){
    std::string temp;
    if(color == "white"){output("\nWHITES WON !\n");}
    else if(color == "black"){output("\nBLACKS WON !\n");}
    std::cin >> temp;
}
void chess_object::display_board(){
    system("clear");
    for(unsigned int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){std::cout << board[i][j];}
        std::cout << " | " << "\n" << std::endl;
    }

    std::cout << "WHITE (" << std::to_string(number_white) << ") [";
    for(unsigned int i = 0; i < blacks_captured.size(); i++){std::cout << blacks_captured[i] << " ";}
    std::cout << "]" << std::endl;

    std::cout << "BLACK (" << std::to_string(number_black) << ") [";
    for(unsigned int i = 0; i < whites_captured.size(); i++){std::cout << whites_captured[i] << " ";}
    std::cout << "]" << std::endl;

    std::cout << "MOVES: " << chess_object::moves << "\n " << std::endl;
}

// CONVERTS BOARD POSITION TO ARRAY POSITION#
std::string chess_object::return_board_position(int pos){
    std::map<int, std::string> chess_map;
    chess_map[1] = "A";
    chess_map[2] = "B";
    chess_map[3] = "C";
    chess_map[4] = "D";
    chess_map[5] = "E";
    chess_map[6] = "F";
    chess_map[7] = "G";
    chess_map[8] = "H";
    
    return chess_map[pos];
}
int chess_object::return_position(std::string c){
    std::map<std::string, unsigned int> chess_map;
    chess_map["A"] = 1;
    chess_map["B"] = 2;
    chess_map["C"] = 3;
    chess_map["D"] = 4;
    chess_map["E"] = 5;
    chess_map["F"] = 6;
    chess_map["G"] = 7;
    chess_map["H"] = 8;
    
    return chess_map[c];
}

// MOVES
bool chess_object::en_passant(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string move_type, std::vector<std::vector<std::string>> en_passant, std::string color, bool checking){
    
    if(color == "white"){
        if(move_type == "right"){
            for(int i = 0; i < en_passant.size(); i++){
                if(en_passant[i][0] == return_board_position(return_position(arr_pos)+1) + arr_num && checking){return true;}
                else if(en_passant[i][0] == return_board_position(return_position(arr_pos)+1) + arr_num && board[stoi(arr_num)][return_position(arr_pos)] == " ♟ "){
                    blacks_captured.push_back(board[stoi(arr_num)][return_position(arr_pos)+1]);
                    number_black--;
                    board[stoi(arr_num)][return_position(arr_pos)+1] = SPACE_GAP;
                    return true;
                }
            }
            return false;
        }
        else if(move_type == "left"){
            for(int i = 0; i < en_passant.size(); i++){
                if(en_passant[i][0] == return_board_position(return_position(arr_pos)-1) + arr_num && checking){return true;}
                else if(en_passant[i][0] == return_board_position(return_position(arr_pos)-1) + arr_num && board[stoi(arr_num)][return_position(arr_pos)] == " ♟ "){
                    blacks_captured.push_back(board[stoi(arr_num)][return_position(arr_pos)-1]);
                    number_black--;
                    board[stoi(arr_num)][return_position(arr_pos)-1] = SPACE_GAP;
                    return true;
                }
            }
            return false;
        }
    }
    else if(color == "black"){
        if(move_type == "right"){
            for(int i = 0; i < en_passant.size(); i++){
                if(en_passant[i][0] == return_board_position(return_position(arr_pos)+1) + arr_num && checking){return true;}
                else if(en_passant[i][0] == return_board_position(return_position(arr_pos)+1) + arr_num && board[stoi(arr_num)][return_position(arr_pos)] == " ♙ "){
                    whites_captured.push_back(board[stoi(arr_num)][return_position(arr_pos)+1]);
                    number_white--;
                    board[stoi(arr_num)][return_position(arr_pos)+1] = SPACE_GAP;
                    return true;
                }
            }
            return false;
        }
        else if(move_type == "left"){
            for(int i = 0; i < en_passant.size(); i++){
                if(en_passant[i][0] == return_board_position(return_position(arr_pos)-1) + arr_num && checking){return true;}
                else if(en_passant[i][0] == return_board_position(return_position(arr_pos)-1) + arr_num && board[stoi(arr_num)][return_position(arr_pos)] == " ♙ "){
                    whites_captured.push_back(board[stoi(arr_num)][return_position(arr_pos)-1]);
                    number_white--;
                    board[stoi(arr_num)][return_position(arr_pos)-1] = SPACE_GAP;
                    return true;
                }
            }
            return false;
        }
    }
    
}
void chess_object::promotion(std::string arr_pos, std::string arr_num, std::string color){
    system("clear");
    std::string input;
   
    if(color == "white"){
        std::cout << "Select a new piece:\n" << "   Q for ♕\n" << "   K for ♘\n" << "   R for ♖\n" << "   B for ♗\n" << "> ";
        std::cin >> input;
        if(input == "Q"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♕ ";
        }
        else if(input == "K"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♘ ";
        }
        else if(input == "R"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♖ ";
        }
        else if(input == "B"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♗ ";
        }
        else{chess_object::promotion(arr_pos, arr_num, color);}
    }
    else if(color == "black"){
        std::cout << "Select a new piece:\n" << "   Q for ♛\n" << "   K for ♞\n" << "   R for ♜\n" << "   B for ♝\n" << "> ";
        std::cin >> input;
        if(input == "Q"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♛ ";
        }
        else if(input == "K"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♞ ";
        }
        else if(input == "R"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♜ ";
        }
        else if(input == "B"){
            board[stoi(arr_num)][return_position(arr_pos)] = " ♝ ";
        }
        else{chess_object::promotion(arr_pos, arr_num, color);}
    }
    
}

bool chess_object::legal_move_king(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num){
    if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)){return true;}
    else if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-1 && stoi(next_num) == stoi(arr_num)){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)+1 && stoi(next_num) == stoi(arr_num)){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-1 && stoi(next_num) == stoi(arr_num)-1){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-1 && stoi(next_num) == stoi(arr_num)+1){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)+1 && stoi(next_num) == stoi(arr_num)-1){return true;}
    else{return false;}
}
bool chess_object::legal_moves_queen(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, bool color, bool checking){
    //rook
    if(checking){
        if(next_pos == arr_pos && stoi(next_num) > stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_1", color, true)){return true;}
        else if(next_pos == arr_pos && stoi(next_num) < stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_2", color, true)){return true;}
        else if(next_num == arr_num && return_position(next_pos) > return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_3", color, true)){return true;}
        else if(next_num == arr_num && return_position(next_pos) < return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_4", color, true)){return true;}
        if(color){
            if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "white", true)){return true;}
            else{return false;}
        }
        else if(color == false){
            if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "black", true)){return true;}
            else{return false;}
        }

    }
    if(next_pos == arr_pos && stoi(next_num) > stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_1", color, false)){return true;}
    else if(next_pos == arr_pos && stoi(next_num) < stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_2", color, false)){return true;}
    else if(next_num == arr_num && return_position(next_pos) > return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_3", color, false)){return true;}
    else if(next_num == arr_num && return_position(next_pos) < return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_4", color, false)){return true;}

    //bishop
    if(color){
        if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "white", false)){return true;}
        else{return false;}
    }
    else if(color == false){
        if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "black", false)){return true;}
        else{return false;}
    }
    
}
bool chess_object::legal_move_knight(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num){
    if(return_position(next_pos) == return_position(arr_pos)-2 && stoi(next_num) == stoi(arr_num)+1){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-1 && stoi(next_num) == stoi(arr_num)+2){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-1 && stoi(next_num) == stoi(arr_num)-2){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)-2 && stoi(next_num) == stoi(arr_num)-1){return true;}

    else if(return_position(next_pos) == return_position(arr_pos)+2 && stoi(next_num) == stoi(arr_num)+1){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)+1 && stoi(next_num) == stoi(arr_num)+2){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)+1 && stoi(next_num) == stoi(arr_num)-2){return true;}
    else if(return_position(next_pos) == return_position(arr_pos)+2 && stoi(next_num) == stoi(arr_num)-1){return true;}

    else{return false;}
}

bool chess_object::legal_move_bishop(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string color, bool checking){
    int int_arr_pos = return_position(arr_pos);
    int int_arr_num = stoi(arr_num);
    int int_next_pos = return_position(next_pos);
    int int_next_num = stoi(next_num);
    std::string target = board[int_next_num][int_next_pos];

    if(color == "black" && return_position(next_pos) < return_position(arr_pos) && stoi(next_num) < stoi(arr_num)){
        int_arr_pos--;
        int_arr_num--;
        while(int_arr_pos >= int_next_pos && int_arr_num >= int_next_num){
            if(board[int_arr_num][int_arr_pos] == target && is_white(target) && checking){return true;}
            else if(board[int_arr_num][int_arr_pos] == target && is_white(target)){  // if opposite
                whites_captured.push_back(target);  // add the object to the captured stack
                number_white--;
                return true;
            } 
            else if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            int_arr_pos--;
            int_arr_num--;
        }
        return true;
    }
    else if(color == "black" && return_position(next_pos) > return_position(arr_pos) && stoi(next_num) < stoi(arr_num)){
        int_arr_pos++;
        int_arr_num--;
        while(int_arr_pos <= int_next_pos && int_arr_num >= int_next_num){
            if(board[int_arr_num][int_arr_pos] == target && is_white(target) && checking){return true;}
            else if(board[int_arr_num][int_arr_pos] == target && is_white(target)){
                whites_captured.push_back(target);
                number_white--;
                return true;
            }
            else if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            int_arr_pos++;
            int_arr_num--;
        }
        return true;
    }
    else if(color == "white" && return_position(next_pos) < return_position(arr_pos) && stoi(next_num) > stoi(arr_num)){
        int_arr_pos--;
        int_arr_num++;
        while(int_arr_pos >= int_next_pos && int_arr_num <= int_next_num){
            if(board[int_arr_num][int_arr_pos] == target && is_black(target) && checking){return true;}
            else if(board[int_arr_num][int_arr_pos] == target && is_black(target)){
                blacks_captured.push_back(target);
                number_black--;
                return true;
            }
            else if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            int_arr_pos--;
            int_arr_num++;
        }
        return true;
    }
    else if(color == "white" && return_position(next_pos) > return_position(arr_pos) && stoi(next_num) > stoi(arr_num)){
        int_arr_pos++;
        int_arr_num++;
        while(int_arr_pos <= int_next_pos && int_arr_num <= int_next_num){
            if(board[int_arr_num][int_arr_pos] == target && is_black(target) && checking){return true;}
            else if(board[int_arr_num][int_arr_pos] == target && is_black(target)){
                blacks_captured.push_back(target);
                number_black--;
                return true;
            }
            else if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            int_arr_pos++;
            int_arr_num++;
        }
        return true;
    }
    else{return false;}
}

bool chess_object::legal_move_rook(std::string next_pos, std::string arr_pos, std::string next_num, std::string arr_num, std::string move_type, bool color, bool checking){
    int int_arr_pos = return_position(arr_pos);
    int int_arr_num = stoi(arr_num);
    int int_next_pos = return_position(next_pos);
    int int_next_num = stoi(next_num);
    std::string target = board[int_next_num][int_next_pos];

    if(move_type == "move_1"){
        int_arr_num++;
        while(int_arr_num < int_next_num){
            if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            else{int_arr_num++;}
        }
        if(target == SPACE_GAP){return true;}
        else if(color == true && is_black(target) && checking){return true;}
        else if(color == true && is_black(target)){
            //blacks_captured.push_back(target);
            //number_black--;
            return true;
        }
        else if(color == false && is_white(target) && checking){return true;}
        else if(color == false && is_white(target)){
            //whites_captured.push_back(target);
            //number_white--;
            return true;
        }
        else{return false;}
    }
    if(move_type == "move_2"){
        int_arr_num--;
        while(int_arr_num > int_next_num){
            if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            else{int_arr_num--;}
        }
        if(target == SPACE_GAP){return true;}
        else if(color == true && is_black(target) && checking){return true;}
        else if(color == true && is_black(target)){
            //blacks_captured.push_back(target);
            //number_black--;
            return true;
        }
        else if(color == false && is_white(target) && checking){return true;}
        else if(color == false && is_white(target)){
            //whites_captured.push_back(target);
            //number_white--;
            return true;
        }
        else{return false;}
    }
    if(move_type == "move_3"){
        int_arr_pos++;
        while(int_arr_pos < int_next_pos){
            if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            else{int_arr_pos++;}
        }
        if(target == SPACE_GAP){return true;}
        if(color == true && is_black(target) && checking){return true;}
        else if(color == true && is_black(target)){
            //blacks_captured.push_back(target);
            //number_black--;
            return true;
        
        }
        if(color == false && is_white(target) && checking){return true;}
        else if(color == false && is_white(target)){
            whites_captured.push_back(target);
            number_white--;
            return true;
        }
        else{return false;}
    }
    if(move_type == "move_4"){
        int_arr_pos--;
        while(int_arr_pos > int_next_pos){
            if(board[int_arr_num][int_arr_pos] != SPACE_GAP){return false;}
            else{int_arr_pos--;}
        }
        if(target == SPACE_GAP){return true;}
        else if(color == true && is_black(target) && checking){return true;}
        else if(color == true && is_black(target)){
            //blacks_captured.push_back(target);
            //number_black--;
            return true;
        }
        else if(color == false && is_white(target) && checking){return true;}
        else if(color == false && is_white(target)){
            //whites_captured.push_back(target);
            //number_white--;
            return true;
        }
        else{return false;}
    }
    else{return false;}
}

void chess_object::move_object(std::string object){
    std::string arr_pos = object.substr(0,1);
    std::string arr_num = object.substr(1,1);

    std::string next_pos = object.substr(2,1);
    std::string next_num = object.substr(3,1);
    
    if(board[stoi(next_num)][return_position(next_pos)] == SPACE_GAP){
        previous_move = board[stoi(arr_num)][return_position(arr_pos)] + " --> " + next_pos + next_num;
        total_moves.push_back(board[stoi(arr_num)][return_position(arr_pos)] + " --> " + next_pos + next_num); 
    }
    else{
        previous_move = board[stoi(arr_num)][return_position(arr_pos)] + " --> " + board[stoi(next_num)][return_position(next_pos)];
        total_moves.push_back(board[stoi(arr_num)][return_position(arr_pos)] + " --> " + board[stoi(next_num)][return_position(next_pos)]);
    }
    
    // increment moves counter
    chess_object::moves++;
    // swaping both objects to create a move
    std::string temp = board[stoi(arr_num)][return_position(arr_pos)];  
    board[stoi(arr_num)][return_position(arr_pos)] = SPACE_GAP;
    board[stoi(next_num)][return_position(next_pos)] = temp;

    if(chess_object::return_black_rounds()){
        chess_object::set_white();
    }
    else if(chess_object::return_white_rounds()){
        chess_object::set_black();
    }
}

void chess_object::swap_object(std::string move, std::string color){
    std::string arr_pos = move.substr(0,1);
    std::string arr_num = move.substr(1,1);
    std::string next_pos = move.substr(2,1);
    std::string next_num = move.substr(3,1);

    if(board[stoi(next_num)][return_position(next_pos)] != SPACE_GAP){
        if(color == "white"){
            blacks_captured.push_back(board[stoi(next_num)][return_position(next_pos)]);
        }
        else if(color == "black"){
            whites_captured.push_back(board[stoi(next_num)][return_position(next_pos)]);
        }
    }

    std::string temp = board[stoi(arr_num)][return_position(arr_pos)];  
    board[stoi(arr_num)][return_position(arr_pos)] = SPACE_GAP;
    board[stoi(next_num)][return_position(next_pos)] = temp;
    moves++;
    if(color == "white"){
        chess_object::set_black();
    }
    else if(color == "black"){
        chess_object::set_white();
    }
}

bool chess_object::legal_move(std::string move, bool checking){
    std::string arr_pos = move.substr(0,1);
    std::string arr_num = move.substr(1,1);
    std::string next_pos = move.substr(2,1);
    std::string next_num = move.substr(3,1);

    const std::string TARGET = board[stoi(next_num)][return_position(next_pos)];

    std::string object = board[stoi(arr_num)][return_position(arr_pos)];
    std::string white_objects [6] = {" ♙ "," ♘ "," ♕ "," ♔ "," ♗ "," ♖ "};
    bool is_white = chess_object::is_white(object);
    bool is_black = chess_object::is_black(object);
    std::string pos = (arr_pos) + arr_num;
    std::string n_pos = (next_pos) + next_num;

    //DEFINING THE RULES OF CHESS

    std::string default_pawn_white [8] = {"A2","B2","C2","D2","E2","F2","G2","H2"};
    std::string default_pawn_black [8] = {"A7","B7","C7","D7","E7","F7","G7","H7"};
    // promotion position
    std::string promotion_white [8] = {"A8","B8","C8","D8","E8","F8","G8","H8"};
    std::string promotion_black [8] = {"A1","B1","C1","D1","E1","F1","G1","H1"};

    static std::vector<std::vector<std::string>> en_passant;
    for(unsigned int a = 0; a < en_passant.size(); a++){
        if(stoi(en_passant[a][1]) != moves){en_passant.erase(en_passant.begin() + a);} // delete from the vector if next move did not capture en passant
    }

    // search if pawn is default
    bool is_default = false;
    if(is_white){for(int i = 0; i < 8; i++){if(pos == default_pawn_white[i]){is_default = true;}}}
    else if(is_black){for(int i = 0; i < 8; i++){if(pos == default_pawn_black[i]){is_default = true;}}}

    if(chess_object::is_check){
        //move the piece, if chess_object::check is true THEN return false
        //else return true
        std::string temp = board[stoi(arr_num)][return_position(arr_pos)];  
        board[stoi(arr_num)][return_position(arr_pos)] = SPACE_GAP;
        board[stoi(next_num)][return_position(next_pos)] = temp;
        //updating the values
        chess_object::is_check = false;
        legal_possible_moves_black.clear();
        legal_possible_moves_white.clear();
        for(unsigned int arr_num = 1; arr_num < 9; arr_num++){
            for(unsigned int arr_pos = 1; arr_pos < 9; arr_pos++){
                std::string object = board[arr_num][arr_pos];
                std::string pos = return_board_position(arr_pos) + std::to_string(arr_num); //position of object
                if(chess_object::is_black(object) && object == " ♚ "){move_prediction(object, pos, "black", true);}
                else if(chess_object::is_white(object) && object == " ♔ "){move_prediction(object, pos, "white", true);}
                else if(chess_object::is_black(object) && object != " ♚ "){move_prediction(object, pos, "black", false);}
                else if(chess_object::is_white(object) && object != " ♔ "){move_prediction(object, pos, "white", false);}
            }
        }
        chess_object::is_check = true;
        if(return_white_rounds()){
            for(unsigned int p = 0; p < legal_possible_moves_black.size(); p++){
                if(legal_possible_moves_black[p] == where_king(" ♔ ")){
                    temp = board[stoi(next_num)][return_position(next_pos)];
                    board[stoi(next_num)][return_position(next_pos)] = SPACE_GAP;
                    board[stoi(arr_num)][return_position(arr_pos)] = temp;
                    return false;
                }
            }
        }
        else if(return_black_rounds()){
            for(unsigned int p = 0; p < legal_possible_moves_white.size(); p++){
                if(legal_possible_moves_white[p] == where_king(" ♚ ")){
                    temp = board[stoi(next_num)][return_position(next_pos)];
                    board[stoi(next_num)][return_position(next_pos)] = SPACE_GAP;
                    board[stoi(arr_num)][return_position(arr_pos)] = temp;
                    return false;
                }
            }
        }
        temp = board[stoi(next_num)][return_position(next_pos)];
        board[stoi(next_num)][return_position(next_pos)] = SPACE_GAP;
        board[stoi(arr_num)][return_position(arr_pos)] = temp;
        chess_object::is_check = false;
        return true;
        
    }

    else if(is_white && chess_object::return_white_rounds() && TARGET != " ♚ " || is_black && chess_object::return_black_rounds() && TARGET != " ♔ " || checking){
        // making sure the move obeys the chess rules
        if(object == " ♙ "){
            //B7C6 - B7A6
            if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)+1 && chess_object::is_black(TARGET) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)+1 && chess_object::is_black(TARGET)){
                //blacks_captured.push_back(TARGET);
                //number_black--;
                return true;
            }
            if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)-1 && chess_object::is_black(TARGET) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)-1 && chess_object::is_black(TARGET)){
                //blacks_captured.push_back(TARGET);
                //number_black--;
                return true;
            }
            //checking en passant
            else if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)-1 && TARGET == SPACE_GAP){
                return chess_object::en_passant(next_pos, arr_pos, next_num, arr_num, "left", en_passant, "white", false);
            }
            else if(stoi(next_num) == stoi(arr_num)+1 && return_position(next_pos) == return_position(arr_pos)+1 && TARGET == SPACE_GAP){
                return chess_object::en_passant(next_pos, arr_pos, next_num, arr_num, "right", en_passant, "white", false);
            }
            if(stoi(next_num) == stoi(arr_num)+1 && TARGET == SPACE_GAP && return_position(next_pos) == return_position(arr_pos) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)+1 && TARGET == SPACE_GAP && return_position(next_pos) == return_position(arr_pos)){
                for(int i = 0; i < 8; i++){ //check if promotion
                    if(n_pos == promotion_white[i]){
                        chess_object::promotion(arr_pos, arr_num, "white");
                    }
                }
               
                return true;
            }
            if(stoi(next_num) == stoi(arr_num)+2 && TARGET == SPACE_GAP && is_default && return_position(next_pos) == return_position(arr_pos) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)+2 && TARGET == SPACE_GAP && is_default && return_position(next_pos) == return_position(arr_pos)){
                std::vector<std::string> temp;
                temp.push_back(n_pos);
                temp.push_back(std::to_string(moves+1));
                
                en_passant.push_back(temp);
                return true;
            }
            else{return false;}
        }
        else if(object == " ♟ "){
            if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)+1 && chess_object::is_white(TARGET) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)+1 && chess_object::is_white(TARGET)){
                //whites_captured.push_back(board[stoi(next_num)][return_position(next_pos)]);
                //number_white--;
                
                return true;
            }
            if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)-1 && chess_object::is_white(TARGET) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)-1 && chess_object::is_white(TARGET)){
                //whites_captured.push_back(board[stoi(next_num)][return_position(next_pos)]);
                //number_white--;
                
                return true;
            }
            else if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)-1 && TARGET == SPACE_GAP){
                return chess_object::en_passant(next_pos, arr_pos, next_num, arr_num, "left", en_passant, "black", false);
            }
            else if(stoi(next_num) == stoi(arr_num)-1 && return_position(next_pos) == return_position(arr_pos)+1 && TARGET == SPACE_GAP){
                return chess_object::en_passant(next_pos, arr_pos, next_num, arr_num, "right", en_passant, "black", false);
            }
            if(stoi(next_num) == stoi(arr_num)-1 && TARGET == SPACE_GAP && return_position(next_pos) == return_position(arr_pos) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)-1 && TARGET == SPACE_GAP && return_position(next_pos) == return_position(arr_pos)){
                for(int i = 0; i < 8; i++){
                    if(n_pos == promotion_black[i]){chess_object::promotion(arr_pos, arr_num, "black");}
                }
                return true;
            }
            if(stoi(next_num) == stoi(arr_num)-2 && TARGET == SPACE_GAP && is_default && return_position(next_pos) == return_position(arr_pos) && checking){return true;}
            else if(stoi(next_num) == stoi(arr_num)-2 && TARGET == SPACE_GAP && is_default && return_position(next_pos) == return_position(arr_pos)){
                std::vector<std::string> temp;
                temp.push_back(n_pos);
                temp.push_back(std::to_string(moves+1));
                en_passant.push_back(temp);
                return true;
            }
            else{return false;}
        }
        if(object == " ♖ " || object == " ♜ "){
            if(next_pos == arr_pos && stoi(next_num) > stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_1", is_white, true) && checking){return true;} // checking
            else if(next_pos == arr_pos && stoi(next_num) > stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_1", is_white, false)){return true;} // vertical
            else if(next_pos == arr_pos && stoi(next_num) < stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_2", is_white, true) && checking){return true;}
            else if(next_pos == arr_pos && stoi(next_num) < stoi(arr_num) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_2", is_white, false)){return true;}

            else if(next_num == arr_num && return_position(next_pos) > return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_3", is_white, true) && checking){return true;}
            else if(next_num == arr_num && return_position(next_pos) > return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_3", is_white, false)){return true;} // horizontal
            else if(next_num == arr_num && return_position(next_pos) < return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_4", is_white, true) && checking){return true;} 
            else if(next_num == arr_num && return_position(next_pos) < return_position(arr_pos) && legal_move_rook(next_pos, arr_pos, next_num, arr_num, "move_4", is_white, false)){return true;} 
            else{return false;}
        }
        if(object == " ♗ "){
            if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "white", true) && checking){return true;}
            else if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "white", false)){return true;}
            else{return false;}
        }
        else if(object == " ♝ "){
            if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "black", true) && checking){return true;}
            else if(abs(return_position(arr_pos)-return_position(next_pos)) == abs(stoi(arr_num)-stoi(next_num)) && legal_move_bishop(next_pos, arr_pos, next_num, arr_num, "black", false)){return true;}
            else{return false;}
        }
        if(object == " ♞ "){
            if(TARGET == SPACE_GAP && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num)){return true;}
            else if(chess_object::is_white(TARGET) && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num) && checking){return true;}
            else if(chess_object::is_white(TARGET) && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num)){
                //whites_captured.push_back(TARGET);
                //number_white--;
                return true;
            }
            else{return false;}
        }
        else if(object == " ♘ "){
            if(TARGET == SPACE_GAP && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num)){return true;}
            else if(chess_object::is_black(TARGET) && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num) && checking){return true;}
            else if(chess_object::is_black(TARGET) && chess_object::legal_move_knight(next_pos, arr_pos, next_num, arr_num)){
                //blacks_captured.push_back(TARGET);
                //number_black--;
                return true;
            }
            else{return false;}
        }
        if(object == " ♕ "){
            //chess_object::is_black(board[stoi(next_num)][return_position(next_pos)])
            if(TARGET == SPACE_GAP && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, true, false) && checking == false){return true;}
            else if(TARGET == SPACE_GAP && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, true, true) && checking){return true;}
            else if(chess_object::is_black(TARGET) && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, true, true) && checking){return true;}
            else if(chess_object::is_black(TARGET) && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, true, false) && checking == false){
                //nothing to do here 'legal_move_rook' captures the target
                return true;
            }
            else{return false;}
        }
        else if(object == " ♛ "){
            if(TARGET == SPACE_GAP && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, false, false) && checking == false){return true;}
            else if(TARGET == SPACE_GAP && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, false, true) && checking){return true;}
            else if(chess_object::is_white(TARGET) && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, false, true) && checking){return true;}
            else if(chess_object::is_white(TARGET) && chess_object::legal_moves_queen(next_pos, arr_pos, next_num, arr_num, false, false) && checking == false){
                //nothing to do here 'legal_move_rook' captures the target
                return true;
            }
            else{return false;}
        }
        if(object == " ♔ "){
            if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && TARGET == SPACE_GAP){

                for(unsigned int i = 0; i < legal_possible_moves_black.size(); i++){
                    if(legal_possible_moves_black[i] == n_pos && legal_possible_moves_black[i+1] != " ♟ "){return false;}
                }
                return true;
            }
            else if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && chess_object::is_black(TARGET) && TARGET != " ♚ " && checking){return true;}
            else if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && chess_object::is_black(TARGET) && TARGET != " ♚ "){
                std::string temp = board[stoi(arr_num)][return_position(arr_pos)];  
                board[stoi(arr_num)][return_position(arr_pos)] = SPACE_GAP;
                board[stoi(next_num)][return_position(next_pos)] = temp;
                chess_object::check();
                for(int i = 0; i < legal_possible_moves_black.size(); i++){
                    if(legal_possible_moves_black[i] == n_pos){
                        //swap back and return false
                        std::string temp = board[stoi(next_num)][return_position(next_pos)];
                        board[stoi(next_num)][return_position(next_pos)] = SPACE_GAP;
                        board[stoi(arr_num)][return_position(arr_pos)] = temp;
                        return false;
                    }
                }
                //blacks_captured.push_back(TARGET);
                //number_black--;
                return true;
            }
            else{return false;}
        }
        else if(object == " ♚ "){
            if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && TARGET == SPACE_GAP){
                for(unsigned int i = 0; i < legal_possible_moves_white.size(); i++){
                    if(legal_possible_moves_white[i] == n_pos && legal_possible_moves_white[i+1] != " ♙ "){return false;}
                }
                return true;
            }
            
            
            else if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && chess_object::is_white(TARGET) && TARGET != " ♔ " && checking){return true;}
            else if(chess_object::legal_move_king(next_pos, arr_pos, next_num, arr_num) && chess_object::is_white(TARGET) && TARGET != " ♔ "){
                std::string temp = board[stoi(arr_num)][return_position(arr_pos)];  
                board[stoi(arr_num)][return_position(arr_pos)] = SPACE_GAP;
                board[stoi(next_num)][return_position(next_pos)] = temp;
                chess_object::check();
                for(int i = 0; i < legal_possible_moves_white.size(); i++){
                    if(legal_possible_moves_white[i] == n_pos){
                        //swap back and return false
                        std::string temp = board[stoi(next_num)][return_position(next_pos)];
                        board[stoi(next_num)][return_position(next_pos)] = SPACE_GAP;
                        board[stoi(arr_num)][return_position(arr_pos)] = temp;
                        return false;
                    }
                }
                //whites_captured.push_back(TARGET);
                //number_white--;
                return true;
            }
            else{return false;} 
        }
    }
    else{return false;}
}
#endif