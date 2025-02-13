
#include "datafile.h"

DataFile::DataFile() {
    this->fs.begin();
    /*File flight_num_file = this->fs.open("flight_number.txt");
    char flight_num[10];
    flight_num_file.read(flight_num, sizeof(flight_num));
    int flight_num_int = String(flight_num).toInt();
    flight_num_int++;
    flight_num_file.seek(0);
    flight_num_file.println(flight_num_int);
    flight_num_file.close();

    Serial.print("Filename:");
    Serial.println(flight_num_int);*/

    this->file = this->fs.open("data");
}

void DataFile::write(String data) {
    this->file.println(data);
    this->file.flush();
}