#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if _WIN64 || _WIN32
#define PAUSE system("pause")
#else
#define PAUSE system("echo press enter to continue; read dummy;")
#endif


void printBit(int n, int size){
	if(size != 1){
		printBit(n >> 1, --size);
	}
	(n&1) ? printf("1") : printf("0");
}

int returnNBit(int n, int bitToReturn) {
	for(; bitToReturn != 0; --bitToReturn) {
		n >>= 1;
	}
	return n&1;
}


/********************************** FUNZIONE PER CALCOLO BIT DI CONTROLLO **********************************/
void calcCheckBit(int mess, int messLen, int* checkBit, int checkBitLen) {
	for(int i = 0; i < checkBitLen; ++i) {
		int checkStart = 1 << i;
		int gap = 1 << i;
		int cont = checkStart;
		
		*checkBit <<= 1;
		
		while(cont <= messLen + checkBitLen) {
			if(cont != checkStart) {
				*checkBit ^= returnNBit(mess,  (messLen + checkBitLen) - cont);
			}
			
			--gap;
			if(gap != 0) {
				++cont;
			} else {
				gap = 1 << i;
				cont += gap + 1;
			}
		}
	}
}


void main() {
	int cont = 0;
	int mess = 0, messLen = 0;
	int checkBit = 0, checkBitLen = 0;
	int pos = 1; // controlla la posizione delle celle contenenti i bit di controllo

	srand(time(NULL));
	

	/********************************** RICHIESTA DATI **********************************/
	printf("inserisci il messaggio da codificare: ");
	char getNum;
	int checkMul = 1;
	do {
		getNum = getc(stdin);
		
		if(getNum - '0' == 0 || getNum - '0' == 1) {
			++messLen;
			++cont;
			
			while(cont == checkMul) {
				++checkBitLen;
				mess <<= 1;
				++cont;
				checkMul <<= 1;
			}
			
			mess <<= 1;
			mess |= getNum - '0';
		}
	} while(getNum != EOF && getNum != '\n');
	
	
	/********************************** CALCOLO BIT DI CONTROLLO **********************************/
	calcCheckBit(mess, messLen, &checkBit, checkBitLen);
	
	
	/********************************** INSERIMENTO BIT DI CONTROLLO **********************************/
	int j = 1;
	for(int i = checkBitLen-1; i >= 0; --i) {
		if(returnNBit(checkBit,  i)) {
			int mul = 1 << (messLen + checkBitLen) - j;
			mess |= mul;
		}
		j <<= 1;
	}
	
	
	/********************************** GENERAZIONE ERRORE **********************************/
	int messError = mess;
	
	if(rand() % 2) {
		messError |= 1 << rand() % (messLen + checkBitLen);
	} else {
		messError &= ~(1 << rand() % (messLen + checkBitLen));
	}
	
	
	/********************************** CONTROLLO/CORREZIONE ERRORE **********************************/
	int errorCheckBit = 0;
	int errorPos  = 0;
	cont = 0;
	calcCheckBit(messError, messLen, &errorCheckBit, checkBitLen);
	
	for(int i = checkBitLen - 1; i >= 0; --i) {
		if(returnNBit(errorCheckBit, i) != returnNBit(messError, ((messLen + checkBitLen) - (1 << cont)))) {
			errorPos += 1 << cont;
		}
		++cont;
	}
	
	
	/********************************** STAMPA DATI **********************************/
	printf("\nlunghezza del messaggio: %d\n", messLen);
	printf("numero bit di controllo: %d\n", checkBitLen);
	printf("lunghezza messaggio completo: %d\n", messLen + checkBitLen);
	
	printf("bit di controllo: ");
	printBit(checkBit, checkBitLen);
	
	printf("\n\nmessaggio: ");
	printBit(mess, messLen + checkBitLen);
	
	if(errorPos == 0) {
		printf("\n\nnessun errore nel messaggio inviato\n");
	} else {
		printf("\n\nerrore trovato in pos: %d\n", errorPos);
	}
	printf("messaggio con errore: ");
	printBit(messError, messLen + checkBitLen);
	
	printf("\n\n");
	PAUSE;
}
