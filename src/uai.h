#ifndef UAI_H
#define UAI_H 1

#include "types.h"
#include "position.h"

#include <string>
#include <sstream>

class Uai
{
    Position current_pos;
    void handle_uai();
    void handle_uainewgame();
    void handle_startpos(std::stringstream& reader);
    void handle_moves(std::stringstream& reader);
    void handle_fen(std::stringstream& reader);
    void handle_position(std::stringstream& reader);
    void handle_input(const std::string& command);

public:
    void run();
};

#endif // !UAI_H
