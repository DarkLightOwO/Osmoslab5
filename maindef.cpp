#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <windows.h>

int main() {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int PACKET_LENGTH = 250;
    int i, j;
    srand(time(NULL));
    double fin[8];

    for (int hh = 0; hh<8; hh++) {
        int CRC_LENGTH = hh+1;
        for(int hhh = 0; hhh < 10; hhh++) {
            int packet[PACKET_LENGTH + CRC_LENGTH];
            int call_packet[PACKET_LENGTH + CRC_LENGTH];
            int generator[] = {1, 0, 1, 1, 1, 0, 1, 1}; // порождающий полином G для делителя: 10111011


        // Создание рандомного пакета данных

            for (i = 0; i < PACKET_LENGTH; i++) {
                packet[i] = rand() % 2;
                call_packet[i] = packet[i];
            }


        // Добавление нулей для вычисления CRC
            for (i = PACKET_LENGTH; i < PACKET_LENGTH + CRC_LENGTH; i++) {
                packet[i] = 0;
            }

        // Вычисление CRC
            for (i = 0; i < PACKET_LENGTH; i++) {
                if (packet[i] == 1) {
                    for (j = 0; j < CRC_LENGTH + 1; j++) {
                        packet[i+j] = packet[i+j] ^ generator[j];
                    }
                }
            }

        // Вывод полученного значения CRC
            printf("Значение CRC: ");
            for (i = PACKET_LENGTH; i < PACKET_LENGTH + CRC_LENGTH; i++) {
                printf("%d", packet[i]);
            }
            printf("\n");

            for (int y = 0; y < PACKET_LENGTH + CRC_LENGTH; y++) {
                call_packet[y] = call_packet[y] + packet[y];
                }

        // Искажение пакета данных и CRC
            int errors_detected = 0;
            int errors_undetected = 0;

            for (i = 0; i < PACKET_LENGTH + CRC_LENGTH; i++) {
                int bit = call_packet[i];
                call_packet[i] = !bit; // инвертирование бита

                int remainder = 0;
                for (j = 0; j < PACKET_LENGTH; j++) {
                    if (call_packet[j] == 1) {
                        for (int k = 0; k < CRC_LENGTH + 1; k++) {
                            call_packet[j+k] = call_packet[j+k] ^ generator[k];
                        }
                    }
                }

                for (int h = 0; h < PACKET_LENGTH + CRC_LENGTH; h++) {
                    if (call_packet[h] == 1) {
                        remainder += 1;
                    }
                }
                if (remainder == 0) {
                    errors_undetected++;

                } else {
                    errors_detected++;
                }
                call_packet[i] = bit; // восстановление исходного бита
            }

        // Вывод отчета об ошибках
            printf("Количество обнаруженных ошибок: %d\n", errors_detected);
            printf("Количество необнаруженных ошибок: %d\n", errors_undetected);

            double persentage = (errors_undetected / (PACKET_LENGTH + CRC_LENGTH)) * 100;

            fin[hh]+=errors_undetected;
        }
        //fin[hh] = fin[hh] / 10;
    }
    for (i = 0; i < 8; i++) {
        printf("При Crc = %d - %lf %\n", i+1, fin[i]/(PACKET_LENGTH)*10);
    }


    return 0;
}
