#include "convertor.h"

#include "linked_list.h"
#include "ast_nodes.h"
#include "byte_code.h"

#include <iostream>




parser::Convertor::~Convertor() { delete blocks; }

parser::Convertor::Convertor(Code_Information* __code_informaiton, utils::Linked_List <Token*>* __implicit_value, Ast_Node_Name_Space* __global_name_space)
    : code_information(__code_informaiton), implicit_value(__implicit_value), global_name_space(__global_name_space) 
        { blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>(); generate_byte_code(); }

void parser::Convertor::generate_byte_code() { set_block(global_name_space->declarations); }

void parser::Convertor::set_block(utils::Linked_List <Ast_Node*>* __code, int __position) {

}

void parser::Convertor::set_block(utils::Linked_List <Ast_Node*>* __code) {

    add_block();

    for (int _ = 0; _ < __code->count; _++) 

        std::cout << "Id -> " << __code->operator[](_)->node_type << std::endl;

}

int parser::Convertor::add_block() { 
    
    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_byte_code_block) byte_code::Byte_Code_Block();
    
    return blocks->add(_byte_code_block); 
    
}


