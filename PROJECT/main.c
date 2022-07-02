#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


//---------------------------------------------------------
/*
DOSYADAN ALINAN IFADE BIR HARF OLDUGUNDA BURAYA GELINIR.
charsForIK ARRAYI HARFLERI SAKLAMAMIZI SAGLAYAN ARRAYDIR. BU ARRAY 20 +1 SONUNCUSU \0 OLACAK SEKILDE DUSUNULMUSTUR. KEYWORD OLSA DA SONUNCUSU \0 OLDUGU ICIN
PROGRAMI ETKILEMEZ VE 20DEN UZUN KEYWORD YOK.
HARF SAYI VE UNDERSCORE ISARETI ALDIKCA DOSYADAN OKUMAYA VE ARRAYE ATAMAYA DEVAM EDER KEYWORD OLSA DA SADECE HARF ALACAGI ICIN PROBLEM OLUSMAZ
IDENTIFIER MI KEYWORD MU OLDUGUNU BELIRLEMEK ICIN BIR FONKSIYONA GIDER
SAGLAMAYAN KARAKTER GELDIGINDE ISE ONU GERI GONDERIR CUNKU BASKA BIR TOKEN IN KARAKTERIDIR. YOLLAMADIGI ZAMAN PROBLEM CIKAR
*/
char identifierOrKeywordPrinter(FILE *inputPSI,FILE *outputLEX , char FIRSTCHAR){
    char charsForIK[21];

    int numberForIndex = 0;

    char newCharFromFile = FIRSTCHAR;   //DOSYADAN ALINMIS CHAR BURADA ATANIR VE YENI TOKEN IN ILK KARAKTERI OLUR

    /*
    ILK KARAKTERI ONCESINDE ALDIGIMIZ ICIN DO WHILE KULLANDIM
    DONGUYE GIRDIGINDE EN BASTA ONCEDEN GELEN KARAKTERI ALIP ATIYOR KARAKTER DEGISIYOR VE UYGUN OLDUKCA CALISAN DONGU
    */
    do{

        if(numberForIndex >= 20){
            printf("IDENTIFIERS CAN'T BE LONGER THAN 20 CHARACHTERS");
            exit(0);
        }

        charsForIK[numberForIndex] = newCharFromFile;
        if(charsForIK[numberForIndex] == -1) break; //DOSYANIN SONUNA ULASILMISSA
        numberForIndex++;
        charsForIK[numberForIndex] = '\0';

        newCharFromFile = fgetc(inputPSI);

    }while(isalpha(newCharFromFile) || isdigit(newCharFromFile) || newCharFromFile=='_' );

    //IDENTIFIER OR KEYWORD HANGISI OLDUGUNU BELIRLEMEMIZI SAGLAYAN METODA GIDER
    declaringIdorKey(charsForIK,outputLEX);

    //YUKARIDAKI WHILE DONGUSUNU BOZAN VE BIR SONRAKI TOKENIN KARAKTERI OLAN KARAKTERI RETURN LE YOLLUYORUZ
    return newCharFromFile;

}

/*
KELIMEYI YANI CHAR DIZISINI VE OUTPUT DOSYASININ POINTER INI ALAN BIR FONKSIYONDUR
CHAR DIZISININ POINTERINI DE ALABILIRDIM FAKAT BURADA TEKRARDAN ARRAYE DONUSTURMEM GEREKECEGI ICIN ALMADIM
PSI DILININ KEYWORDLERINI ICEREN BASKA BIR DIZIYLE KARSILASTIRMA YAPILIR ESLESIYORSA KEYWORD ESLESMIYORSA IDENTIFIER DIR
ICINDE DOSYAYA YAZDIRMA ISLEMINI DE GERCEKLESTIRIR
*/
void declaringIdorKey(char wordFromPrinter[],FILE *outputLEX){
    char keywords[18][9]= {"break","case","char","const","continue","do","else","enum","float","for","goto","if","int","long","record","return","static","while"};

    for(int b = 0 ; wordFromPrinter[b] != '\0' ; b++){
        wordFromPrinter[b] = tolower(wordFromPrinter[b]);
    }

    for(int a = 0 ; a <= 18 ; a++){
        if(strcmp(keywords[a],wordFromPrinter) ==0){
            fputs("Keyword(",outputLEX);
            break;

        }
        else if(a==18){

            for(int c = 0 ; wordFromPrinter[c] != '\0' ; c++){
                if(isalpha(wordFromPrinter[c]))
                    wordFromPrinter[c] = toupper(wordFromPrinter[c]);
            }
            fputs("Identifier(",outputLEX);
            break;

        }

    }
    fputs(wordFromPrinter,outputLEX);
    fputs(")",outputLEX);
    fprintf(outputLEX,"\n");

}
//-------------------------------------------------------------

/*
DOSYALARI ALIR VE MAINDE " ISARETI GELDIGINDE CAGIRILACAGI ICIN KARAKTERI ALMASINA GEREK YOKTUR.
DOSYADAN HARF HARF ALIR VE ALDIKCA DOSYAYA YAZAR. DOSYANIN SONUNA GELMEDIKCE VE " ISARETI GELMEDIKCE DEVAM EDER
IF KONTROLU ILE DOSYANIN SONUNA GELINMISSE VE SON KARAKTER " DEGILSE YANI SON WHILE DONGUSUNDE " ALINMAMISSA (SON WHILE DONGUSUNDE DOSYANIN SONUNA GELINDIGI ICIN CALISMAYACAK)
HATA VEREREK PROGRAM SONA ERER
*/

void stringConstant(FILE *inputPSI,FILE *outputLEX ){
    fputs("StringConstant(",outputLEX);

    char newCharFromFile = fgetc(inputPSI);

    while(newCharFromFile != '"' && !(feof(inputPSI))){

        fputc(newCharFromFile,outputLEX);
        newCharFromFile = fgetc(inputPSI);
    }
    if((feof(inputPSI)) && newCharFromFile !='"'){
        printf("THERE IS A ERROR! STRINGS MUST HAVE TWICE COTATION MARK");
        exit(0);
    }
    fputs(")",outputLEX);
    fprintf(outputLEX,"\n");
}

/*
MAIN METODUNDA ALINAN KARAKTER SAYI OLDUGUNDA (DIGER DURUMLARDA SAYI ALINSA DAHI O KISMA DAHIL OLACAGI ICIN MAINDE ALINAN SAYI HER TURLU INTEGER DIGIT OLUR)
DOSYADAN KARAKTER ALMAYA DEVAM EDER VE ALDIGI KARAKTERLER INTEGER OLDUKCA VE KARAKTER SAYISI 10 U GECMEYECEK KADAR OLDUKCA ALIR.
10 TANE INTEGERI ARRAY E EKLEYIP TEKRAR KARAKTER ALDIGINDA WHILE DONGUSU SAYIDAN DOLAYI DURUR VE
IF BLOGU CALISIRSA INTEGER 10 U GECIYOR ANLAMINA GELIR VE HATA VEREREK PROGRAM SONLANIR
UYGUN KOSULLARI SAGLIYORSA PRINT ETME KISMI DA CALISIR
*/

//--------------------------------------------------------------------

char digitPrinter(FILE *inputPSI,FILE *outputLEX, char charachter){
    char integerNumbersArray[11] ;
    char charFromPSI = charachter;
    int numberOfIntegers = 0;

    do{
        integerNumbersArray[numberOfIntegers++] = charFromPSI;
        integerNumbersArray[numberOfIntegers] = '\0';
        charFromPSI = fgetc(inputPSI);
    }while(numberOfIntegers <10 && isdigit(charFromPSI) && !feof(inputPSI));

    if(isdigit(charFromPSI)){
        printf("THERE IS A ERROR! INTEGER CONSTANTS CAN'T BE LONGER THAN 10"); //LOCAL EKLE
        exit(0);
    }

    fputs("IntCont(",outputLEX);
    fputs(integerNumbersArray,outputLEX);
    fputs(")",outputLEX);
    fprintf(outputLEX,"\n");

    return charFromPSI;

}



//---------------------------------------------------------------------
/*
MAIN KISMINDA GELEN KARAKTER OPERATOR CIKIYORSA YANI ASAGIDAKI FUNCT. O ZAMAN BU METODLAR
KULLANILARAK OPERATORUN TIPI DOSYAYA YAZDIRILIR.MAINDEN GELEN CHAR VE METODLARDA
YENI ALINAN KARAKTER  ++ -- := DURUMLARINI SAGLIYORSA DAHA ONCELIKLI OLDUGU ICIN
IF LE KONTROL EDILIR BUNLAR DOSYAYA YAZILIR. SAGLAMIYORSA MAINDEN GELEN CHAR TEKLI KONTROL
EDILIR + - * / DURUMLARINI SAGLIYORSA YAZDIRILIR. EGER BURAYI DA SAGLAMIYORSA : VEYA = TEKLI
OLACAGI ICIN (ORN +:+) HATA VERIR VE PROGRAMDAN CIKILIR
*/
bool operatorControl(char charFromFile){
    char charsForPsi[] = {'+','-','*','/',':','='};

    for(int a = 0 ; charsForPsi[a] != '\0' ; a++){
        if(charsForPsi[a] == charFromFile) return true;
    }
    return false;
}

char declarationOfOperators(FILE *inputPSI , FILE * outputLEX , char charFromMain){

    char nextCharFromFile = fgetc(inputPSI);
    char arrayToPrint[3];
    char charToReturn ;

    fputs("Operator(",outputLEX);

    if((charFromMain == '+' && nextCharFromFile == '+') || (charFromMain == '-' && nextCharFromFile == '-') || (charFromMain == ':' && nextCharFromFile =='=')){

        fputc(charFromMain,outputLEX);
        fputc(nextCharFromFile,outputLEX);
        fputs(")",outputLEX);
        fprintf(outputLEX,"\n");

        charToReturn = fgetc(inputPSI);

    }
    else if(charFromMain == '+' || charFromMain == '-' || charFromMain == '*' || charFromMain == '/'){
        fputc(charFromMain,outputLEX);
        fputs(")",outputLEX);
        fprintf(outputLEX,"\n");


        charToReturn = nextCharFromFile;

    }
    else{
        printf(": or = charachters cant be alone");
        exit(0);
    }
    return charToReturn;
}


//----------------------------------------------------------------------
/*
BRACKET KONTROL METODU. DOSYADAN KARAKTER ALINDIGINDA BU KARAKTERIN BRACKET OLUP OLMADIGINI
ANLAMAMIZI SAGLAR. BRACKET ISE RETURN TRUE DEGILSE RETURN FALSE
*/
bool bracketControl(char charFromMain){
    char bracketsForPSI[6] = {'(','{','[',']','}',')'};

    for(int a = 0 ; a < 6 ; a++){
        if(bracketsForPSI[a] == charFromMain)
            return true;
    }
    return false;
}


/*
MAIN METODUNDAN CAGIRILIR. COMMENT OLUP OLMADIGINI KONTROL ETMEK ICIN DOSYADAN 1 TANE DAHA KARAKTER ALINIR ILK KARAKTER MAINDE ALINMISTI ZATEN.
(* YI SAGLIYORSA COMMENTTIR SAGLAMIYORSA BRACKETTIR VE DOGRUDAN BRACKETPRINTER METODUNA GIDER
(* YI SAGLIYORSA *) IFADESI OLUSMADIGI VE DOSYANIN SONUNA GELINMEDIGI SURECE ATLANMAYA DEVAM EDER. EGER KI *) DURUMU OLUSMAZSA VE DOSYANIN SONUNA
GELINDIGI ICIN WHILE DAN CIKILIYORSA IF ILE KONTROL EDILIR VE HATA VERIR PROGRAMDAN CIKAR
RETURN OLARAK DA NEXTCHAR I GONDERIR CUNKU BRACKET ISE COMMENT KONTROLU ICIN ALDIGIMIZ KARAKTER BASKA BIR TOKEN IN KARAKTERIDIR GONDERMEZSEK KARISIKLIGA SEBEP OLUR
*/
char bracketOrCommentController(char charFromMain , FILE *inputPSI , FILE *outputLEX){
    char nextChar = fgetc(inputPSI);

    if(charFromMain == '(' && nextChar == '*'){
        charFromMain = fgetc(inputPSI);
        nextChar = fgetc(inputPSI);

        while((charFromMain!= '*' || nextChar !=')') && !feof(inputPSI)){ //???
            charFromMain = nextChar;
            nextChar = fgetc(inputPSI);
        }
        if(charFromMain != '*' && nextChar != ')'){
            printf("THERE IS A MISTAKE. COMMENTS MUST HAVE BEEN CLOSED SOMEWHERE BEFORE ENDING FILE");
            exit(0);
        }
        nextChar = fgetc(inputPSI);
        return nextChar;

    }
    else{
        bracketPrinter(charFromMain , outputLEX);
        return nextChar;
    }
}

/*
BRACKET KONTROL METODUNDA CAGIRILIR.
IFADENIN BRACKET OLDUGU KESINLESITIGINDE BURAYA GELINIR VE PSI DILINE AIT BRACKETLARDAN SAGLAYAN LEX DOSYASINA YAZDIRILIR.
*/
void bracketPrinter(char bracketFromFunction , FILE *outputLEX){
    if(bracketFromFunction == '(' || bracketFromFunction == '{' || bracketFromFunction =='['){
        fputs("Left",outputLEX);
    }
    else{
        fputs("Right",outputLEX);
    }

    if(bracketFromFunction == '(' || bracketFromFunction == ')'){
        fputs("Par",outputLEX);
    }
    else if(bracketFromFunction == '[' || bracketFromFunction == ']'){
        fputs("SquareBracket",outputLEX);
    }
    else{
        fputs("CurlyBracket",outputLEX);
    }
    fprintf(outputLEX,"\n");

}

//--------------------------------------------------------------------------------------


int main()
{

    //inputPSI ACILAN DOSYA -- outputLEX LEXICAL ANALYZER SONNUCU YAZILACAK DOSYA
    FILE *inputPSI;
    FILE *outputLEX;

    /*
    DOSYA OKUMA VE DOSYA YAZMA ICIN 2 KISIM VAR. HATA DURUMLARINDA PROGRAMDAN CIKILIR
    */
    if((inputPSI = fopen("document.psi","r")) == NULL){ //--------------?? LEX VE PSI E CEVIR EN SON
        printf("WE COULDN'T OPEN THE FILE");
        exit(0);
    }

    if((outputLEX = fopen("output.lex","a")) ==NULL){
        printf("THERE IS A MISTAKE");
        exit(0);
    }
    //--------------------------------------------------------------------------


    //!feof(inputPSI) -> Dosyanýn sonuna gelinip gelinmediðini kontrol eden kýsým
    //feof -> dosyanin sonuna gelinesiye kadar 0 , dosyanin sonuna gelinmisse 0 harici deger verir
    //if(charachter == -1) return -> Dosyanýn sonuna gelindiðinde 1 fazla çalýþmasýný önlemek için gerekli

    bool takenCharFromFunction = false; //ICINDEKI IF BLOKLARINDAN SONRA FONKSIYONLARDAN GERIYE KARAKTER GELIP GELMEDIGINI OGRENMEK ICIN GEREKLI
	char charachter ;

	while(!feof(inputPSI)){ //DOSYANIN SONUNA GELINMEDIGI SURECE CALISACAK WHILE DONGUSU
		if(!takenCharFromFunction){ //FONKSIYONLARDAN KARAKTER ALINMAMISSA BURADA DOSYADAN KARAKTER ALINIR
			charachter = fgetc(inputPSI);
		}

		if(charachter ==-1) break;  //dosyanýn sonuna gelindiginde fazladan dönmemesi icin donguden cikilir

		//DOSYADAN ALINAN KARAKTER HARF ISE IDENTIFIER VEYA KEYWORD KONTROLUNE GIDER
		if(isalpha(charachter)){
            charachter =identifierOrKeywordPrinter(inputPSI,outputLEX,charachter);
            takenCharFromFunction = true;
        }

        //DOSYADAN GELEN KARAKTER BOSLUK VEYA ALT SATIRA GECME ISE WHILE DONGUSUYLE ATLANIR ORNEK OLARAK BOSLUK GELDIKCE DEVAM EDER
        else if(charachter ==' ' || charachter =='\n'){
            while(charachter ==' ' || charachter == '\n'){
                charachter = fgetc(inputPSI);
            }
            takenCharFromFunction = true;
        }

        //DOSYADAN ALINAN KARAKTER " OLDUGU ZAMAN STRING ANLAMINA GELIR VE STRING METODUNA GIDER. GERIYE KARAKTER GELMEDIGI ICIN KARAKTER ALINIR BURADA
        else if(charachter == '"'){
            stringConstant(inputPSI,outputLEX);
            charachter = fgetc(inputPSI);
            takenCharFromFunction = true;
        }

        //DOSYADAN ALINAN KARAKTER ; OLDUGU ZAMAN ENDOFLINE ANLAMINA GELIR VE DOSYAYA DOGRUDAN YAZDIRIR YENI KARAKTERI ALIR VE BLOK TAMAMLANIR
        else if(charachter == ';'){
            fputs("EndOfLine",outputLEX);
            fprintf(outputLEX,"\n");
            charachter = fgetc(inputPSI);
        	takenCharFromFunction = true;

        }

        //DOSYADAN ALINAN KARAKTER SAYI OLDUGU ZAMAN INTEGER METODUNA GIDER
		else if(isdigit(charachter)){
            charachter =digitPrinter(inputPSI,outputLEX,charachter);

            takenCharFromFunction = true;

        }

        //DOSYADAN ALINAN KARAKTER OPERATOR OLDUGUNDA CALISIR BURADA YAZILI OLAN METODLA KONTROLU GERCEKLESIR TRUE ISE CALISIR BLOK VE OPERATOR BELIRLEME METODUNA GIDER
        else if(operatorControl(charachter)){
            charachter =declarationOfOperators(inputPSI ,outputLEX , charachter);
            takenCharFromFunction = true;
        }

        //DOSYADAN ALINAN KARAKTER BRACKET OLDUGUNDA CALISIR BURADA YAZILI OLAN METODLA KONTROLU GERCEKLESIR TRUE ISE CALISIR BLOK
        //BRACKET VEYA COMMENT BELIRLEME METODUNA GIDER
        else if(bracketControl(charachter)){
            charachter = bracketOrCommentController(charachter ,inputPSI ,outputLEX);
            takenCharFromFunction = true;
        }

        //ELSE DURUMUNDA ISE PSI DILINE YANI VERILEN DOKUMANDAKI KARAKTERLER HARIC ORNEK ~@é TARZI IFADELERDE HATA VERIR VE PROGRAMDAN CIKILIR
        else{
            printf("INVALID CHARACHTER FOR PSI LANGUAGE");
            exit(0);
        }

	}

    //DOSYA KAPATMA
    fclose(inputPSI);
}
