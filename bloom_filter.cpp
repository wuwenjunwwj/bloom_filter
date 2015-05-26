#include "bloom_filter.hpp"
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
using namespace std;
bitmap_t* bitmap_create(uint64_t count){
     int bytes = (count +7 ) >> 3;//
     bitmap_t* c =(bitmap_t*)calloc(bytes, 0); 
     return c;
    }   
void bitmap_destroy(bitmap_t* bitmap_){
    free(bitmap_);
  	}   

bool bloom_filter::bloom_check(uint64_t bit){
	if(bitmap_get(bitmap_, bit)==1) return true;
	return false;
	}
int bloom_filter::get_bits(const std::string& data){
	int index = hash_url_by_md5(data, count);
	return index;
	}
int bloom_filter::bloom_insert(uint64_t bit){
	if(bloom_check(bit)){
		std::cout<<"already exist"<<endl;
		return 1;
		}
	bitmap_set(bitmap_,bit);
	std::cout<<"not exist"<<endl;
	return 0;
	}
int bloom_filter::bloom_insert(const std::string& data){
	int index = get_bits(data);
	bloom_insert(index);
	}
int bloom_filter::bloom_clr(uint64_t bit){
	bitmap_clr(bitmap_,bit);
	}
void bloom_filter::bloom_reset(){
	if(bitmap_){
		 bitmap_destroy(bitmap_);
	    this->bitmap_ =  bitmap_create(count);
	 }
	}
int main(){
	bloom_filter* bloom_ = new bloom_filter(32);
	const std::string string1 = "http://www.baidu.com";
	bloom_ -> bloom_insert(string1);
	bloom_ -> bloom_insert(string1);
	const std::string string2 = "http://www.sogou.com/aaaaaaaa";
	const std::string string3 = "http://www.sogou.com/httttpbbbbbbbb";
	bloom_ -> bloom_insert(string2);
	bloom_ -> bloom_insert(string3);
	delete bloom_;
	//bloom_destroy(bloom_);
	}
