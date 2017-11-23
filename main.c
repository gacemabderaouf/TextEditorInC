#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define addPage     if(i<filesize){\
                    numberOfPages++;\
                    printf("                                                                ");\
                    printf("%i Pages Loaded...\r",numberOfPages);\
                    tableOfAdrs[numberOfPages-1].startingAdr=i;\
                    tableOfAdrs[numberOfPages-1].centerTag=centerTag;\
                    tableOfAdrs[numberOfPages-1].leftTag=leftTag;\
                    tableOfAdrs[numberOfPages-1].rightTag=rightTag;\
                    tableOfAdrs[numberOfPages-1].justifyTag=justifyTag;\
                    tableOfAdrs[numberOfPages-1].currentTag=currentTag;\
                    tableOfAdrs[numberOfPages-1].iCanJump=iCanJump;}
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
/**< Structures du programme */
struct Tpage
{
    int startingAdr;
    int currentTag;
    int iCanJump;
    int centerTag,leftTag,rightTag,justifyTag;
};
typedef struct Tpage Tpage;
struct TwordAndCoords
{
    char *word;
    unsigned int coords;
};
typedef struct TwordAndCoords TwordAndCoords;
//Declaration des variable globales du programme.
/***********************************************************************************************************/
int numberOfWords=0,numberOfChars=0,numberOfLines=0,numberOfPages=0,currentPage,displayedLines,continuer;
int  centerTag=0,leftTag=0,rightTag=0,justifyTag=0,iCanJustify=1;
int whereInIndexBuffer=-1,numberofIndexFiles=0;
float Loaded;
int toBeHighlighted=-1,iCanHighlight,highLightCoordsInBuffer;
int tableOfFoundCoords[500],frequency=0;
int currentLineinApproxSearch=0,currentIndexFile=0;
int linesCoords[10000],numberofCoords=0;
char toSrchWord[76],newWord[76],replacing=0,entredName[20];
TwordAndCoords indexBuffer[10000];
Tpage *tableOfAdrs;
HANDLE hconsole;

/***********************************************************************************************************/
/**< Remplacer le mot de coord coords par le mot newWord dans le fichier nommé fileName */
void replaceInFile(char *fileName,int coords,char *newWord);
/**< Ecrire le caractere car dans le fichier pointé par pf dans la position i */
void putInFile(FILE *pf,int i,char car);
/**< Lire le caractere de la position i du fichier pointé par pf */
char file(FILE *pf,int i);
/**< Calculer le nombre des balises /c/,/g/,/d/ & /j/ */
void doTagsStatics(FILE *pf,int *centerTag,int *leftTag,int *rightTag,int *justifyTag);
/**< Centrer le buffer buff */
void centerIt(char *buff);
/**< Aligner à gauche le buffer buff  */
void leftIt(char *buff);
/**< Aligner à droite le buffer buff  */
void rightIt(char *buff);
/**< Justifier le buffer buff  */
void justifyIt(char *buff);
/**< Extraire le mot de la position i dans le fichier pointé par pf*/
void extractWord(FILE *pf,int i,char *word);
/**< Traiter le buffer buff en fonction de l'operation*/
void treatBuffer(char* buff,int operation);
/**< verifier s'il ya une balise à la position i*/
int isAtag(FILE *pf,int i,int size,int *op);
/**< verifier s'il y'a pas une balise fermente de l'operation op */
int noClosingTag(int op,int centerTag,int leftTag,int rightTag,int justifyTag);
/**< Soustraire 1 de la balise courante 'op' */
void substructop(int op,int *centerTag,int *leftTag,int *rightTag,int *justifyTag);
/**< Mettre le mot pointé par word dans le buffer buff à la position whereinbuffer */
void putwordinlinebuffer(char *buff,char *word,int *whereinbuffer);
/**< faire les statistiques necessaires sur le fichier pointé par pf */
void firstCheck(FILE *pf,Tpage* tableOfAdrs);
/**< Faire le traitement necessaire de la page qui a pour caracteristiques 'pageCaracteristics'*/
void TreatePage(FILE *pf,Tpage pageCaracteristics);
/**< Lire les boutons du clavier*/
void readArrowKeys(FILE *pf);
/**< Ajouter au buffer de l'index le mot pointé par *word */
void addToIndexBuffer(char *word,int wordsCoords);
/**< Trier le buffer de l'index */
void Sortit();
/**< Enregistrer le fichier de l'index */
void sortAndSave();
/**< Chercher une page par les coordonnés 'coords' d'un mot  */
void searchPageByCoords(FILE *pf,int coords);
/**< Chercher le mot pointé par word dans le fichier Index */
void searchInIndexFile(char *word,FILE *linescoordsfile,FILE *searchInFile);
/**< Chercher le mot pointé par *word */
void searchWord(FILE **pf,char *word);
/**< Faire une autre vérification sur le fichier pointé par pf */
void anotherCheck(FILE *pf,Tpage *tableOfAdrs);
/**< Chercher un mot approximativement */
void searchWordApproximatly(FILE **pf,char *word);
/**< Surligner le mot cherché approximativement */
void highLightApproximateWords(FILE **pf,char *word);
/**< Supprimer les fichiers index à la fin de l'exécution. */
void deleteIndexFiles();
/**< Calculer la distance de Levenshtein */
/***********************************************************************************************************/
    int levenshtein(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}
void replaceInFile(char *fileName,int coords,char *newWord)
{
    char oldWord[76],foundChar;
    int fileSize,i;
    FILE *oldFile=fopen(fileName,"r");
    FILE *newFile=fopen("tempo.txt","w");
    fseek(oldFile,0,SEEK_END);
    fileSize=ftell(oldFile);
    i=0;
    while (i<fileSize)
    {
        if (i==coords)
        {
            extractWord(oldFile,i,oldWord);
            i=i+strlen(oldWord);
            fputs(newWord,newFile);
        }
        else
        {
            foundChar=file(oldFile,i);
            fputc(foundChar,newFile);
            if (foundChar=='\n') i=i+2;
            else i++;
        }
    }
    fclose(oldFile);
    remove(fileName);
    fclose(newFile);
    rename("tempo.txt",fileName);
}
void putInFile(FILE *pf,int i,char car)
{
    fseek(pf,i,SEEK_SET);
    fputc(car,pf);
}
char file(FILE *pf,int i)
{
    fseek(pf,i,SEEK_SET);
    return fgetc(pf);
}
void doTagsStatics(FILE *pf,int *centerTag,int *leftTag,int *rightTag,int *justifyTag)
{
    int i,j,length;
    char buff[512];
    char tricar[4];
    rewind(pf);
    while (fgets(buff,512,pf)!=NULL)
    {
        length=strlen(buff)-2;
        for (i=0;i<length;i++)
        {
            if (buff[i]=='/')
            {
                for (j=0;j<3;j++)
                {
                    tricar[j]=buff[i+j];
                }
                if (strcmp(tricar,"/c/")==0) (*centerTag)++;
                else if (strcmp(tricar,"/g/")==0) (*leftTag)++;
                else if (strcmp(tricar,"/d/")==0) (*rightTag)++;
                else if (strcmp(tricar,"/j/")==0) (*justifyTag)++;
            }
        }
    }
}
void justifyIt(char *buff)
{
    int numberOfSpaces=0,Spaces,rest,nmbrMot=0;
    char tempo[76];
    int i=0,j=0,t=strlen(buff)-1,tt,ok=0;


    if(iCanJustify)
    {
        while (buff[t]==' ' && t>0) t--;
        while(i<=t)
        {
            if(buff[i]!=' ') ok=1;
            else { if(ok)nmbrMot++; numberOfSpaces++; ok=0;}
            i++;
        }

        Spaces=(numberOfSpaces+(74-t))/(nmbrMot);
        rest=(numberOfSpaces+(74-t))%(nmbrMot);

        ok=1;
        j=0;
        i=0;
        while (buff[i]==' ' && i<=t) i++;
        while(i<=t)
        {

            while(buff[i]!=' ' && i<=t)
            {
                if(i==highLightCoordsInBuffer && ok) {highLightCoordsInBuffer=j; ok=0; }
                tempo[j]=buff[i];
                i++; j++;
            }
            while (buff[i]==' '  && i<=t) i++;
            if(rest>0) tt=-1;
            else tt=0;
            rest--;
            while(tt<Spaces)
            {
                tempo[j]=' ';
                j++;
                tt++;
            }
        }

        for(i=0;i<75;i++)buff[i]=tempo[i];
    }
    else while(t<75) {t++; buff[t]=' '; }
    buff[75]='\0';
}

void centerIt(char *buff)
{
    int numberOfSpaces;
    char tempo[76];
    int i,j=0,t=strlen(buff)-1,tt=0,ok=1;

    while (buff[j]==' ') j++;
    while (buff[t]==' ') t--;
    numberOfSpaces=(74-(t-j))/2;
    if(numberOfSpaces!=0)
    {
        for (i=j;i<=t;i++)
        {
            tempo[tt]=buff[i];
            if(i==highLightCoordsInBuffer && ok) {highLightCoordsInBuffer=tt; ok=0;}
            tt++;
        }
        ok=1;
        for (i=0;i<numberOfSpaces;i++) buff[i]=' ';
        j=0;
        while(tt>0)
        {
            buff[i]=tempo[j];
            if(j==highLightCoordsInBuffer && ok) {highLightCoordsInBuffer=i; ok=0; }
            i++;
            tt--;
            j++;
        }
        while(i<75) {buff[i]=' '; i++;}
    }
    buff[75]='\0';
}
void leftIt(char *buff)
{
    int j=0,i,length=strlen(buff)-1;
    while (buff[j]==' ') j++;
    while (buff[length]==' ') length--;
    for (i=j;i<=length;i++) buff[i-j]=buff[i];
    for (i=length-j+1;i<75;i++) buff[i]=' ';
    buff[75]='\0';
    highLightCoordsInBuffer-=j;
}
void rightIt(char *buff)
{
    int t=strlen(buff)-1,j=0,i;
    while (buff[t]==' ') t--;
    while (buff[j]==' ') j++;
    for (i=t;i>=j;i--) buff[i+(74-t)]=buff[i];
    for (i=0;i<j+(74-t);i++) buff[i]=' ';
    buff[75]='\0';
    highLightCoordsInBuffer+=74-t;
}
void extractWord(FILE *pf,int i,char *word)
{
    int j=-1;
    while((toupper(file(pf,i))>=65 && toupper(file(pf,i))<=90) || ((file(pf,i))>=48 && file(pf,i)<=57 ) )
    {
        j++;
        word[j]=file(pf,i);
        i++;
    }
    word[j+1]='\0';
}
void treatBuffer(char* buff,int operation)
{
    int j;
    if (operation==0) centerIt(buff);
    else if (operation==1) leftIt(buff);
    else if(operation==2) rightIt(buff);
    else if(operation==4) justifyIt(buff);
    printf("                                |");
    if (!iCanHighlight)
    {
        if (operation!=3)
        {
                printf("%s",buff);
                printf("|%i",31*(currentPage-1)+displayedLines);
                printf("\n");
        }
        else
        {
            for (j=strlen(buff);j<75;j++)
            {
                buff[j]=' ';
            }
            buff[75]='\0';
            printf("%s|%i",buff,31*(currentPage-1)+displayedLines);
            printf("\n");
        }
    }
    else
    {
        iCanHighlight=0;
        char tempo;
        int length;
        if (replacing) length=strlen(newWord);
        else length=strlen(toSrchWord);
        tempo=buff[highLightCoordsInBuffer];
        buff[highLightCoordsInBuffer]='\0';
        printf("%s",buff);
        buff[highLightCoordsInBuffer]=tempo;
        tempo=buff[highLightCoordsInBuffer+length];
        buff[highLightCoordsInBuffer+length]='\0';
        if (replacing) SetConsoleTextAttribute(hconsole,15|BACKGROUND_GREEN);
        else SetConsoleTextAttribute(hconsole,15|BACKGROUND_RED);
        printf("%s",buff+highLightCoordsInBuffer);
        SetConsoleTextAttribute(hconsole,7|0);
        replacing=0;
        buff[highLightCoordsInBuffer+length]=tempo;
        if (operation==3)
        {
         for (j=strlen(buff);j<75;j++)
            {
                buff[j]=' ';
            }
            buff[75]='\0';
        }
        printf("%s|%i\n",buff+highLightCoordsInBuffer+length,31*(currentPage-1)+displayedLines);
    }
}
int isAtag(FILE *pf,int i,int size,int *op)
{
    char code[4];
    if (i>(size-3)) return 0;
    else
    {
        code[0]=file(pf,i);
        code[1]=file(pf,i+1);
        code[2]=file(pf,i+2);
        code[3]='\0';
        if (strcmp(code,"/c/")==0)
        {
            *op=0;
            return 1;
        }
        else if (strcmp(code,"/g/")==0)
        {
            *op=1;
            return 1;
        }
        else if (strcmp(code,"/d/")==0)
        {
            *op=2;
            return 1;
        }
        else if (strcmp(code,"/j/")==0)
        {
            *op=4;
            return 1;
        }
        else return 0;
    }

}
int noClosingTag(int op,int centerTag,int leftTag,int rightTag,int justifyTag)
{
    if (op==0 && centerTag==0) return 1;
    else if (op==1 && leftTag==0) return 1;
    else if (op==2 && rightTag==0) return 1;
    else if (op==4 && justifyTag==0) return 1;
    else return 0;
}
void substructop(int op,int *centerTag,int *leftTag,int *rightTag,int *justifyTag)
{
    if (op==0) *centerTag=*centerTag-1;
    else if (op==1) *leftTag=*leftTag-1;
    else if (op==2) *rightTag=*rightTag-1;
    else if (op==4) *justifyTag=*justifyTag-1;
}
void putwordinlinebuffer(char *buff,char *word,int *whereinbuffer)
{
    int j;
    for (j=0;j<strlen(word);j++)
    {
        *whereinbuffer=*whereinbuffer+1;
        buff[*whereinbuffer]=word[j];
    }
}
void firstCheck(FILE *pf,Tpage* tableOfAdrs)
{
    int filesize,i=0,whereinbuffer=-1;
    int currentTag=3;
    int detectedTag=3;
    int iCanJump=1;
    int wordlengthgth=0;
    char mot[76],foundChar;
    fseek(pf,0,SEEK_END);
    filesize=ftell(pf);
    doTagsStatics(pf,&centerTag,&leftTag,&rightTag,&justifyTag);
    addPage;
    system("cls");
    printf("\n\n");
    while (i<filesize)
    {
        extractWord(pf,i,mot);
        wordlengthgth=strlen(mot);
        if (wordlengthgth==0)
        {
            foundChar=file(pf,i);
            if (foundChar=='/')
            {
                if (isAtag(pf,i,filesize,&detectedTag))
                {
                    if (currentTag!=3)
                    {
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                        if (detectedTag==currentTag)
                        {
                            numberOfLines++;
                            i=i+3;
                            currentTag=3;
                            iCanJump=0;
                            if (numberOfLines%31==0 || i>=filesize-1)
                            {
                               addPage
                            }
                            whereinbuffer=-1;
                        }
                        else
                        {
                            whereinbuffer++;
                            i++;
                            if(whereinbuffer>74)
                            {
                                numberOfLines++;
                                iCanJump=1;
                                if (numberOfLines%31==0 || i>=filesize-1)
                                {
                                   addPage
                                }
                                whereinbuffer=0;
                            }
                        }
                    }
                    else
                    {
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                        if(noClosingTag(detectedTag,centerTag,leftTag,rightTag,justifyTag))
                        {
                            whereinbuffer++;
                            i++;
                            if(whereinbuffer>74)
                            {
                                numberOfLines++;
                                iCanJump=1;
                                if (numberOfLines%31==0 || i>=filesize-1)
                                {
                                   addPage
                                }
                                whereinbuffer=0;
                            }
                        }
                        else
                        {
                            currentTag=detectedTag;
                            i=i+3;
                        }
                    }
                }
                else
                {
                    whereinbuffer++;
                    i++;
                    if(whereinbuffer>74)
                    {
                        numberOfLines++;
                        iCanJump=1;
                        if (numberOfLines%31==0 || i>=filesize-1)
                        {
                            addPage
                        }
                        whereinbuffer=0;
                    }
                    numberOfChars++;
                }

            }
            else if (foundChar=='\n')
            {
                i=i+2;
                if (iCanJump)
                {
                    numberOfLines++;
                    if (numberOfLines%31==0 || i>=filesize-1)
                    {
                        addPage
                    }
                    whereinbuffer=-1;
                }
                else
                {
                    iCanJump=1;
                }
            }
            else
            {
                 whereinbuffer++;
                 i++;
                 if(whereinbuffer>74)
                    {
                        numberOfLines++;
                        iCanJump=1;
                        if (numberOfLines%31==0 || i>=filesize-1)
                        {
                            addPage
                        }
                        whereinbuffer=0;
                    }
                if (foundChar!=' ') numberOfChars++;//On ne calcule pas le nombre d'espaces
            }
        }
        else
        {
            if(74-whereinbuffer>=wordlengthgth)
            {
                addToIndexBuffer(mot,i);
                numberOfWords++;
                whereinbuffer=whereinbuffer+wordlengthgth;
                i=i+wordlengthgth;
                numberOfChars=numberOfChars+wordlengthgth;
            }
            else
            {
                numberOfLines++;
                iCanJump=1;
                if (numberOfLines%31==0 || i>=filesize-1)
                {
                    addPage
                }
                whereinbuffer=-1;
            }
        }
    }
    if (whereinbuffer!=-1)
    {
        numberOfLines++;
    }
    if (whereInIndexBuffer!=-1) sortAndSave();
}
void TreatePage(FILE *pf,Tpage pageCaracteristics)
{
    int filesize,i=pageCaracteristics.startingAdr,whereinbuffer=-1,t;
    int currentTag=pageCaracteristics.currentTag;
    int detectedTag=3;
    int centerTag=pageCaracteristics.centerTag;
    int leftTag=pageCaracteristics.leftTag;
    int rightTag=pageCaracteristics.rightTag;
    int justifyTag=pageCaracteristics.justifyTag;
    int iCanJump=pageCaracteristics.iCanJump;
    displayedLines=0;
    char mot[76],linebuffer[76],foundChar;
    fseek(pf,0,SEEK_END);
    filesize=ftell(pf);
    SetConsoleTextAttribute(hconsole,14);
    printf("\n                                ---%s",entredName);
    SetConsoleTextAttribute(hconsole,7|0);
    printf("\n                                %c",218);
    for (t=0;t<75;t++) printf("%c",196);
    printf("%c\n",191);
    while (i<filesize && displayedLines!=31 )
    {
        extractWord(pf,i,mot);
        if (strlen(mot)==0)
        {
            foundChar=file(pf,i);
            if (foundChar=='/')
            {
                if (isAtag(pf,i,filesize,&detectedTag))
                {
                    if (currentTag!=3)
                    {
                        if (detectedTag==currentTag)
                        {
                            iCanJustify=0;
                            linebuffer[whereinbuffer+1]='\0';
                            displayedLines++;
                            treatBuffer(linebuffer,detectedTag);
                            currentTag=3;
                            iCanJump=0;
                            whereinbuffer=-1;
                            i=i+3;
                            iCanJustify=1;
                        }
                        else
                        {
                            whereinbuffer++;
                            if(whereinbuffer>74)
                            {
                                linebuffer[75]='\0';
                                displayedLines++;
                                treatBuffer(linebuffer,currentTag);
                                whereinbuffer=0;
                                iCanJump=1;
                            }
                            linebuffer[whereinbuffer]=foundChar;
                            i++;
                        }
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                    }
                    else
                    {
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                        if(noClosingTag(detectedTag,centerTag,leftTag,rightTag,justifyTag))
                        {
                            whereinbuffer++;
                            if(whereinbuffer>74)
                            {
                                linebuffer[75]='\0';
                                displayedLines++;
                                treatBuffer(linebuffer,currentTag);
                                whereinbuffer=0;
                                iCanJump=1;
                            }
                            linebuffer[whereinbuffer]=foundChar;
                            i++;
                        }
                        else
                        {
                            currentTag=detectedTag;
                            i=i+3;
                        }
                    }
                }
                else
                {
                    whereinbuffer++;
                    if(whereinbuffer>74)
                    {
                        linebuffer[75]='\0';
                        displayedLines++;
                        treatBuffer(linebuffer,currentTag);
                        whereinbuffer=0;
                        iCanJump=1;
                    }
                    linebuffer[whereinbuffer]=foundChar;
                    i++;
                }

            }
            else if (foundChar=='\n')
            {
                if (iCanJump)
                {
                    iCanJustify=0;
                    linebuffer[whereinbuffer+1]='\0';
                    displayedLines++;
                    treatBuffer(linebuffer,currentTag);
                    whereinbuffer=-1;
                    iCanJustify=1;
                }
                else
                {
                    iCanJump=1;
                }
                i=i+2;
            }
            else
            {
                 whereinbuffer++;
                 if(whereinbuffer>74)
                {
                    linebuffer[75]='\0';
                    displayedLines++;
                    treatBuffer(linebuffer,currentTag);
                    whereinbuffer=0;
                    iCanJump=1;
                }
                linebuffer[whereinbuffer]=foundChar;
                i++;
            }
        }
        else
        {
            if(74-whereinbuffer>=strlen(mot))
            {
                if (i==toBeHighlighted)
                {
                    iCanHighlight=1;
                    highLightCoordsInBuffer=whereinbuffer+1;
                }
                putwordinlinebuffer(linebuffer,mot,&whereinbuffer);
                i=i+strlen(mot);
            }
            else
            {
                linebuffer[whereinbuffer+1]='\0';
                displayedLines++;
                treatBuffer(linebuffer,currentTag);
                iCanJump=1;
                whereinbuffer=-1;
            }
        }
    }
    if (whereinbuffer!=-1)
    {
        linebuffer[whereinbuffer+1]='\0';
        displayedLines++;
        treatBuffer(linebuffer,currentTag);
    }
        printf("                                %c",192);
        for (t=0;t<75;t++) printf("%c",196);
        printf("%c\n",217);
        for (t=0;t<31-displayedLines;t++) printf("\n");
        printf("                                                                   ");
        printf("Page %i/%i",currentPage,numberOfPages);
        printf("\n\n                                ");
        printf("[Nombre de lignes:%i]----[Nombre de mots:%i]----[Nombre de characteres:%i]\n",numberOfLines,numberOfWords,numberOfChars);
        SetConsoleTextAttribute(hconsole,14);
        printf("\n\n    [ESC]:Quit === [0-9]:Choose Page By Number === [Arrows]:Move Down Or Up === [R/r]:search a word === [F/f]:replace a searched word\n    ");
        SetConsoleTextAttribute(hconsole,7|0);
}
void readArrowKeys(FILE *pf)
{
    int Key;
    Key=getch();
    if (Key==224 || Key==0)
        {
            Key=getch();
            switch (Key)
            {
                case 72:
                {
                    currentPage--;
                    if (currentPage<1) currentPage=numberOfPages;
                    break;
                }
                case 80:
                {
                    currentPage++;
                    if (currentPage>numberOfPages) currentPage=1;
                    break;
                }
                default: readArrowKeys(pf);
            }
        }
        else
        {
            if (Key>=48 && Key<=57)
            {
                printf("\n    ?----Entrer le numero de la page:");
                scanf("%i",&currentPage);
                if (currentPage<1) currentPage=1;
                else if (currentPage>numberOfPages) currentPage=numberOfPages;
            }
            else if (toupper(Key)==82)
            {
                printf("\n    ===>Entrer Le mot a chercher:");
                scanf("%s",toSrchWord);
                searchWord(&pf,toSrchWord);
            }
            else if (Key==27) continuer=0;
            else readArrowKeys(pf);
        }
}
void addToIndexBuffer(char *word,int wordsCoords)
{
    whereInIndexBuffer++;
    if (whereInIndexBuffer==10000)
    {
        whereInIndexBuffer--;
        sortAndSave();
        whereInIndexBuffer=0;
    }
    (indexBuffer[whereInIndexBuffer]).coords=wordsCoords;
    indexBuffer[whereInIndexBuffer].word=malloc(76*sizeof(char));
    strcpy((indexBuffer[whereInIndexBuffer]).word,word);
}

void Sortit()
{

    int i=0,swapped=1,tempocoords;
    char *tempoword;
    while (swapped)
    {
        swapped=0;
        i=0;
        while (i<whereInIndexBuffer)
        {
            if (strcmp(indexBuffer[i].word,indexBuffer[i+1].word)>0)
            {
                swapped=1;
                tempoword=indexBuffer[i].word;
                indexBuffer[i].word=indexBuffer[i+1].word;
                indexBuffer[i+1].word=tempoword;
                tempocoords=indexBuffer[i].coords;
                indexBuffer[i].coords=indexBuffer[i+1].coords;
                indexBuffer[i+1].coords=tempocoords;
            }
            i++;
        }
    }
}
void sortAndSave()
{
    char fileName[20]="!";
    char linescoordsfilename[20]="!";
    char *tempo=fileName;
    int i;
    Sortit(whereInIndexBuffer);
    numberofIndexFiles++;
    sprintf(fileName,"./index/%i.txt",numberofIndexFiles);
    sprintf(linescoordsfilename,"./index/_%i.txt",numberofIndexFiles);
    FILE *savingFile=fopen(fileName,"w");
    FILE *linescoords=fopen(linescoordsfilename,"w");
    for (i=0;i<=whereInIndexBuffer;i++)
    {
        if (strcmp(tempo,indexBuffer[i].word)==0)
        {
            fprintf(savingFile," %i",(indexBuffer[i]).coords);
        }
        else
        {
            fprintf(savingFile," \n");
            fprintf(linescoords,"%li ",ftell(savingFile));
            fprintf(savingFile,"%s %i",(indexBuffer[i]).word,(indexBuffer[i]).coords);
            tempo=indexBuffer[i].word;
        }
    }
    fclose(savingFile);
    fclose(linescoords);
}
void searchPageByCoords(FILE *pf,int coords)
{
    toBeHighlighted=coords;
    int bi=0,bs=numberOfPages-1,mid,trouve=0;
    if (coords>tableOfAdrs[numberOfPages-1].startingAdr)
    {
        currentPage=numberOfPages;
        system("cls");
        TreatePage(pf,tableOfAdrs[numberOfPages-1]);
    }
    else
    {
        while (bi<=bs && !trouve)
        {
            mid=(bi+bs)/2;
            if (coords>=tableOfAdrs[mid].startingAdr && coords<tableOfAdrs[mid+1].startingAdr) trouve=1;
            else if (tableOfAdrs[mid].startingAdr>coords) bs=mid-1;
            else bi=mid+1;
        }
        currentPage=mid+1;
        system("cls");
        TreatePage(pf,tableOfAdrs[mid]);
    }
}
void searchInIndexFile(char *word,FILE *linescoordsfile,FILE *searchInFile)
{
    char foundWord[76];
    int j=0,coord;
    int bi,bs,mid,trouve=0,test;
    while (fscanf(linescoordsfile,"%i",(linesCoords+j))!=EOF)
    {
        j++;

    }
    bi=0;
    bs=j-1;
    while (bi<=bs && !trouve)
    {
        mid=(bi+bs)/2;
        fseek(searchInFile,linesCoords[mid],SEEK_SET);
        fscanf(searchInFile,"%s",foundWord);
        if (strcmp(word,foundWord)==0) trouve=1;
        else if (strcmp(word,foundWord)<0) bs=mid-1;
        else bi=mid+1;
    }
    if (trouve)
    {
        test=fscanf(searchInFile,"%i",&coord);
        while (test!=EOF && test!=0)
        {
            frequency++;
            tableOfFoundCoords[frequency-1]=coord;
            test=fscanf(searchInFile,"%i",&coord);
        }
    }
}
void searchWord(FILE **pf,char *word)
{
    int displayedwords=0,nextOne=0,d,continuer=1;
    int searchApprox;
    void fillTheTable()
    {
        int j;
        FILE *indexFile;
        FILE *linesCoords;
        char filesName[20];
        for (j=1;j<=numberofIndexFiles;j++)
        {
            sprintf(filesName,"./index/%i.txt",j);
            indexFile=fopen(filesName,"r");
            sprintf(filesName,"./index/_%i.txt",j);
            linesCoords=fopen(filesName,"r");
            searchInIndexFile(word,linesCoords,indexFile);
        }
        fclose(indexFile);
        fclose(linesCoords);
    }
    fillTheTable();
    if (frequency==0)
    {
        printf("\n    LE MOT A CHERCHER N'EXISTE PAS! Voulez vous faire une recherche approximative(1/0)?:");
        scanf("%i",&searchApprox);
        if (searchApprox) highLightApproximateWords(pf,word);
        system("cls");
    }
    else
    {
        while (displayedwords<frequency && continuer)
        {
            nextOne=0;
            displayedwords++;
            searchPageByCoords(*pf,tableOfFoundCoords[displayedwords-1]);
            while(!nextOne && continuer)
            {
                d=getch();
                if (d==13)
                {
                    nextOne=1;
                    system("cls");
                }
                else if (d==27)
                {
                    continuer=0;
                }
                else if (toupper(d)==70)
                {
                    replacing=1;
                    printf("Entrer Le Nouveau Mot:");
                    scanf("%s",newWord);
                    fclose(*pf);
                    replaceInFile(entredName,tableOfFoundCoords[displayedwords-1],newWord);
                    (*pf)=fopen(entredName,"r");
                    anotherCheck(*pf,tableOfAdrs);
                    searchPageByCoords(*pf,tableOfFoundCoords[displayedwords-1]);
                    frequency=0;
                    fillTheTable();
                    d=getch();
                    displayedwords=0;
                    if (d==27) continuer=0;
                    else if (d==13) nextOne=1;
                }
            }
        }
    }
}
void anotherCheck(FILE *pf,Tpage *tableOfAdrs)
{
    int filesize,i=0,whereinbuffer=-1;
    int currentTag=3;
    int detectedTag=3;
    int iCanJump=1;
    int wordlengthgth=0;
    numberOfChars=0;numberofIndexFiles=0;numberOfLines=0;numberOfPages=0;numberOfWords=0,whereInIndexBuffer=-1;
    char mot[76],foundChar;
    fseek(pf,0,SEEK_END);
    filesize=ftell(pf);
    addPage;
    system("cls");
    printf("\n\n");
    while (i<filesize)
    {
        extractWord(pf,i,mot);
        wordlengthgth=strlen(mot);
        if (wordlengthgth==0)
        {
            foundChar=file(pf,i);
            if (foundChar=='/')
            {
                if (isAtag(pf,i,filesize,&detectedTag))
                {
                    if (currentTag!=3)
                    {
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                        if (detectedTag==currentTag)
                        {
                            numberOfLines++;
                            i=i+3;
                            currentTag=3;
                            iCanJump=0;
                            if (numberOfLines%31==0 || i>=filesize-1)
                            {
                               addPage
                            }
                            whereinbuffer=-1;
                        }
                        else
                        {
                            whereinbuffer++;
                            i++;
                            if(whereinbuffer>74)
                            {
                                numberOfLines++;
                                iCanJump=1;
                                if (numberOfLines%31==0 || i>=filesize-1)
                                {
                                   addPage
                                }
                                whereinbuffer=0;
                            }
                        }
                    }
                    else
                    {
                        substructop(detectedTag,&centerTag,&leftTag,&rightTag,&justifyTag);
                        if(noClosingTag(detectedTag,centerTag,leftTag,rightTag,justifyTag))
                        {
                            whereinbuffer++;
                            i++;
                            if(whereinbuffer>74)
                            {
                                numberOfLines++;
                                iCanJump=1;
                                if (numberOfLines%31==0 || i>=filesize-1)
                                {
                                   addPage
                                }
                                whereinbuffer=0;
                            }
                        }
                        else
                        {
                            currentTag=detectedTag;
                            i=i+3;
                        }
                    }
                }
                else
                {
                    whereinbuffer++;
                    i++;
                    if(whereinbuffer>74)
                    {
                        numberOfLines++;
                        iCanJump=1;
                        if (numberOfLines%31==0 || i>=filesize-1)
                        {
                            addPage
                        }
                        whereinbuffer=0;
                    }
                    numberOfChars++;
                }

            }
            else if (foundChar=='\n')
            {
                i=i+2;
                if (iCanJump)
                {
                    numberOfLines++;
                    if (numberOfLines%31==0 || i>=filesize-1)
                    {
                        addPage
                    }
                    whereinbuffer=-1;
                }
                else
                {
                    iCanJump=1;
                }
            }
            else
            {
                 whereinbuffer++;
                 i++;
                 if(whereinbuffer>74)
                    {
                        numberOfLines++;
                        iCanJump=1;
                        if (numberOfLines%31==0 || i>=filesize-1)
                        {
                            addPage
                        }
                        whereinbuffer=0;
                    }
                if (foundChar!=' ') numberOfChars++;//spaces
            }
        }
        else
        {
            if(74-whereinbuffer>=wordlengthgth)
            {
                addToIndexBuffer(mot,i);
                numberOfWords++;
                whereinbuffer=whereinbuffer+wordlengthgth;
                i=i+wordlengthgth;
                numberOfChars=numberOfChars+wordlengthgth;
            }
            else
            {
                numberOfLines++;
                iCanJump=1;
                if (numberOfLines%31==0 || i>=filesize-1)
                {
                    addPage
                }
                whereinbuffer=-1;
            }
        }
    }
    if (whereinbuffer!=-1)
    {
        numberOfLines++;
    }
    if (whereInIndexBuffer!=-1) sortAndSave();
}
void searchWordApproximatly(FILE **pf,char *word)
{
    FILE *fileOfLinesCoords;
    FILE *fileOfWords;
    char fileName[20];
    char extractedWord[76];
    int coord;
    int continueInThisFile=1;
    frequency=0;
    currentLineinApproxSearch=(currentLineinApproxSearch%10000)+1;
    if (currentLineinApproxSearch==1)
    {
        currentIndexFile++;
        if (currentIndexFile<=numberofIndexFiles)
        {
            numberofCoords=0;
            sprintf(fileName,"./index/_%i.txt",currentIndexFile);
            fileOfLinesCoords=fopen(fileName,"r");
            while (fscanf(fileOfLinesCoords,"%i",&(linesCoords[numberofCoords]))!=EOF)
            {
                numberofCoords++;
            }
            fclose(fileOfLinesCoords);
        }
    }
    if (currentIndexFile<=numberofIndexFiles)
    {
        sprintf(fileName,"./index/%i.txt",currentIndexFile);
        fileOfWords=fopen(fileName,"r");
        while (continueInThisFile)
        {
            if (currentLineinApproxSearch>numberofCoords)
            {
                continueInThisFile=0;
                currentLineinApproxSearch=0;
            }
            else
            {
                fseek(fileOfWords,linesCoords[currentLineinApproxSearch-1],SEEK_SET);
                fscanf(fileOfWords,"%s",extractedWord);
                if (levenshtein(word,extractedWord)<=1)
                {
                    while (fscanf(fileOfWords,"%i",&coord)!=0)
                    {
                        frequency++;
                        tableOfFoundCoords[frequency-1]=coord;
                    }
                    continueInThisFile=0;
                }
                else
                {
                    currentLineinApproxSearch++;
                }
            }
        }
        fclose(fileOfWords);
    }
}
void highLightApproximateWords(FILE **pf,char *word)
{
    int displayedwords=0;
    int continuer=1;
    int nextOne=0;
    int d;
    void fillTheTable()
    {
        int j;
        FILE *indexFile;
        FILE *linesCoords;
        char filesName[20];
        for (j=1;j<=numberofIndexFiles;j++)
        {
            sprintf(filesName,"./index/%i.txt",j);
            indexFile=fopen(filesName,"r");
            sprintf(filesName,"./index/_%i.txt",j);
            linesCoords=fopen(filesName,"r");
            searchInIndexFile(word,linesCoords,indexFile);
            fclose(indexFile);
            fclose(linesCoords);
        }
    }
    do
    {
        searchWordApproximatly(pf,word);
        if (frequency==0)
        {
            SetConsoleTextAttribute(hconsole,10);
            printf("\n    No other similar words!(press any key to continue)\n    ");
            SetConsoleTextAttribute(hconsole,7|0);
            getch();
        }
        else
        {
            while (displayedwords<frequency && continuer)
            {
                system("cls");
                nextOne=0;
                displayedwords++;
                extractWord(*pf,tableOfFoundCoords[displayedwords-1],toSrchWord);
                searchPageByCoords(*pf,tableOfFoundCoords[displayedwords-1]);
                while(!nextOne && continuer)
                {
                    d=getch();
                    if (d==13)
                    {
                        nextOne=1;
                    }
                    else if (d==27)
                    {
                        continuer=0;
                    }
                    else if (toupper(d)==70)
                {
                    replacing=1;
                    printf("Entrer Le Nouveau Mot:");
                    scanf("%s",newWord);
                    fclose(*pf);
                    replaceInFile(entredName,tableOfFoundCoords[displayedwords-1],newWord);
                    (*pf)=fopen(entredName,"r");
                    anotherCheck(*pf,tableOfAdrs);
                    searchPageByCoords(*pf,tableOfFoundCoords[displayedwords-1]);
                    frequency=0;
                    fillTheTable();
                    d=getch();
                    displayedwords=0;
                    if (d==27) continuer=0;
                    else if (d==13) nextOne=1;
                }
                }
            }
        }
    }while(frequency!=0 && continuer);
    currentLineinApproxSearch=0;
    currentIndexFile=0;
}
void deleteIndexFiles()
{
    char toBeDeleted[20];
    int j;
    system("pause");
    for (j=1;j<=numberofIndexFiles;j++)
    {
        sprintf(toBeDeleted,"./index/%i.txt",j);
        remove(toBeDeleted);
        sprintf(toBeDeleted,"./index/_%i.txt",j);
        remove(toBeDeleted);
    }
}
int main()
{
    printf("Entrer le nom de votre fichier:");
    scanf("%s",entredName);
    sprintf(entredName,"%s.txt",entredName);
    FILE *pf=NULL;
    pf=fopen(entredName,"r");
    if (pf!=NULL)
    {
        hconsole=GetStdHandle(STD_OUTPUT_HANDLE);
        tableOfAdrs=malloc(10000*sizeof(Tpage));
        system("color 02");
        firstCheck(pf,tableOfAdrs);
        system("cls");
        system("color 07");
        currentPage=1;
        continuer=1;
        do
        {
            frequency=0;
            iCanHighlight=0;
            toBeHighlighted=-1;
            system("cls");
            TreatePage(pf,tableOfAdrs[currentPage-1]);
            readArrowKeys(pf);
        } while (continuer);
        fclose(pf);
        deleteIndexFiles();
    }
    else
    {
        printf("\n\nImpossible d'ouvrir le fichier!, veuillez verifier son nom.\n\n");
    }
    return 0;
}
