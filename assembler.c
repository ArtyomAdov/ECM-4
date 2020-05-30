#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getCommand(char* cmd) {
	if(!strcmp(cmd, "READ")) return 0x10;
	if(!strcmp(cmd, "WRITE")) return 0x11;
	if(!strcmp(cmd, "LOAD")) return 0x20;
	if(!strcmp(cmd, "STORE")) return 0x21;
	if(!strcmp(cmd, "ADD")) return 0x30;
	if(!strcmp(cmd, "SUB")) return 0x31;
	if(!strcmp(cmd, "DIVIDE")) return 0x32;
	if(!strcmp(cmd, "MUL")) return 0x33;
	if(!strcmp(cmd, "JUMP")) return 0x40;
	if(!strcmp(cmd, "JNEG")) return 0x41;
	if(!strcmp(cmd, "JZ")) return 0x42;
	if(!strcmp(cmd, "HALT")) return 0x43;
	if(!strcmp(cmd, "SUBC")) return 0x66;
	return -1;
}

int main(int argc, char* argv[])
{
	if(argc != 3) { //если количество аргументов не равно 3 (кол-во аргументов, файл.sa файл.o)
		printf("2 arguments needed\nsat <inputFile.sa> <outputFile.o>\n");
		return -1;
	}
    
	char *p = strrchr(argv[1], '.'); //проверка расширения файла
	if (p == NULL || strlen(argv[1]) <= 3 || strcmp(p, ".sa") != 0) //если нет расширения или нет имени файла или расширение не соответствует
        printf("FIRST ARG ERROR\n"); //ошибка
	p = strrchr(argv[2], '.'); //проверка расширения
	if (p == NULL || strlen(argv[2]) <= 2 || strcmp(p, ".o") != 0)
        printf("SECOND ARG ERROR\n");

	FILE *input, *output;

	if((input = fopen(argv[1], "r")) == NULL) //проверка открытия файлов
        printf("FIRST FILE ERROR"); //ошибка
	if((output = fopen(argv[2], "wb+")) == NULL)
        printf("SECOND FILE ERROR"); //ошибка
    while(!feof(input)){ //чтение первого файла
		int a, b;
		char str[10], in[3], out[10], s[1024];
		fgets(s, 1024, input); //считывание из файла в массив символов
		int check = sscanf(s, "%s %s %s ", in, str, out); //количество полей считавшихся из файла
		if(check != 3 || strlen(in) != 2 || !(strlen(out) == 2 || strlen(out) == 5)) { //если поля не 3, длина адреса не 2, длина операнда не 2, или длина операнда равена 5
            printf("ERROR WRONG INPUT %s %s %s \n", in, str, out); //ошибка
            break;
        }
		int i;
		for( i = 0; i < strlen(in); i++)
            if(!(in[i] >= '0' && in[i] <= '9')) { //проверка, что в адресе находятся все цифры
                printf("ERROR FIRST ARGUMENT INPUT\n"); //оишбка
                break;
            }
//		if(strlen(out) == 2) { //если
//			if(strcmp(str, "=") == 0) {
//                printf("ERROR WRONG EQUEL\n");
//                break;
//            }
//			for(int i = 0; i < strlen(out); i++)
//                if(!(out[i] >= '0' && out[i] <= '9')) {
//                    printf("ERROR WRONG OUT1\n");
//                    break;
//                }
//		} else {
//			if(strcmp(str, "=") != 0) {
//                printf("ERROR DONT FIND EQUEL\n");
//                break;
//            }
//			if(out[0] != '+') {
//                printf("ERROR NOT FIND +\n");
//                break;
//            }
//			for(int i = 1; i < strlen(out); i++)
//                if(!((out[i] >= '0' && out[i] <= '9') || (out[i] >= 'A' && out[i] <= 'F'))) {
//                    printf("ERROR WRONT OUT2\n");
//                    break;
//                }
//		}
//		if(strlen(str) == 1) {
//			if(str[0] != '=') {
//                printf("ERROR EQUEL COMMAND\n");
//                break;
//            }
//		}
//		else if(getCommand(str) == -1) {
//            printf("ERROR COMMAND\n");
//            break;
//        }
//
//		/*char* ch = strchr(s,';');
//		if(ch==NULL){printf("ERROR NO COMMENT\n");break;}*/
        
        
		a = atoi(in); //конвертация адреса в целочисленный тип
//		if(strlen(out) == 5) {
//			b = (int)strtol(out, NULL, 16);
//			if(b < 0x0000 || b > 0x3fff) {
//                printf("ERROR WRONG NUMBER\n");
//                break;
//            }
//		}
		b = atoi(out);
		int data[3]; //массив из трех полей (адрес, команда, операнд)
		data[0] = a; //запись адреса
		data[1] = getCommand(str); //запись команды
		data[2] = b; //запись операнда

		fwrite(data, sizeof(int) * 3, 1, output); //запись в объектный файл
	}
    
	fclose(input);
	fclose(output);


    return 0;
}
