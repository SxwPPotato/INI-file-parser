#include <iostream>
#include <fstream>
#include <algorithm>
#include<Windows.h>
#include <string>
#include <vector>
#include <map>

class ini_parser {
private:
    std::ifstream file;
    std::string q;
    
    std::vector<std::string> file_cont;

    std::map<std::string, std::string> map_string;

    std::string str = "";// элемент для строк
    std::string str2 = "";
    bool section_error = true; // исключений для секций
    bool val_err = true; // исключений для переменных

    int count = 0;

    std::string get_value_string(std::string section, std::string var) {
        std::string el = "";
        if (!map_string.empty() ) {
            if (map_string.find(var) != map_string.end()) {
                return map_string[var];
            }

        }
       
        for (int i = 0; i < file_cont.size() - 1; i++) {

            if (file_cont[i] == '[' + section + ']') {

                i++;
                while (file_cont[i] != "") {
                    str = "";
                    str = str + file_cont[i][0] + file_cont[i][1] + file_cont[i][2] + file_cont[i][3];

                    if (str == var)
                    {
                        el = "";
                        count = 0;
                        count = str.size() + 3;
                        while (file_cont[i][count] != NULL) {
                            if (file_cont[i][count] == '.') {
                                el = el + ',';

                                count++;
                            }
                            else {
                                el = el + file_cont[i][count];
                                count++;
                            }
                        }

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
        else if (val_err == true) throw std::runtime_error("Данной переменной не существует");

        map_string[var] = el;

        return el;
    }


public:

    template<class T>
    T get_value(std::string section, std::string var) {
        static_assert(sizeof(T) == -1, "not implemented type for get_value");
    }



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

    ~ini_parser() {
        file.close();
        std::cout << "Деструктор создан" << std::endl;
    }

};


template<>
std::string ini_parser::get_value(std::string section, std::string var) {
    return get_value_string(section, var);
}

template<>
int ini_parser::get_value(std::string section, std::string var) {
    std::string str_val = get_value_string(section, var);

    return stoi(str_val);
}

template<>
float ini_parser::get_value(std::string section, std::string var) {
    std::string str_val = get_value_string(section, var);

    return stof(str_val);
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");
    try {
        std::string name_file = "";
        std::string section = "";
        std::string var = "";

      /*  std::cout << "Введите название файла из перечисленных: \nfile\n";
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
        }*/

        std::cout << "\nTest section\n";

        ini_parser parser("file");

        std::cout << parser.get_value<float>("Section1", "var1") << std::endl;
        std::cout << parser.get_value<int>("Section1", "var1") << std::endl;
        std::cout << parser.get_value<std::string>("Section1", "var2") << std::endl;
        std::cout << parser.get_value<std::string>("Section1", "var2") << std::endl;
    }
    catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
