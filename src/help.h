#ifndef _HELP_H_
#define _HELP_H_
#include<time.h>
#include<stdlib.h>
#include<stdint.h>
#include<fstream>
#include<iostream>
#include<vector>
void flip_bit(uint8_t *ai_data, int i_size, int i_num_error,std::vector<int>& pos);
int binary_read(std::string s_filepath, char* s_buff, int i_buff_size,int opt = 0); 
void binary_write(std::string s_filepath,uint8_t *ar_data,int i_size);
#endif
