//YOUR CONTROLS:                                                                Run it: "apt install g++ geany libgmp-dev libssl-dev". Open the .cpp in Geany.
int digit_length = 200; //50000 max                                             //Append "-lgmp -lcrypto" to Geany's compile & build commands. Hit F9 once. F5 to run.
//digit_length is the length of a (jumping) value, near which
//prime gaps are found, for randomness. Bigger = more secure.

/*Version 9.0.0
Get a secure number only you can modify, which anyone can
keep verifying, and you can insert personal messages (data
authentication independent of encryption type & symmetry).*/

#include <cstring>
#include <filesystem>
#include <fstream>
#include <gmp.h>
#include <iostream>
#include <openssl/evp.h>
using namespace std;
int main()
{	int raw_byte;
	char file_byte;
	ifstream in_stream;
	ofstream out_stream;
	
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
		
		//Creates the number file (takes a sha512sum hash of the 512 ciphertext).
		string input; in_stream.open("Personal/private/512_ciphertext"); getline(in_stream, input); in_stream.close();
		unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, input.c_str(), input.length(), hash, nullptr);
		string sha512sum; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {sha512sum += symbols[hash[a] >> 4]; sha512sum += symbols[hash[a] & 0xf];}
		
		out_stream.open("Personal/number"); out_stream << sha512sum; out_stream.close();
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
		string path_to_keys                      = path;   path_to_keys                      += "/private/512_keys"                 ;
		string path_to_ciphertext                = path;   path_to_ciphertext                += "/private/512_ciphertext"           ;
		string path_to_private_seeds             = path;   path_to_private_seeds             += "/private/private_seeds"            ;
		string path_to_public                    = path;   path_to_public                    += "/public"                           ;
		string path_to_number                    = path;   path_to_number                    += "/number"                           ;
		string path_to_representing_sha512sum_of = path;   path_to_representing_sha512sum_of += "/private/representing_sha512sum_of";
		string path_to_the_sha512sum             = path;   path_to_the_sha512sum             += "/private/the_sha512sum"            ;
		
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
			
			//Creates the number file (takes a sha512sum hash of the 512 ciphertext).
			string input; in_stream.open(path_to_ciphertext); getline(in_stream, input); in_stream.close();
			unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, input.c_str(), input.length(), hash, nullptr);
			string sha512sum; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {sha512sum += symbols[hash[a] >> 4]; sha512sum += symbols[hash[a] & 0xf];}
			
			out_stream.open(path_to_number); out_stream << sha512sum; out_stream.close();
		}
		
		//Writes user message to new file "representing_sha512sum_of".
		out_stream.open(path_to_representing_sha512sum_of);
		if(existence_of_message == true) {out_stream << message;}
		out_stream << "\n";
		out_stream.close();
		
		//Appends new number to file "representing_sha512sum_of".
		in_stream.open(path_to_number);
		out_stream.open(path_to_representing_sha512sum_of, ios::app);
		getline(in_stream, line); out_stream << line << "\n0";
		in_stream.close();
		out_stream.close();
		
		//Keeps appending 0s to file "representing_sha512sum_of" until its sha512sum hash is composed of 256 1 bits and 256 0 bits.
		for(int a = 0; a < 100000; a++)
		{	//Takes a hash.
			string input;
			in_stream.open(path_to_representing_sha512sum_of);
			getline(in_stream, line); input += line; input += '\n';
			getline(in_stream, line); input += line; input += '\n';
			getline(in_stream, line); input += line;
			in_stream.close();
			
			unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, input.c_str(), input.length(), hash, nullptr);
			string sha512sum; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {sha512sum += symbols[hash[a] >> 4]; sha512sum += symbols[hash[a] & 0xf];}
			
			out_stream.open(path_to_the_sha512sum); out_stream << sha512sum; out_stream.close();
			
			//Checks if hash is evenly bit-distributed.
			in_stream.open(path_to_the_sha512sum);
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
			out_stream.open(path_to_representing_sha512sum_of, ios::app);
			out_stream << "0";
			out_stream.close();
			
			if(a == 99999) {cout << "\nError_d2\n"; return 0;}
		}
		
		//Appends only 256 old keys to file "public" (based on bits from hash in file "the_sha512sum").
		string bin;
		in_stream.open(path_to_the_sha512sum);
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
		
		//Appends user message and new number to file "public" (all from file "representing_sha512sum_of").
		in_stream.open(path_to_representing_sha512sum_of);
		out_stream.open(path_to_public, ios::app);
		in_stream.get(file_byte);
		for(; !in_stream.eof(); in_stream.get(file_byte)) {out_stream.put(file_byte);}
		in_stream.close();
		out_stream.close();
		
		cout << "\nModified!"
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
		unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, ciphertext.c_str(), ciphertext.length(), hash, nullptr);
		string hash_of_ciphertext; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {hash_of_ciphertext += symbols[hash[a] >> 4]; hash_of_ciphertext += symbols[hash[a] & 0xf];}
		
		//Compares hash to the number in the "number" file.
		string temp_number; in_stream.open(path_to_number); getline(in_stream, temp_number); in_stream.close();
		if(hash_of_ciphertext != temp_number) {cout << "\nFAILED! That number is not the hash of the ciphertext. Old public file?\n"; in_stream.close(); return 0;}
		
		//Takes hash of user message & number.
		string last_3_lines;
		last_3_lines += message; last_3_lines += '\n';
		last_3_lines += number ; last_3_lines += '\n';
		last_3_lines += zeros  ;
		
		EVP_Q_digest(nullptr, "SHA512", nullptr, last_3_lines.c_str(), last_3_lines.length(), hash, nullptr);
		string hash_of_last_3_lines; for(int a = 0; a < 64; a++) {hash_of_last_3_lines += symbols[hash[a] >> 4]; hash_of_last_3_lines += symbols[hash[a] & 0xf];}
		
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
