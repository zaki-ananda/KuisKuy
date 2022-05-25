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
	Soal * soalPertama;
} Kuis;


int menu_awal(UserInfo * user);
int login(UserInfo * user);
void merge(Kuis * kuis, int low, int mid, int high);
void mergesort(Kuis *kuis, int low, int high);
void outputNilai(int kuisID, int userID);
int menu_pilihKuis();
void menu_murid(UserInfo user);
	void kerjakanKuis(int kuisID, int userID);
		Kuis readFromFile_kuis(int kuisID);
		void kerjakanSoal(char * judulKuis, Soal * soal, int soalCounter);
		void submitJawaban(Kuis kuis, int userID);
void menu_guru(UserInfo user);
	void buatKuis(void);
		void inputSoal(Soal * soal);
		int assign_kuisID(Kuis kuis);
		void writeToFile_kuis(Kuis kuis);
	void penilaian(int kuisID);
	


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

void merge(Kuis * kuis, int low, int mid, int high){
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;
    Kuis left[n1];
    Kuis right[n2];

    for(i = 0; i < n1; i++){
        left[i] = kuis[low+i];
    }
	for(j = 0; j < n2; j++){
        right[j] = kuis[mid+1+j];
    }

    i = 0;
    j = 0;
    k = low;

   while(i < n1 && j < n2) {
        if(strcmp(left[i].judul, right[j].judul) <= 0){
            kuis[k] = left[i];
            i++;
        } else{
            kuis[k] = right[j];
            j++;
        }
        k++;
    }
	while (i < n1) { 
        kuis[k] = left[i];
        i++; 
        k++; 
    } 
    while(j < n2){
        kuis[k] = right[j];
        j++;
        k++;
    }
}

void mergesort(Kuis *kuis, int low, int high){
    if(low < high){
        int mid = (low + high) / 2;

        mergesort(kuis, low, mid);
        mergesort(kuis, mid + 1, high);
        merge(kuis, low, mid, high);
    }
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
	
	int kuisArrSize = counter;
	Kuis kuisArr[kuisArrSize];
	
	
	rewind(daftarKuis);
	for(counter = 0; !feof(daftarKuis); ++counter){
		fscanf(daftarKuis, " %d %[^\n]s", &kuisArr[counter].ID, kuisArr[counter].judul);
	}
	
	mergesort(kuisArr, 0, kuisArrSize-1);
	
	do{
		system("CLS");
		system("clear");
		printf("Pilih salah satu kuis di bawah ini!\n");
		rewind(daftarKuis);
		for(counter = 0; counter < kuisArrSize; ++counter){
			printf("%d. %s\n", counter+1, kuisArr[counter].judul);
		}
		printf("0. Keluar\n");
		printf("Input: ");
		scanf(" %d", &menuInput);
	} while(menuInput < 0 || menuInput > counter);
	
	if(menuInput == 0)
		return 0;
	else
		return kuisArr[menuInput-1].ID;
}

//Function menu user beserta menunya
void menu_murid(UserInfo user){
    int pilihan;
    int repeatMenu;
    int retVal;
    
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
				retVal = menu_pilihKuis();
				if(retVal != 0)
					kerjakanKuis(retVal, user.ID);
				break;
			case 2:
				printf("Cari kuis");
				break;
			case 3:
				retVal = menu_pilihKuis();
				if(retVal != 0)
					outputNilai(retVal, user.ID);
				break;
			case 0:
				repeatMenu = 0;
				break;
		}
	} while(repeatMenu);
}

void kerjakanKuis(int kuisID, int userID){
	int soalCounter = 1;
	int invalidInput;
	char menuInput;
	
	Soal * soalSekarang;
	
	Kuis kuis = readFromFile_kuis(kuisID);

	soalSekarang = kuis.soalPertama;
	do {
		system ("CLS");
		system("clear");
		
		kerjakanSoal(kuis.judul, soalSekarang, soalCounter);
		
		if (soalSekarang->prev != NULL)
			printf("(<) Soal Sebelumnya\n");
		if (soalSekarang->next != NULL) 
			printf("(>) Soal Berikutnya\n");
		else 
			printf("(>) Kumpulkan jawaban\n");
		
		do{
			printf("Input: ");
			scanf(" %c", &menuInput);
			invalidInput = (menuInput != '<') && (menuInput != '>');
			if(invalidInput)
				printf("ERROR: Input tidak sesuai format!\n");
		} while(invalidInput);
		
		switch(menuInput){
			case '<':
				soalSekarang = soalSekarang->prev;
				--soalCounter;
				break;
			case '>':
				if(soalSekarang->next != NULL){
					soalSekarang = soalSekarang->next;
					++soalCounter;
				} else{
					submitJawaban(kuis, userID);
					soalSekarang = NULL;
				}
		}
	} while (soalSekarang != NULL);
	
	system("CLS");
	system("clear");
	
	printf("Soal berhasil disubmit!\n");
	scanf(" %*s");
}

Kuis readFromFile_kuis(int kuisID){
	Kuis kuis;
	FILE * kuisFile;
	Soal * soalSekarang;
	char filename[32];
	char pil_counter;
	int kuisID_fromFile;
	
	kuis.ID = kuisID;
	sprintf(filename, "%04d.txt", kuis.ID);
	kuisFile = fopen(filename, "r");
	
	soalSekarang = malloc(sizeof(Soal)); //inisialisasi linked list soal
	kuis.soalPertama = soalSekarang;
	
	for(soalSekarang->prev = NULL; !feof(kuisFile); soalSekarang = soalSekarang->next){ //looping baca soal
		printf("DBG ftell %ld\n", ftell(kuisFile));
		fscanf(kuisFile, "\n %[^\n]s", soalSekarang->tanya); 	
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter) //looping baca pilihan jawaban
			fscanf(kuisFile, "\n%*c. %[^\n]", soalSekarang->pil_jawaban[pil_counter - 'A']);
		soalSekarang->jawaban = ' ';
		
		//Buat soal baru apabila masih ada soal yang perlu dibaca
		if(!feof(kuisFile)){
			soalSekarang->next = malloc(sizeof(Soal));
			(soalSekarang->next)->prev = soalSekarang;
		} else
			soalSekarang->next = NULL;
	}
	
	//Buka file daftarkuis.txt untuk mendapatkan judul kuis
	FILE * daftarKuis = fopen("daftarkuis.txt", "r");
	while(!feof(daftarKuis)){
		fscanf(daftarKuis, " %d %[^\n]s", &kuisID_fromFile, kuis.judul);
		if(kuisID_fromFile == kuis.ID)
			break;
	}
	
	return kuis;
}

void kerjakanSoal(char * judulKuis, Soal * soal, int soalCounter){
	char pil_counter;
	int invalidInput;
	char pil_jawaban;
	
	do{
		system("CLS");
		system("clear");
		
		printf("===== %s =====\n", judulKuis);
		
		printf("\nNo %d. ", soalCounter);
		printf("%s\n", soal->tanya);

		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter){
			printf("%c. ", pil_counter);
			printf("%s\n", soal->pil_jawaban[pil_counter - 'A']);
		}
		
		//Output jawaban apabila user kembali ke soal sebelumnya
		if(soal->jawaban != ' ')
			printf("Jawaban sebelumnya: %c", soal->jawaban);
		
		printf("\nJawab: ");
		scanf(" %c", &pil_jawaban);
		
		invalidInput = (pil_jawaban < 'A') || (pil_jawaban > 'D');
		if(!invalidInput)
			soal->jawaban = pil_jawaban;
	} while(invalidInput);
}

void submitJawaban(Kuis kuis, int userID){
	FILE * kuisData;
	FILE * jawabanFile;
	Soal * soalSekarang;
	char filename[32];
	
	
	sprintf(filename, "%04d_jawaban_%04d.txt", kuis.ID, userID);
	jawabanFile = fopen(filename, "w");
	for(soalSekarang = kuis.soalPertama; soalSekarang != NULL; soalSekarang = soalSekarang->next)
		fprintf(jawabanFile, "%c", soalSekarang->jawaban);

	sprintf(filename, "%04d_data.txt", kuis.ID);
	kuisData = fopen(filename, "a");
	fprintf(kuisData, "\n%04d", userID);

	fflush(jawabanFile);
	fflush(kuisData);
}

//Function menu admin beserta menunya
void menu_guru(UserInfo user){
    int pilihan;
    int repeatMenu;
    int retVal;
    
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
				retVal = menu_pilihKuis();
				if(retVal != 0)
					penilaian(retVal);
				break;
			case 3:
				retVal = menu_pilihKuis();
				if(retVal != 0)
					outputNilai(retVal, 0);
				//scanf(" %*s");
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
	
	
	kuisBaru.soalPertama = malloc(sizeof(Soal));
	Soal * soalSekarang = kuisBaru.soalPertama;
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
	
	system("CLS");
	system("clear");
	printf("Kuis berhasil ditambahkan!");
	scanf(" %*s");
}

void inputSoal(Soal * soal){
	char pil_counter;
	int invalidInput;
	
	system("CLS");
	system("clear");
	
	printf("Pertanyaan: \n");
	scanf(" %[^\n]s ", soal->tanya);
	
	for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter){
		printf("%c. ", pil_counter);
		scanf(" %[^\n]s ", soal->pil_jawaban[pil_counter - 'A']);
	}
	
	do{
		printf("Kunci Jawaban: ");
		scanf(" %c", &soal->jawaban);
		invalidInput = (soal->jawaban < 'A') || (soal->jawaban > 'D');
		if(invalidInput)
			printf("ERROR: Jawaban harus berupa pilihan ABCD\n\n");
	} while(invalidInput);
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
	Soal * soalSekarang = kuis.soalPertama;
	char pil_counter;
	
	char filename[32];
	sprintf(filename, "%04d.txt", kuis.ID);
	FILE * soal_file = fopen(filename, "w");
	
	sprintf(filename, "%04d_jawaban.txt", kuis.ID);
	FILE * jawaban_file = fopen(filename, "w");
	
	sprintf(filename, "%04d_data.txt", kuis.ID);
	FILE * data_file = fopen(filename, "w");
	
	for( ; soalSekarang != NULL; soalSekarang = soalSekarang->next){
		fprintf(soal_file, "\n%s", soalSekarang->tanya);
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter)
			fprintf(soal_file, "\n%c. %s", pil_counter, soalSekarang->pil_jawaban[pil_counter - 'A']);
		
		fprintf(jawaban_file, "%c", soalSekarang->jawaban);
	}
	fprintf(data_file, "NOSCORE");
	
	fflush(data_file);
	fflush(soal_file);
	fflush(jawaban_file);
}

//Membuka [kuisID]_data.txt dan membaca userID yang ada (User yang telah mengerjakan kuis)
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
	
	if(kuisData == NULL || feof(kuisData)){
		remove(filename);
		printf("Kuis belum dikerjakan oleh siapapun!\n");
		scanf(" %*s");
		return;
	}
	
	printf("DBG Opened %s\n", filename);
	fscanf(kuisData, " %[^\n]s", inputToken);
	printf("DBG Reached here!\n");
	scored = strcmp(inputToken, "SCORED") == 0;
	if(scored){
		printf("Kuis sudah pernah dinilai.\n Apakah Anda ingin mengulang proses penilaian? (Y/N)\n");
		printf("Input: ");
		scanf(" %c", &input_conf);
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
	
	system("CLS");
	system("clear");
	printf("Penilaian berhasil dilakukan!\n");
	scanf(" %*s");
}

//Membuka [kuisID]_data.txt dan membaca userID yang ada serta nilai kuisnya,
//kemudian mencari userID yang sesuai pada akun.txt serta mendapatkan nama user
//dengan userID tersebut. Kemudian melakukan output nama serta nilai
//Apabila userID 0, maka fungsi akan mengoutput nilai seluruh pengerjaan
void outputNilai(int kuisID, int userID){
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
		scanf(" %*s");
		return;
	}
	
	while(!feof(kuisData)){
		fscanf(kuisData, " %[^\n]%*c", inputToken);
		sscanf(inputToken, " %d %f", &userID_kuisdata, &nilai_current);
		if(userID == 0){ //Output semua nilai
			rewind(user_db);
			while(!feof(user_db)){
				fscanf(user_db, " %[^\n]%*c", nama_current);
				fscanf(user_db, " %*d %d", &userID_userdb);
				if(userID_kuisdata == userID_userdb){
					printf("%s %.0f\n", nama_current, nilai_current);
					break;
				}
			}
		} else{
			if(userID_kuisdata == userID){
				printf("Nilai Anda adalah %.0f\n", nilai_current);
				break;
			}
		}
	}
	scanf(" %*s");
}

