#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <t2fs.h>
#include <support.h>

typedef struct t2fs_record registro_dir;

typedef struct diretorio{
    char* name;
    char* pai_pathname;
    int current_entry;
    int MTFNumber;
    int blockSize;
    int byteSize;
    FILA2 entradas;
}DIRETORIO;

FILA2 dirList;


DIRETORIO* buscaDiretorioPai(char *pathname, int pathname_len){
    DIRETORIO *paiAtual, *dirAtual;
    registro_dir *entradaAtual;
    char pathcpy[pathname_len], pathPaiAtual[pathname_len];
    char* name_token;
    int found = 0, error = 0, tokenEqualsAtual = 0;

    strcpy(pathcpy, pathname);
    name_token = strtok(pathcpy,"/");
    strcpy(pathPaiAtual, "/");

    while(!found && !error){
	tokenEqualsAtual = 0;
        FirstFila2(&dirList);
        do{
	    dirAtual = GetAtIteratorFila2(&dirList);
printf("\ntoken: %s \ndirAtual: %s \npathPaiAtual: %s \n paiDiretorio: %s",name_token,dirAtual->name,pathPaiAtual,dirAtual->pai_pathname);
            if(strcmp(name_token, dirAtual->name) == 0)
                if(strcmp(pathPaiAtual, dirAtual->pai_pathname) == 0){
                    tokenEqualsAtual = 1;
                    strcat(pathPaiAtual, name_token);
		    strcat(pathPaiAtual, "/");
                    paiAtual = dirAtual;
                }
        }while(NextFila2(&dirList) == 0 && !tokenEqualsAtual);
        /*if(dirAtual == NULL){
            FirstFila2(paiAtual->entradas);
            entradaAtual = GetAtIteratorFila2(paiAtual->entradas);
            while(entradaAtual != NULL){
                if(strcmp(name_token, entradaAtual.name) == 0){
                    strcat(pathPaiAtual, "/");
                    strcat(pathPaiAtual, name_token);
                    opendir2(pathPaiAtual);  //apenas cria uma estrutura nova se for realmente um diretório
                    tokenEqualsAtual = 1;
                }
                else
                    NextFila2(paiAtual->entradas);
            }
        }*/
        name_token = strtok(NULL, "/");
	printf("\ntokenEqualsAtual = %d",tokenEqualsAtual);
        if(tokenEqualsAtual && name_token == NULL)
            found = 1;
        if(!tokenEqualsAtual){
            error = 1;
            paiAtual = NULL;
        }
    }
    return paiAtual;
}

void readEntradas(char* sector_buffer){


    struct t2fs_record registroAux;
    //char *sector_buffer;

    //read_sector(setor, sector_buffer);


    registroAux.TypeVal = *((BYTE *)(sector_buffer));
    strncpy(registroAux.name, sector_buffer + 1, 51);
    registroAux.blocksFileSize = *((DWORD *)(sector_buffer + 52));
    registroAux.bytesFileSize = *((DWORD *)(sector_buffer + 56));
    registroAux.MFTNumber = *((DWORD *)(sector_buffer + 60));


    printf("\n TypeVal = %d", registroAux.TypeVal);
    printf("\n name = %s", registroAux.name);
    printf("\n blocksFileSize = %d", registroAux.blocksFileSize);
    printf("\n bytesFileSize = %d", registroAux.bytesFileSize);
    printf("\n MFTNumber = %d", registroAux.MFTNumber);
}



int main() {    //COMENTA O TESTE QUE TU NAO QUISER TESTAR
                //OU NAO, PODE TENTAR EXECUTAR OS DOIS JUNTOS MAS NAO SEI SE DA CERTO

   //TESTE DO BUSCA DIRETORIO PAI
  char *pathname = "/diretorio2/diretorio3/";

    CreateFila2(&dirList);

    DIRETORIO *dir1 = malloc(sizeof(DIRETORIO));
    dir1->name = "diretorio1";
    dir1->pai_pathname = "/";
    AppendFila2(&dirList, dir1);

    DIRETORIO *dir2 = malloc(sizeof(DIRETORIO));
    dir2->name = "diretorio2";
    dir2->pai_pathname = "/";
    AppendFila2(&dirList, dir2);

    DIRETORIO *dir3 = malloc(sizeof(DIRETORIO));
    dir3->name = "diretorio3";
    dir3->pai_pathname = "/diretorio2/";
    AppendFila2(&dirList, dir3);


    DIRETORIO *paizao;
    paizao = buscaDiretorioPai(pathname, strlen(pathname));
    if(paizao != NULL){
    	printf("O nome do paizao é %s", paizao->name);
    	printf("\n Nome do pai do diretorio3 : %s", dir3->pai_pathname);
    }

    //TESTE DO READ ENTRADAS
     struct t2fs_record registroTeste;

    registroTeste.TypeVal = 2;
    strcpy(registroTeste.name, "testezera");
    registroTeste.blocksFileSize = 1;
    registroTeste.bytesFileSize = 1;
    registroTeste.MFTNumber = 1;

    char buffer[64];
    memcpy(buffer, &registroTeste, sizeof(registroTeste));
    /*strcpy(buffer, (char*)registroTeste.TypeVal);
    strcat(buffer, (char*)registroTeste.name);
    strcat(buffer, (char*)registroTeste.blocksFileSize);
    strcat(buffer, (char*)registroTeste.bytesFileSize);
    strcat(buffer, (char*)registroTeste.MFTNumber);*/
    printf("\n buffer = %s", buffer);

    readEntradas(buffer);



    return 0;
}
