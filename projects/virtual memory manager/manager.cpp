#include <iostream>
#include <fstream>

int main(int argc, char ** argv) {
    if(argc != 2){
        std::cout << "Not enough enough argumetns\n\t Usage ./a.out file.txt\n";
        return 1;
    }
    std::string filename = argv[1];
    std::string num;
    std::fstream address_file;
    address_file.open(filename, std::ios::in);

    if(!address_file){
        std::cout << "Error in opening file: " << filename << " \n";
        return 0;
    }
    int mem_num;
    while(!address_file.eof()){
        address_file >> mem_num;
        std::cout << mem_num << "\n";
    }

    address_file.close();
    return 0;
}
