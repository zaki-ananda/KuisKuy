#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	char nama[64];
	int mode;
	int ID;
} UserInfo;
enum {GURU = 1, SISWA = 2}; //Enum untuk UserInfo.mode

typedef struct Soal{
	char tanya[256];
	char pil_jawaban[4][256];
	struct Soal * next;
	struct Soal * prev;
	char jawaban;
} Soal;

typedef struct{
	char judul[64];
	int ID;
	Soal * soal;
} Kuis;


int menu_awal(UserInfo * user);
int login(UserInfo * user);
int menu_pilihKuis();
void menu_murid(UserInfo user);
void menu_guru(UserInfo user);
	void buatKuis(void);
		void inputSoal(Soal * soal);
		int assign_kuisID(Kuis kuis);
		void writeToFile_kuis(Kuis kuis);
	void penilaian(int kuisID);
	void outputNilai(int kuisID);


int main(void){
	UserInfo user;
	int userQuit;
	
	do{
		userQuit = menu_awal(&user);
		printf("DBG usermode %d\n", user.mode);
		scanf(" %*s");
		if(!userQuit){
			if(user.mode == GURU){
				printf("DBG Nama: %s\n", user.nama);
				printf("DBG Mode: %d\n", user.mode);
				menu_guru(user);
			}
			if(user.mode == SISWA){
				printf("DBG Nama: %s\n", user.nama);
				printf("DBG Mode: %d\n", user.mode);
				menu_murid(user);				
			}
		}
	} while(!userQuit);
	return 0;
}


//Menu sebelum user melakukan login
//Akan mereturn userQuit
int menu_awal(UserInfo * user){
	int repeatMenu;
	int menu;
	int loginSuccess;
	int userQuit;
	
	do{
		system("clear");
		system("CLS");
		
		userQuit = 0;
		repeatMenu = 1;
		
		printf("1. Login\n");
		printf("2. Exit\n");
		printf("Input: ");
		scanf(" %d", &menu);
		switch(menu){
			case 1:
				loginSuccess = login(user);
				if(loginSuccess) repeatMenu = 0;
				break;
			case 2:
				userQuit = 1;
				repeatMenu = 0;
				break;
		}
	} while(repeatMenu);
	printf("DBG menu_awal() returned %d\n", userQuit);
	return userQuit;
}

//Membaca file login.txt dan membandingkannya dengan login attempt
//Apabila berhasil, assign UserInfo yang sesuai dari akun.txt ke user, kemudian return 1
//Return 0 apabila tidak berhasil dan user ingin keluar dari menu login
int login(UserInfo * user){
	FILE * login_db = fopen("login.txt", "r");
	FILE * user_db = fopen("akun.txt", "r");
	
	if(login_db == NULL || user_db == NULL){
		printf("ERROR: File untuk login tidak ditemukan!\n");
		exit(0);
	}
	
	int db_count;
	int loginSuccess;
	int loginRetry;
	
	char input_conf;
	char input_user[16];
	char input_pass[16];
	char db_user[16];
	char db_pass[16];
	
	
	do{
		system("clear");
		system("CLS");
		
		db_count = 0;
		loginSuccess = 0;
		loginRetry = 0;
		
		printf("User: ");
		scanf(" %15s", input_user);
		printf("DBG User: %s\n", input_user);
		printf("Password: ");
		scanf(" %15s", input_pass);
		printf("DBG Password: %s\n", input_pass);
		
		rewind(login_db);
		rewind(user_db);
		
		while(!feof(login_db) && !loginSuccess){
			fscanf(login_db, " %15s %15s", db_user, db_pass);
			printf("DBG User: %s\n", db_user);
			printf("DBG User: %s\n", db_pass);
			if(strcmp(input_user, db_user) == 0
				&& strcmp(input_pass, db_pass) == 0)
			{
				while(db_count-- > 0){
					fscanf(user_db, " %63[^\n]%*c %d %d", user->nama, &user->mode, &user->ID);
					printf("DBG Nama: %s\n", user->nama);
					printf("DBG Mode: %d - %d\n", user->mode, db_count);
				}
				fscanf(user_db, " %63[^\n]%*c %d %d", user->nama, &user->mode, &user->ID);
				printf("DBG Nama: %s\n", user->nama);
				printf("DBG Mode: %d\n", user->mode);
				loginSuccess = 1;
				loginRetry = 0;
			}
			else
				++db_count;
		}
		scanf(" %*s");
		if(!loginSuccess){
			printf("ERROR: Login gagal!\n");
			printf("Apakah Anda ingin mencoba login ulang? (Y/N)\n");
			printf("Input: ");
			scanf(" %c", &input_conf);
			if(input_conf == 'Y')
				loginRetry = 1;
			else
				loginRetry = 0;
		}
	} while(loginRetry);
	fclose(login_db);
	fclose(user_db);
	printf("DBG login() returned %d\n", loginSuccess);
	return loginSuccess;
}	

//Membaca daftarkuis.txt dan mereturn kuisID apabila user memilih sebuah kuis
//Return 0 jika user ingin kembali 
int menu_pilihKuis(){
	FILE * daftarKuis = fopen("daftarkuis.txt", "r");
	Kuis inputTest;
	int counter = 0;
	int menuInput;
	
	system("CLS");
	system("clear");
	
	while(!feof(daftarKuis)){
		if(fscanf(daftarKuis, " %d %[^\n]s", &inputTest.ID, inputTest.judul) == 2)
			++counter;
		else{
			printf("ERROR: File daftarkuis.txt tidak sesuai format!\n");
			exit(0);
		}
	}
	printf("DBG kuisCounter: %d\n", counter);
	
	Kuis kuisArr[counter];
	
	printf("Pilih salah satu kuis di bawah ini!\n");
	rewind(daftarKuis);
	for(counter = 0; !feof(daftarKuis); ++counter){
		fscanf(daftarKuis, " %d %[^\n]s", &kuisArr[counter].ID, kuisArr[counter].judul);
		printf("%d. %s\n", counter+1, kuisArr[counter].judul);
	}
	printf("0. Keluar\n");
	printf("Input: ");
	scanf(" %d", &menuInput);
	
	
	if(menuInput == 0)
		return 0;
	else
		return kuisArr[menuInput-1].ID;
}

//Function menu user beserta menunya
void menu_murid(UserInfo user){
    int pilihan;
    int repeatMenu;
    
    do{
		system("CLS");
		system("clear");
		
		repeatMenu = 1;
		
		printf("===================");
		printf(" Selamat datang %s ",user.nama);
		printf("===================");
		//Tinggal di sesuaikan menu dengan pilihan nanti
		printf("\n1. Lihat Daftar Kuis");
		printf("\n2. Cari Kuis");
		printf("\n3. Cek Nilai Pengerjaan Kuis");
		printf("\n\n0. Logout");
		fflush(stdin);
		printf("\n\nPilihan: ");
		scanf(" %d", &pilihan);
		
		switch(pilihan){
			case 1:
				printf("Lihat daftar kuis\n");
				break;
			case 2:
				printf("Cari kuis");
				break;
			case 3:
				printf("Cek nilai pengerjaan kuis");
				break;
			case 0:
				repeatMenu = 0;
				break;
		}
	} while(repeatMenu);
}

//Function menu admin beserta menunya
void menu_guru(UserInfo user){
    int pilihan;
    int repeatMenu;
    
    do{
		system("CLS");
		system("clear");
		
		repeatMenu = 1;
		
		printf("===================");
		printf(" Selamat datang di KuisKuy ");
		printf("===================");
		printf("\n1. Buat Kuis Baru");
		printf("\n2. Lakukan Penilaian Kuis");
		printf("\n3. Cek Nilai Kuis Siswa");
		printf("\n\n0. Logout");
		//fflush(stdin);
		printf("\n\nPilihan: ");
		scanf(" %d", &pilihan);
		
		switch(pilihan){
			case 1:
				buatKuis();
				printf("DBG Finished buatKuis\n");
				break;
			case 2:
				penilaian(menu_pilihKuis());
				printf("DBG Finished Lakukan penilaian kuis\n");
				scanf(" %*s");
				break;
			case 3:
				outputNilai(menu_pilihKuis());
				scanf(" %*s");
				break;
			case 0:
				repeatMenu = 0;
				break;
		}
	} while(repeatMenu);
}

void buatKuis(void){
	Kuis kuisBaru;
	int soalCounter = 1;
	int repeatMembuatSoal;
	char input_conf;
	
	system("CLS");
	system("clear");
	
	printf("Judul kuis\n");
	scanf(" %[^\n]s", kuisBaru.judul);
	
	
	kuisBaru.soal = malloc(sizeof(Soal));
	Soal * soalSekarang = kuisBaru.soal;
	soalSekarang->prev = NULL;
	
	do{
		printf("\n===== Soal %d =====\n", soalCounter);
		inputSoal(soalSekarang);
		
		printf("Lanjut membuat soal berikutnya? (Y/N)\n");
		printf("Input: ");
		scanf(" %c", &input_conf);
		if(input_conf == 'N'){
			repeatMembuatSoal = 0;
			soalSekarang->next = NULL;
		} else{
			soalSekarang->next = malloc(sizeof(Soal));
			soalSekarang = soalSekarang->next;
			++soalCounter;
			repeatMembuatSoal = 1;
		}
		
	} while(repeatMembuatSoal);
	
	kuisBaru.ID = assign_kuisID(kuisBaru);
	
	writeToFile_kuis(kuisBaru);
}

void inputSoal(Soal * soal){
	char pil_counter;
	
	printf("Pertanyaan: \n");
	scanf(" %[^\n]s ", soal->tanya);
	
	for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter){
		printf("%c. ", pil_counter);
		scanf(" %[^\n]s ", soal->pil_jawaban[pil_counter - 'A']);
	}
	
	printf("Kunci Jawaban: ");
	scanf(" %c", &soal->jawaban);
}
	
int assign_kuisID(Kuis kuis){
	FILE * daftarKuis = fopen("daftarkuis.txt", "r");
	int kuisID;
	
	while(!feof(daftarKuis)){
		fscanf(daftarKuis, " %d %*[^\n]s", &kuisID);
	}
	++kuisID;
	
	freopen("daftarkuis.txt", "a", daftarKuis);
	fprintf(daftarKuis, "\n%04d %s", kuisID, kuis.judul);
	fflush(daftarKuis);
	
	return kuisID;
}	

void writeToFile_kuis(Kuis kuis){
	Soal * soalSekarang = kuis.soal;
	char pil_counter;
	
	char filename[32];
	sprintf(filename, "%04d.txt", kuis.ID);
	FILE * soal_file = fopen(filename, "w");
	
	sprintf(filename, "%04d_jawaban.txt", kuis.ID);
	FILE * jawaban_file = fopen(filename, "w");
	
	for( ; soalSekarang != NULL; soalSekarang = soalSekarang->next){
		fprintf(soal_file, "\n%s", soalSekarang->tanya);
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter)
			fprintf(soal_file, "\n%c. %s", pil_counter, soalSekarang->pil_jawaban[pil_counter - 'A']);
		
		fprintf(jawaban_file, "%c", soalSekarang->jawaban);
	}
	fflush(soal_file);
	fflush(jawaban_file);
}

//Membuka [kuisID].txt dan membaca userID yang ada (User yang telah mengerjakan kuis)
//Kemudian membuka [kuisID].jawaban.txt (kunci jawaban) dan membandingkannya
//dengan [kuisID].jawaban.[userID].txt (jawaban dari user). Kemudian menuliskan
//nilainya di [kuisID].txt
void penilaian(int kuisID){
	char filename[32];
	char inputToken[32];
	char input_conf;
	FILE * kuisData;
	FILE * jawabanFile;
	int scored;
	int kuisData_size;
	int i;
	int j_counter;
	int j_size;
	int benar_counter;
	
	sprintf(filename, "%04d_data.txt", kuisID);
	kuisData = fopen(filename, "r");
	
	fscanf(kuisData, " %[^\n]s", inputToken);
	scored = strcmp(inputToken, "SCORED") == 0;
	if(scored){
		printf("Kuis sudah dinilai. Apakah Anda ingin mengulang proses penilaian? (Y/N)\n");
		printf("Input: ");
		scanf("%c", &input_conf);
		if(input_conf == 'N')
			return;
	}
	
	sprintf(filename, "%04d_jawaban.txt", kuisID);
	jawabanFile = fopen(filename, "r");
	fscanf(jawabanFile, " %*s");
	j_size = ftell(jawabanFile);
	char jawaban_kunci [j_size];
	char jawaban [j_size];
	rewind(jawabanFile);
	fscanf(jawabanFile, "%s", jawaban_kunci);
	
	kuisData_size = 0;
	while(!feof(kuisData)){
		fscanf(kuisData, " %[^\n]s", inputToken);
			++kuisData_size;
	}
	
	int userID[kuisData_size];
	float nilai[kuisData_size];
	
	rewind(kuisData);
	fscanf(kuisData, " %*[^\n]s");
	for(i = 0; i < kuisData_size; ++i){
		fscanf(kuisData, " %[^\n]s", inputToken);
		sscanf(inputToken, " %d %*d", &userID[i]);
		sprintf(filename, "%04d_jawaban_%04d.txt", kuisID, userID[i]);
		printf("DBG filename %s\n", filename);
		freopen(filename, "r", jawabanFile);
		fscanf(jawabanFile, "%s", jawaban);
		benar_counter = 0;
		for(j_counter = 0; j_counter < j_size; ++j_counter){
			printf("DBG Jawaban %c %c\n", jawaban[j_counter], jawaban_kunci[j_counter]);
			if(jawaban[j_counter] == jawaban_kunci[j_counter])
				benar_counter++;
		}
		nilai[i] = ((float)benar_counter/(float)j_size) * 100.0;
	}
	
	sprintf(filename, "%04d_data.txt", kuisID);
	remove(filename);
	freopen(filename, "w", kuisData);
	fprintf(kuisData, "SCORED");
	for(i = 0; i < kuisData_size; ++i){
		fprintf(kuisData, "\n%04d %.0lf", userID[i], round(nilai[i]));
	}
	fflush(kuisData);
	fclose(kuisData);
	fclose(jawabanFile);
}

//Membuka [kuisID]_data.txt dan membaca userID yang ada serta nilai kuisnya,
//kemudian mencari userID yang sesuai pada akun.txt serta mendapatkan nama user
//dengan userID tersebut. Kemudian melakukan output nama serta nilai
void outputNilai(int kuisID){
	char filename[32];
	char inputToken[64];
	char nama_current[64];
	FILE * kuisData;
	FILE * user_db;
	int userID_kuisdata;
	int userID_userdb;
	float nilai_current;
	
	
	sprintf(filename, "%04d_data.txt", kuisID);
	kuisData = fopen(filename, "r");
	if(kuisData == NULL){
		printf("Kuis belum dinilai!\n");
		scanf(" %*s");
		return;
	}
	user_db = fopen("akun.txt", "r");
	
	system("CLS");
	system("clear");
	
	fscanf(kuisData, " %[^\n]s", inputToken);
	if(strcmp(inputToken, "NOSCORE") == 0){
		printf("Kuis ini belum dinilai\n");
		return;
	}
	
	while(!feof(kuisData)){
		fscanf(kuisData, " %[^\n]%*c", inputToken);
		sscanf(inputToken, " %d %f", &userID_kuisdata, &nilai_current);
		rewind(user_db);
		while(!feof(user_db)){
			fscanf(user_db, " %[^\n]%*c", nama_current);
			fscanf(user_db, " %*d %d", &userID_userdb);
			if(userID_kuisdata == userID_userdb){
				printf("%s %.0f\n", nama_current, nilai_current);
				break;
			}
		}
	}
}
