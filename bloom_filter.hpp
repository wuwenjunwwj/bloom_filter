#include <iostream>
#include <stdint.h>
#include <openssl/md5.h>
#include "pthread.hpp"
typedef void bitmap_t;
bitmap_t* bitmap_create(uint64_t count);
void bitmap_destroy(bitmap_t* bitmap_);
void bitmap_clr(bitmap_t* bitmap_);
inline static int bitmap_get (bitmap_t* bitmap_, uint64_t bit){
	int byte = ((char*)bitmap_)[bit >> 3]; 
	return byte & 1 << (bit & 0x07) ? 1 : 0;
	}
inline static int bitmap_set(bitmap_t* bitmap_, uint64_t bit){
	char*  byte = ((char*)bitmap_)+ (bit >> 3);
	*byte |= 1<< (bit & 0x07);
	return (*byte) & 1 << (bit & 0x07) ? 1 : 0;
	}
inline static int bitmap_clr(bitmap_t *bitmap_, uint64_t bit){
	unsigned char *p = (unsigned char *)bitmap_ + (bit >> 3);
	unsigned char byte = *p;
	*p &= ~(1 << (bit & 0x07));
	return byte & 1 << (bit & 0x07) ? 1 : 0;
	}
inline  static int hash_url_by_md5(const std::string& url, int number) {
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	MD5_Update(&md5_ctx, (unsigned char*)url.data(), url.length());
	unsigned long md5[16/sizeof(unsigned long)];
	MD5_Final((unsigned char*)md5, &md5_ctx);
	return md5[0] % number;
}


class bloom_filter{
	public:
	  bloom_filter(uint64_t  count){
		  this->count = count;
		  this->bitmap_ =  bitmap_create(count);		
		  };
	  ~bloom_filter(){
		  if(bitmap_) bitmap_destroy(bitmap_);
		  }
	  bool bloom_check(uint64_t bit);
	  int bloom_insert(uint64_t bit);
	  int bloom_insert(const std::string& data);
	  int bloom_clr(uint64_t bit);
	  void bloom_reset();
	private:
	    int get_bits(const std::string& data);
		bitmap_t* bitmap_;
		uint64_t  count;
		mutable pthread_rwlock m_lock;
	};

