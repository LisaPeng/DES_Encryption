#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <QFileDialog>

/* Initial Permutation Table */
static char IP_table[] = {
        58, 50, 42, 34, 26, 18, 10,  2,
        60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6,
        64, 56, 48, 40, 32, 24, 16,  8,
        57, 49, 41, 33, 25, 17,  9,  1,
        59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5,
        63, 55, 47, 39, 31, 23, 15,  7
};

/*Expansion table */
static char E_table[] = {
        32,  1,  2,  3,  4,  5,
        4,  5,  6,  7,  8,  9,
        8,  9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32,  1
};

/* Permuted Choice 1 Table */
static char PC1[] = {
        57, 49, 41, 33, 25, 17,  9,
        1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27,
        19, 11,  3, 60, 52, 44, 36,

        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29,
        21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 Table */
static char PC2[] = {
        14, 17, 11, 24,  1,  5,
        3, 28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8,
        16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

/* Iteration Shift Array */
static char iteration_shift[] = {
        /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
        1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};

/* The S-Box tables */
static char S_table[8][64] = {
        {
                14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
                3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
                4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
                15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13
        }, {
                15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
                9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
                0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
                5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9
        }, {
                10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
                1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
                13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
                11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12
        }, {
                7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
                1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
                10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
                15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14
        }, {
                2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
                8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
                4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
                15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3
        }, {
                12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
                0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
                9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
                7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13
        }, {
                4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
                3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
                1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
                10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12
        }, {
                13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
                10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
                7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
                0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11
        }
};

/* Post S-Box permutation */
static char P_table[] = {
        16,  7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
};

/* Inverse of Initial Permutation Table */
int IP_inverse_table[] = {
        40,  8, 48, 16, 56, 24, 64, 32,
        39,  7, 47, 15, 55, 23, 63, 31,
        38,  6, 46, 14, 54, 22, 62, 30,
        37,  5, 45, 13, 53, 21, 61, 29,
        36,  4, 44, 12, 52, 20, 60, 28,
        35,  3, 43, 11, 51, 19, 59, 27,
        34,  2, 42, 10, 50, 18, 58, 26,
        33,  1, 41,  9, 49, 17, 57, 25};

/*
 * The DES function
 * input: 64 bit message
 * key: 64 bit key for encryption/decryption
 * mode: 'e' = encryption; 'd' = decryption
 */

/* initial permutation */
void IP(uint64_t input, uint32_t* left, uint32_t* right) {
    *left = 0;
    *right = 0;

    int i = 0;
    for (; i < 32; i++) {
        *left <<= 1;
        // index = 57, 49, 41 ...
        int index = IP_table[i] - 1;
        *left += (input >> (63 - index)) & 0x1;
    }

    for (; i < 64; i++) {
        *right <<= 1;
        int index = IP_table[i] - 1;
        *right += (input >> (63 - index)) & 0x1;
    }

    //printf("%x\n", left);
    //printf("%x\n", right);
    //return right;
}

/* F(R,K) function */
uint64_t E(uint32_t R) {
    uint64_t s_input = 0;
    int i =0;
    for (; i < 48; i++) {
        s_input <<= 1;
        int index = E_table[i] - 1;
        s_input += (R >> (31 - index)) & 0x1;
    }
    return s_input;
}

uint64_t sub_key[16]        = {0};
void K(char *kdest[16], uint64_t key){
    int i, j;
    /* 28 bits */
    uint32_t C = 0;
    uint32_t D = 0;
    /* 48 bits */

    uint64_t s_input            = 0;
    /* 56 bits */
    uint64_t permuted_choice_1  = 0;
    uint64_t permuted_choice_2  = 0;

    /* initial key schedule calculation */
    for (i = 0; i < 56; i++) {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64-PC1[i])) & 0x1;
    }
    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);

    /* Calculation of the 16 keys */
    for (i = 0; i< 16; i++) {

        /* key schedule */
        // shifting Ci and Di
        for (j = 0; j < iteration_shift[i]; j++) {
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
        }

        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D ;

        sub_key[i] = 0;

        for (j = 0; j < 48; j++) {
            sub_key[i] <<= 1;
            sub_key[i] += (permuted_choice_2 >> (56-PC2[j])) & 0x1;
        }
        sprintf(kdest[i], "key%d: %x", i+1, (uint32_t)sub_key[i]);
    }
}

uint32_t P(uint32_t s_output) {
    uint32_t p_output = 0;
    int i = 0;
    for (; i < 32; i++){
        p_output <<= 1;
        int index = P_table[i] - 1 ;
        p_output += (s_output >> (31 - index)) & 0x1;
    }
    return p_output;
}

uint32_t S(uint64_t s_input) {
    uint32_t s_output = 0;
    for (int i = 0; i < 8; i++) {
        s_output <<= 4;
        uint8_t tmp = (uint8_t)S_table[i][(s_input >> ((7 - i) * 6)) & 0x3f];
        s_output += tmp;
    }
    return s_output;
}

uint32_t F(uint32_t R, uint64_t key) {
    uint64_t s_input;
    uint32_t s_output;
    uint32_t p_output;
    //XORing expanded Ri with Ki
    s_input = E(R) ^ key;


    //printf("s_input: %x",(uint32_t)((s_input>>32)&0xffffffff));
    //printf("%x\n",(uint32_t)(s_input&0xffffffff));
    s_output = S(s_input);
    p_output = P(s_output);
    //printf("%x\n", p_output);
    return p_output;
}

/* inverse initial permutation */
uint64_t IP_inverse(uint32_t left, uint32_t right) {
    uint64_t final = 0;
    uint64_t tmp = (((uint64_t)left) << 32) + right;

    for (int i = 0; i < 64; i++) {
        final <<= 1;
        int index = IP_inverse_table[i] - 1;
        final += (tmp >> (63 - index)) & 0x1;
    }

    return final;
}

void exchange(uint32_t* a, uint32_t* b) {
    uint32_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * The DES function
 * input: 64 bit message
 * key: 64 bit key for encryption/decryption
 * mode: 'e' = encryption; 'd' = decryption
 */
void des(char *kdest[16], char *desdest[16], uint64_t in, uint64_t key, char mode){
    uint32_t left = 0, right = 0;
    IP(in, &left, &right);

    //16Rounds
    K(kdest, key);
    for(int i = 0; i < 16; i++){
        /*
       * Encryption/Decryption
       * XORing expanded Ri with Ki
       */
        uint32_t right1;
        if (mode == 'e') {
            // encryption_jiami
            right1 = left ^ F(right, sub_key[i]);

        } else {
            // decryption_jiemi
            right1 = left ^ F(right, sub_key[15-i]);

        }
        uint32_t left1 = right;

        left = left1;
        right = right1;

        sprintf(desdest[i], "result%d：%x%x", i,left, right);
        //printf("left%d: %x\n", i,left);
        //printf("right%d: %x\n\n", i,right);
    }

    exchange(&left, &right);
    S(0x8f879f8a14f0);
    uint64_t output = IP_inverse(left,right);

    int i;
    sprintf(desdest[i], "%x%x\n", (uint32_t)((output>>32)&0xffffffff), (uint32_t)(output&0xffffffff));
    printf("%x%x\n", (uint32_t)((output>>32)&0xffffffff), (uint32_t)(output&0xffffffff));
}

void File_K(uint64_t key){
    int i, j;
    /* 28 bits */
    uint32_t C = 0;
    uint32_t D = 0;
    /* 48 bits */

    uint64_t s_input            = 0;
    /* 56 bits */
    uint64_t permuted_choice_1  = 0;
    uint64_t permuted_choice_2  = 0;

    /* initial key schedule calculation */
    for (i = 0; i < 56; i++) {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64-PC1[i])) & 0x1;
    }
    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);

    /* Calculation of the 16 keys */
    for (i = 0; i< 16; i++) {

        /* key schedule */
        // shifting Ci and Di
        for (j = 0; j < iteration_shift[i]; j++) {
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
        }

        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D ;

        sub_key[i] = 0;

        for (j = 0; j < 48; j++) {
            sub_key[i] <<= 1;
            sub_key[i] += (permuted_choice_2 >> (56-PC2[j])) & 0x1;
        }
    }
}


uint64_t ntohl64(uint64_t host) {
    uint64_t ret = 0;
    uint32_t high,low;
    low = (uint32_t)(host & 0xFFFFFFFF);
    high = (uint32_t)((host >> 32) & 0xFFFFFFFF);
    low = ntohl(low);
    high = ntohl(high);
    ret = low;
    ret <<= 32;
    ret |= high;
    return ret;
}

/*
 * The DES_file function
 * input: file
 * key: 64 bit key for encryption/decryption
 * mode: 'e' = encryption; 'd' = decryption
 */
uint64_t des_file(uint64_t in, char mode) {
    uint32_t left = 0, right = 0;
    IP(in, &left, &right);

    for(int i = 0; i < 16; i++){
        /*
       * Encryption/Decryption
       * XORing expanded Ri with Ki
       */
        uint32_t right1;
        if (mode == 'e') {
            // encryption_jiami
            right1 = left ^ F(right, sub_key[i]);

        } else {
            // decryption_jiemi
            right1 = left ^ F(right, sub_key[15-i]);

        }
        uint32_t left1 = right;

        left = left1;
        right = right1;

    }

    exchange(&left, &right);
    uint64_t output = IP_inverse(left,right);
    return output;

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->in_result->setText("abcdef1234567890");
    ui->key->setText("aaaaaaaaaaaaaaa");
    ui->file_key->setText("aaaaaaaaaaaaaaa");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//encryption jiami
void MainWindow::on_encryption_clicked()
{
    // to get the in_result
    uint64_t in = 0xabcdef1234567890;//0xabcdef1234567890;
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->text();
    QString a=ui->in_result->text();

    for(int i=0;i<a.size();i++)
    {
        QChar qc=a.at(i);
        if(qc.isDigit())
        {
            in=(in<<4)+qc.toLatin1()-48;
        }
        else if(qc.isLetter())
        {
            in =(in<<4)+qc.toLatin1()-87;
        }
    }

    //to get key
    uint64_t key = 0xaaaaaaaaaaaaaaaa;//0xaaaaaaaaaaaaaaaa
    QLineEdit *lineEdit2 = new QLineEdit;
    lineEdit2->text();
    QString b=ui->key->text();

    for(int i=0;i<b.size();i++)
    {
        QChar qc=b.at(i);
        if(qc.isDigit())
        {
            key=(key<<4)+qc.toLatin1()-48;
        }
        else if(qc.isLetter())
        {
            key =(key<<4)+qc.toLatin1()-87;
        }
    }

    const unsigned BIT_CNT = 16;
    char **k_str = new char*[BIT_CNT];
    for (unsigned i = 0; i != BIT_CNT; ++i)
        k_str[i] = new char[50];

    const unsigned RESULT_CNT = 16;
    char **result_str = new char*[RESULT_CNT];//please change the size according to your need
    for (unsigned i = 0; i != RESULT_CNT; ++i)
        result_str[i] = new char[100];

    des(k_str, result_str, in, key, 'e');

    ui->key_result->clear();
    ui->tran_result->clear();

    QString out(result_str[0]);
    ui->out_result->setText(out);

    for (unsigned i = 0; i != 16; ++i)
        new QListWidgetItem(tr(k_str[i]), ui->key_result);

    for (unsigned i = 0; i != 16; ++i)
        new QListWidgetItem(tr(result_str[i]), ui->tran_result);

    for (unsigned i = 0; i != BIT_CNT; ++i)
        delete [] k_str[i];
    for (unsigned i = 0; i != RESULT_CNT; ++i)
        delete [] result_str[i];
    delete [] k_str;
    delete [] result_str;
}

//decryption jiemi
void MainWindow::on_decryption_clicked()
{
    // to get the out_result
    uint64_t in = 0x47a8fc11244b6fa2;//0x47a8fc11244b6fa2
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->text();
    QString a=ui->out_result->text();

    for(int i=0;i<a.size();i++)
    {
        QChar qc=a.at(i);
        if(qc.isDigit())
        {
            in=(in<<4)+qc.toLatin1()-48;
        }
        else if(qc.isLetter())
        {
            in =(in<<4)+qc.toLatin1()-87;
        }
    }

    //to get key
    uint64_t key = 0xaaaaaaaaaaaaaaaa;//0xaaaaaaaaaaaaaaaa
    QLineEdit *lineEdit2 = new QLineEdit;
    lineEdit2->text();
    QString b=ui->key->text();

    for(int i=0;i<b.size();i++)
    {
        QChar qc=b.at(i);
        if(qc.isDigit())
        {
            key=(key<<4)+qc.toLatin1()-48;
        }
        else if(qc.isLetter())
        {
            key =(key<<4)+qc.toLatin1()-87;
        }
    }

    const unsigned BIT_CNT = 16;
    char **k_str = new char*[BIT_CNT];
    for (unsigned i = 0; i != BIT_CNT; ++i)
        k_str[i] = new char[50];


    const unsigned RESULT_CNT = 16;
    char **result_str = new char*[RESULT_CNT];//please change the size according to your need
    for (unsigned i = 0; i != RESULT_CNT; ++i)
        result_str[i] = new char[100];


    des(k_str, result_str, in, key, 'd');

    // to clear the QListWidget
    ui->key_result->clear();
    ui->tran_result->clear();

    // to display the in_result
    QString out(result_str[0]);
    ui->in_result->setText(out);

    for (unsigned i = 0; i != 16; ++i)
        new QListWidgetItem(tr(k_str[i]), ui->key_result);

    for (unsigned i = 0; i != 16; ++i)
        new QListWidgetItem(tr(result_str[i]), ui->tran_result);

    for (unsigned i = 0; i != BIT_CNT; ++i)
        delete [] k_str[i];
    for (unsigned i = 0; i != RESULT_CNT; ++i)
        delete [] result_str[i];
    delete [] k_str;
    delete [] result_str;
}

void MainWindow::on_selectFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Filek");
    std::cout << path.toStdString() << std::endl;
    //ui->file_in->setText(path.toStdString());
}

void MainWindow::on_selectPathButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Open Filek");
    std::cout << path.toStdString() << std::endl;
    //QString file_full, file_name, file_path;
    //QFileInfo fi;

    //file_full = QFileDialog::getOpenFileName(this);
    //QFileInfo fi = QFileInfo(file_full);
    //QString file_path = fi.absolutePath();

    //std::cout << ba.data();
    //ui->file_in->setText(model.filePath(tree->currentIndex()));
}

void MainWindow::on_file_encryption_clicked()
{
    char *file = "/Users/lisa/Desktop/b.pdf";
    uint64_t file_key = 0xaaaaaaaaaaaaaaaa;
    File_K(file_key);

    FILE *file_in, *file_out;
    file_in = fopen(file, "rb");
    file_out = fopen("/Users/lisa/Desktop/c", "wb");

    fseek(file_out, 24, SEEK_CUR);

    uint64_t len = 0;
    ssize_t n;
    uint64_t content;

    while ((n = fread(&content, 1, 8, file_in)) > 0) {
             if (n < 8) {
                 // fill zeros
                 len += n * 8;
                 content = content << (64 - n * 8) >> (64 - n * 8);
             } else {
                 len += 64;
             }

             // change the order
              content = ntohl64(content);
              content = des_file(content, 'e');

           uint64_t result = ntohl64(content);
           fwrite(&result, sizeof(result), 1, file_out);
    }

    len = des_file(len, 'e');
    len = ntohl64(len);

    // fill the flags
    fseek(file_out, 0, SEEK_SET);
    fwrite(&len, 8, 1, file_out);
    fwrite(&len, 8, 1, file_out);
    fwrite(&len, 8, 1, file_out);

    fclose(file_in);
    fclose(file_out);
}

void MainWindow::on_file_decryption_clicked() {
    char *file = "/Users/lisa/Desktop/c";
    uint64_t file_key = 0xaaaaaaaaaaaaaaaa;
    File_K(file_key);

    FILE *file_in, *file_out;
    file_in = fopen(file, "rb");
    file_out = fopen("/Users/lisa/Desktop/d", "wb");

    fseek(file_in, 0, SEEK_END);
    long file_size = ftell(file_in);

    fseek(file_in, 0, SEEK_SET);

    uint64_t len1, len2, len3, len;
    fread(&len1, 1, 8, file_in);
    fread(&len2, 1, 8, file_in);
    fread(&len3, 1, 8, file_in);

    if (len1 == len2) {
        len = len1;
    } else if (len1 == len3) {
        len = len1;
    } else if (len2 == len3) {
        len = len2;
    } else {
        printf("voting error");
        return;
    }

    len = ntohl64(len);
    len = des_file(len, 'd');

    uint64_t len_ = len / 8;
    uint64_t remain = 8 - len_ % 8;

    if (len_ + 24 + remain != file_size) {
        printf("error2\n");
        return;
    }

    uint64_t unit_len = len_ / 8;
    uint8_t unit_len_extra = (uint8_t)(len_ % 8);

    uint64_t content;
    for (uint64_t i = 0; i < unit_len; i++) {
        fread(&content, 1, 8, file_in);
        content = ntohl64(content);

        content = des_file(content, 'd');

        content = ntohl64(content);

        fwrite(&content, 1, 8, file_out);
    }

    uint8_t content_decoded_8;
        fread(&content, 1, 8, file_in);

        content = ntohl64(content);

        content = des_file(content, 'd');

        /**
         * The reason why I not use ntohl64 here is:
         * Following data are written by byte.
         */
        for (uint8_t i = 0; i < unit_len_extra; i++) {
            content_decoded_8 = (uint8_t)((content >> ((7-i)*8)) & 0xff);
            fwrite(&content_decoded_8, 1, 1, file_out);
        }

        fclose(file_in);
        fclose(file_out);
}
