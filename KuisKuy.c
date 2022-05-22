#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	char nama[64];
	int mode;
} userInfo;
enum {GURU = 1, SISWA = 2}; //Enum untuk userInfo.mode

int login(userInfo * user);


int main(void){
	userInfo user;
	int loginSuccess = 0;
	int userQuit = 0;
	int userLogout = 0;
	int repeatMenu;
	int menu;
	
	do{
		
		repeatMenu = 1;
		while(repeatMenu){
			system("clear");
			system("CLS");
			
			printf("1. Login\n");
			printf("2. Exit\n");
			printf("Input: ");
			scanf(" %d", &menu);
			switch(menu){
				case 1:
					loginSuccess = login(&user);
					if(loginSuccess) repeatMenu = 0;
					break;
				case 2:
					userQuit = 1;
					repeatMenu = 0;
					break;
			}
		}
		
		if(!userQuit){
			userLogout = 0;
			do{
				if(user.mode == 1){
					printf("DBG Nama: %s\n", user.nama);
					printf("DBG Mode: %d\n", user.mode);
				}
				if(user.mode == 2){
					printf("DBG Nama: %s\n", user.nama);
					printf("DBG Mode: %d\n", user.mode);
				}
			}while(!userLogout);
		}
	} while(!userQuit);
	return 0;
}

int login(userInfo * user){
	FILE * login_db = fopen("login.txt", "r");
	FILE * user_db = fopen("akun.txt", "r");
	
	int db_count = 0;
	int loginSuccess = 0;
	int loginRetry = 0;
	
	char input_conf;
	char input_user[16];
	char input_pass[16];
	char db_user[16];
	char db_pass[16];
	
	
	do{
		system("clear");
		system("CLS");
		
		printf("User: ");
		scanf(" %15s", input_user);
		printf("DBG User: %s\n", input_user);
		printf("Password: ");
		scanf(" %15s", input_pass);
		printf("DBG Password: %s\n", input_pass);
		
		rewind(login_db);
		rewind(user_db);
		
		while(!feof(login_db) && !loginSuccess){
			fscanf(login_db, " %s", db_user);
			printf("DBG User: %s\n", db_user);
			fscanf(login_db, " %s", db_pass);
			printf("DBG User: %s\n", db_pass);
			if(strcmp(input_user, db_user) == 0
				&& strcmp(input_pass, db_pass) == 0)
			{
				while(db_count-- > 0){
					fscanf(user_db, " %63[^\n] %d", user->nama, &user->mode);
					printf("DBG Nama: %s\n", user->nama);
					printf("DBG Mode: %d - %d\n", user->mode, db_count);
				}
				fscanf(user_db, " %63[^\n]s", user->nama);
				printf("DBG Nama: %s\n", user->nama);
				fscanf(user_db, " %d", &user->mode);
				printf("DBG Mode: %d\n", user->mode);
				loginSuccess = 1;
				loginRetry = 0;
			}
			else
				++db_count;
		}
		
		if(!loginSuccess){
			printf("ERROR: Login gagal!\n");
			printf("Apakah Anda ingin kembali ke menu sebelumnya? (Y/N)\n");
			printf("Input: ");
			scanf(" %c", &input_conf);
			if(input_conf == 'Y')
				loginRetry = 0;
			else
				loginRetry = 1;
		}
	} while(loginRetry);			
	return loginSuccess;
}	