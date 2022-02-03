#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
using namespace std;

#define MAX_NUME 40 
#define MAX_CAPITOL 70 
#define MAX_CLASA 200
#define MAX_SUBCLASA 200
#define MAX_INDICATIE 400
#define MAX_EFECTE 400
#define MAX_RA 400
#define MAX_CI 300
#define MAX_DOZA 200
#define MAX_ADM 200
#define MAX_MA 400
#define MAX_IM 200
#define MAX_BOALA 800

ifstream fin("Medicament.txt");

char exitt[5] = "exit";
char ext[4] = "ext";
const char delim[4] = ",()";


typedef struct {
    char nume_medicament[MAX_NUME];
    char super_clasa[MAX_CAPITOL];
    char clasa[MAX_CLASA];     //daca da clasa, ce medicamente fac parte din acea clasa
    char sub_clasa[MAX_SUBCLASA];
    char indicatie[MAX_INDICATIE];    //da o boala => medicamentele prescrise pentru boala aia
    char efecte[MAX_EFECTE];
    char reactii_adverse[MAX_RA];
    char contra_indicatii[MAX_CI];
    char doza[MAX_DOZA];
    char administrare[MAX_ADM];
    char mecanism_actiune[MAX_MA];
    char interactiuni_medicamentoase[MAX_IM];
}medicament;

medicament DATA[500];   //assume that we ll have at most 160 different drugs introduced

typedef struct {
    char numeBoala[300];
    char medicamente[11][40];
    int numarMedicamente;

}boala;

boala Boli[MAX_BOALA];
boala ReactiiAdverse[MAX_BOALA];
int lengthBoli, lengthRA;

typedef struct {
    char numeClasaMare[100];
    char prescurtareClasaMare[10];
    char numeMedicamente[60][40];
    int numarDeMedicamente;
}clasaMare1;

clasaMare1 clasaMare[300];

typedef struct {
    char numeClasa[100];
    char prescurtareClasa[10];
    char numeMedicamente[60][40];
    int numarDeMedicamente;
}clasa1;

clasa1 clasa[300], subClasa[300];

int lengthClasaMare, lengthClasa, lengthSubClasa;
int m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, max_boala;


int optimal(char s1[], char s2[])   //comparison between 2 stirngs 
{
    int cost[3][202];
    int n = strlen(s1);
    int m = strlen(s2);
    for (int i = 1; i <= m; i++)
    {
        cost[1][i] = 0;
        cost[2][i] = 0;
    }
    for (int i = 1; i <= n; i++)
    {
        cost[1][0] = i - 1;
        cost[2][0] = i;
        for (int j = 1; j <= m; j++)
        {
            int c = 0, min;
            if (i == 1)
                cost[1][j] = j;
            min = cost[1][j] + 1;
            if (min > cost[2][j - 1] + 1)
                min = cost[2][j - 1] + 1;
            if (s1[i - 1] != s2[j - 1])
                c = 1;
            if (min > cost[1][j - 1] + c)
                min = cost[1][j - 1] + c;
            cost[2][j] = min;
        }
        for (int j = 1; j <= m; j++)
            cost[1][j] = cost[2][j];
    }
    return cost[2][m];
}

void eliminateSpaces(char* word) {
    //final spaces
    int l = strlen(word) - 1;
    while (word[l] == ' ')  l--;

    word[l + 1] = '\0';

    //inner double spaces
    int i = 0;
    while (i <= l) {
        if (word[i] == ' ' && word[i + 1] == ' ') {
            for (int j = i + 1; j < l; j++) {
                word[j] = word[j + 1];
            }
            l--;
        }
        else
            i++;
    }
}

char* toLower(char* string) {
    char* temp = (char*)malloc((strlen(string) + 10) * sizeof(char));
    strcpy(temp, string);
    for (int i = 0; i < strlen(string); i++) {
        if (temp[i] >= 'A' && temp[i] <= 'Z')
            temp[i] = temp[i] - ('Z' - 'z');
    }
    return temp;
}

void addInClasaMare(char word[], char numeMedicament[]) {       //add the drug and also the super class
    if (strcmp(word, "---") == 0 || strcmp(word, "--") == 0)
        return;

    int l = strlen(word);
    char* tempNumeIntreg = (char*)malloc(sizeof(char) * (l + 3));
    char* tempPrescurtare = (char*)malloc(sizeof(char) * 10);
    int o = 0;
    while (word[o] != '|' && o < l) o++;
    strcpy(tempNumeIntreg, word);
    tempNumeIntreg[o] = '\0';

    strcpy(tempPrescurtare, word + o + 1);

    for (int i = 0; i < lengthClasaMare; i++) {
        if (strcmp(toLower(clasaMare[i].numeClasaMare), toLower(tempNumeIntreg)) == 0) {
            //add the drug in the 2D string for the drugs
            strcpy(clasaMare[i].numeMedicamente[clasaMare[i].numarDeMedicamente], numeMedicament);
            clasaMare[i].numarDeMedicamente++;
            return;
        }
    }

    strcpy(clasaMare[lengthClasaMare].numeClasaMare, tempNumeIntreg);
    strcpy(clasaMare[lengthClasaMare].prescurtareClasaMare, tempPrescurtare);
    strcpy(clasaMare[lengthClasaMare].numeMedicamente[clasaMare[lengthClasaMare].numarDeMedicamente], numeMedicament);
    clasaMare[lengthClasaMare].numarDeMedicamente++;
    lengthClasaMare++;
}

void addInClasa(char word[], char numeMedicament[]) {       //add the drug and also the super class
    if (strcmp(word, "---") == 0 || strcmp(word, "--") == 0)
        return;
    
    int l = strlen(word);
    char* tempNumeIntreg = (char*)malloc(sizeof(char) * (l + 3));
    char* tempPrescurtare = (char*)malloc(sizeof(char) * 10);
    int o = 0;
    while (word[o] != '|' && o < l) o++;
    strcpy(tempNumeIntreg, word);
    tempNumeIntreg[o] = '\0';

    strcpy(tempPrescurtare, word + o + 1);

    for (int i = 0; i < lengthClasa; i++) {
        if (strcmp(toLower(clasa[i].numeClasa), toLower(tempNumeIntreg)) == 0) {
            //add the drug in the 2D string for the drugs
            strcpy(clasa[i].numeMedicamente[clasa[i].numarDeMedicamente], numeMedicament);
            clasa[i].numarDeMedicamente++;
            return;
        }
    }

    strcpy(clasa[lengthClasa].numeClasa, tempNumeIntreg);
    strcpy(clasa[lengthClasa].prescurtareClasa, tempPrescurtare);
    strcpy(clasa[lengthClasa].numeMedicamente[clasa[lengthClasa].numarDeMedicamente], numeMedicament);
    clasa[lengthClasa].numarDeMedicamente++;
    lengthClasa++;
}

void addInSubClasa(char word[], char numeMedicament[]) {
    if (strcmp(word, "---") == 0 || strcmp(word, "--") == 0)
        return;

    int l = strlen(word);
    char* tempNumeIntreg = (char*)malloc(sizeof(char) * (l + 3));
    char* tempPrescurtare = (char*)malloc(sizeof(char) * 10);
    int o = 0;
    while (word[o] != '|' && o < l) o++;
    strcpy(tempNumeIntreg, word);
    tempNumeIntreg[o] = '\0';

    if (o < l)
        strcpy(tempPrescurtare, word + o + 1);
    else
        tempPrescurtare[0] = '\0';

    for (int i = 0; i < lengthSubClasa; i++) {
        if (strcmp(toLower(subClasa[i].numeClasa), toLower(tempNumeIntreg)) == 0) {
            //add the drug in the 2D string for the drugs
            strcpy(subClasa[i].numeMedicamente[subClasa[i].numarDeMedicamente], numeMedicament);
            subClasa[i].numarDeMedicamente++;
            return;
        }
    }

    strcpy(subClasa[lengthSubClasa].numeClasa, tempNumeIntreg);
    strcpy(subClasa[lengthSubClasa].prescurtareClasa, tempPrescurtare);
    strcpy(subClasa[lengthSubClasa].numeMedicamente[subClasa[lengthSubClasa].numarDeMedicamente], numeMedicament);
    subClasa[lengthSubClasa].numarDeMedicamente++;
    lengthSubClasa++;
}

void computeIndicatie(medicament medi) {

    char* token;
    token = strtok(medi.indicatie, delim);
    while (token) {
        if (token[0] == ' ')
            strcpy(token, token + 1);

        eliminateSpaces(token);

        if (strlen(token) > max_boala)
            max_boala = strlen(token);

        if (strcmp(token, "HTA") == 0) {
            for (int i = 0; i < lengthBoli; i++) {
                if (strcmp(Boli[i].numeBoala, token) == 0) {
                    //the HTA disease has already introduced and we found it
                    if (Boli[i].numarMedicamente < 10)
                        strcpy(Boli[i].medicamente[Boli[i].numarMedicamente++], medi.nume_medicament);
                    return;
                }
            }
            //if HTA was not introduced => introduce it 
            strcpy(Boli[lengthBoli].numeBoala, token);
            strcpy(Boli[lengthBoli].medicamente[Boli[lengthBoli].numarMedicamente++], medi.nume_medicament);
            lengthBoli++;
            return;
        }

        if (strcmp(token, "hTA") == 0) {
            for (int i = 0; i < lengthBoli; i++) {
                if (strcmp(Boli[i].numeBoala, token) == 0) {
                    //the HTA disease has already introduced and we found it
                    if (Boli[i].numarMedicamente < 10)
                        strcpy(Boli[i].medicamente[Boli[i].numarMedicamente++], medi.nume_medicament);
                    return;
                }
            }
            //if HTA was not introduced => introduce it 
            strcpy(Boli[lengthBoli].numeBoala, token);
            strcpy(Boli[lengthBoli].medicamente[Boli[lengthBoli].numarMedicamente++], medi.nume_medicament);
            lengthBoli++;
            return;
        }

        int ok = 0;
        for (int i = 0; i < lengthBoli && !ok; i++) {
            if (strcmp(toLower(Boli[i].numeBoala), toLower(token)) == 0) {
                //the disease has already introduced and we found it
                if (Boli[i].numarMedicamente < 10)
                    strcpy(Boli[i].medicamente[Boli[i].numarMedicamente++], medi.nume_medicament);
                ok = 1;
            }
        }

        if (!ok) {
            //the disease was not introduced by now 
            strcpy(Boli[lengthBoli].numeBoala, token);
            strcpy(Boli[lengthBoli].medicamente[Boli[lengthBoli].numarMedicamente++], medi.nume_medicament);
            lengthBoli++;
        }
        token = strtok(NULL, delim);
    }
}

void computeRA(medicament medi) {
    char* token;
    token = strtok(medi.reactii_adverse, delim);
    while (token) {
        if (token[0] == ' ')
            strcpy(token, token + 1);
        eliminateSpaces(token);
        /*
        if (token[0] == '(')
            strcpy(token, token + 1);
        if (token[strlen(token) - 1] == ')')
            token[strlen(token) - 1] = '\0';*/

        if (strlen(token) > max_boala)
            max_boala = strlen(token);

        if (strcmp(token, "HTA") == 0) {
            for (int i = 0; i < lengthBoli; i++) {
                if (strcmp(ReactiiAdverse[i].numeBoala, token) == 0) {
                    //the HTA disease has already introduced and we found it
                    if (ReactiiAdverse[i].numarMedicamente < 10)
                        strcpy(ReactiiAdverse[i].medicamente[ReactiiAdverse[i].numarMedicamente++], medi.nume_medicament);
                    return;
                }
            }
            //if HTA was not introduced => introduce it 
            strcpy(ReactiiAdverse[lengthRA].numeBoala, token);
            strcpy(ReactiiAdverse[lengthRA].medicamente[ReactiiAdverse[lengthRA].numarMedicamente++], medi.nume_medicament);
            lengthRA++;
            return;
        }

        if (strcmp(token, "hTA") == 0) {
            for (int i = 0; i < lengthRA; i++) {
                if (strcmp(ReactiiAdverse[i].numeBoala, token) == 0) {
                    //the HTA disease has already introduced and we found it
                    if (ReactiiAdverse[i].numarMedicamente < 10)
                        strcpy(ReactiiAdverse[i].medicamente[ReactiiAdverse[i].numarMedicamente++], medi.nume_medicament);
                    return;
                }
            }
            //if HTA was not introduced => introduce it 
            strcpy(ReactiiAdverse[lengthRA].numeBoala, token);
            strcpy(ReactiiAdverse[lengthRA].medicamente[ReactiiAdverse[lengthRA].numarMedicamente++], medi.nume_medicament);
            lengthRA++;
            return;
        }

        int ok = 0;
        for (int i = 0; i < lengthRA && !ok; i++) {
            if (strcmp(toLower(ReactiiAdverse[i].numeBoala), toLower(token)) == 0) {
                //the disease has already introduced and we found it
                if (ReactiiAdverse[i].numarMedicamente < 10)
                    strcpy(ReactiiAdverse[i].medicamente[ReactiiAdverse[i].numarMedicamente++], medi.nume_medicament);
                ok = 1;
            }
        }

        if (!ok) {
            //the disease was not introduced by now 
            strcpy(ReactiiAdverse[lengthRA].numeBoala, token);
            strcpy(ReactiiAdverse[lengthRA].medicamente[ReactiiAdverse[lengthRA].numarMedicamente++], medi.nume_medicament);
            lengthRA++;
        }
        token = strtok(NULL, delim);
    }
}



int main()
{
    
    int i = 0;
    //read the data
    while (fin.getline(DATA[i].nume_medicament, MAX_NUME))
    {
        eliminateSpaces(DATA[i].nume_medicament);

        if (strlen(DATA[i].nume_medicament) > m1)
            m1 = strlen(DATA[i].nume_medicament);


        fin.getline(DATA[i].super_clasa, MAX_CAPITOL); //read the chapter or the super class and put it in the vector if it has not been put yet
        addInClasaMare(DATA[i].super_clasa, DATA[i].nume_medicament);

        if (strlen(DATA[i].super_clasa) > m2)
            m2 = strlen(DATA[i].super_clasa);


        fin.getline(DATA[i].clasa, MAX_CLASA);    //read the class and put it in the vector if it has not been put yet
        addInClasa(DATA[i].clasa, DATA[i].nume_medicament);

        if (strlen(DATA[i].clasa) > m3)
            m3 = strlen(DATA[i].clasa);


        fin.getline(DATA[i].sub_clasa, MAX_SUBCLASA);    //read the subclass and put it in the vector if it has not been put yet
        addInSubClasa(DATA[i].sub_clasa, DATA[i].nume_medicament);

        if (strlen(DATA[i].sub_clasa) > m4)
            m4 = strlen(DATA[i].sub_clasa);


        fin.getline(DATA[i].indicatie, MAX_INDICATIE);

        if (strlen(DATA[i].indicatie) > m5)
            m5 = strlen(DATA[i].indicatie);

        if(DATA[i].indicatie[0] != '-')
            computeIndicatie(DATA[i]);


        fin.getline(DATA[i].efecte, MAX_EFECTE);

        if (strlen(DATA[i].efecte) > m6)
            m6 = strlen(DATA[i].efecte);


        fin.getline(DATA[i].reactii_adverse, MAX_RA);

        if (strlen(DATA[i].reactii_adverse) > m7)
            m7 = strlen(DATA[i].reactii_adverse);

        if (DATA[i].reactii_adverse[0] != '-')
            computeRA(DATA[i]);


        fin.getline(DATA[i].contra_indicatii, MAX_CI);

        if (strlen(DATA[i].contra_indicatii) > m8)
            m8 = strlen(DATA[i].contra_indicatii);


        fin.getline(DATA[i].doza, MAX_DOZA);

        if (strlen(DATA[i].doza) > m9)
            m9 = strlen(DATA[i].doza);


        fin.getline(DATA[i].administrare, MAX_ADM);     

        if (strlen(DATA[i].administrare) > m10)
            m10 = strlen(DATA[i].administrare);


        fin.getline(DATA[i].mecanism_actiune, MAX_MA); //read all the data

        if (strlen(DATA[i].mecanism_actiune) > m11)
            m11 = strlen(DATA[i].mecanism_actiune);


        fin.getline(DATA[i].interactiuni_medicamentoase, MAX_IM);

        if (strlen(DATA[i].interactiuni_medicamentoase) > m12)
            m12 = strlen(DATA[i].interactiuni_medicamentoase);

        i++;
    }

    /*
    //display the maximum lengths of strings
    cout << "   NUME     " << m1 << "\n";
    cout << "   CAPITOL  " << m2 << "\n";
    cout << "   CLASA    " << m3 << "\n";
    cout << "   SUBCLASA " << m4 << "\n";
    cout << "   IND      " << m5 << "\n";
    cout << "   EFECTE   " << m6 << "\n";
    cout << "   RA       " << m7 << "\n";
    cout << "   CI       " << m8 << "\n";
    cout << "   DOZA     " << m9 << "\n";
    cout << "   ADM      " << m10 << "\n";
    cout << "   MA       " << m11 << "\n";
    cout << "   IM       " << m12 << "\n";
    cout << "   BOALA    " << max_boala << "\n\n";*/
    

    int size = i;
    /*
    for (int j = 0; j < size; j++) {
        cout << DATA[j].nume_medicament << ", ";
    }
    */
    char input[80];
    int countClass = 0;
    //we want to read from the input a disease of name of a drug
    cin.getline(input, 150);

    while(strcmp(toLower(input), ext) != 0 && strcmp(toLower(input), exitt) != 0) {
        //read the input

        int ok = 0;

        //check for empty
        if (strcmp(input, "") == 0) {
            cout << "Introdu ceva!";
            ok = 1;
        }

        //chech for hta or Hta
        if (!ok && strcmp(toLower(input), "hta") == 0) {
            //if it is hiper TA => HTA
            if (input[0] == 'H') {
                for (int i = 0; i < lengthBoli && !ok; i++) {
                    if (strcmp(Boli[i].numeBoala, "HTA") == 0) {
                        ok = 1;
                        cout << "       Posibilele medicamente prescrise (ca indicatii) pentru " << Boli[i].numeBoala << " sunt:\n\n            ";
                        int o;
                        for (o = 0; o < Boli[i].numarMedicamente - 1; o++) {
                            cout << Boli[i].medicamente[o] << ", ";
                        }
                        cout << Boli[i].medicamente[o] << ".";
                    }
                }
                if (!ok) {
                    ok = 1;
                    cout << "       Nu am gasit niciun medicament care trateaza HTA.\n\n";
                }
                else cout << "\n";

                int ok2 = 0;

                for (int i = 0; i < lengthRA && !ok2; i++) {
                    if (strcmp(ReactiiAdverse[i].numeBoala, "HTA") == 0) {
                        ok2 = 1;
                        cout << "\n       Medicamentele care produc " << ReactiiAdverse[i].numeBoala << " ca reactia adversa sunt:\n\n            ";
                        int o;
                        for (o = 0; o < ReactiiAdverse[i].numarMedicamente - 1; o++) {
                            cout << ReactiiAdverse[i].medicamente[o] << ", ";
                        }
                        cout << ReactiiAdverse[i].medicamente[o] << ".";
                    }
                }
                if (!ok2) {
                    cout << "\n       Nu am gasit niciun medicament care are ca reactie adversa HTA.\n\n";
                }
                   
            }
            else {
                //it is hipo TA => hTA
                for (int i = 0; i < lengthBoli && !ok; i++) {
                    if (strcmp(Boli[i].numeBoala, "hTA") == 0) {
                        ok = 1;
                        cout << "       Posibilele medicamente prescrise (ca indicatii) pentru " << Boli[i].numeBoala << " sunt:\n\n            ";
                        int o;
                        for (o = 0; o < Boli[i].numarMedicamente - 1; o++) {
                            cout << Boli[i].medicamente[o] << ", ";
                        }
                        cout << Boli[i].medicamente[o] << ".";
                    }
                }
                if (!ok) {
                    ok = 1;
                    cout << "       Nu am gasit niciun medicament care trateaza hTA.\n\n";
                }
                else cout << "\n";

                int ok2 = 0;

                for (int i = 0; i < lengthRA && !ok2; i++) {
                    if (strcmp(ReactiiAdverse[i].numeBoala, "hTA") == 0) {
                        ok2 = 1;
                        cout << "\n       Medicamentele care produc " << ReactiiAdverse[i].numeBoala << " ca reactia adversa sunt:\n\n            ";
                        int o;
                        for (o = 0; o < ReactiiAdverse[i].numarMedicamente - 1; o++) {
                            cout << ReactiiAdverse[i].medicamente[o] << ", ";
                        }
                        cout << ReactiiAdverse[i].medicamente[o] << ".";
                    }
                }
                if (!ok2) {
                    cout << "\n       Nu am gasit niciun medicament care are ca reactie adversa hTA.\n\n";
                }
            }
        }


        //check first the super class
        for (int j = 0; j < lengthClasaMare && !ok; j++) {
            if (optimal(toLower(clasaMare[j].numeClasaMare), toLower(input)) < 4 || strcmp(toLower(clasaMare[j].prescurtareClasaMare), toLower(input)) == 0) {
                //we found that the input is a chapter and we want to show all the drugs which are from that chapter
                ok = 1;
                cout << "\n   Medicamentele care fac parte din capitolul/clasa mare " << clasaMare[j].numeClasaMare << ":\n\n       ";
                int k = 0;
                if (clasaMare[j].numarDeMedicamente > 15) {
                    for (k = 0; k < 15; k++) {
                        cout << clasaMare[j].numeMedicamente[k] << ", ";
                    }
                    cout << clasaMare[j].numeMedicamente[k];
                }
                else {
                    for (k = 0; k < clasaMare[j].numarDeMedicamente - 1; k++) {
                        cout << clasaMare[j].numeMedicamente[k] << ", ";
                    }
                    cout << clasaMare[j].numeMedicamente[k]<<".";
                }
            }
        }


        //check if the input represents a class
        for (int j = 0; j < lengthClasa && !ok; j++) {
                if (optimal(toLower(clasa[j].numeClasa), toLower(input)) < 3 || strcmp(toLower(clasa[j].prescurtareClasa), toLower(input)) == 0) {
                    //we found that the input is a class and we want to show all the drugs which are from that class
                    ok = 1;
                    cout << "\n   Medicamentele care fac parte din clasa " << clasa[j].numeClasa << ":\n\n       ";
                    int k = 0;
                    if (clasa[j].numarDeMedicamente > 15) {
                        for (k = 0; k < 15; k++) {
                            cout << clasa[j].numeMedicamente[k] << ", ";
                        }
                        cout << clasa[j].numeMedicamente[k];
                    }
                    else {
                        for (k = 0; k < clasa[j].numarDeMedicamente - 1; k++) {
                            cout << clasa[j].numeMedicamente[k] << ", ";
                        }
                        cout << clasa[j].numeMedicamente[k]<<".";
                    }
                }
        }


        //check if the input represents a subclass
        for (int j = 0; j < lengthSubClasa && !ok; j++) {
            if (optimal(toLower(subClasa[j].numeClasa), toLower(input)) < 3 || strcmp(toLower(subClasa[j].prescurtareClasa), toLower(input)) == 0) {
                //we found that the input is a subclass and we want to show all the drugs which are from that subclass
                ok = 1;
                cout << "\n   Medicamentele care fac parte din subclasa " << subClasa[j].numeClasa << ":\n\n       ";
                int k = 0;
                if (subClasa[j].numarDeMedicamente > 15) {
                    for (k = 0; k < 15; k++) {
                        cout << subClasa[j].numeMedicamente[k] << ", ";
                    }
                    cout << subClasa[j].numeMedicamente[k];
                }
                else {
                    for (k = 0; k < subClasa[j].numarDeMedicamente - 1; k++) {
                        cout << subClasa[j].numeMedicamente[k] << ", ";
                    }
                    cout << subClasa[j].numeMedicamente[k] << ".";
                }
            }
        }
            

        //if it is not a class, a chapter or a subclass, the it has to be either a disease, either a drug
        //check for being a drug
        if (!ok) {
            for (i = 0; i < size; i++) {
                if (optimal(toLower(input), toLower(DATA[i].nume_medicament)) < 3) { //medicament found !!!! 
                    //print all the results!
                    ok = 1;
                    cout << "\n\n   NUME MEDICAMENT:                " << DATA[i].nume_medicament << "\n";
                    cout << "   CLASA MARE / CAPITOL:           " << DATA[i].super_clasa << "\n";
                    cout << "   CLASA:                          " << DATA[i].clasa << "\n";
                    cout << "   SUBCLASA:                       " << DATA[i].sub_clasa << "\n";
                    cout << "   INDICATII:                      " << DATA[i].indicatie << "\n";
                    cout << "   EFECTE:                         " << DATA[i].efecte << "\n";
                    cout << "   REACTII ADVERSE:                " << DATA[i].reactii_adverse << "\n";
                    cout << "   CONTRA INDICATII:               " << DATA[i].contra_indicatii << "\n";  
                    cout << "   DOZA:                           " << DATA[i].doza << "\n";
                    cout << "   ADMINISTRARE                    " << DATA[i].administrare << "\n";
                    cout << "   MECANISM DE ACTIUNE:            " << DATA[i].mecanism_actiune << "\n";
                    cout << "   INTERACTIUNI MEDICAMENTOASE:    " << DATA[i].interactiuni_medicamentoase;
                }
            }
        }

        if (!ok) {
            //it is definitely a disease
            //it can be either form indicatie, either from reactii adverse

            //indicatie
            for (i = 0; i < lengthBoli; i++) {
                if (optimal(toLower(input), toLower(Boli[i].numeBoala)) < 2) {
                    cout << "\n   Posibilele medicamente prescrise (ca indicatii) pentru " << Boli[i].numeBoala << " sunt:\n\n       ";
                    int o;
                    for ( o = 0; o < Boli[i].numarMedicamente - 1; o++) {
                        cout << Boli[i].medicamente[o] << ", ";
                    }
                    cout << Boli[i].medicamente[o] << "." << "\n\n";
                    ok = 1;
                    break;
                }
            }

            //reactie adversa
            for (i = 0; i < lengthRA; i++) {
                if (optimal(toLower(input), toLower(ReactiiAdverse[i].numeBoala)) < 2) {
                    cout << "\n   Medicamentele care produc " << ReactiiAdverse[i].numeBoala << " ca reactia adversa sunt:\n\n       ";
                    int o;
                    for (o = 0; o < ReactiiAdverse[i].numarMedicamente - 1; o++) {
                        cout << ReactiiAdverse[i].medicamente[o] << ", ";
                    }
                    cout << ReactiiAdverse[i].medicamente[o] << ".";
                    ok = 1;
                    break;
                }
            }
        }
            

        if (!ok) 
            cout << "Nu am gasit nimic! \n\n";
        else
            cout << "\n\n";

        cin.getline(input, 150);
    }

    return 0;
}
