#include "cpu.h"
#include <string.h>

int CU() {
    int value = 0;
	int command = 0;
	int operand = 0;
	int local_value = 0;

	sc_memoryGet(inst_counter, &value);

	if (sc_commandDecode(value, &command, &operand)) {
		sc_regSet(E, 1);
		sc_regSet(T, 1);
		return 1;
	}
    if (command >= 0x30 && command <= 0x33) {
		ALU(command, operand);
	} else {
        int f;
        switch (command) {
            case (READ):
				printf("> ");
            	char inputValue[30]; //строка введенного значения
             	scanf("%s", inputValue);
            	int check = 0, num = 0;
            	if(inputValue[0] == '+' || inputValue[0] == '-') check = 1; //если есть знак + или - перед числом то смещаемся на один символ
            	for(int i = check; i < strlen(inputValue); i++){ //цикл до количества цифр
					if(!(inputValue[i] >= '0' && inputValue[i] <= '9')) //проверяем все ли символы - цифры
						f = 1; //
				}
				num = atoi(inputValue);
				printf("num10 %d == num16 %x",num,num);
				fflush(stdout);
				sleep(5); //конверация к целочисленному типу
				if(!check && num <= 8191) //если это положительное число и оно не выходит за рамки 14 бит
				{	printf("ONE");
					sc_memorySet(operand, ((num & 8191))); //записываем в память и добавляем в начало 1 для обозначения признака команды
				}
				if(check && num < 0 && abs(num) <= 8191) //если оно отрицательное и не выходит за границы
				{	printf("TWO");
					sc_memorySet(operand,((abs(num) & 8191) + 24576)); //записываем в память и добавляем две единицы в начало для признака команды и знака
				}
				if(check && num > 0 && num <= 16383) //если это команда
					{printf("THREE");
					sc_memorySet(operand, ((num & 8191) + ( 1 << 14))); //записываем в память
					}
					if(!f) inst_counter++;
				break;
			case (WRITE):
				printf("< ");
				sc_memoryGet(operand, &local_value);
				if((local_value >> 13) & 1) { //если число отрицательное
					printf("-%d\n", local_value & 8191); //выводим с минусом
				} else
				{
					printf("%d\n", local_value & 8191);
					fflush(stdout);
					sleep(5);
				}
				break;
			case (LOAD):
				sc_memoryGet(operand, &accumulator);
				break;
			case (STORE):
				sc_memorySet(operand, accumulator);
				accumulator = 0;
				break;
			case (JUMP):
				if((operand - 1 < 0) || (operand - 1 > 99 ))
				{
					sc_regSet(M,1);
				}
				inst_counter = operand - 1;
				break;
			case (JNEG):
				if((accumulator >> 14) == 1){
					if(((accumulator >> 13 ) & 1) == 1){
						if(operand-1 < 64){
							inst_counter=operand-1;
						}
						else{
							sc_regSet(M, 1);
						}
					}
				}
				break;
			case (JZ):
				if((accumulator >> 14) == 1){
					if(((accumulator >> 13) & 1) == 0){
						if((accumulator & 8191) == 0){
							if((operand-1) < 64){
								inst_counter = operand - 1;
							}
							else{
								sc_regSet(M, 1);
							}
						}
					}
				}
				break;
			case (HALT):
				sc_regSet(T, 0);
				return 1;
				break;
        }
    }
}

void func(int a, int b, int operand) { //функция сложения
    if (a == b) { //если знаки совпадают
        if ((accumulator & 8191) + (mem[operand] & 8191) > 8191) //если сложение двух значений выходит за рамки 14 бит
            sc_regSet(FLAG_OVERFLOW, 1); //поднимаем флаг переполнения стека
        accumulator = abs(((accumulator & 8191) + (mem[operand] & 8191)) & 8191) + (1 << 14) + (a << 13); //запись в аккумулятор модуля сложения и добавление признаков команды и знака
    } else { //если знаки не совпадают
        if ((accumulator & 8191) > (mem[operand] & 8191)) //если значение в аккумуляторе отрицательное
            accumulator = abs((accumulator & 8191) - (mem[operand] & 8191)) + (1 << 14) + (a << 13); //разность значений и добавление признаков команды и знака
        else if ((accumulator & 8191) < (mem[operand] & 8191)) { //если значение в ячейке памяти отрицательное
            accumulator = abs((accumulator & 8191) - (mem[operand] & 8191)) + (1 << 14); //разность значений и добавление признака команды
            if (a == 1) //если значение аккумулятора отрицательное
                accumulator &= ~(1 << 13); //то оно станет положительным
            else
                accumulator |= 1 << 13; //иначе оно станет отрицательным
        } else
            accumulator = 1 << 14; //значение аккумулятора равно нулю
    }
}

int ALU(int command, int operand) {
	int local_value, a, b;
	sc_memoryGet(operand, &local_value);
	switch(command){
		case ADD:
			if(local_value>>14==1 && accumulator>>14==1){
				b=local_value>>13&1;
				a=accumulator>>13&1;
				if(a==b){
					if ((accumulator & 8191) + (local_value & 8191) > 8191) {
						sc_regSet(P, 1);
						return -1;
					}
					else{
						accumulator = (accumulator & 8191) + (local_value & 8191) + (1 << 14) + (a << 13);
						break;
					}
				}
				else{
					if((accumulator & 8191)>(local_value & 8191)){
						accumulator = (accumulator & 8191) - (local_value & 8191) + (1<<14) + (a<<13);
						break;
					}
					if((accumulator & 8191)<(local_value & 8191)){
						accumulator = (local_value & 8191) - (accumulator & 8191) + (1<<14) + (b<<13);
						break;
					}
					if((accumulator & 8191)==(local_value & 8191)){
						accumulator = (local_value & 8191) - (accumulator & 8191) + (1<<14);
						break;
					}
				}
			}
			return -1;
		case SUB:
			if(local_value>>14==1 && accumulator>>14==1){
				a=accumulator>>13&1;
				b=local_value>>13&1;
				if(a==b){
					if((accumulator & 8191)>(local_value & 8191)){
						accumulator = (accumulator & 8191) - (local_value & 8191) + (1<<14) + (a<<13);
						break;
					}
					if((accumulator & 8191)<(local_value & 8191)){
						accumulator = (local_value & 8191) - (accumulator & 8191) + (1<<14) + (b<<13);
						break;
					}
					if((accumulator & 8191)==(local_value & 8191)){
						accumulator = (local_value & 8191) - (accumulator & 8191) + (1<<14);
						break;
					}
				}
				else{
					if((accumulator & 8191) + (local_value & 8191)> 8191){
						sc_regSet(P, 1);
						return -1;
					}
					else{
						accumulator = (accumulator & 8191) + (local_value & 8191) + (1<<14) + (a<<13);
						break;
					}
				}
			}
			return -1;
	}
}