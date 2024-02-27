#include <iostream>
#include <fstream>
#include <algorithm>
#include<Windows.h>
#include <string>
#include <vector>

class ini_parser {
private:
    std::ifstream file;
    std::string q;
    
    std::vector<std::string> file_cont;

    std::string str = "";// элемент для строк
    std::string str2 = "";
    bool section_error = true; // исключений для секций
    bool val_err = true; // исключений для переменных

    int count = 0;

public:
    ini_parser(std::string fname) {
        file.open(fname + ".ini");
        if (file.is_open())
        {
            std::cout << "Констурктор создан. Файл открыт" << std::endl;
            
            

            while (!file.eof() ) {
                getline(std::move(file),q);
                if (q[0] != ';') {
                    file_cont.push_back(std::move(q));
                }
            }

            for (int i = 0; i < file_cont.size(); i++) {
                for (int j = 0; j < file_cont[i].size(); j++) {
                    if (file_cont[i][j] == ';') {
                        file_cont[i].erase(j);
                    }
                }
            }
        }
        else {
            throw std::runtime_error("Не удалось открыть файл");
        }
    }

    template<class T>
    T get_value(std::string section_name, std::string val_name) {
        T el = 0;
        for (int i = 0; i < file_cont.size() - 1 ; i++) {

            if (file_cont[i] == '[' + section_name + ']') {
                el = 0;
                i++;
                while (file_cont[i] != "") {
                    str = "";
                    str = str + file_cont[i][0] + file_cont[i][1] + file_cont[i][2] + file_cont[i][3];

                    if (str  == val_name)
                    {
                        count = 0;
                        count = str.size() + 3;
                        str = "";
                        while (file_cont[i][count] != NULL) {
                            str = str + file_cont[i][count];
                            count++;
                        }
                        el += std::stof(str);
                        val_err = false;
                    }
                    if (i < file_cont.size() - 1) {
                        i++;
                    }
                    else break;

                }
                section_error = false;
            }

        }
        if (section_error == true) throw std::runtime_error("Данной секции не существует");
        else if(val_err == true) throw std::runtime_error("Данной переменной не существует");
            return el;
    }

    template<>
    std::string get_value(std::string section_name, std::string val_name) {
        std::string str2 = "";
        for (int i = 0; i < file_cont.size(); i++) {

            if (file_cont[i] == '[' + section_name + ']') {
                i++;
                while (file_cont[i] != "") {
                    str = "";
                    str = str + file_cont[i][0] + file_cont[i][1] + file_cont[i][2] + file_cont[i][3];

                    if (str == val_name)
                    {
                        count = str.size() + 3;
                        str = "";
                        while (file_cont[i][count] != NULL) {
                            str = str + file_cont[i][count];
                            count++;
                        }

                        str2 += str;
                        val_err = false;
                    }
                    if (i < file_cont.size() - 1) {
                        i++;
                    }
                    else break;

                }
                section_error = false;
            } 

        }
        if(section_error == true) throw std::runtime_error("Данной секции не существует");
        else if (val_err == true) throw std::runtime_error("Данной переменной не существует");
        return str2;
    }

    ~ini_parser() {
        file.close();
        std::cout << "Деструктор создан" << std::endl;
    }

};


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");
    try {
        std::string name_file = "";
        std::string section = "";
        std::string var = "";

        std::cout << "Введите название файла из перечисленных: \nfile\n";
        std::cin >> name_file;
        ini_parser parser(name_file);

        std::cout << "Введите название секции из перечисленных \nSection1 \nSection2 \nSection3 \nSection4 \n";
        std::cin >> section;
        if (section == "Section3") std::cout << "Извините данная секция является пустой\n";
        while (section == "Section3") {
            std::cout << "Введите название секции из перечисленных \n Section1 \n Section2 \n Section3 \n Section4 \n";
            std::cin >> section;
            if (section == "Section3") std::cout << "Данная секция является пустой";
        }
       
        if (section == "Section1") {

            std::cout << "\n" << "Введите название переменной из перечисленных \nvar1 \nvar2 \nvar3\n";
        }
        else if (section == "Section2") {

            std::cout << "\n" << "Введите название переменной из перечисленных \nvar1 \nvar2 \n";
        }
        else if (section == "Section4") {

            std::cout << "\n" << "Введите название переменной из перечисленных \nMode \nVid\n";
        }
        std::cin >> var;
        if (var == "var1") {
            auto el = parser.get_value<float>(section, var);
            std::cout << "\n" << "Найден элемент: " << el << std::endl;
        }
        else {
            auto el = parser.get_value<std::string>(section, var);
            std::cout << "\n" << "Найден элемент: " << el << std::endl;
        }
    }
    catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
