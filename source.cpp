#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

// C++ program to encode and decode a string using 
// Huffman Coding. 
#define MAX_TREE_HT 256 

// to map each character its huffman value 
std::map<char, std::string> codes;

// to store the frequency of character of the input data 
std::map<char, int> freq;

// A Huffman tree node 
struct MinHeapNode
{
	char data;             // One of the input characters 
	int freq;             // Frequency of the character 
	MinHeapNode* left, * right; // Left and right child 

	MinHeapNode(char data, int freq)
	{
		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// utility function for the priority queue 
struct compare
{
	bool operator()(MinHeapNode* l, MinHeapNode* r)
	{
		return (l->freq > r->freq);
	}
};

// utility function to print characters along with 
// there huffman value 
void printCodes(struct MinHeapNode* root, std::string str)
{
	if (!root)
		return;
	if (root->data != '$')
		std::cout << root->data << ": " << str << "\n";
	printCodes(root->left, str + "0");
	printCodes(root->right, str + "1");
}

// utility function to store characters along with 
// there huffman value in a hash table, here we 
// have C++ STL map 
void storeCodes(struct MinHeapNode* root, std::string str)
{
	if (root == NULL)
		return;
	if (root->data != '$')
		codes[root->data] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}

// STL priority queue to store heap tree, with respect 
// to their heap root node value 
std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, compare> minHeap;

// function to build the Huffman tree and store it 
// in minHeap 
void HuffmanCodes(int size)
{
	struct MinHeapNode* left, * right, * top;
	for (std::map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
		minHeap.push(new MinHeapNode(v->first, v->second));
	while (minHeap.size() != 1)
	{
		left = minHeap.top();
		minHeap.pop();
		right = minHeap.top();
		minHeap.pop();
		top = new MinHeapNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		minHeap.push(top);
	}
	storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its 
// frequency in input string 
void calcFreq(std::string str, int n)
{
	for (int i = 0; i < str.size(); i++)
		freq[str[i]]++;
}

// function iterates through the encoded string s 
// if s[i]=='1' then move to node->right 
// if s[i]=='0' then move to node->left 
// if leaf node append the node->data to our output string 
std::string decode_file(struct MinHeapNode* root, std::string s)
{
	std::string ans = "";
	struct MinHeapNode* curr = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		// reached leaf node 
		if (curr->left == nullptr and curr->right == nullptr)
		{
			ans += curr->data;
			curr = root;
		}
	}
	// cout<<ans<<endl; 
	return ans + '\0';
}

struct dataBlock {
	int count = 0;
	char letter = '\0';
	double probabilty = 0.0;
};

void OpenFile(dataBlock log[], uint32_t& total, std::vector<char> &chStream) {
	// Open file
	std::fstream inFile;
	inFile.open("data.txt");

	int tabIndex = 0;

	char ch = '\0';

	while (!inFile.eof())
	{
		inFile.get(ch);
		chStream.push_back(ch);

		log[(int)ch].count++;
		log[(int)ch].letter = ch;
		++total;
	}

	// Calculate Stat
	if (total > 0)
		for (int x = 0; x < 256; ++x) {
			log[x].probabilty = log[x].count / total;
		}

	inFile.close();

	return;
}

void CalculateStat(dataBlock log[], uint32_t total) {
	double sTotal = 0.0;

	for (unsigned int ndx = 0; ndx < 256; ++ndx) {
		log[ndx].probabilty = double (log[ndx].count) / double( total );
		sTotal += log[ndx].probabilty;
	}

	return;
}

bool operator>(dataBlock l, dataBlock r) {
	if (l.probabilty > r.probabilty)
		return true;
	else
		return false;
}

void swap(dataBlock& xp, dataBlock& yp)
{
	dataBlock temp = xp;
	xp = yp;
	yp = temp;
}

// A function to implement bubble sort  
void bubbleSort(dataBlock arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
}

void SortLog(dataBlock log[])
{
	bubbleSort(log, 256);
}

void DisplayLog(dataBlock log[], uint32_t sTotal) 
{
	// Display Stats
	std::cout << "\nStats" << std::endl;
	for (int ndx = 0; ndx < 256; ++ndx) {
		std::cout << "Character: " << log[ndx].letter << '\t';
		std::cout << "Count: " << log[ndx].count << '\t';
		std::cout << "Probablity: " << log[ndx].probabilty << std::endl;
	}

	std::cout << "Stat Total: " << sTotal;
}

// Testing commit
int main()
{
	uint32_t total = 0;
	std::vector<char> chStream;

	// For all uint_32
	dataBlock log[256] = { {0, '\0', 0} };

	OpenFile(log, total, chStream);
	CalculateStat(log, total);

	// Organize in ascending order.
	SortLog(log);

	// Convert vec to string
	std::string str(chStream.begin(), chStream.end());
	std::string encodedString, decodedString;

	calcFreq(str, str.length());
	HuffmanCodes(str.length());
	std::cout << "Character With there Frequencies:\n";

	for (auto v = codes.begin(); v != codes.end(); v++)
		std::cout << v->first << ' ' << v->second << std::endl;

	for (auto i : str)
		encodedString += codes[i];

	std::cout << "\nEncoded Huffman data:\n" << encodedString << std::endl;

	decodedString = decode_file(minHeap.top(), encodedString);
	std::cout << "\nDecoded Huffman Data:\n" << decodedString << std::endl;

	std::cout << "\nHit any key." << std::endl;
	std::cin.get();

	return 0;
}