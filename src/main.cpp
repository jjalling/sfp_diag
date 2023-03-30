#include <cerrno>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

#pragma pack(push, 1)
struct sff8472_info {
    uint8_t identifier;
    uint8_t ext_identifier;
    uint8_t connector;
    uint8_t bytes[8];
    uint8_t encoding;
    uint8_t signal_rate;
    uint8_t rate_identifier;
    uint8_t length_km;
    uint8_t length_10;
    uint8_t length_om2_10;
    uint8_t length_om1_10;
    uint8_t length_om4_10;
    uint8_t length_om3_10;
    char vendor[16];
    uint8_t transceiver;
    uint8_t vendor_oui[3];
    char vendor_pn[16];
    uint8_t vendor_rev[4];
    uint16_t wavelength;
    uint8_t fiber_channel_speed;
    uint8_t cc_base;
    uint8_t res2[4];
    char serial[16];
    char date[8];

};

struct sff8472_diag {
    uint8_t res[96];
    int16_t Temperature;
    uint16_t voltage;
    uint16_t tx_bias;
    uint16_t tx_power;
    uint16_t rx_power_avg;
};
#pragma pack(pop)

int main(int argc, char *argv[])
{
    sff8472_info info;
    sff8472_diag diag;
    std::fstream file;
    std::string i2c_path;

    if (argc == 2)
        i2c_path = std::string(argv[1]);
    else
        i2c_path = "/sys/bus/i2c/devices";

    std::string filename(i2c_path + "/0-0050/eeprom");
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << filename << " open failed: " << strerror(errno) << std::endl;
        return EPERM;
    }
	file.read(reinterpret_cast<char *>(&info), sizeof(info));
    file.close();

    filename = i2c_path + "/0-0051/eeprom";
    file.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << filename << " open failed: " << strerror(errno) << std::endl;
        return EPERM;
    }
	file.read(reinterpret_cast<char *>(&diag), sizeof(diag));
    file.close();

    std::string vendor(info.vendor,16);
    std::string pn(info.vendor_pn,16);
    std::string serial(info.serial,16);
    std::string date(info.date,8);

#if 0
    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "Part number: " << pn << std::endl;
    std::cout << "Serial number: " << serial << std::endl;
    std::cout << "Date code: " << date << std::endl;
    std::cout << "Nominal speed: " << (int)info.signal_rate*100 << " [Mbit/s]" << std::endl;
    std::cout << "Wavelength: " << __builtin_bswap16(info.wavelength) << " [nm]" << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Temperature: " << (int16_t)__builtin_bswap16(diag.Temperature)*(1.0/256) << " [C]" << std::endl;
    std::cout << "Voltage: " << __builtin_bswap16(diag.voltage) / 10000.0 << " [V]" << std::endl;
    std::cout << "TX Bias current: " << (__builtin_bswap16(diag.tx_bias) / 1000.0) << " [mA]" << std::endl;
    std::cout << "TX Power: " << 10 * log10(__builtin_bswap16(diag.tx_power) / 10000.0) << " [dBm]" << std::endl;
    std::cout << "RX Power (avg.): " << 10 * log10(__builtin_bswap16(diag.rx_power_avg) / 10000.0) << " [dBm]" << std::endl;
#else
    std::cout << "\"Vendor\";" << vendor << std::endl;
    std::cout << "\"Part number\";" << pn << std::endl;
    std::cout << "\"Serial number\";" << serial << std::endl;
    std::cout << "\"Date code\";" << date << std::endl;
    std::cout << "\"Nominal speed\";" << (int)info.signal_rate*100 << ";Mbps" << std::endl;
    std::cout << "\"Wavelength\";" << __builtin_bswap16(info.wavelength) << ";nm" << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\"Temperature\";" << (int16_t)__builtin_bswap16(diag.Temperature)*(1.0/256) << ";C" << std::endl;
    std::cout << "\"Voltage\";" << __builtin_bswap16(diag.voltage) / 10000.0 << ";V" << std::endl;
    std::cout << "\"TX Bias current\";" << (__builtin_bswap16(diag.tx_bias) / 1000.0) << ";mA" << std::endl;
    std::cout << "\"TX Power\";" << 10 * log10(__builtin_bswap16(diag.tx_power) / 10000.0) << ";dBm" << std::endl;
    std::cout << "\"RX Power (avg.)\";" << 10 * log10(__builtin_bswap16(diag.rx_power_avg) / 10000.0) << ";dBm" << std::endl;
#endif
    return 0;
}

