#include <iostream>
#include <string>
using namespace std;

struct Kendaraan {
    string nopol;
    string pemilik;
    string status;
    Kendaraan *kiri;
    Kendaraan *kanan;
};

struct StackNode {
    string aksi;
    string nopol;
    StackNode *next;
};

struct QueueNode {
    string nopol;
    QueueNode *next;
};

Kendaraan *root = NULL;
StackNode *top = NULL;
QueueNode *antriDepan = NULL;
QueueNode *antriBelakang= NULL;

int kapasitasParkir = 3;
int jumlahParkir = 0;

// ================= PTB =================

Kendaraan* buatNode(string nopol, string pemilik) {
    Kendaraan *baru = new Kendaraan;
    baru->nopol = nopol;
    baru->pemilik = pemilik;
    baru->status = "Di Luar Parkiran";
    baru->kiri = NULL;
    baru->kanan = NULL;
    return baru;
}

Kendaraan* insert(Kendaraan *root, string nopol, string pemilik) {
    if(root == NULL) {
        return buatNode(nopol, pemilik);
    }

    if(nopol < root->nopol) {
        root->kiri = insert(root->kiri, nopol, pemilik);
    } else if(nopol > root->nopol) {
        root->kanan = insert(root->kanan, nopol, pemilik);
    }

    return root;
}

Kendaraan* cari(Kendaraan *root, string nopol) {
    if(root == NULL || root->nopol == nopol) {
        return root;
    }

    if(nopol < root->nopol) {
        return cari(root->kiri, nopol);
    }

    return cari(root->kanan, nopol);
}

void tampil(Kendaraan *root) {
    if(root != NULL) {
        tampil(root->kiri);

        cout << "Nomor Polisi : " << root->nopol << endl;
        cout << "Nama Pemilik : " << root->pemilik << endl;
        cout << "Status       : " << root->status << endl;
        cout << "---------------------------" << endl;

        tampil(root->kanan);
    }
}

Kendaraan* minValue(Kendaraan *node) {
    Kendaraan *current = node;

    while(current && current->kiri != NULL) {
        current = current->kiri;
    }

    return current;
}

Kendaraan* hapus(Kendaraan *root, string nopol) {
    if(root == NULL) {
        return root;
    }

    if(nopol < root->nopol) {
        root->kiri = hapus(root->kiri, nopol);
    } else if(nopol > root->nopol) {
        root->kanan = hapus(root->kanan, nopol);
    } else {
        if(root->kiri == NULL) {
            Kendaraan *temp = root->kanan;
            delete root;
            return temp;
        }
        else if(root->kanan == NULL) {
            Kendaraan *temp = root->kiri;
            delete root;
            return temp;
        }

        Kendaraan *temp = minValue(root->kanan);

        root->nopol = temp->nopol;
        root->pemilik = temp->pemilik;
        root->status = temp->status;

        root->kanan = hapus(root->kanan, temp->nopol);
    }

    return root;
}

// ================= STACK =================

void push(string aksi, string nopol) {
    StackNode *baru = new StackNode;
    baru->aksi = aksi;
    baru->nopol = nopol;
    baru->next = top;
    top = baru;
}

void undo() {
    if(top == NULL) {
        cout << "Tidak ada aksi untuk di-undo" << endl;
        return;
    }

    StackNode *hapusNode = top;
    Kendaraan *k = cari(root, top->nopol);

    if(top->aksi == "masuk") {
        if(k != NULL) {
            k->status = "Di Luar Parkiran";
            jumlahParkir--;
            cout << "Undo: Kendaraan keluar dari parkiran" << endl;
        }
    }
    else if(top->aksi == "keluar") {
        if(k != NULL) {
            k->status = "Di Parkiran";
            jumlahParkir++;
            cout << "Undo: Kendaraan masuk kembali ke parkiran" << endl;
        }
    }

    top = top->next;
    delete hapusNode;
}

// ================= QUEUE =================

void enqueue(string nopol) {
    QueueNode *baru = new QueueNode;
    baru->nopol = nopol;
    baru->next = NULL;

    if(antriDepan == NULL) {
        antriDepan = antriBelakang = baru;
    } else {
        antriBelakang->next = baru;
        antriBelakang = baru;
    }
}

string dequeue() {
    if(antriDepan == NULL) {
        return "";
    }

    QueueNode *hapusNode = antriDepan;
    string nopol = hapusNode->nopol;

    antriDepan = antriDepan->next;

    if(antriDepan == NULL) {
        antriBelakang = NULL;
    }

    delete hapusNode;
    return nopol;
}

void tampilAntrian() {
    if(antriDepan == NULL) {
        cout << "Antrian kosong" << endl;
        return;
    }

    QueueNode *bantu = antriDepan;

    while(bantu != NULL) {
        cout << bantu->nopol << " menunggu giliran masuk parkiran" << endl;
        bantu = bantu->next;
    }
}

// ================= FITUR =================

void tambahKendaraan() {
    string nopol, pemilik;

    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    cout << "Masukkan Nama Pemilik : ";
    getline(cin, pemilik);

    if(cari(root, nopol) != NULL) {
        cout << "Data kendaraan sudah ada" << endl;
        return;
    }

    root = insert(root, nopol, pemilik);

    cout << "Data kendaraan berhasil ditambahkan" << endl;
}

void kendaraanMasuk() {
    string nopol;

    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    Kendaraan *k = cari(root, nopol);

    if(k == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    if(k->status == "Di Parkiran") {
        cout << "Kendaraan sudah berada di parkiran" << endl;
        return;
    }

    if(jumlahParkir < kapasitasParkir) {
        k->status = "Di Parkiran";
        jumlahParkir++;

        push("masuk", nopol);

        cout << "Kendaraan berhasil masuk" << endl;
    }
    else {
        enqueue(nopol);
        cout << "Parkiran penuh, kendaraan masuk ke dalam antrian" << endl;
    }
}

void kendaraanKeluar() {
    string nopol;

    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    Kendaraan *k = cari(root, nopol);

    if(k == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    if(k->status == "Di Luar Parkiran") {
        cout << "Kendaraan sedang tidak berada di parkiran" << endl;
        return;
    }

    k->status = "Di Luar Parkiran";
    jumlahParkir--;

    push("keluar", nopol);

    cout << "Kendaraan berhasil keluar" << endl;

    if(antriDepan != NULL) {
        string next = dequeue();
        Kendaraan *antri = cari(root, next);

        if(antri != NULL) {
            antri->status = "Di Parkiran";
            jumlahParkir++;

            cout << "Kendaraan berikutnya langsung masuk ke area parkir dari antrian" << endl;
        }
    }
}

void hapusKendaraan() {
    string nopol;

    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    if(cari(root, nopol) == NULL) {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    root = hapus(root, nopol);

    cout << "Data kendaraan berhasil dihapus" << endl;
}

// ================= MAIN =================

int main() {
    int pilih;

    do {
        cout << "\n===== SISTEM PARKIR BANDARA GSA =====" << endl;
        cout << "1. Tambah Data Kendaraan" << endl;
        cout << "2. Tampil Kendaraan" << endl;
        cout << "3. Kendaraan Masuk" << endl;
        cout << "4. Kendaraan Keluar" << endl;
        cout << "5. Hapus Data Kendaraan" << endl;
        cout << "6. Undo Aksi Terakhir" << endl;
        cout << "7. Tampilkan Antrian" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih Menu : ";
        cin >> pilih;

        switch(pilih) {
            case 1:
                tambahKendaraan();
                break;

            case 2:
                tampil(root);
                break;

            case 3:
                kendaraanMasuk();
                break;

            case 4:
                kendaraanKeluar();
                break;

            case 5:
                hapusKendaraan();
                break;

            case 6:
                undo();
                break;

            case 7:
                tampilAntrian();
                break;

            case 0:
                cout << "Program selesai" << endl;
                break;

            default:
                cout << "Menu tidak tersedia" << endl;
        }

    } while(pilih != 0);

    return 0;
}
