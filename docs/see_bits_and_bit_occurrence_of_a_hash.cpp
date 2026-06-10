//YOUR CONTROLS:                                                                                    Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.
char your_hash[] = {"6a74cb02b60a820f33139bd65c17861daf6f0707efcd5e0a03808317be46488e"};
//Enter a SHA-256, or SHA-512, or any hash containing symbols "0123456789abcdef".



#include <iostream>
using namespace std;
int main()
{	string bits;
	for(long long a = 0; your_hash[a] != '\0'; a++)
	{	if     (your_hash[a] == '0') {bits += "0000";}
		else if(your_hash[a] == '1') {bits += "0001";}
		else if(your_hash[a] == '2') {bits += "0010";}
		else if(your_hash[a] == '3') {bits += "0011";}
		else if(your_hash[a] == '4') {bits += "0100";}
		else if(your_hash[a] == '5') {bits += "0101";}
		else if(your_hash[a] == '6') {bits += "0110";}
		else if(your_hash[a] == '7') {bits += "0111";}
		else if(your_hash[a] == '8') {bits += "1000";}
		else if(your_hash[a] == '9') {bits += "1001";}
		else if(your_hash[a] == 'a') {bits += "1010";}
		else if(your_hash[a] == 'b') {bits += "1011";}
		else if(your_hash[a] == 'c') {bits += "1100";}
		else if(your_hash[a] == 'd') {bits += "1101";}
		else if(your_hash[a] == 'e') {bits += "1110";}
		else if(your_hash[a] == 'f') {bits += "1111";}
		else                         {cout << "\nError: hash contains symbols other than \"0123456789abcdef\".\n"; return 0;}
	}
	
	long long zeros = 0, ones = 0;
	for(long long a = 0; bits[a] != '\0'; a++)
	{	if(bits[a] == '0') {zeros++;}
		else               {ones ++;}
	}
	
	cout << " zero bits: " <<  zeros << "\n"
	     << "  one bits: " <<  ones  << "\n"
	     << "total bits: " << (zeros + ones) << "\n\n"
	
	     << "the bits:\n" << bits << "\n";
}
