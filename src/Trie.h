/*
 * Trie.h
 *
 *  Created on: 08-Sep-2019
 *      Author: bullu
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <iostream>
#include <vector>

using namespace std;

class Node{
public:
	Node * arr[26]; string data;
	Node(){
		for(int i=0;i<26;i++){
			arr[i] = NULL;
		}
		data = "";
	}
};

class Trie{
public:
	Node * root;
	Trie(){
		root = new Node();
	}
	void insert(string command){
		int c;
		Node * head = root;
		for(int i=0;i<command.size();i++){
			c = command[i] - 97;
			if(head->arr[c] == NULL){
				head->arr[c] = new Node();
			}
			head = head->arr[c];
		}
		head->data = command;
	}
	void traverse(Node * root, vector<string>& answer){
		if(root->data != "") answer.push_back(root->data);
		for(int i=0;i<26;i++){
			if(root->arr[i] != NULL){
				traverse(root->arr[i], answer);
			}
		}
	}
	vector<string> findPrefixStrings(string prefixCommand){
		Node * head = root; vector<string> result; int c;
		for(int i=0;i<prefixCommand.size();i++){
			c = prefixCommand[i] - 97;
			if(head->arr[c] == NULL) return result;
			else head = head->arr[c];
		}
		traverse(head, result);
		return result;
	}
};

class Commands{
public:
	Trie t;
	Commands(){
		t.insert("alias");
		t.insert("adduser");
		t.insert("cal");
		t.insert("cat");
		t.insert("cd");
		t.insert("chmod");
		t.insert("chown");
		t.insert("curl");
		t.insert("date");
		t.insert("df");
		t.insert("diff");
		t.insert("echo");
		t.insert("exit");
		t.insert("find");
		t.insert("finger");
		t.insert("free");
		t.insert("grep");
		t.insert("groups");
		t.insert("gzip");
		t.insert("head");
		t.insert("history");
		t.insert("hostname");
		t.insert("kill");
		t.insert("less");
		t.insert("ls");
		t.insert("man");
		t.insert("mkdir");
		t.insert("mv");
		t.insert("passwd");
		t.insert("ping");
		t.insert("ps");
		t.insert("pwd");
		t.insert("rm");
		t.insert("shutdown");
		t.insert("sort");
		t.insert("ssh");
		t.insert("sudo");
		t.insert("tail");
		t.insert("tar");
		t.insert("top");
		t.insert("uname");
		t.insert("w");
		t.insert("wc");
		t.insert("whoami");
	}
};

Commands c;

vector<string> findPrefixStrings(string prefixCommand){
	return c.t.findPrefixStrings(prefixCommand);
}

#endif /* TRIE_H_ */
