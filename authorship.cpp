//YOUR CONTROLS:                                                                Run it: "apt install g++ geany libgmp-dev". Open the .cpp in Geany.
int digit_length = 200; //50000 max                                             //Append "-lgmp" to Geany's compile & build commands. Hit F9 once. F5 to run.
//digit_length is the length of a (jumping) value, near which
//prime gaps are found, for randomness. Bigger = more secure.

/*Version 9.0.1
Get a secure number only you can modify, which anyone can
keep verifying, and you can insert personal messages (data
authentication independent of encryption type & symmetry).*/

#include <cstring>
#include <filesystem>
#include <fstream>
#include <gmp.h> //For primes
#include <iostream>
#include <string>
using namespace std;
int main()
{	int raw_byte;
	char file_byte;
	ifstream in_stream;
	ofstream out_stream;
	
	std::string SHA_512_input;
	std::string SHA_512_output;
	
	cout << "\n(1) Create   Your number which others save."
	     << "\n(2) Modify   Creates a disposable public file with which others verify."
	     << "\n(3) Verify   Their new public file modifies their number file which you saved."
	
	     << "\n\nOption: ";
	
	int user_option; cin >> user_option;
	if((user_option != 1) && (user_option != 2) && (user_option != 3)) {cout << "\nInvalid.\n"; return 0;}
	
	//_______________________________________________________Create___________________________________________________//
	if(user_option == 1)
	{	//Checks if files already exists.
		in_stream.open("Personal");
		if(!in_stream.fail()) {cout << "\n\"Personal\" folder already exists. Move it.\n"; in_stream.close(); return 0;}
		in_stream.close();
		
		//Creates seeds file.
		{	//Gets path, fixes it if dropped.
			cout << "Just once, drop/enter any file of " << digit_length << "+ random first bytes:\n";
			string path; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
			if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
			in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
			
			//Checks file size.
			long long size = filesystem::file_size(path);
			if(size < digit_length) {cout << "\nFile too small.\n"; return 1;}
			
			//Copies bytes, writes them modified.
			filesystem::create_directories("Personal/private");
			in_stream.open(path);
			out_stream.open("Personal/private/private_seeds");
			for(int a = 0; a < digit_length; a++)
			{	in_stream.get(file_byte); raw_byte = file_byte;
				if(raw_byte < 0) {raw_byte += 256;} out_stream.put((raw_byte % 10) + 48);
			}
			in_stream.close();
			out_stream.close();
		}
		
		//Loads seeds.
		char seeds[50001] = {'\0'}; cout << "Wait ~10 minutes...\n";
		long long size = filesystem::file_size("Personal/private/private_seeds"); if(size != digit_length) {cout << "\nBad private_seeds.\n"; return 1;} //Checks file size.
		in_stream.open("Personal/private/private_seeds"); for(int a = 0; a < digit_length; a++) {in_stream.get(seeds[a]);} in_stream.close();            //Loads value.
		if(seeds[0] == '0') {seeds[0] = '5';}                                                                                                            //Forces its length.
		mpz_t in, out; mpz_init(in); mpz_init(out); mpz_set_str(in, seeds, 10); mpz_nextprime(out, in); mpz_get_str(seeds, 10, out);                     //Makes it prime.
		mpz_t prime, two; mpz_init(prime); mpz_init(two); mpz_set_str(prime, seeds, 10); mpz_set_ui(two, 2);                                             //Dedicates.
		
		//Generates 512 keys (randomness). Each key is a prime of length digit_length.
		out_stream.open("Personal/private/512_keys");
		for(int a = 0; a < 512; a++)
		{	//Generates randomness (by concatenating consecutive prime gaps).
			string key; unsigned long long wanted_length = digit_length;
			for(long long gap = 2; key.length() < wanted_length;)
			{	mpz_add(prime, prime, two);
				int primality = mpz_probab_prime_p(prime, 25);
				if(primality == false) {gap += 2;}
				else {key += to_string(gap); gap = 2;}
			}
			key.resize(digit_length);
			char tmp[50001] = {'\0'}; strcpy(tmp, key.c_str()); mpz_set_str(in, tmp, 10); mpz_nextprime(out, in); mpz_get_str(tmp, 10, out); key = tmp; //Makes it prime.
			out_stream << key << "\n";
		}
		out_stream.close();
		
		//Updates seeds file.
		out_stream.open("Personal/private/private_seeds");
		for(long long gap = 2, a = 0; a < digit_length;)
		{	mpz_add(prime, prime, two);
			int primality = mpz_probab_prime_p(prime, 25);
			if(primality == false) {gap += 2;}
			else {mpz_get_str(seeds, 10, prime); out_stream << seeds[digit_length - 2]; gap = 2; a++;}
		}
		out_stream.close();
		
		//Generates ciphertext.
		//(Generates 512 encrypted items using the 512 keys. Each item is prime gaps following a key).
		in_stream.open("Personal/private/512_keys");
		out_stream.open("Personal/private/512_ciphertext");
		for(int a = 0; a < 512; a++)
		{	char key[50001] = {'\0'}; string line; getline(in_stream, line); strcpy(key, line.c_str());
			mpz_set_str(prime, key, 10);
			
			//Generates randomness (by concatenating consecutive prime gaps).
			string ciphertext; unsigned long long wanted_length = digit_length;
			for(long long gap = 2; ciphertext.length() < wanted_length;)
			{	mpz_add(prime, prime, two);
				int primality = mpz_probab_prime_p(prime, 25);
				if(primality == false) {gap += 2;}
				else {ciphertext += to_string(gap); gap = 2;}
			}
			ciphertext.resize(digit_length); out_stream << ciphertext;
		}
		in_stream.close();
		out_stream.close();
		
		//Creates the number file (takes a SHA-512 hash of the 512 ciphertext).
		in_stream.open("Personal/private/512_ciphertext"); getline(in_stream, SHA_512_input); in_stream.close();
		{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		
		out_stream.open("Personal/number"); out_stream << SHA_512_output; out_stream.close();
		cout << "\nYour \"Personal\" folder is ready. Publish your number.\n";
	}
	
	//_______________________________________________________Modify___________________________________________________//
	if(user_option == 2)
	{	//Gets path, fixes it if dropped.
		cout << "Drop/enter Personal folder:\n"; string path; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
		if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
		
		//Prompts user for message to be authenticated with event.
		cout << "Enter message to be authenticated (9k char max) else press enter:\n";
		char message[10000] = {'\0'};
		cin.getline(message, 10000);
		message[9000] = '\0';
		bool existence_of_message = false;
		if(message[0] != '\0') {existence_of_message = true;}
		
		//Creates paths.
		string path_to_keys                    = (path + "/private/512_keys"               );
		string path_to_ciphertext              = (path + "/private/512_ciphertext"         );
		string path_to_private_seeds           = (path + "/private/private_seeds"          );
		string path_to_public                  = (path + "/public"                         );
		string path_to_number                  = (path + "/number"                         );
		string path_to_representing_SHA_512_of = (path + "/private/representing_SHA-512_of");
		string path_to_the_SHA_512             = (path + "/private/the_SHA-512"            );
		
		//Copies ciphertext to new file "public".
		string line;
		in_stream.open (path_to_ciphertext);
		out_stream.open(path_to_public    );
		getline(in_stream, line); out_stream << line << "\n";
		in_stream.close();
		out_stream.close();
		
		//Loads old keys to RAM.
		string old_keys;
		in_stream.open(path_to_keys);
		for(int a = 0; a < 512; a++) {getline(in_stream, line); old_keys += line;}
		in_stream.close();
		
		//Same as option 1.
		{	//Loads seeds.
			char seeds[50001] = {'\0'}; cout << "Wait ~10 minutes...\n";
			long long size = filesystem::file_size(path_to_private_seeds); if(size != digit_length) {cout << "\nBad private_seeds.\n"; return 1;} //Checks file size.
			in_stream.open(path_to_private_seeds); for(int a = 0; a < digit_length; a++) {in_stream.get(seeds[a]);} in_stream.close();            //Loads value.
			if(seeds[0] == '0') {seeds[0] = '5';}                                                                                                 //Forces its length.
			mpz_t in, out; mpz_init(in); mpz_init(out); mpz_set_str(in, seeds, 10); mpz_nextprime(out, in); mpz_get_str(seeds, 10, out);          //Makes it prime.
			mpz_t prime, two; mpz_init(prime); mpz_init(two); mpz_set_str(prime, seeds, 10); mpz_set_ui(two, 2);                                  //Dedicates.
			
			//Generates 512 keys (randomness). Each key is a prime of length digit_length.
			out_stream.open(path_to_keys);
			for(int a = 0; a < 512; a++)
			{	//Generates randomness (by concatenating consecutive prime gaps).
				string key; unsigned long long wanted_length = digit_length;
				for(long long gap = 2; key.length() < wanted_length;)
				{	mpz_add(prime, prime, two);
					int primality = mpz_probab_prime_p(prime, 25);
					if(primality == false) {gap += 2;}
					else {key += to_string(gap); gap = 2;}
				}
				key.resize(digit_length);
				char tmp[50001] = {'\0'}; strcpy(tmp, key.c_str()); mpz_set_str(in, tmp, 10); mpz_nextprime(out, in); mpz_get_str(tmp, 10, out); key = tmp; //Makes it prime.
				out_stream << key << "\n";
			}
			out_stream.close();
			
			//Updates seeds file.
			out_stream.open(path_to_private_seeds);
			for(long long gap = 2, a = 0; a < digit_length;)
			{	mpz_add(prime, prime, two);
				int primality = mpz_probab_prime_p(prime, 25);
				if(primality == false) {gap += 2;}
				else {mpz_get_str(seeds, 10, prime); out_stream << seeds[digit_length - 2]; gap = 2; a++;}
			}
			out_stream.close();
			
			//Generates ciphertext.
			//(Generates 512 encrypted items using the 512 keys. Each item is prime gaps following a key).
			in_stream.open(path_to_keys);
			out_stream.open(path_to_ciphertext);
			for(int a = 0; a < 512; a++)
			{	char key[50001] = {'\0'}; string line; getline(in_stream, line); strcpy(key, line.c_str());
				mpz_set_str(prime, key, 10);
				
				//Generates randomness (by concatenating consecutive prime gaps).
				string ciphertext; unsigned long long wanted_length = digit_length;
				for(long long gap = 2; ciphertext.length() < wanted_length;)
				{	mpz_add(prime, prime, two);
					int primality = mpz_probab_prime_p(prime, 25);
					if(primality == false) {gap += 2;}
					else {ciphertext += to_string(gap); gap = 2;}
				}
				ciphertext.resize(digit_length); out_stream << ciphertext;
			}
			in_stream.close();
			out_stream.close();
			
			//Creates the number file (takes a SHA-512 hash of the 512 ciphertext).
			in_stream.open(path_to_ciphertext); getline(in_stream, SHA_512_input); in_stream.close();
			{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			
			out_stream.open(path_to_number); out_stream << SHA_512_output; out_stream.close();
		}
		
		//Writes user message to new file "representing_SHA-512_of".
		out_stream.open(path_to_representing_SHA_512_of);
		if(existence_of_message == true) {out_stream << message;}
		out_stream << "\n";
		out_stream.close();
		
		//Appends new number to file "representing_SHA-512_of".
		in_stream.open(path_to_number);
		out_stream.open(path_to_representing_SHA_512_of, ios::app);
		getline(in_stream, line); out_stream << line << "\n0";
		in_stream.close();
		out_stream.close();
		
		//Keeps appending 0s to file "representing_SHA-512_of" until its SHA-512 hash is composed of 256 1 bits and 256 0 bits.
		for(int a = 0; a < 100000; a++)
		{	//Takes a hash.
			SHA_512_input = "";
			long long size = filesystem::file_size(path_to_representing_SHA_512_of); SHA_512_input.resize(size);
			in_stream.open(path_to_representing_SHA_512_of);
			in_stream.read(&SHA_512_input[0], size);
			in_stream.close();
			
			{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
			
			out_stream.open(path_to_the_SHA_512); out_stream << SHA_512_output; out_stream.close();
			
			//Checks if hash is evenly bit-distributed.
			in_stream.open(path_to_the_SHA_512);
			int zeros_counter = 0;
			in_stream.get(file_byte);
			for(; !in_stream.eof(); in_stream.get(file_byte))
			{	if     (file_byte == '0') {zeros_counter += 4;} //hex 0 consists of 4 "0" bits, and so on.
				else if(file_byte == '1') {zeros_counter += 3;}
				else if(file_byte == '2') {zeros_counter += 3;}
				else if(file_byte == '3') {zeros_counter += 2;}
				else if(file_byte == '4') {zeros_counter += 3;}
				else if(file_byte == '5') {zeros_counter += 2;}
				else if(file_byte == '6') {zeros_counter += 2;}
				else if(file_byte == '7') {zeros_counter += 1;}
				else if(file_byte == '8') {zeros_counter += 3;}
				else if(file_byte == '9') {zeros_counter += 2;}
				else if(file_byte == 'a') {zeros_counter += 2;}
				else if(file_byte == 'b') {zeros_counter += 1;}
				else if(file_byte == 'c') {zeros_counter += 2;}
				else if(file_byte == 'd') {zeros_counter += 1;}
				else if(file_byte == 'e') {zeros_counter += 1;}
				else if(file_byte == 'f') {zeros_counter += 0;}
				else                      {cout << "\n\nError_d1\n\n"; in_stream.close(); return 0;}
			}
			in_stream.close();
			
			if(zeros_counter == 256) {break;}
			out_stream.open(path_to_representing_SHA_512_of, ios::app);
			out_stream << "0";
			out_stream.close();
			
			if(a == 99999) {cout << "\nError_d2\n"; return 0;}
		}
		
		//Appends only 256 old keys to file "public" (based on bits from hash in file "the_SHA-512").
		string bin;
		in_stream.open(path_to_the_SHA_512);
		in_stream.get(file_byte);
		for(; !in_stream.eof(); in_stream.get(file_byte))
		{	if     (file_byte == '0') {bin += "0000";}
			else if(file_byte == '1') {bin += "0001";}
			else if(file_byte == '2') {bin += "0010";}
			else if(file_byte == '3') {bin += "0011";}
			else if(file_byte == '4') {bin += "0100";}
			else if(file_byte == '5') {bin += "0101";}
			else if(file_byte == '6') {bin += "0110";}
			else if(file_byte == '7') {bin += "0111";}
			else if(file_byte == '8') {bin += "1000";}
			else if(file_byte == '9') {bin += "1001";}
			else if(file_byte == 'a') {bin += "1010";}
			else if(file_byte == 'b') {bin += "1011";}
			else if(file_byte == 'c') {bin += "1100";}
			else if(file_byte == 'd') {bin += "1101";}
			else if(file_byte == 'e') {bin += "1110";}
			else if(file_byte == 'f') {bin += "1111";}
			else {cout << "\n\nError_d3\n\n"; in_stream.close(); return 0;}
		}
		in_stream.close();
		
		out_stream.open(path_to_public, ios::app);
		int old_keys_read_bookmark = 0;
		for(int a = 0; a < 512; a++)
		{	if(bin[a] == '1')
			{	for(int b = 0; b < digit_length; b++) {out_stream << old_keys[old_keys_read_bookmark]; old_keys_read_bookmark++;}
			}
			else {old_keys_read_bookmark += digit_length;}
		}
		out_stream << "\n";
		out_stream.close();
		
		//Appends presence & absence indication to file "public".
		out_stream.open(path_to_public, ios::app);
		out_stream << bin << "\n";
		out_stream.close();
		
		//Appends user message and new number to file "public" (all from file "representing_SHA-512_of").
		in_stream.open(path_to_representing_SHA_512_of);
		out_stream.open(path_to_public, ios::app);
		in_stream.get(file_byte);
		for(; !in_stream.eof(); in_stream.get(file_byte)) {out_stream.put(file_byte);}
		in_stream.close();
		out_stream.close();
		
		cout << "\nModified! (Your \"number\" file was overwritten with a new number)."
		     << "\nPublish the \"public\" file so they can update your number.\n";
	}
	
	//_______________________________________________________Verify___________________________________________________//
	if(user_option == 3)
	{	//Gets path, fixes it if dropped.
		cout << "Drop/enter their disposable public file:\n"; string path; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
		if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
		
		string path_to_public = path;
		
		//Gets path, fixes it if dropped.
		cout << "\nDrop/enter their number file:\n"; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
		if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
		
		string path_to_number = path;
		
		//Loads file "public" to RAM.
		in_stream.open(path_to_public);
		string ciphertext      ;   getline(in_stream, ciphertext      );
		string keys            ;   getline(in_stream, keys            );
		string presence_absence;   getline(in_stream, presence_absence);
		string message         ;   getline(in_stream, message         );
		string number          ;   getline(in_stream, number          );
		string zeros           ;   getline(in_stream, zeros           );
		in_stream.close();
		
		//Checks for substance.
		if((ciphertext.length()       != (unsigned long long)(digit_length * 512))
		|| (keys.length()             != (unsigned long long)(digit_length * 256))
		|| (presence_absence.length() != (unsigned long long)(               512))
		|| (number.length()           != (unsigned long long)(               128))
		|| (zeros[0]                  ==                                     '\0')) {cout << "\nBad \"public\" file.\n"; return 0;}
		
		//Checks message.
		if(message[0] != '\0')
		{	for(unsigned int a = 0; a < (unsigned long long)message.length(); a++)
			{	if((message[a] < 32) || (message[a] > 126)) {cout << "\nMessage corrupted.\n"; return 0;}
			}
		}
		
		//Checks presence_absence.
		int occur_0 = 0;
		int occur_1 = 0;
		for(int a = 0; presence_absence[a] != '\0'; a++)
		{	if     (presence_absence[a] == '0') {occur_0++;}
			else if(presence_absence[a] == '1') {occur_1++;}
		}
		if((occur_0 != 256) || (occur_1 != 256)) {cout << "\nPresence & absence indication must have 256 zeros and 256 ones.\n"; return 0;}
		
		//Takes hash of ciphertext.
		SHA_512_input = ciphertext;
		{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		
		//Compares hash to the number in the "number" file.
		string temp_number; in_stream.open(path_to_number); getline(in_stream, temp_number); in_stream.close();
		if(SHA_512_output != temp_number) {cout << "\nFAILED! That number is not the hash of the ciphertext. Old public file?\n"; in_stream.close(); return 0;}
		
		//Takes hash of user message & number.
		string last_3_lines;
		last_3_lines += message; last_3_lines += '\n';
		last_3_lines += number ; last_3_lines += '\n';
		last_3_lines += zeros  ;
		
		SHA_512_input = last_3_lines;
		{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		string hash_of_last_3_lines = SHA_512_output;
		
		//Decrypts ciphertext to which keys are present.
		int ciphertext_read_bookmark = 0;
		int keys_read_bookmark       = 0;
		for(int a = 0; a < 512; a++)
		{	if(presence_absence[a] == '1')
			{	mpz_t prime, two; mpz_init(prime); mpz_init(two); mpz_set_ui(two, 2);
				char key[50001] = {'\0'}; for(int b = 0; b < digit_length; b++) {key[b] = keys[keys_read_bookmark]; keys_read_bookmark++;}
				mpz_set_str(prime, key, 10);
				
				//Generates randomness (by concatenating consecutive prime gaps).
				string recreated_ciphertext; unsigned long long wanted_length = digit_length;
				for(long long gap = 2; recreated_ciphertext.length() < wanted_length;)
				{	mpz_add(prime, prime, two);
					int primality = mpz_probab_prime_p(prime, 25);
					if(primality == false) {gap += 2;}
					else {recreated_ciphertext += to_string(gap); gap = 2;}
				}
				recreated_ciphertext.resize(digit_length);
				
				//Checks it.
				for(int b = 0; b < digit_length; b++)
				{	if(recreated_ciphertext[b] != ciphertext[ciphertext_read_bookmark]) {cout << "\nFAILED! Incorrect keys.\n"; return 0;}
					ciphertext_read_bookmark++;
				}
			}
			else {ciphertext_read_bookmark += digit_length;}
		}
		
		//Compares presence & absence indication to (the bits of) the hash of user message & number.
		string bin_hash_of_last_3_lines;
		for(int a = 0; hash_of_last_3_lines[a] != '\0'; a++)
		{	if     (hash_of_last_3_lines[a] == '0') {bin_hash_of_last_3_lines += "0000";}
			else if(hash_of_last_3_lines[a] == '1') {bin_hash_of_last_3_lines += "0001";}
			else if(hash_of_last_3_lines[a] == '2') {bin_hash_of_last_3_lines += "0010";}
			else if(hash_of_last_3_lines[a] == '3') {bin_hash_of_last_3_lines += "0011";}
			else if(hash_of_last_3_lines[a] == '4') {bin_hash_of_last_3_lines += "0100";}
			else if(hash_of_last_3_lines[a] == '5') {bin_hash_of_last_3_lines += "0101";}
			else if(hash_of_last_3_lines[a] == '6') {bin_hash_of_last_3_lines += "0110";}
			else if(hash_of_last_3_lines[a] == '7') {bin_hash_of_last_3_lines += "0111";}
			else if(hash_of_last_3_lines[a] == '8') {bin_hash_of_last_3_lines += "1000";}
			else if(hash_of_last_3_lines[a] == '9') {bin_hash_of_last_3_lines += "1001";}
			else if(hash_of_last_3_lines[a] == 'a') {bin_hash_of_last_3_lines += "1010";}
			else if(hash_of_last_3_lines[a] == 'b') {bin_hash_of_last_3_lines += "1011";}
			else if(hash_of_last_3_lines[a] == 'c') {bin_hash_of_last_3_lines += "1100";}
			else if(hash_of_last_3_lines[a] == 'd') {bin_hash_of_last_3_lines += "1101";}
			else if(hash_of_last_3_lines[a] == 'e') {bin_hash_of_last_3_lines += "1110";}
			else if(hash_of_last_3_lines[a] == 'f') {bin_hash_of_last_3_lines += "1111";}
			else                                    {cout << "\n\nError_d4\n\n"; return 0;}
		}
		
		if(presence_absence != bin_hash_of_last_3_lines) {cout << "\nFAILED! Hash of \"message + new number\" does not match key presence & absence.\n"; return 0;}
		cout << "\nVerified!";
		
		//Updates file "number" with the new number in file "public".
		out_stream.open(path_to_number); out_stream << number; out_stream.close();
		
		//Appends message to file "number".
		if(message[0] != '\0')
		{	out_stream.open(path_to_number, ios::app); out_stream << "\n\n" << message << "\n"; out_stream.close();
			cout << "\nThe \"number\" file was updated. And their message was appended to it.\n";
		}
		else {cout << "\nThe \"number\" file was updated. And they left no message.\n";}
	}
}
