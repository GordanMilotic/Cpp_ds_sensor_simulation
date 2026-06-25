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
        std::cout << "Datoteka nema mjerenja." << '\n';
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
    double node_sum1 = 0;
    double node_sum2 = 0;
    double node_sum3 = 0;
    double node_avg1 = 0;
    double node_avg2 = 0;
    double node_avg3 = 0;
    double total_sum = 0;
    double total_avg = 0;
    int total_measurement = 0;
    double temperature;

    std::vector<double> sensor_node1{ };
    std::vector<double> sensor_node2{ };
    std::vector<double> sensor_node3{ };

    int option;
    std::cout << "Odaberite zeljenu opciju." << '\n';
    std::cout << "1. Obradi postojece podatke" << '\n';
    std::cout << "2. Obrisi podatke iz cvorova osim zadnjeg." << '\n';
    std::cout << "Opcija: ";
    std::cin >> option;
    std::cout << '\n';

    if (option == 2) {
        keepLastMeasurement("C:\\mqtt_nodes\\node1.txt");
        keepLastMeasurement("C:\\mqtt_nodes\\node2.txt");
        keepLastMeasurement("C:\\mqtt_nodes\\node3.txt");

        std::cout << "Datoteke su ociscene. Preostalo je samo zadnje mjerenje." << '\n';
        return 0;
    }
    else if (option != 1) {
        std::cout << "Neispravan odabir." << '\n';
        return 0;
    }

    std::ifstream in1("C:\\mqtt_nodes\\node1.txt");
    if (!in1) {
        std::cerr << "Failed to open node1.txt" << '\n';
        return 1;
    }

    while (in1 >> temperature) {
        sensor_node1.push_back(temperature);
    }

    std::ifstream in2("C:\\mqtt_nodes\\node2.txt");
    if (!in2) {
        std::cerr << "Failed to open node2.txt" << '\n';
        return 1;
    }

    while (in2 >> temperature) {
        sensor_node2.push_back(temperature);
    }

    std::ifstream in3("C:\\mqtt_nodes\\node3.txt");
    if (!in3) {
        std::cerr << "Failed to open node3.txt" << '\n';
        return 1;
    }

    while (in3 >> temperature) {
        sensor_node3.push_back(temperature);
    }
    

    double low_limit = 18.0;
    double high_limit = 28.0;


    std::cout << "Simulacija distribuiranog sustava za obradu senzorskih podataka" << '\n';
    std::cout << '\n';

    double min_temp1 = sensor_node1[0];
    double min_temp2 = sensor_node2[0];
    double min_temp3 = sensor_node3[0];
    double max_temp1 = sensor_node1[0];
    double max_temp2 = sensor_node2[0];
    double max_temp3 = sensor_node3[0];

    std::cout << "Cvor 1: ";

    for (double values : sensor_node1) {
        std::cout << values << " ";
    }
    std::cout << '\n';

    std::cout << "Cvor 2: ";

    for (double values : sensor_node2) {
        std::cout << values << " ";
    }
    std::cout << '\n';

    std::cout << "Cvor 3: ";

    for (double values : sensor_node3) {
        std::cout << values << " ";
    }
    std::cout << '\n';
    std::cout << '\n';

    for (double values : sensor_node1) {
        node_sum1 += values;
    }
    std::cout << "Suma temperatura 1. cvora: " << node_sum1 << '\n';

    for (double values : sensor_node2) {
        node_sum2 += values;
    }
    std::cout << "Suma temperatura 2. cvora: " << node_sum2 << '\n';

    for (double values : sensor_node3) {
        node_sum3 += values;
    }
    std::cout << "Suma temperatura 3. cvora: " << node_sum3 << '\n';
    std::cout << '\n';

    node_avg1 = node_sum1 / sensor_node1.size();
    std::cout << "Prosjek temperatura 1. cvora: " << node_avg1 << '\n';

    node_avg2 = node_sum2 / sensor_node2.size();
    std::cout << "Prosjek temperatura 2. cvora: " << node_avg2 << '\n';

    node_avg3 = node_sum3 / sensor_node3.size();
    std::cout << "Prosjek temperatura 3. cvora: " << node_avg3 << '\n';
    std::cout << '\n';

    total_sum = node_sum1 + node_sum2 + node_sum3;
    std::cout << "Ukupna suma temperatura svih cvorova: " << total_sum << '\n';

    total_measurement = sensor_node1.size() + sensor_node2.size() + sensor_node3.size();
    std::cout << "Ukupan broj mjerenja: " << total_measurement << '\n';

    total_avg = total_sum / total_measurement;
    std::cout << "Ukupan prosjek svih cvorova: " << total_avg << '\n';
    std::cout << '\n';

    for (double values : sensor_node1) {
        if (values < min_temp1) {
            min_temp1 = values;
        }
    }

    std::cout << "Najmanja temperatura 1. cvora: " << min_temp1 << '\n';

    for (double values : sensor_node2) {
        if (values < min_temp2) {
            min_temp2 = values;
        }
    }
    std::cout << "Najmanja temperatura 2. cvora: " << min_temp2 << '\n';

    for (double values : sensor_node3) {
        if (values < min_temp3) {
            min_temp3 = values;
        }
    }
    std::cout << "Najmanja temperatura 3. cvora je: " << min_temp3 << '\n';
    std::cout << '\n';

    for (double values : sensor_node1) {
        if (values > max_temp1) {
            max_temp1 = values;
        }
    }
    std::cout << "Najveca temperatura 1. cvora je: " << max_temp1 << '\n';

    for (double values : sensor_node2) {
        if (values > max_temp2) {
            max_temp2 = values;
        }
    }
    std::cout << "Najveca temperatura 2. cvora je: " << max_temp2 << '\n';

    for (double values : sensor_node3) {
        if (values > max_temp3) {
            max_temp3 = values;
        }
    }
    std::cout << "Najveca temperatura 3. cvora je: " << max_temp3 << '\n';
    std::cout << '\n';

    for (double values : sensor_node1) {
        if (values < low_limit) {
            std::cout << "Upozorenje za nisku temperaturu u 1. cvoru: " << values 
                      << " (Min temp: " << low_limit << " C)" << '\n';
        }
        else if (values > high_limit) {
            std::cout << "Upozorenje za visoku temperaturu u 1. cvoru: " << values 
                      << " (Max temp: " << high_limit << " C)" << '\n';
        }
    }

    for (double values : sensor_node2) {
        if (values < low_limit) {
            std::cout << "Upozorenje za nisku temperaturu u 2. cvoru: " << values 
                      << " (Min temp: " << low_limit << " C)" << '\n';
        }
        else if (values > high_limit) {
            std::cout << "Upozorenje za visoku temperaturu u 2. cvoru: " << values 
                      << " (Max temp: " << high_limit << " C)" << '\n';
        }
    }

    for (double values : sensor_node3) {
        if (values < low_limit) {
            std::cout << "Upozorenje za nisku temperaturu u 3. cvoru: " << values
                << " (Min temp: " << low_limit << " C)" << '\n';
        }
        else if (values > high_limit) {
            std::cout << "Upozorenje za visoku temperaturu u 3. cvoru: " << values
                << " (Max temp: " << high_limit << " C)" << '\n';
        }
    }
    std::cout << '\n';

    

    return 0;
}
