#include <iostream>
#include <vector>
#include <fstream>
#include <string>

void keepLastMeasurement(const std::string& fileName) 
{
    std::vector<double> tempData = { };

    double value;

    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Greska prilikom otvaranja." << '\n';
        return;
    }

    while (in >> value) {
        tempData.push_back(value);
    }

    in.close();

    if (tempData.empty()) {
        std::cout << "Datoteka " << fileName << " nema mjerenja." << '\n';
        return;
    }

    double lastValue;
    lastValue = tempData.back();

    std::ofstream out(fileName, std::ios::trunc);

    if (!out) {
        std::cerr << "Greska prilikom otvaranja datoteke za pisanje." << '\n';
        return;
    }

    out << lastValue;

    out.close();
    std::cout << "Zadrzano je samo zadnje mjerenje u datoteci." << '\n';

}

int main()
{
    int numberOfNodes;
    double temperature;

    std::vector<std::string> mqttFiles;
    std::string mqttFolder = "C:\\mqtt_nodes\\";

    std::cout << "Unesite zeljeni broj cvorova: ";
    std::cin >> numberOfNodes;
    std::cout << '\n';

    if (numberOfNodes <= 0) {
        std::cout << "Greska prilikom kreiranja cvorova, minimalan broj cvorova je 1." << '\n';
        return 0;
    }

    for (int i = 1; i <= numberOfNodes; i++) {
        std::string nodeFileName = mqttFolder + "node" + std::to_string(i) + ".txt";
        mqttFiles.push_back(nodeFileName);
    }

    // std::vector<std::vector<double>> sensor_nodes(mqttFiles.size()); //mapiranje u jedan vector, velicina ovisi o mqttfiles vectoru

    for (int i = 0; i < mqttFiles.size(); i++) {
        std::ifstream in(mqttFiles[i]);

        if (!in) {
            std::cerr << "Datoteka ne postoji: " << mqttFiles[i] << '\n';
            std::ofstream out(mqttFiles[i], std::ios::trunc);

            if (!out) {
                std::cerr << "Greska, datoteku nije moguce kreirati." << '\n';
                return 1;
            }

            std::cout << "Datoteka " << mqttFiles[i] << " je uspjesno kreirana." << '\n';
        }  
    }

    std::cout << "Simulacija distribuiranog sustava za obradu senzorskih podataka" << '\n';
    std::cout << '\n';

    while (true) {

        std::vector<double> node_sums;
        std::vector<double> node_avg;

        double low_limit = 18.0;
        double high_limit = 28.0;
        double total_sum = 0;
        double total_avg = 0;
        int total_measurement = 0;
        int option;

        std::cout << "Odaberite zeljenu opciju." << '\n';
        std::cout << "1. Obradi postojece podatke" << '\n';
        std::cout << "2. Zadrzi samo zadnje mjerenje svakog cvora." << '\n';
        std::cout << "3. Izlaz iz programa." << '\n';
        std::cout << "Opcija: ";
        std::cin >> option;
        std::cout << '\n';

        if (option == 1) {
            std::vector<std::vector<double>> sensor_nodes(mqttFiles.size());

            for (int i = 0; i < mqttFiles.size(); i++) { //zajednicko otvaranje datoteke
                std::ifstream in(mqttFiles[i]);

                if (!in) {
                    std::cerr << "Greska prilikom otvaranja datoteke: " << mqttFiles[i] << '\n';
                    return 1;
                }

                while (in >> temperature) {
                    sensor_nodes[i].push_back(temperature);
                }
            }

            bool validData = true;
            //provjera cvora za kolicinu podataka
            for (int i = 0; i < sensor_nodes.size(); i++) {
                if (sensor_nodes[i].empty()) {
                    std::cout << "Cvor " << i+1 << " je prazan." << '\n';
                    validData = false;
                }
            }

            if (validData != true) {
                std::cout << "Obrada nije moguca. Cvorovi moraju imati barem jendo mjerneje." << '\n';
                continue;
            }
            std::cout << '\n';

            for (int i = 0; i < sensor_nodes.size(); i++) { //ispis pojedinih vrijednosti cvorova
                std::cout << "Vrijednost cvora " << i + 1 << ": ";
                for (double value : sensor_nodes[i]) {
                    std::cout << value << " ";
                }
                std::cout << '\n';
            }

            std::cout << '\n';

            for (int i = 0; i < sensor_nodes.size(); i++) {
                double current_node_sum = 0;

                for (double values : sensor_nodes[i]) {
                    current_node_sum += values;
                }

                node_sums.push_back(current_node_sum);

                double current_node_avg = current_node_sum / sensor_nodes[i].size();
                node_avg.push_back(current_node_avg);
            }
            //suma i prosjek
            for (int i = 0; i < sensor_nodes.size(); i++) {
                std::cout << "Suma temperatura " << i + 1 << ". cvora: " << node_sums[i] << '\n';
                std::cout << "Prosjek temperatura " << i + 1 << ". cvora: " << node_avg[i] << '\n';
            }

            std::cout << '\n';

            for (int i = 0; i < sensor_nodes.size(); i++) {
                total_sum += node_sums[i];
                total_measurement += sensor_nodes[i].size();
            }

            total_avg = total_sum / total_measurement;

            std::cout << "Ukupna suma temperatur svih cvorova: " << total_sum << '\n';
            std::cout << "Ukupan broj svih mjerenja: " << total_measurement << '\n';
            std::cout << "Ukupan prosjek svih mjerenja: " << total_avg << '\n';

            std::cout << '\n';

            std::vector<double> min_temps;
            std::vector<double> max_temps;

            for (int i = 0; i < sensor_nodes.size(); i++) {
                double current_min = sensor_nodes[i][0];
                double current_max = sensor_nodes[i][0];

                for (double values : sensor_nodes[i]) {

                    if (values < current_min) {
                        current_min = values;
                    }

                    if (values > current_max) {
                        current_max = values;
                    }
                }

                min_temps.push_back(current_min);
                max_temps.push_back(current_max);
            }
            //minimum i maximum
            for (int i = 0; i < sensor_nodes.size(); i++) {
                std::cout << "Najmanja temperatura " << i + 1 << ". cvora: " << min_temps[i] << '\n';
                std::cout << "Najveca temperatura " << i + 1 << ". cvora: " << max_temps[i] << '\n';
            }

            std::cout << '\n';
            //upozorenje za niske i visoke temp
            for (int i = 0; i < sensor_nodes.size(); i++) {
                for (double values : sensor_nodes[i]) {
                    if (values < low_limit) {
                        std::cout << "Upozorenje, niska temperatura u " << i + 1
                            << ". cvoru: " << values << " C" << '\n';
                    }
                    else if (values > high_limit) {
                        std::cout << "Upozorenje, visoka temperatura u " << i + 1
                            << ". cvoru: " << values << " C" << '\n';
                    }
                }
            }
            std::cout << "-------------------------------------------------------------" << '\n';
        }

        else if (option == 2) {
            for (int i = 0; i < mqttFiles.size(); i++) {
                keepLastMeasurement(mqttFiles[i]);
            }

            std::cout << "Postupak je zavrsen." << '\n';
        }
        else if (option == 3) {
            std::cout << "Izlaz iz programa." << '\n';
            break;
        }
        else {
            std::cout << "Neispravan odabir." << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
