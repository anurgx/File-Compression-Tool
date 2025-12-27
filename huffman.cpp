#include <bits/stdc++.h>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root, string code,
                   unordered_map<char, string>& huffmanCode) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

Node* buildHuffmanTree(const unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto& p : freq) {
        pq.push(new Node(p.first, p.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

void compress(const string& inputFile, const string& compressedFile) {
    ifstream in(inputFile);
    if (!in) {
        cout << "Cannot open input file" << endl;
        return;
    }

    string text((istreambuf_iterator<char>(in)),
                 istreambuf_iterator<char>());
    in.close();

    unordered_map<char, int> freq;
    for (char c : text) freq[c]++;

    Node* root = buildHuffmanTree(freq);

    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    ofstream out(compressedFile);
    for (char c : text) {
        out << huffmanCode[c];
    }
    out.close();

    cout << "File compressed successfully." << endl;
}

void decompress(const string& compressedFile,
                const string& outputFile,
                Node* root) {

    ifstream in(compressedFile);
    ofstream out(outputFile);

    Node* curr = root;
    char bit;

    while (in >> bit) {
        if (bit == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (!curr->left && !curr->right) {
            out << curr->ch;
            curr = root;
        }
    }

    in.close();
    out.close();

    cout << "File decompressed successfully." << endl;
}

int main() {
    string inputFile = "input.txt";
    string compressedFile = "compressed.txt";
    string decompressedFile = "decompressed.txt";

    compress(inputFile, compressedFile);

    ifstream in(inputFile);
    string text((istreambuf_iterator<char>(in)),
                 istreambuf_iterator<char>());
    in.close();

    unordered_map<char, int> freq;
    for (char c : text) freq[c]++;

    Node* root = buildHuffmanTree(freq);

    decompress(compressedFile, decompressedFile, root);

    return 0;
}


