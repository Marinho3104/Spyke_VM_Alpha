#include "convertor.h"

#include "convertor_ast.h"
#include "linked_list.h"
#include "ast_nodes.h"
#include "byte_code.h"

#include <iostream>




parser::Convertor::~Convertor() { delete blocks; }

parser::Convertor::Convertor(Code_Information* __code_informaiton, utils::Linked_List <Token*>* __implicit_value, Ast_Node_Name_Space* __global_name_space)
    : code_information(__code_informaiton), implicit_value(__implicit_value), global_name_space(__global_name_space), current_block(0)
        { blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>(); generate_byte_code(); }

void parser::Convertor::print(const char* __information) { if (0) return; std::cout << "\t" << __information << "\n" << std::endl; }

void parser::Convertor::generate_byte_code() { set_block(global_name_space->declarations); }

void parser::Convertor::set_block(utils::Linked_List <Ast_Node*>* __code, int __position) {

    // std::cout << "Set Block" << std::endl;

    byte_code::Byte_Code_Block* _previous_block = current_block;

    current_block = blocks->operator[](__position);

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code;

    for (int _ = 0; _ < __code->count; _++) {

        _byte_code = get_byte_code_of_ast(this, __code->operator[](_));

        current_block->block->join(
            _byte_code
        );

        delete _byte_code;

    }

    current_block = _previous_block;

}

void parser::Convertor::set_block(utils::Linked_List <Ast_Node*>* __code) {

    // std::cout << "Set Block" << std::endl;

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code;

    add_block();

    byte_code::Byte_Code_Block* _previous_block = current_block;

    current_block = blocks->last->object;

    for (int _ = 0; _ < __code->count; _++) {

        _byte_code = get_byte_code_of_ast(this, __code->operator[](_));

        current_block->block->join(
            _byte_code
        );

        delete _byte_code;

    }  

    current_block = _previous_block;

}

int parser::Convertor::add_block() { 
    
    // std::cout << "Add Block" << std::endl;

    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_byte_code_block) byte_code::Byte_Code_Block();
    
    return blocks->add(_byte_code_block); 
    
}

byte_code::Compiled_Byte_Code* parser::Convertor::get_compiled_byte_code() {

    return 
        new byte_code::Compiled_Byte_Code(
            blocks
        );

}


