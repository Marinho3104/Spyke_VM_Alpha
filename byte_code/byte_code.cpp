#include "byte_code.h"

#include "linked_list.h"

#include <iostream>


byte_code::Byte_Code::~Byte_Code() {}

byte_code::Byte_Code::Byte_Code(char __code, short __argument) : code(__code), argument(__argument) {}



byte_code::Byte_Code_Block::~Byte_Code_Block() { delete block; }

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) : block(__block) {}

byte_code::Byte_Code_Block::Byte_Code_Block() { block = new utils::Linked_List <Byte_Code*>(); }

