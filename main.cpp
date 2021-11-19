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
   a->F + b->F, '\0', a, b // a.F < b.F, so a is left one, and b right one
  );
  q.push(head);
 }
 
 top = q.top();
 return ;
}

void dfs(Node* n) {
 n->debug();

 if(n->C == '\0') {
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

// 0010101 = 21 -> c
// c -> 21 = 10101
// 100101 = 00100101


//for depth first traversal to get the code for each cahracter
void encode(string& s) {
 cerr << '\n';
 dfs(top);
 cerr << "\nThe encoded user data is: ";

 for(auto& e: s) {
  for(bool b: char_path[e]) {
   cerr << b;
   encoding.push_back(b);
  }
 }

 compress();
}

void decode() {
 Node* curr = top;
 string orig;

 for(auto e: encoding) {
  if(curr->C != '\0') {
   orig.push_back(curr->C);
   curr = top;
  }
  curr = e ? curr->R : curr->L;
 }

 orig.push_back(curr->C);
 cerr << orig;
}

int main() {
 freopen("source.txt", "r", stdin); // cin
 freopen("encoded.txt", "w", stdout); // cout
 freopen("logs.txt", "w", stderr); // cerr

 string s;
 getline(cin, s, '\0');
 build_huffman_tree(s);
 encode(s);
 cerr << "\n\nThe original user data decoded from encoding is:\n------------------------------------------------\n\n";
 decode();

 return 0;
}
