#include<boost/lexical_cast.hpp>
#include<boost/archive/binary_oarchive.hpp>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<climits>
#include<boost/serialization/vector.hpp>
using namespace std;
int main(int argc,char* argv[]){
	using boost::lexical_cast;
	string s_filename = "./golden_file";
	int i_size, i_num_error;
	if (argc!=3) {
		cout<<"Input param <size> <num_error>"<<endl;
		exit(-1);
	}else{
		try{

			i_size = lexical_cast<int>(argv[1]);
			i_num_error = lexical_cast<int>(argv[2]);
			cout<<"size\t"<<i_size<<"#error\t"<<i_num_error<<endl;
			
		}catch(boost::bad_lexical_cast ){
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
	vector<int> ai_gold(i_size);
	srand((int)time(0));
	for(int i =0;i<i_size;i++){
		ai_gold[i] = rand()%INT_MAX;
	}

	// write golden data into file, serialized for future use.
	ofstream ofs(s_filename);
	boost::archive::binary_oarchive oa(ofs);
	oa << ai_gold;
	ofs.close();
		
	
	/**
	 * This part is used for generating CRC
	 */

	return 0;
}
