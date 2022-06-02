#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>

void file_validation(std::string &file_name){
    std::cout << "Enter the file name using the following format: [example.txt]" << std::endl;
    std::cin >> file_name;
    if(file_name.find(".txt") == std::string::npos){
        std::cout << "File format is not correct. Please try again." << std::endl;
        file_validation(file_name);
    }
    if(file_name.find(" ") != std::string::npos){
        std::cout << "File name cannot contain spaces. Please try again." << std::endl;
        file_validation(file_name);
    }
    std::ifstream in(file_name);
    if(!in){
        std::cout << "File does not exist. Please try again." << std::endl;
        file_validation(file_name);
    }
    std::string line;
    std::getline(in, line);
    if(line.empty()){
        std::cout << "File is empty. Please try again." << std::endl;
        file_validation(file_name);
    }
    in.close();
}

void counter(std::string file_name, std::map<std::string, std::vector<int>> &word_map, std::vector<std::string> &url) {
    std::istringstream iss;
    std::string line;
    std::string word;
    int line_num = 0;

    std::ifstream in(file_name);
    if (!in) {
        throw std::runtime_error("Could not open file");
    }
    while(std::getline(in, line)) {
        iss.clear();
        iss.str(line);
        line_num++;
        // Store all the words and their line_num in the word_map, but don't include symbols or numbers
        while (std::getline(in, line)) {
        iss.clear();
        iss.str(line);
        line_num++;

        while (iss >> word) {
                // Find URLs and store them in url vector
                if (word.find("http") != std::string::npos || word.find("www") != std::string::npos) {
                    url.push_back(word);
                }
                if (word != "") {   
                    // Convert all words to lowercase and do not include numbers, symbols and spaces
                    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                    if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos) {
                        // If the word is not in the map, add it to the map
                        if (word_map.find(word) == word_map.end()) {
                            word_map[word] = std::vector<int>();
                        }
                        // Add the line number to the vector of line numbers for the word
                        word_map[word].push_back(line_num);
                    }
                }
            }
        }
    }
    in.close();
    std::cout << "File successfully read." << std::endl;
}
void output(std::string outfile, std::map<std::string, std::vector<int>> word_map, std::vector<std::string> url)
{
    // Header for the word map.
    std::ofstream out(outfile);
    for(int i=0; i<70; i++)  out << "*";
    out << "\n|       " << std::left << std::setw(10) << "Word" << "|     " << std::left << std::setw(10) << "Count" << "|     " << std::left << std::setw(30) << "Lines in which it appears" << "|" << std::endl;
    for(int i=0; i<70; i++) out << "*";
    out << std::endl;

    // Output from word map where the word has appeared more than once
    for (auto it = word_map.begin(); it != word_map.end(); ++it) {
        if (it->second.size() > 1) {
            out << "|" << std::left << std::setw(15) << it->first << "|     " << std::left << std::setw(10) << it->second.size() << "|     ";
            for (int i = 0; i < it->second.size(); i++) {
                out << it->second[i];
                if (i != it->second.size() - 1) {
                    out << ",";
                }
            }
            out << std::endl;
        }
    }

    // Second header but instead of words it's for urls
    for(int i=0; i<18; i++) out << "*";
    out << "\n|       " << std::left << std::setw(10) << "URL" << "| " << std::endl;
    for(int i=0; i<18; i++) out << "*";
    out << std::endl;
    // Output from url vector
    for (int i = 0; i < url.size(); i++) {
        out << url[i] << std::endl;
    }
    out.close();
    std::cout << "Results have been outputted to: " << outfile << std::endl;
}

int main()
{
    std::string file_name;
    std::string outfile="Rezultatai.txt";
    
    std::map<std::string, std::vector<int>> word_map;
    std::vector<std::string> url;
    file_validation(file_name);
    counter(file_name, word_map, url);
    output(outfile, word_map, url);
    
    
    return 0;
}