#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>

void counter(std::string fname, std::map<std::string, std::vector<int>> &word_map) {
    std::istringstream iss;
    std::string line;
    std::string word;
    int line_num = 0;

    std::ifstream in(fname);
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
}
void output(std::string outfile, std::map<std::string, std::vector<int>> &word_map)
{
    std::ofstream out(outfile);
    for(int i=0; i<70; i++)  out << "*";
    out << "\n|       " << std::left << std::setw(10) << "Word" << "|     " << std::left << std::setw(10) << "Count" << "|     " << std::left << std::setw(30) << "Lines in which it appears" << "|" << std::endl;
    for(int i=0; i<70; i++) out << "*";
    out << std::endl;

    //output results from word_map where the word was repeated more than once and line_map
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

    out.close();

}

int main()
{
    std::string fname="example.txt";
    std::string outfile="Rezultatai.txt";
    
    std::map<std::string, std::vector<int>> word_map;

    counter(fname, word_map);
    output(outfile, word_map);
    
    
    return 0;
}