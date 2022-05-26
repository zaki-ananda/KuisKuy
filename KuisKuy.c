#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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
void registrasi();

void merge(Kuis * kuis, int low, int mid, int high);
void mergesort(Kuis *kuis, int low, int high);
void outputNilai(int kuisID, int userID);
int menu_pilihKuis();
void menu_murid(UserInfo user);
	void kerjakanKuis(int kuisID, int userID);
		Kuis readFromFile_kuis(int kuisID);
		void kerjakanSoal(char * judulKuis, Soal * soal, int soalCounter);
		void submitJawaban(Kuis kuis, int userID);
	int searchKuis ();
		void linear_search(Kuis * kuis, int n, char key[64], int searchResult[100]);
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
		//printf("DBG usermode %d\n", user.mode);
		//scanf(" %*s");
		system("Pause");
		if(!userQuit){
			if(user.mode == GURU){
				//printf("DBG Nama: %s\n", user.nama);
				//printf("DBG Mode: %d\n", user.mode);
				menu_guru(user);
			}
			if(user.mode == SISWA){
				//printf("DBG Nama: %s\n", user.nama);
				//printf("DBG Mode: %d\n", user.mode);
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
		system("CLS");
		
		userQuit = 0;
		repeatMenu = 1;
		
		printf("========== KuisKuy ==========\n\n");
		printf("1. Login\n");
		printf("2. Registrasi\n");
		printf("0. Exit\n");
		printf("\nInput: ");
		scanf(" %d", &menu);
		switch(menu){
			case 1:
				loginSuccess = login(user);
				if(loginSuccess) repeatMenu = 0;
				break;
			case 2:
				registrasi();
				break;
			case 0:
				userQuit = 1;
				repeatMenu = 0;
				break;
		}
	} while(repeatMenu);
	//printf("DBG menu_awal() returned %d\n", userQuit);
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
		system("CLS");
		
		db_count = 0;
		loginSuccess = 0;
		loginRetry = 0;
		
		printf("User: ");
		scanf(" %15s", input_user);
		//printf("DBG User: %s\n", input_user);
		printf("Password: ");
		scanf(" %15s", input_pass);
		//printf("DBG Password: %s\n", input_pass);
		
		rewind(login_db);
		rewind(user_db);
		
		//Melakukan pencarian terhadap username dan password yang cocok pada login.txt
		while(!feof(login_db) && !loginSuccess){
			fscanf(login_db, " %15s %15s", db_user, db_pass);
			//printf("DBG User: %s\n", db_user);
			//printf("DBG User: %s\n", db_pass);
			
			//Jika ditemukan username dan password yang cocok (login sukses)
			if(strcmp(input_user, db_user) == 0
				&& strcmp(input_pass, db_pass) == 0)
			{
				//Lakukan pencarian pada akun.txt untuk mendapatkan nama dan mode user
				while(db_count-- > 0){
					fscanf(user_db, " %63[^\n]%*c %d %d", user->nama, &user->mode, &user->ID);
					//printf("DBG Nama: %s\n", user->nama);
					//printf("DBG Mode: %d - %d\n", user->mode, db_count);
				}
				fscanf(user_db, " %63[^\n]%*c %d %d", user->nama, &user->mode, &user->ID);
				//printf("DBG Nama: %s\n", user->nama);
				//printf("DBG Mode: %d\n", user->mode);
				loginSuccess = 1;
				loginRetry = 0;
			}
			else
				++db_count;
		}
		//scanf(" %*s");
		//system("Pause");
		
		if(!loginSuccess){
			printf("\nERROR: Login gagal!\n");
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
	
	return loginSuccess;
}	

void registrasi(){
	FILE * login_db = fopen("login.txt", "r");
	FILE * user_db = fopen("akun.txt", "r");
	char input_user[16];
	char input_pass[16];
	char db_user[16];
	char nama[64];
	int repeatMenu;
	int invalidInput;
	int userID;
	int userMode;
	
	do{
		system("CLS");
		
		repeatMenu = 0;
		
		printf("Username: ");
		scanf(" %s", input_user);
		printf("Password: ");
		scanf(" %s", input_pass);
		
		while(login_db != NULL && !feof(login_db)){
			fscanf(login_db, " %s %*s", db_user);
			if(strcmp(input_user, db_user) == 0){
				printf("Username tersebut sudah terdaftar!\n");
				repeatMenu = 1;
				system("Pause");
				break;
			}
		}
	} while(repeatMenu);
	
	printf("Nama Lengkap: ");
	scanf(" %[^\n]%*c", nama);
	
	userID = 1;
	while(user_db != NULL && !feof(user_db)){
		fscanf(user_db, " %*[^\n] %*d %d", &userID);
	}
	++userID;
	
	do{
		printf("\nPilih Mode: \n1. Guru\n2. Siswa\nInput: ");
		scanf(" %d", &userMode);
		invalidInput = userMode != 1 && userMode != 2;
		if(invalidInput)
			printf("ERROR: Input harus sesuai format!\n");
	} while(invalidInput);
	
	freopen("login.txt", "a", login_db);
	freopen("akun.txt", "a", user_db);
	fprintf(login_db, "\n%s %s", input_user, input_pass);
	fprintf(user_db, "\n%s\n%d %04d", nama, userMode, userID);
	
	fflush(login_db);
	fflush(user_db);
	fclose(login_db);
	fclose(user_db);
	
	printf("\nRegistrasi berhasil!\n");
	system("Pause");
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

		#pragma omp task
		mergesort(kuis, low, mid);
		#pragma omp task
		mergesort(kuis, mid + 1, high);
		#pragma omp taskwait
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
	
	if(daftarKuis == NULL){
		printf("ERROR: File daftarkuis.txt tidak ditemukan atau belum ada kuis sama sekali!\n");
		system("Pause");
		return 0;
	}
	
	system("CLS");
	
	//Menghitung banyaknya kuis yang terdaftar
	while(!feof(daftarKuis)){
		if(fscanf(daftarKuis, " %d %[^\n]s", &inputTest.ID, inputTest.judul) == 2)
			++counter;
		else{
			printf("ERROR: File daftarkuis.txt tidak sesuai format!\n");
			exit(0);
		}
	}
	//printf("DBG kuisCounter: %d\n", counter);
	
	int kuisArrSize = counter;
	Kuis kuisArr[kuisArrSize];
	
	
	//Memasukkan kuis yang terdaftar ke kuisArr
	rewind(daftarKuis);
	for(counter = 0; !feof(daftarKuis); ++counter){
		fscanf(daftarKuis, " %d %[^\n]s", &kuisArr[counter].ID, kuisArr[counter].judul);
	}
	
	#pragma omp parallel num_threads(omp_get_max_threads())
	{
		#pragma omp single
		mergesort(kuisArr, 0, kuisArrSize-1);
	}
	
	//Menampilkan opsi kuis yang ada + error handling input
	do{
		system("CLS");
		printf("Pilih salah satu kuis di bawah ini!\n");
		rewind(daftarKuis);
		for(counter = 0; counter < kuisArrSize; ++counter){
			printf("%d. %s\n", counter+1, kuisArr[counter].judul);
		}
		printf("0. Keluar\n");
		printf("Input: ");
		scanf(" %d", &menuInput);
	} while(menuInput < 0 || menuInput > counter);
	
	fclose(daftarKuis);
	
	if(menuInput == 0) //User ingin keluar
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
				retVal = searchKuis();
				if(retVal != 0)
					kerjakanKuis(retVal, user.ID);
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
	
	//Looping dari soal pertama hingga soal terakhir
	soalSekarang = kuis.soalPertama;
	do {
		system ("CLS");
		
		kerjakanSoal(kuis.judul, soalSekarang, soalCounter);
		
		if (soalSekarang->prev != NULL)
			printf("(<) Soal Sebelumnya\n");
		if (soalSekarang->next != NULL) 
			printf("(>) Soal Berikutnya\n");
		else 
			printf("(>) Kumpulkan jawaban\n");
		
		//Error handling input
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
				} else{ //Soal terakhir
					submitJawaban(kuis, userID);
					soalSekarang = NULL;
				}
		}
	} while (soalSekarang != NULL);
	
	system("CLS");
	
	printf("Jawaban berhasil disubmit!\n");
	system("Pause");
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
	
	//inisialisasi linked list soal
	soalSekarang = malloc(sizeof(Soal)); 
	kuis.soalPertama = soalSekarang;
	
	//Looping pembacaan soal
	for(soalSekarang->prev = NULL; !feof(kuisFile); soalSekarang = soalSekarang->next){
		fscanf(kuisFile, "\n %[^\n]s", soalSekarang->tanya); 	
		
		//looping baca pilihan jawaban
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter) 
			fscanf(kuisFile, "\n%*c. %[^\n]", soalSekarang->pil_jawaban[pil_counter - 'A']);
		
		//Inisialisasi default value jawaban
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
	
	fclose(kuisFile);
	fclose(daftarKuis);
	
	return kuis;
}

void kerjakanSoal(char * judulKuis, Soal * soal, int soalCounter){
	char pil_counter;
	int invalidInput;
	char pil_jawaban;
	
	//Looping untuk error handling input pilihan jawaban
	do{
		system("CLS");

		printf("===== %s =====\n", judulKuis);
		
		printf("\nNo %d. ", soalCounter);
		printf("%s\n", soal->tanya);

		//Looping output pilihan jawaban
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter){
			printf("%c. ", pil_counter);
			printf("%s\n", soal->pil_jawaban[pil_counter - 'A']);
		}
		
		//Output jawaban apabila user kembali ke soal sebelumnya dan user telah menjawabnya
		if(soal->jawaban != ' ')
			printf("Jawaban sebelumnya: %c", soal->jawaban);
		
		printf("\nJawab: ");
		scanf(" %c", &pil_jawaban);
		
		invalidInput = (pil_jawaban < 'A') || (pil_jawaban > 'D');
		if(!invalidInput) //If input is valid
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
	
	//Looping untuk memasukkan jawaban ke file
	for(soalSekarang = kuis.soalPertama; soalSekarang != NULL; soalSekarang = soalSekarang->next)
		fprintf(jawabanFile, "%c", soalSekarang->jawaban);

	//Masukkan userID ke [kuisID]_data.txt
	//menandakan bahwa user telah mengerjakan kuis
	sprintf(filename, "%04d_data.txt", kuis.ID);
	kuisData = fopen(filename, "a");
	fprintf(kuisData, "\n%04d", userID);

	fflush(jawabanFile);
	fflush(kuisData);
	fclose(jawabanFile);
	fclose(kuisData);
}

void linear_search(Kuis * kuis, int n, char key[64], int searchResult[100]) {
    int i, counter = 0;
    
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (i = 0; i < n; i++) {
        if (strstr(kuis[i].judul, key) != NULL) { 
			#pragma omp critical
			{ 
				searchResult[counter] = i;
				++counter;
			}
    	}
	}
	searchResult[counter] = -1;
}

int searchKuis () {
	FILE * daftarKuis = fopen("daftarkuis.txt", "r");
    char key[64];
	int counter = 0;
	int searchResult[100];
	int menuInput, i_result, i = 0;
    
    if(daftarKuis == NULL){
		printf("ERROR: File daftarkuis.txt tidak ditemukan atau belum ada kuis sama sekali!\n");
		system("Pause");
		return 0;
	}
    
	system("CLS");
	
	// nyari ukuran
	rewind(daftarKuis);
	for(counter = 0; !feof(daftarKuis); ++counter){
		fscanf(daftarKuis, " %*d %*[^\n]s");
	}
	
	int kuisArrSize = counter;
	Kuis kuisArr[kuisArrSize];
	
	// masukin isi dari file ke kuisArr
	rewind(daftarKuis);
	for(counter = 0; !feof(daftarKuis); ++counter){
		fscanf(daftarKuis, " %d %[^\n]s", &kuisArr[counter].ID, kuisArr[counter].judul);
		//printf("%s", kuisArr[counter].judul);
	}
	
	printf("\n======== Pencarian Kuis ========\n\n");
	printf("Masukkan search key: ");
	scanf(" %[^\n]s", key);
	printf("\n");
	
	linear_search(kuisArr, kuisArrSize, key, searchResult);
	
	//Output menu hasil pencarian + error handling input pemilihan kuis
	do{
		system("CLS");
		if(searchResult[0] == -1) {
			printf("Kuis tidak ditemukan");
			system("Pause");
			return 0;
		} 
		else {
			//Looping output hasil pencarian
			for(i=0 ; searchResult[i] != -1; ++i) {
				i_result = searchResult[i];
				printf("%d. %s\n", i+1, kuisArr[i_result].judul);
			}
		}
		
		printf("\n=================================\n");
		printf("0. Keluar\n");
		printf("Input: ");
		scanf(" %d", &menuInput);
	} while(menuInput < 0 || menuInput > i);
	
	fclose(daftarKuis);
	
	if(menuInput == 0)
		return 0;
	else{ //Return ID dari kuis yang dipilih
		i_result = searchResult[menuInput-1];
		//printf("DBG returned kuisID%d\n", kuisArr[i_result].ID);
		scanf( "%*s");
		return kuisArr[i_result].ID;
	}
}

//Function menu admin beserta menunya
void menu_guru(UserInfo user){
    int pilihan;
    int repeatMenu;
    int retVal;
    
    do{
		system("CLS");
		
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
				//printf("DBG Finished buatKuis\n");
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
				//system("Pause");
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
	
	printf("Judul kuis\n");
	scanf(" %[^\n]s", kuisBaru.judul);
	
	//Inisialisasikan linked list soal
	kuisBaru.soalPertama = malloc(sizeof(Soal));
	Soal * soalSekarang = kuisBaru.soalPertama;
	soalSekarang->prev = NULL;
	
	//Looping Input soal hingga user sudah selesai
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
	printf("Kuis berhasil ditambahkan!\n");
	system("Pause");
}

void inputSoal(Soal * soal){
	char pil_counter;
	int invalidInput;
	
	system("CLS");
	
	printf("Pertanyaan: \n");
	scanf(" %[^\n]s ", soal->tanya);
	
	//Looping pilihan jawaban A-D
	for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter){
		printf("%c. ", pil_counter);
		scanf(" %[^\n]s ", soal->pil_jawaban[pil_counter - 'A']);
	}
	
	//Input kunci jawaban + error handling
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
	
	//Apabila file tidak ditemukan / Kuis pertama yang dibuat
	if(daftarKuis == NULL){
		freopen("daftarkuis.txt", "a", daftarKuis);
		fprintf(daftarKuis, "0001 %s", kuis.judul);
		fflush(daftarKuis);
		fclose(daftarKuis);
		return 1;
	}
	
	//Pada akhir loop, nilai kuisID adalah ID dari kuis yang terakhir ditambahkan.
	while(!feof(daftarKuis)){
		fscanf(daftarKuis, " %d %*[^\n]s", &kuisID);
	}
	++kuisID;
	
	freopen("daftarkuis.txt", "a", daftarKuis);
	fprintf(daftarKuis, "\n%04d %s", kuisID, kuis.judul);
	
	fflush(daftarKuis);
	fclose(daftarKuis);
	
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
	
	//Masukkan pertanyaan dan pil. jawaban kuis ke file [kuisID].txt
	//serta kunci jawaban ke [kuisID]_jawaban.txt
	for( ; soalSekarang != NULL; soalSekarang = soalSekarang->next){
		fprintf(soal_file, "\n%s", soalSekarang->tanya);
		for(pil_counter = 'A'; pil_counter <= 'D'; ++pil_counter)
			fprintf(soal_file, "\n%c. %s", pil_counter, soalSekarang->pil_jawaban[pil_counter - 'A']);
		
		fprintf(jawaban_file, "%c", soalSekarang->jawaban);
	}
	
	//Untuk membedakan kuis yang sudah dinilai dengan yang belum
	fprintf(data_file, "NOSCORE"); 
	
	fflush(data_file);
	fflush(soal_file);
	fflush(jawaban_file);
	fclose(data_file);
	fclose(soal_file);
	fclose(jawaban_file);
}

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
		system("Pause");
		return;
	}
	
	fscanf(kuisData, " %[^\n]s", inputToken);
	scored = strcmp(inputToken, "SCORED") == 0;
	if(scored){
		printf("Kuis sudah pernah dinilai.\n Apakah Anda ingin mengulang proses penilaian? (Y/N)\n");
		printf("Input: ");
		scanf(" %c", &input_conf);
		if(input_conf == 'N')
			return;
	} else if(strcmp(inputToken, "NOSCORE") != 0){
		printf("ERROR: Tidak ada flag pada file data kuis!\n");
		system("Pause");
		return;
	}
	
	//Buka file [kuisID]_jawaban.txt dan hitung banyaknya jawaban/soal pada kuis
	sprintf(filename, "%04d_jawaban.txt", kuisID);
	jawabanFile = fopen(filename, "r");
	fscanf(jawabanFile, " %*s");
	j_size = ftell(jawabanFile);
	
	char jawaban_kunci [j_size];
	char jawaban [j_size];
	
	//Salin isi [kuisID]_jawaban.txt ke array jawaban_kunci
	rewind(jawabanFile);
	fscanf(jawabanFile, "%s", jawaban_kunci);
	
	//Hitung banyaknya user yang sudah mengerjakan kuis
	kuisData_size = 0;
	while(!feof(kuisData)){
		fscanf(kuisData, " %[^\n]s", inputToken);
		++kuisData_size;
	}
	
	int userID[kuisData_size];
	float nilai[kuisData_size];
	
	//Lakukan penilaian pada semua user yang mengerjakan kuis
	rewind(kuisData);
	fscanf(kuisData, " %*[^\n]s"); //Lewati flag SCORED/NOSCORE
	for(i = 0; i < kuisData_size; ++i){
		//Baca satu line saja (bisa saja berisi userID saja atau userID + nilai hasil koreksi sebelumnya)
		fscanf(kuisData, " %[^\n]s", inputToken);
		sscanf(inputToken, " %d %*d", &userID[i]);
		
		//Buka file yang berisi jawaban user
		sprintf(filename, "%04d_jawaban_%04d.txt", kuisID, userID[i]);
		freopen(filename, "r", jawabanFile);
		fscanf(jawabanFile, "%s", jawaban);
		
		
		//Bandingkan jawaban user dengan kunci jawaban
		benar_counter = 0;
		#pragma omp parallel for num_threads(omp_get_max_threads())
		for(j_counter = 0; j_counter < j_size; ++j_counter){
			//printf("DBG Jawaban %c %c\n", jawaban[j_counter], jawaban_kunci[j_counter]);
			if(jawaban[j_counter] == jawaban_kunci[j_counter]){
				#pragma omp critical
				benar_counter++;
			}
		}
		//printf("DBG benar_counter %d\n", benar_counter);
		system("Pause");
		nilai[i] = ((float)benar_counter/(float)j_size) * 100.0;
	}
	
	//Hapus hasil penilaian sebelumnya dan masukkan hasil penilaian baru ke [kuisID]_data.txt
	sprintf(filename, "%04d_data.txt", kuisID);
	remove(filename);
	freopen(filename, "w", kuisData);
	fprintf(kuisData, "SCORED");
	for(i = 0; i < kuisData_size; ++i){ //Memasukkan userID dan nilai terbaru
		fprintf(kuisData, "\n%04d %.0lf", userID[i], round(nilai[i]));
	}
	
	fflush(kuisData);
	fclose(kuisData);
	fclose(jawabanFile);
	
	system("CLS");
	printf("Penilaian berhasil dilakukan!\n");
	system("Pause");
}

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
		printf("ERROR: Tidak ada file data!\n");
		system("Pause");
	}
	
	fscanf(kuisData, " %[^\n]s", inputToken);
	
	if(strcmp(inputToken, "NOSCORE") == 0){
		printf("Kuis ini belum dinilai\n");
		system("Pause");
		return;
	} else if(strcmp(inputToken, "SCORED") != 0){
		printf("ERROR: Tidak ada flag penilaian pada file data kuis!\n");
		system("Pause");
		return;
	}
	
	user_db = fopen("akun.txt", "r");
	
	system("CLS");
	
	//Looping seluruh isi [kuisID]_data.txt
	while(!feof(kuisData)){
		fscanf(kuisData, " %[^\n]%*c", inputToken);
		nilai_current = -1; //Apabila tidak ada di .txt, maka nilainya -1
		sscanf(inputToken, " %d %f", &userID_kuisdata, &nilai_current);
		
		//Output semua nilai
		if(userID == 0){ 
			//Cari nama dari userID yang ditemukan dengan membaca file akun.txt
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
		
		//Output nilai dari sebuah user saja
		else{
			if(userID_kuisdata == userID){
				printf("Nilai Anda adalah %.0f\n", nilai_current);
				break;
			}
		}
	}
	
	fclose(kuisData);
	fclose(user_db);
	system("Pause");
}
