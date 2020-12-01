#include"help.h"
using namespace std;
void flip_bit(uint8_t *ai_data, int i_size, int i_num_error, vector<int> &pos){
	srand((int)time(0));
	for(int i =0;i<i_num_error;++i){
		int i_index = rand()%i_size;
		int i_bit = rand()%8;
		ai_data[i_index] ^= 1<<i_bit;
		pos.push_back(i_index);
	}
}
int binary_read(string s_filepath, char* s_buff, int i_buff_size, int opt){
	ifstream ifs(s_filepath, ios::in|ios::binary);
	while(ifs.read(s_buff,sizeof(char)*i_buff_size)){
	}
	int readedBytes = ifs.gcount();
	uint8_t *readByte = (uint8_t*) s_buff;
	if(opt==1){
		cout<<s_filepath<<":"<<endl;
		for(int i = 0;i<readedBytes;++i){
			cout<<hex<<(unsigned) readByte[i]<<" ";
		}
	}
	cout<<endl;
	ifs.close();
	return readedBytes;
}

void binary_write(string s_filepath,uint8_t *ar_data,int i_size){
	ofstream ofs(s_filepath,ios::out|ios::binary);
	for(int i=0;i<i_size;++i){
		ofs.write((char*)ar_data+i,sizeof(uint8_t));
	}
	ofs.close();
}
