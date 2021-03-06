#include "database.h"
#include "article.h"
#include "newsgroup.h"

#include "badngnumber.h"
#include "badartnumber.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <fstream>

using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::endl;

database::database() {
	std::__fs::filesystem::create_directory("root");
	std::ifstream counter("root/counter.txt");
	string nbr;
	try {
		getline(counter, nbr);
		ctr = std::stoi(nbr) + 1;
	} catch (...) {
		ctr = 0;
	}
	counter.close();
}

database::~database() = default;

int database::update_ctr() {
	std::ofstream counter("root/counter.txt");
	counter << ctr;
	counter.close();
	return ctr++;
}

vector<pair<int, string> > database::list_groups() const{
	vector<pair<int, string> > groups;
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");

		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		string title;
		string id;
		getline(infile, id);
		getline(infile, title);
		groups.push_back(std::make_pair(std::stoi(id), title));
		infile.close();
	}
	return groups;
}

bool database::delete_group(int ng_id_nbr) {
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");
		string id;
		getline(infile, id);
		if (std::stoi(id) == ng_id_nbr) {
			std::__fs::filesystem::remove_all(dir_entry.path().u8string());
			return true;
		}
	}
	return false;
}

bool database::create_group(string ng_name) {
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");
		string id;
		string title;
		getline(infile, id);
		getline(infile, title);
		infile.close();
		if (title == ng_name) {
			return false;
		}
	}
	int id_nbr = update_ctr();
	std::__fs::filesystem::create_directory("root/" + std::to_string(id_nbr));
	std::ofstream outfile("root/" + std::to_string(id_nbr) + "/" +  "info.txt");
	outfile << id_nbr << "\n" << ng_name;
	outfile.close();
	return true;
}

vector<pair<int, string> > database::list_articles(int ng_id_nbr) const {
	vector<pair<int, string> > groups;
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root/" + std::to_string(ng_id_nbr))) {
		if (dir_entry.path().u8string() == "root/" + std::to_string(ng_id_nbr) + "/info.txt") {
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/id.txt");
		string id;
		getline(infile, id);
		infile.close();
		std::ifstream article(dir_entry.path().u8string() + "/article.txt");
		string title;
		getline(article, title);
		groups.push_back(std::make_pair(std::stoi(id), title));
		article.close();
	}
	return groups;
}

bool database::read(int ng_id_nbr, int art_id_nbr, string& title, string& author, string& text) const{
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");
		string id;
		getline(infile, id);
		infile.close();
		if (std::stoi(id) == ng_id_nbr) {
			for (auto ng_entry : std::__fs::filesystem::directory_iterator(dir_entry.path())) {
				if (ng_entry.path().u8string() == "root/" + id + "/info.txt") {	
					continue;
				}
				std::ifstream fileid(ng_entry.path().u8string() + "/id.txt");
				string art_id;
				getline(fileid, art_id);
				fileid.close();
				if(std::stoi(art_id) == art_id_nbr) {
						std::ifstream article(ng_entry.path().u8string() + "/article.txt");
						getline(article, title);
						getline(article, author);
						getline(article, text);
						article.close();
						return true;
				}
			}
			throw BadARTException();	
		}
	}
	throw BadNGException();
	return false;
}

bool database::write(int ng_id_nbr, string title, string author, string text){

	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");
		string id;
		getline(infile, id);
		infile.close();
		if (std::stoi(id) == ng_id_nbr) {
			std::__fs::filesystem::create_directory(dir_entry.path().u8string() + "/" + std::to_string(ctr));
			std::ofstream outfile(dir_entry.path().u8string() + "/" + std::to_string(ctr) + "/article.txt");
			std::ofstream outid(dir_entry.path().u8string() + "/" + std::to_string(ctr) + "/id.txt");

			outfile << title << "\n" << author << "\n" << text;
			outid << update_ctr();
			outfile.close();
			outid.close();
			return true;
		} 
	}
	throw BadNGException();
	return false;
}

bool database::delete_article(int ng_id_nbr, int art_id_nbr){
	for (auto dir_entry : std::__fs::filesystem::directory_iterator("root")) {
		if (dir_entry.path().u8string() == "root/counter.txt") {	
			continue;
		}
		std::ifstream infile(dir_entry.path().u8string() + "/info.txt");
		string ng_id;
		getline(infile, ng_id);
		infile.close();
		
		if (std::stoi(ng_id) == ng_id_nbr) {
			for (auto ng_entry : std::__fs::filesystem::directory_iterator(dir_entry.path())) {
				if (ng_entry.path().u8string() == "root/" + ng_id + "/info.txt") {	
					continue;
				}
				std::ifstream idfile(ng_entry.path().u8string() + "/id.txt");
				string art_id;
				getline(idfile, art_id);
				infile.close();
				if(std::stoi(art_id) == art_id_nbr) {
					std::__fs::filesystem::remove_all(ng_entry.path());
					return true;
				}
			}
			throw BadARTException();
		}
	}
	throw BadNGException();
	return false;
}
