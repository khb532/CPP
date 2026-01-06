#pragma GCC optimize "Os"
#pragma GCC target "arch=haswell"

#include <unistd.h>
#include <cstdint>
#include <type_traits>
#include <limits>
#include <utility>
#include <immintrin.h>

#define may(expr)	 __builtin_expect(!!(expr),1)
#define maynot(expr) __builtin_expect(!!(expr),0)
#define memcpy(dst,src,len)	 __builtin_memcpy(dst,src,len)


namespace {	// IO

inline constexpr uint32_t BUF_SIZE = 1u<<6;
inline constexpr uint32_t OBUF_SIZE= 1u<<6;

alignas(64) uint8_t
	buf[BUF_SIZE],
	obuf[OBUF_SIZE],
	nbuf[32];
uint8_t
	*ptr,	*end,
	*optr,	*oend,
	*nptr,	*nend;

inline void initIO(){
	ptr	=buf;	end	=buf+BUF_SIZE;
	optr=obuf;	oend=obuf+OBUF_SIZE;
	nptr=nend=nbuf+31;
	read(0,buf,BUF_SIZE);
}
inline void refill(){
	ssize_t left = end - ptr;
	memcpy(buf,ptr,left);
	read(0,buf+left,BUF_SIZE-left);
	ptr = buf;
}
inline void flush(){
	write(1,obuf,optr-obuf);
	optr=obuf;
}
inline void checkRd(uint32_t n){if(maynot((end - ptr) < n))refill();}
inline void checkWr(uint32_t n){if(maynot((oend- optr)< n)) flush();}

template <typename _8>
inline void putChar(_8 c){
	checkWr(2);
	*optr++ = c;
}
inline void writeStr(const char* str, ssize_t len){
	checkWr(len);
	memcpy(optr,str,len);
	optr+=len;
}

template <typename T>
inline void readInt(T &v){
	checkRd(32);
	v=0;
	bool neg = false;
	if constexpr(std::is_signed_v<T>){
		if(maynot(*ptr=='-')){
			++ptr;
			neg = true;
	}}
	while(*ptr & 0x10)
		v = v * 10 + (*ptr++ & 0x0F);
	++ptr;
	if constexpr(std::is_signed_v<T>){if(neg) v =- v;}
}

template <typename T>
inline void writeInt(T v){
	checkWr(32);
	if constexpr(std::is_signed_v<T>){
		if(maynot(v<0)){
			putChar('-');
			v=-v;
	}}
	nptr=nend;
	do{
		*--nptr = (v % 10) | '0';
	}while(v/=10);
	memcpy(optr,nptr,nend-nptr);
	optr += nend-nptr;
}

template <typename float_t>
inline void readFloatFast(float_t& v){
	checkRd(32);
	v=0;
	uint32_t n=0, e=1;
	bool neg = false;
	if(maynot(*ptr=='-')){
		++ptr;
		neg = true;
	}
	while(*ptr & 0x10)
		n = n * 10 + (*ptr++ & 0x0F);
	if(*ptr != '.'){
		++ptr;
		v = static_cast<float_t>(n);
		return;
	} ++ptr;
	while(*ptr & 0x10){
		n = n * 10 + (*ptr++ & 0x0F);
		e *= 10;
	} ++ptr;
	v = (static_cast<float_t>(n)) / e;
	return;
}

constexpr uint64_t _writeFloatFast_fracScale(float)	{return 1'000'000'00ull;}
constexpr uint64_t _writeFloatFast_fracScale(double){return 1'000'000'000'000'000'0ull;}
template <typename float_t>
inline void writeFloatFast(float_t v){
	checkWr(32);
	if(maynot(v<0)){
		putChar('-');
		v=-v;
	}
	uint64_t i = static_cast<uint64_t>(v);
	writeInt(i);
	putChar('.');
	writeInt(static_cast<uint64_t>((v - i) * _writeFloatFast_fracScale(v)));
	while(*(optr-1)=='0')--optr;
	if(maynot(*(optr-1)=='.'))--optr;
}

namespace{	// junkyard

template <typename T>
inline T _getNum(){
	T v{};
	if constexpr (std::is_integral_v<T>) readInt(v);
	else if constexpr (std::is_floating_point_v<T>) readFloatFast(v);
	return v;
}
struct getNum{
	template <typename T>
	operator T()const{return _getNum<T>();}
};
inline getNum next;

template <typename T>
inline void readNumSingle(T &v) requires std::is_integral_v<T> {readInt(v);}
template <typename T>
inline void readNumSingle(T &v) requires std::is_floating_point_v<T> {readFloatFast(v);}
template <typename... Ts>
inline void readNum(Ts&... xs) {
	(readNumSingle(xs), ...);
}

template <typename T>
using bare_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <size_t N>
inline void writeVarSingle(const char (&s)[N]){writeStr(s,N-1);}
inline void writeVarSingle(const char* s){
	const char* p = (char*)(__builtin_memchr(s,'\0',OBUF_SIZE));
	writeStr(s, p - s);
}
inline void writeVarSingle(char c)			{putChar(c);}
inline void writeVarSingle(unsigned char c)	{putChar(c);}
inline void writeVarSingle(signed char c)	{putChar(c);}
template <typename T>
inline void writeVarSingle(T&& v)
	requires	(std::is_integral_v	<bare_t<T>> &&
				!std::is_same_v		<bare_t<T>, char> &&
				!std::is_same_v		<bare_t<T>, signed char> &&
				!std::is_same_v		<bare_t<T>, unsigned char>){
	writeInt(static_cast<bare_t<T>>(v));
}
template <typename T>
inline void writeVarSingle(T&& v)
	requires std::is_floating_point_v<bare_t<T>>{
		writeFloatFast(static_cast<bare_t<T>>(v));
}

template <typename... Ts>
inline void writeVar(Ts&&... xs){
	(writeVarSingle(std::forward<Ts>(xs)) , ...);
}

}
}	// end io

extern "C"
int __libc_start_main(){
	initIO();
	
	int32_t a,b;
	readNum(a,b);
	writeVar(
		(a+b),"\n",(a-b),"\n",(a*b),"\n",(a/b),"\n",(a%b)
	);

	flush();
	_exit(0);
}

#ifdef _WIN64
#include <cstdio>
int main(){
	freopen("input.txt","r",stdin);
	__libc_start_main();
}
#else
int main(){}
#endif