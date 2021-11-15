#include <bits/stdc++.h>
using namespace std;

struct Node {
 int F;
 char C;
 Node* L;
 Node* R;

 Node(int f, char c, Node* l, Node* r) {
  F = f; C = c; L = l; R = r;
 }
 void debug() {
  cerr << "Info about " << C << ": " << "freq(" << F << ")\n"; 
 }
};

Node* top; // it's the top of HuffmanTree
vector<bool> encoding; // it's our whole EncodedData
map<Node*, vector<bool>> path; // stores path till current node
map<char, vector<bool>> char_path; // stores code for leaves
ifstream fin;
ofstream fout;

class CMP {
public:
 bool operator() (Node* a, Node* b) {
  return a->F > b->F;
 }
};

void build_huffman_tree(string& s) {
 map<char, int> occ;
 for(auto& e: s) occ[e]++;

 priority_queue<Node*, vector<Node*>, CMP> q;
 for(auto& e: occ) {
  q.push( new Node (
   e.second, e.first, nullptr, nullptr
  ) );
 }

 while(q.size() > 1) {
  Node* a = q.top(); q.pop(); // minimum
  Node* b = q.top(); q.pop(); // second minimum
  // a->debug();
  // b->debug();

  Node* head = new Node (
   a->F + b->F, '*', a, b // a.F < b.F, so a is left one, and b right one
  );
  q.push(head);
 }
 
 top = q.top();
 return ;
}

void dfs(Node* n) {
 n->debug();

 if(n->C == '*') {
  path[n->L] = path[n];
  path[n->R] = path[n];
  path[n->L].push_back(0);
  path[n->R].push_back(1);
  
  dfs(n->L);
  dfs(n->R);
 } else {
  char_path[n->C] = path[n];
 }
}

void encode(string& s) {
 cerr << '\n';
 dfs(top);
 cerr << "\nThe encoded user data is: ";
 for(auto& e: s) {
  for(bool b: char_path[e]) {
   // fout << b;
   cerr << b;
   encoding.push_back(b);
  }
 }

 fout.write((char*) &encoding, sizeof encoding);
}

void decode() {
 Node* curr = top;
 for(auto e: encoding) {
  if(curr->C != '*') {
   cerr << curr->C;
   curr = top;
  }
  curr = e ? curr->R : curr->L;
 }
 cerr << curr->C;
}

int main() {
 fin.open("source.txt");
 fout.open("encoded.bin", ios::binary);
 freopen("logs.txt", "w", stderr);
 string s;
 getline(fin, s, '\n');

 cerr << "The data recieved from user is: " << s << '\n'; 
 build_huffman_tree(s);
 encode(s);
 fin.close();
 fout.close();
 cerr << "\nThe original user data decoded from encoding is: ";
 decode();

 ifstream debug;
 debug.open("encoded.bin", ios::binary);
 vector<bool> buffer(istreambuf_iterator<char>(debug), {});
 for(auto e: buffer) {
  cerr << e;
 }

 debug.close();
 return 0;
}