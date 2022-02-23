#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h> 

int formatting_entry (char* strhexa){
    char copy[4];
    int i;

    memset(copy,'0',4);
    i = 0;
 
   while (i < 4){

        if (strhexa[i] == 0){

            switch (i)
                {
                case 0:
                    strcpy(strhexa,copy);
                    return 2;
            
                case 1:
                    copy[3] = strhexa[0];                                    
                    strcpy(strhexa,copy);
                    return 3;

                case 2:
                    copy[2] = strhexa[0];
                    copy[3] = strhexa[1];
                    strcpy(strhexa,copy);
                    return 4;

                case 3:
                    copy[1] = strhexa[0];
                    copy[2] = strhexa[1];
                    copy[3] = strhexa[2];
                    strcpy(strhexa,copy);
                    return 5;
            }        
    
        }
        i++;
    }
    return 1;
}

int is_a_hexa (char* strhexa){
    int i;
    char copy[4];

    i = 0;
    if (strhexa[0] == '0' && strhexa[1] == 'x'){
        i = 2;
    }
   
    while (i < strlen(strhexa)){

        if (strhexa[i] <= 'f' && strhexa[i] >= 'a'){
            strhexa[i] = toupper(strhexa[i]);
        }

        if (strhexa[i] < '0' || strhexa[i] > 'F'){

            if (strhexa[i] != 0){
                printf("Valeur non-hexadécimale en paramètre \n");
                return 1;
            }
        }     

        if (strhexa[i] < 'A' && strhexa[i] > '9'){ 
            printf("Valeur non-hexadécimale en paramètre \n");
            return 1;
        }
        i++;
    }

    if (strhexa[6] != 0){ 
        printf("Paramètre non-représentable sur 16bit\n");
        return 1;
    }

    if (strhexa[0] == '0' && strhexa[1] == 'x'){ 
        i = 0;
        while (i < 4) {
            copy[i] = strhexa[i+2];
            i++;
        }
        strcpy(strhexa,copy);
    }

    else if (strhexa[4] != 0){ 
            printf("Paramètre non-représentable sur 16bit\n");
            return 1;
        }

    return 0;
}

int reverse_binary (char* binary){
    char inverse[4];
    int i1;
    int i;

    i1 = 3;
    i = 0;

    strcpy(inverse,binary);

    while (i < 4){
        binary[i] = inverse[i1];
        i1--;
        i++;
    }
    return 1;
}

int hexa_to_binary (char* tab,char* strhexa){

    if (is_a_hexa(strhexa)){
        return 1;
    }

    formatting_entry(strhexa);

    int a;
    int i;
    int i1;
    char *tabinverse;

    tabinverse = malloc(4*sizeof(char));
    i = 0;

    while (i < 4){

        if (strhexa[i] < 'G' && strhexa[i] > '@'){
            a = strhexa[i] - '0' - 7;
        }
        else {
            a = strhexa[i] - '0'; 
        }

        memset(tabinverse,0,4);
        i1 = 0;

        while (i1 < 4){
            tabinverse[i1] = a%2 +'0';
            a = a/2;
            i1++;
        }
        reverse_binary (tabinverse);
        i++;
    }
    free(tabinverse);
    return 0;
}

int binary_to_integer (char* binary){

    int somme;
    int i;

    i = 0;

    while (i < 16){
        somme = somme + (( binary[i] - '0') * pow(2,15 - i));
        i++;
    }

    return somme;
}

char pl_NAND (char A, char B){
    if (A == '1' && B == '1'){
        return '0';
    }
    return '1';     
}

char pl_NOR (char A, char B){
    if (A == '0' && B == '0'){
        return '1';
    }
    return '0';
}

char pl_XOR (char A, char B){

    char C;

    C = (pl_NAND (pl_NOR (pl_NOR(A,B),pl_NOR(A,B)) , pl_NAND(A,B)));

    return pl_NAND (C,C);
}

char pl_AND (char A, char B){

    return pl_NAND(pl_NAND (A,B) , pl_NAND(A,B));
}

char pl_NOT (char A){

    return pl_NAND (A,A);
}

char pl_OR (char A, char B){

    return pl_NAND(pl_NOR (A,B) , pl_NOR (A,B));
}

char add_1b (char A,char B,char Cin,char *Cout){

    char retenue;

    retenue = pl_AND(Cin,pl_XOR(A,B));
    *Cout = pl_OR(pl_AND(A,B),retenue);

    return pl_XOR(Cin,pl_XOR(A,B));
}

char add_16b (char* A, char* B, char* sum){

    char* Cout;
    char Cin;
    int i;

    Cout = malloc(1);
    *Cout = '0';
    i = 15;

    while (i > -1){

        Cin = *Cout;
        sum[i] = add_1b(A[i],B[i],Cin,Cout);
        i--;
    }

    if (*Cout == '1'){
        return 1;

    }
    return 0;
}

int main(int argc,char* argv[]){

    char *binaire; 
    char *binaire1;
    char* hexa;
    char* hexa1;
    int error;
    int overflow;
    int result;
    char* sum;

    if (argc != 3){
        printf("Il faut 2 paramètres \n");
        return EXIT_FAILURE;
    }

    hexa = malloc(6*sizeof(char));
    hexa1 = malloc(6*sizeof(char));
    sum = malloc(16*sizeof(char));
    binaire = malloc(16*sizeof(char));
    binaire1 = malloc(16*sizeof(char));
    memset(binaire,0,16);
    memset(binaire1,0,16);

    strcpy(hexa,argv[1]);
    strcpy(hexa1,argv[2]);
   
    error = hexa_to_binary(binaire,hexa);
    error = hexa_to_binary(binaire1,hexa1) + error;
    overflow = add_16b(binaire,binaire1,sum);

    if (error != 0){
        return EXIT_FAILURE;
    }

    result = binary_to_integer(sum);

    printf("-------------------------------------------------------\n");
    printf("0x%x ",result); 
    printf("(%s) ",sum);
    printf("overflow : %d ",overflow);
    printf("error : %d\n",error);
    printf("-------------------------------------------------------\n");

    free(binaire);
    free(binaire1);
    free(sum);

    return EXIT_SUCCESS;
}