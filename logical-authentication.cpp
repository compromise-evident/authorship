/*Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.

Version 10.0.0
Get a secure number only you can modify, which anyone can
keep verifying, and you can insert personal messages (data
authentication independent of encryption type & symmetry).*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
int main()
{	std::ifstream in_stream;
	std::ofstream out_stream;
	
	std::string SHA_512_input;
	std::string SHA_512_output;
	
	std::cout << "\n(1) Create   Your number which others save."
	          << "\n(2) Modify   Creates a disposable public file with which others verify."
	          << "\n(3) Verify   Their new public file modifies their number file which you saved."
	          << "\n\nOption: ";
	std::string o; std::getline(std::cin, o); if((o != "1") && (o != "2") && (o != "3")) {std::cout << "\nBad option.\n"; return 0;}
	
	//Create.
	//WZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMW
	if(o == "1")
	{	//Exits if files already exists.
		if(std::filesystem::exists("Personal")) {std::cout << "\n\"Personal\" folder already exists.\n"; return 0;}
		
		//Generates 512 keys. From rolling-code.cpp.
		{	//Gets path.
			std::cout << "\nJust once, drop/enter any file, preferably one with many random bytes:\n";
			std::string path; std::getline(std::cin, path); if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
			if(!std::filesystem::exists(path)) {std::cout << "\nNo path " << path << "\n"; return 1;}
			std::filesystem::create_directories("Personal/private");
			
			//Loads seed.
			in_stream.open(path); if(!in_stream) {std::cout << "\nCan't open file for reading. (Loads seed).\n"; return 1;}
			std::string seed = ""; for(char file_byte; in_stream.get(file_byte);) {seed.push_back(file_byte);}
			in_stream.close();
			
			//Generates pseudorandomness.
			out_stream.open("Personal/private/512_keys"); if(!out_stream) {std::cout << "\nCan't open file for writing. (Generates pseudorandomness).\n"; return 1;}
			SHA_512_input = seed;
			for(unsigned long long a = 0; a < 512000; a++)
			{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
				unsigned long long pseudorandom_byte = std::stoull(SHA_512_output.substr(0, 16), 0, 16);
				out_stream.put((pseudorandom_byte % 94) + 33); if((a + 1) % 1000 == 0) {out_stream << "\n";}
				SHA_512_input = SHA_512_output;
			}
			out_stream.close();
			
			//Updates seed file.
			out_stream.open("Personal/private/rolling-seed"); if(!out_stream) {std::cout << "\nCan't open file for writing. (Updates seed file).\n"; return 1;}
			out_stream << SHA_512_output;
			out_stream.close();
		}
		
		//Generates 512 ciphertext.
		in_stream.open ("Personal/private/512_keys"      ); if( !in_stream) {std::cout << "\nCan't open file for reading. (Generates 512 ciphertext).\n"; return 1;}
		out_stream.open("Personal/private/512_ciphertext"); if(!out_stream) {std::cout << "\nCan't open file for writing. (Generates 512 ciphertext).\n"; return 1;}
		for(; std::getline(in_stream, SHA_512_input);)
		{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			out_stream << SHA_512_output;
		}
		in_stream.close();
		out_stream.close();
		
		//Creates the number file.
		in_stream.open ("Personal/private/512_ciphertext"); if( !in_stream) {std::cout << "\nCan't open file for reading. (Creates the number file).\n"; return 1;}
		out_stream.open("Personal/number"                ); if(!out_stream) {std::cout << "\nCan't open file for writing. (Creates the number file).\n"; return 1;}
		if(std::getline(in_stream, SHA_512_input))
		{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			out_stream << SHA_512_output;
		}
		in_stream.close();
		out_stream.close();
		
		std::cout << "\nYour \"Personal\" folder is ready. Publish your number.\n";
	}
	
	//Modify.
	//WZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMW
	if(o == "2")
	{	//Gets path.
		std::cout << "\nDrop/enter Personal folder:\n";
		std::string path; std::getline(std::cin, path); if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		if(!std::filesystem::exists(path)) {std::cout << "\nNo path " << path << "\n"; return 1;}
		
		//Gets personal message.
		std::cout << "\nEnter a personal message of any length, else press enter:\n";
		std::string message; std::getline(std::cin, message);
		
		//Checks message.
		if(!message.empty())
		{	for(unsigned long long a = 0; a < message.length(); a++)
			{	if((message[a] < 32) || (message[a] > 126)) {std::cout << "\nMessage corrupted.\n"; return 0;}
			}
		}
		
		//Creates paths.
		std::string path_to_keys                    = (path + "/private/512_keys"               );
		std::string path_to_ciphertext              = (path + "/private/512_ciphertext"         );
		std::string path_to_rolling_seed            = (path + "/private/rolling-seed"           );
		std::string path_to_public                  = (path + "/public"                         );
		std::string path_to_number                  = (path + "/number"                         );
		std::string path_to_representing_SHA_512_of = (path + "/private/representing_SHA-512_of");
		std::string path_to_the_SHA_512             = (path + "/private/the_SHA-512"            );
		
		//Copies ciphertext to public file.
		std::filesystem::remove_all(path_to_public);
		std::filesystem::copy_file(path_to_ciphertext, path_to_public);
		
		//Loads old keys to RAM.
		in_stream.open(path_to_keys); if(!in_stream) {std::cout << "\nCan't open file for reading. (Loads old keys to RAM).\n"; return 1;}
		std::string old_keys; for(std::string file_line; std::getline(in_stream, file_line);) {old_keys += file_line;}
		in_stream.close();
		
		//Generates 512 keys. From rolling-code.cpp.
		{	//Loads seed.
			in_stream.open(path_to_rolling_seed); if(!in_stream) {std::cout << "\nCan't open file for reading. (Loads seed).\n"; return 1;}
			std::string seed = ""; for(char file_byte; in_stream.get(file_byte);) {seed.push_back(file_byte);}
			in_stream.close();
			
			//Generates pseudorandomness.
			out_stream.open(path_to_keys); if(!out_stream) {std::cout << "\nCan't open file for writing. (Generates 512 keys).\n"; return 1;}
			SHA_512_input = seed;
			for(unsigned long long a = 0; a < 512000; a++)
			{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
				unsigned long long pseudorandom_byte = std::stoull(SHA_512_output.substr(0, 16), 0, 16);
				out_stream.put((pseudorandom_byte % 94) + 33); if((a + 1) % 1000 == 0) {out_stream << "\n";}
				SHA_512_input = SHA_512_output;
			}
			out_stream.close();
			
			//Updates seed file.
			out_stream.open(path_to_rolling_seed); if(!out_stream) {std::cout << "\nCan't open file for writing. (Updates seed file).\n"; return 1;}
			out_stream << SHA_512_output;
			out_stream.close();
		}
		
		//Generates 512 ciphertext.
		in_stream.open (path_to_keys      ); if( !in_stream) {std::cout << "\nCan't open file for reading. (Generates 512 ciphertext).\n"; return 1;}
		out_stream.open(path_to_ciphertext); if(!out_stream) {std::cout << "\nCan't open file for writing. (Generates 512 ciphertext).\n"; return 1;}
		for(; std::getline(in_stream, SHA_512_input);)
		{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			out_stream << SHA_512_output;
		}
		in_stream.close();
		out_stream.close();
		
		//Creates the number file.
		in_stream.open (path_to_ciphertext); if( !in_stream) {std::cout << "\nCan't open file for reading. (Creates the number file).\n"; return 1;}
		out_stream.open(path_to_number    ); if(!out_stream) {std::cout << "\nCan't open file for writing. (Creates the number file).\n"; return 1;}
		if(std::getline(in_stream, SHA_512_input))
		{	{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			out_stream << SHA_512_output;
		}
		in_stream.close();
		out_stream.close();
		
		//Writes message and new number to file "representing_SHA-512_of".
		in_stream.open (path_to_number                 ); if( !in_stream) {std::cout << "\nCan't open file for reading. (Appends message and new number).\n"; return 1;}
		out_stream.open(path_to_representing_SHA_512_of); if(!out_stream) {std::cout << "\nCan't open file for writing. (Appends message and new number).\n"; return 1;}
		if(!message.empty()) {out_stream << message;} out_stream << "\n";
		std::string file_line; if(std::getline(in_stream, file_line)) {out_stream << file_line << "\n";}
		in_stream.close();
		out_stream.close();
		
		//Keeps appending 0s to file "representing_SHA-512_of" until the file's SHA-512 hash is composed of 256 1 bits and 256 0 bits.
		std::string bits;
		for(;;)
		{	//Appends 0.
			out_stream.open(path_to_representing_SHA_512_of, std::ios::app); if(!out_stream) {std::cout << "\nCan't open file for writing. (Keeps appending).\n"; return 1;}
			out_stream << "0";
			out_stream.close();
			
			//Takes a hash.
			in_stream.open (path_to_representing_SHA_512_of); if( !in_stream) {std::cout << "\nCan't open file for reading. (Takes a hash).\n"; return 1;}
			out_stream.open(path_to_the_SHA_512            ); if(!out_stream) {std::cout << "\nCan't open file for writing. (Takes a hash).\n"; return 1;}
			SHA_512_input = ""; for(char file_byte; in_stream.get(file_byte);) {SHA_512_input.push_back(file_byte);}
			{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			out_stream << SHA_512_output;
			in_stream.close();
			out_stream.close();
			
			//Checks if hash is evenly bit-distributed.
			bits = "";
			for(int a = 0; a < 128; a++)
			{	if     (SHA_512_output[a] == '0') {bits += "0000";}
				else if(SHA_512_output[a] == '1') {bits += "0001";}
				else if(SHA_512_output[a] == '2') {bits += "0010";}
				else if(SHA_512_output[a] == '3') {bits += "0011";}
				else if(SHA_512_output[a] == '4') {bits += "0100";}
				else if(SHA_512_output[a] == '5') {bits += "0101";}
				else if(SHA_512_output[a] == '6') {bits += "0110";}
				else if(SHA_512_output[a] == '7') {bits += "0111";}
				else if(SHA_512_output[a] == '8') {bits += "1000";}
				else if(SHA_512_output[a] == '9') {bits += "1001";}
				else if(SHA_512_output[a] == 'a') {bits += "1010";}
				else if(SHA_512_output[a] == 'b') {bits += "1011";}
				else if(SHA_512_output[a] == 'c') {bits += "1100";}
				else if(SHA_512_output[a] == 'd') {bits += "1101";}
				else if(SHA_512_output[a] == 'e') {bits += "1110";}
				else if(SHA_512_output[a] == 'f') {bits += "1111";}
				else                              {std::cout << "\nError 1\n"; return 0;}
			}
			
			int zeros = 0;
			for(int a = 0; a < 512; a++) {if(bits[a] == '0') {zeros++;}}
			if(zeros == 256) {break;}
		}
		
		//Appends 256 old keys to public file (based on bits from hash in file "the_SHA-512").
		out_stream.open(path_to_public, std::ios::app); if(!out_stream) {std::cout << "\nCan't open file for writing. (Appends 256 old keys).\n"; return 1;}
		out_stream << "\n";
		int old_keys_read_bookmark = 0;
		for(int a = 0; a < 512; a++)
		{	if(bits[a] == '1')
			{	for(int b = 0; b < 1000; b++) {out_stream << old_keys[old_keys_read_bookmark]; old_keys_read_bookmark++;}
			}
			else {old_keys_read_bookmark += 1000;}
		}
		out_stream.close();
		
		//Appends presence & absence indication  to public file.
		//Appends file "representing_SHA-512_of" to public file.
		in_stream.open (path_to_representing_SHA_512_of); if( !in_stream) {std::cout << "\nCan't open file for reading. (Appends).\n"; return 1;}
		out_stream.open(path_to_public, std::ios::app  ); if(!out_stream) {std::cout << "\nCan't open file for writing. (Appends).\n"; return 1;}
		out_stream << "\n" << bits << "\n";
		for(char file_byte; in_stream.get(file_byte);) {out_stream.put(file_byte);}
		in_stream.close();
		out_stream.close();
		
		std::cout << "\nModified! (Your \"number\" file was overwritten with a new number)."
		          << "\nPublish the \"public\" file so they can update your number.\n";
	}
	
	//Verify.
	//WZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMWZMW
	if(o == "3")
	{	//Gets path.
		std::cout << "\nDrop/enter their disposable public file:\n";
		std::string path; std::getline(std::cin, path); if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		if(!std::filesystem::exists(path)) {std::cout << "\nNo path " << path << "\n"; return 1;}
		
		std::string path_to_public = path;
		
		//Gets path.
		std::cout << "\nDrop/enter their number file:\n";
		std::getline(std::cin, path); if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		if(!std::filesystem::exists(path)) {std::cout << "\nNo path " << path << "\n"; return 1;}
		
		std::string path_to_number = path;
		
		//Loads public file to RAM.
		in_stream.open(path_to_public); if(!in_stream) {std::cout << "\nCan't open file for reading. (Loads public file to RAM).\n"; return 1;}
		std::string ciphertext      ;   std::getline(in_stream, ciphertext      );
		std::string keys            ;   std::getline(in_stream, keys            );
		std::string presence_absence;   std::getline(in_stream, presence_absence);
		std::string message         ;   std::getline(in_stream, message         );
		std::string number          ;   std::getline(in_stream, number          );
		std::string zeros           ;   std::getline(in_stream, zeros           );
		in_stream.close();
		
		//Checks for substance.
		if((ciphertext.length()       !=  65536)
		|| (keys.length()             != 256000)
		|| (presence_absence.length() !=    512)
		|| (number.length()           !=    128)
		|| (zeros.empty()                      )) {std::cout << "\nBad \"public\" file.\n"; return 0;}
		
		//Checks message.
		if(!message.empty())
		{	for(unsigned long long a = 0; a < message.length(); a++)
			{	if((message[a] < 32) || (message[a] > 126)) {std::cout << "\nMessage corrupted.\n"; return 0;}
			}
		}
		
		//Checks presence_absence.
		int occur_0 = 0;
		int occur_1 = 0;
		for(int a = 0; a < 512; a++)
		{	if     (presence_absence[a] == '0') {occur_0++;}
			else if(presence_absence[a] == '1') {occur_1++;}
		}
		if((occur_0 != 256) || (occur_1 != 256)) {std::cout << "\nPresence & absence indication must have 256 zeros and 256 ones.\n"; return 0;}
		
		//Takes hash of ciphertext.
		SHA_512_input = ciphertext;
		{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		
		//Compares hash to the number in the "number" file.
		in_stream.open(path_to_number); if(!in_stream) {std::cout << "\nCan't open file for reading. (Compares hash to the number).\n"; return 1;}
		std::string file_line; std::getline(in_stream, file_line);
		in_stream.close();
		if(SHA_512_output != file_line) {std::cout << "\nFAILED! That number is not the hash of the ciphertext. Old public file?\n"; return 0;}
		
		//Takes hash of user message & number.
		std::string last_3_lines;
		last_3_lines += message; last_3_lines += '\n';
		last_3_lines += number ; last_3_lines += '\n';
		last_3_lines += zeros  ;
		
		SHA_512_input = last_3_lines;
		{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		std::string hash_of_last_3_lines = SHA_512_output;
		
		//Decrypts ciphertext to which keys are present.
		int ciphertext_read_bookmark = 0;
		int       keys_read_bookmark = 0;
		for(int a = 0; a < 512; a++)
		{	if(presence_absence[a] == '1')
			{	SHA_512_input = ""; for(int b = 0; b < 1000; b++) {SHA_512_input += keys[keys_read_bookmark]; keys_read_bookmark++;}
				{std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} SHA_512_output = ""; const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
				
				//Checks it.
				for(int b = 0; b < 128; b++)
				{	if(SHA_512_output[b] != ciphertext[ciphertext_read_bookmark]) {std::cout << "\nFAILED! Incorrect keys.\n"; return 0;}
					ciphertext_read_bookmark++;
				}
			}
			else {ciphertext_read_bookmark += 128;}
		}
		
		//Compares presence & absence indication to (the bits of) the hash of user message & number.
		std::string bits_of_hash_of_last_3_lines;
		for(int a = 0; a < 128; a++)
		{	if     (hash_of_last_3_lines[a] == '0') {bits_of_hash_of_last_3_lines += "0000";}
			else if(hash_of_last_3_lines[a] == '1') {bits_of_hash_of_last_3_lines += "0001";}
			else if(hash_of_last_3_lines[a] == '2') {bits_of_hash_of_last_3_lines += "0010";}
			else if(hash_of_last_3_lines[a] == '3') {bits_of_hash_of_last_3_lines += "0011";}
			else if(hash_of_last_3_lines[a] == '4') {bits_of_hash_of_last_3_lines += "0100";}
			else if(hash_of_last_3_lines[a] == '5') {bits_of_hash_of_last_3_lines += "0101";}
			else if(hash_of_last_3_lines[a] == '6') {bits_of_hash_of_last_3_lines += "0110";}
			else if(hash_of_last_3_lines[a] == '7') {bits_of_hash_of_last_3_lines += "0111";}
			else if(hash_of_last_3_lines[a] == '8') {bits_of_hash_of_last_3_lines += "1000";}
			else if(hash_of_last_3_lines[a] == '9') {bits_of_hash_of_last_3_lines += "1001";}
			else if(hash_of_last_3_lines[a] == 'a') {bits_of_hash_of_last_3_lines += "1010";}
			else if(hash_of_last_3_lines[a] == 'b') {bits_of_hash_of_last_3_lines += "1011";}
			else if(hash_of_last_3_lines[a] == 'c') {bits_of_hash_of_last_3_lines += "1100";}
			else if(hash_of_last_3_lines[a] == 'd') {bits_of_hash_of_last_3_lines += "1101";}
			else if(hash_of_last_3_lines[a] == 'e') {bits_of_hash_of_last_3_lines += "1110";}
			else if(hash_of_last_3_lines[a] == 'f') {bits_of_hash_of_last_3_lines += "1111";}
			else                                    {std::cout << "\nError 2\n"; return 0;  }
		}
		
		if(presence_absence != bits_of_hash_of_last_3_lines) {std::cout << "\nFAILED! Hash of \"message + new number\" does not match key presence & absence.\n"; return 0;}
		std::cout << "\nVerified!";
		
		//Updates the number file with the new number in the public file.
		out_stream.open(path_to_number); if(!out_stream) {std::cout << "\nCan't open file for writing. (Updates the number file).\n"; return 1;}
		out_stream << number;
		out_stream.close();
		
		//Appends message to the number file.
		if(!message.empty())
		{	out_stream.open(path_to_number, std::ios::app); if(!out_stream) {std::cout << "\nCan't open file for writing. (Appends message).\n"; return 1;}
			out_stream << "\n\n" << message << "\n";
			out_stream.close();
			std::cout << "\nThe \"number\" file was updated. And their message was appended to it.\n";
		}
		else {std::cout << "\nThe \"number\" file was updated. And they left no message.\n";}
	}
}
