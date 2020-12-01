#include<boost/lexical_cast.hpp>
#include<boost/archive/binary_oarchive.hpp>
#include<iostream>
#include<fstream>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<climits>
#include<boost/crc.hpp>
#include"help.h"
#define NUM_CHUNK 10
using namespace std;
int main(int argc,char* argv[]){
	using boost::lexical_cast;
	string s_filename = "./golden_file";
	int i_size, i_num_error;
	char s_buff[15000]; 
	vector<int> changed_data;
	int readedBytes=0;
	uint8_t *ai_gold;
	int i_debug_mode =0;
	if (argc<3||argc>4) {
		cout<<"Input param <size> <num_error>"<<endl;
		exit(-1);
	}else{
		try{

			i_size = lexical_cast<int>(argv[1]);
			i_num_error = lexical_cast<int>(argv[2]);
			cout<<"size\t"<<i_size<<"#error\t"<<i_num_error<<endl;
			if(argc==4){
				i_debug_mode = lexical_cast<int>(argv[3]);
			}
		}	
		catch(boost::bad_lexical_cast ){
			cout<<" throw exception"<<endl;
			exit(-1);
		}
	}

	/**
	 * This part is used for generating golden data,
	 * and writting these data to a binary file with
	 * serialization.
	 */

	
	// generate golden data
	ai_gold = new uint8_t [i_size];	
	srand((int)time(0));
	for(int i =0;i<i_size;i++){
		ai_gold[i] = (uint8_t)rand()%256;
		//cout<<hex<<unsigned(ai_gold[i])<<" ";
	}
	cout<<endl;

	// write golden data into file, serialized for future use.
	binary_write("./golden_file",ai_gold,i_size);
	//binary_read("./golden_file",s_buff,1500);
	/**
	 * This part is used to simulate bit flip
	 */

	flip_bit(ai_gold,i_size,i_num_error,changed_data);
	binary_write("./changed_file",ai_gold,i_size);
	//binary_read("./changed_file",s_buff,1500);
	
	cout<<"flip data position :";
	for(auto &a: changed_data){
		cout<<dec<< a<<" ";
	}
	cout<<endl;

	readedBytes = binary_read("./golden_file",s_buff,15000,i_debug_mode);
	/**
	 * This part is used for generating CRC
	 */
	int i_chunk_size = i_size / NUM_CHUNK;
	cout<<i_chunk_size<<endl;
	cout<<"changed chunk index:";
	for(auto &a: changed_data){
		cout<<dec<< a/i_chunk_size<<" ";
	}
	cout<<endl;	
	uint32_t crc[10];
	ai_gold = (uint8_t*)s_buff;
	for(int i=0;i<NUM_CHUNK;++i){
		boost::crc_32_type crc_32;
		crc_32 = for_each(ai_gold+(i*i_chunk_size),ai_gold+(i*i_chunk_size+i_chunk_size-1),crc_32);
		crc[i] = crc_32();
		cout<<hex<<crc[i]<<endl;
	}
	ofstream ofs("./crc.dat",ios::out|ios::binary);
	ofs.write((char*)crc,sizeof(crc));
	ofs.close();
	//binary_read("./crc.dat",s_buff,1500,1);

	/**
	 * This part is used to generate FEC chunk
	 */

	uint8_t fec[i_chunk_size];
	for(int i=0;i<i_size;++i){
		int index = i%i_chunk_size;
		fec[index] ^=ai_gold[i]; 
	}
	binary_write("./fec.dat",fec,i_chunk_size);

	/**
	 * simulate decompress
	 */

	readedBytes = binary_read("./changed_file",s_buff,15000,i_debug_mode);
	uint8_t *changed = (uint8_t*) s_buff;
	uint32_t p_crc[10];
	for(int i =0;i<NUM_CHUNK;++i){
		boost::crc_32_type crc_32;
		crc_32 = for_each(changed+(i*i_chunk_size),changed+(i*i_chunk_size+i_chunk_size-1),crc_32);
		p_crc[i] = crc_32();
		cout<<hex<<p_crc[i]<<endl;
		if(p_crc[i]!=crc[i]){
			cout<< "Error detected in chunk "<< i<<endl;
		}
	} 

	char *imgbuff = new char[3000000];
	changed_data.clear();
	readedBytes = binary_read("./1.png",imgbuff,3000000);
	uint8_t *img_gold = (uint8_t*)imgbuff;
	binary_write("./img/img_golden.dat",img_gold,readedBytes-1);
	
	i_chunk_size = readedBytes / NUM_CHUNK;

	cout << "img crc" <<endl;
	for(int i=0;i<NUM_CHUNK;++i){
		boost::crc_32_type crc_32;
		crc_32 = for_each(img_gold+(i*i_chunk_size),img_gold+(i*i_chunk_size+i_chunk_size-1),crc_32);
		crc[i] = crc_32();
		cout<<hex<<crc[i]<<endl;
	}

	flip_bit(img_gold,readedBytes,i_num_error,changed_data);
	cout<<"flip data position :";
	for(auto &a: changed_data){
		cout<<dec<< a<<" ";
	}
	cout<<endl;
	cout<<"changed chunk index:";
	for(auto &a: changed_data){
		cout<<dec<< a/i_chunk_size<<" ";
	}
	cout<<endl;

	for(int i =0;i<NUM_CHUNK;++i){
		boost::crc_32_type crc_32;
		crc_32 = for_each(img_gold+(i*i_chunk_size),img_gold+(i*i_chunk_size+i_chunk_size-1),crc_32);
		p_crc[i] = crc_32();
		cout<<hex<<p_crc[i]<<endl;
		if(p_crc[i]!=crc[i]){
			cout<< "Error detected in chunk "<< i<<endl;
		}
	} 
	uint8_t *img_fec = new uint8_t[i_chunk_size];
	for(int i=0;i<readedBytes;++i){
		int index = i%i_chunk_size;
		img_fec[index] ^=img_gold[i]; 
	}
	binary_write("./img/img.dat",fec,NUM_CHUNK);
	ofs.open("./img/crc.dat",ios::out|ios::binary);
	ofs.write((char*)crc,sizeof(crc));
	ofs.close();
	
	delete[] img_fec;
	delete[] imgbuff;
	//delete[] ai_gold;

	return 0;
}
