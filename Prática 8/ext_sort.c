#include<stdio.h>
#include<string.h>
#include "ext_sort.h"


void merge_files(char* output, char* input1, char* input2, int page_size) {
// Criar buffer na memoria de tamanho page_size e fazer tudo por páginas: 
// pegar uma página de cada entrada, salvar no buffer e fazer o merge no 
// buffer de saída até completar uma página então gravar no disco e começar
// uma página nova.

    int size = 20;
    FILE* out = open_file(output, "w");
    FILE* in1 = open_file(input1, "r");
    FILE* in2 = open_file(input2, "r");

    char* read1[page_size];
    char* read2[page_size];
    char* a;
    char* b;

    while(a && b) {
        if(atoi(read1) < atoi(read2)){
            fprintf(out, "%s", read1);
            a = fgets(read1, size, in1);
            printf("a\n");
        } else {
            fprintf(out, "%s", read2);
            b = fgets(read2, size, in2);
            printf("b\n");
        }       
    } 

    if(a) {
        while(a = fgets(read1, size, in1)) {
            printf("a\n");
            fprintf(out, "%s", read1);
        }
    }
    if(b) {
        while(b = fgets(read2, size, in2)) {
            printf("b\n");
            fprintf(out, "%s", read2);
        }
    }
}

void merge(int* buffer, int left, int median, int right){
    int *left_buf, *right_buf;
    int i = 0, j = 0, k = 0;
    int left_len, right_len;
    right_len = right - median;
    left_len = median - left;
    left_buf = (int*) malloc(sizeof(int)*left_len);
    right_buf = (int*) malloc(sizeof(int)*right_len);
    for(i = 0; i < left_len; i++){
        left_buf[i] = buffer[left+i];
    }
    for(i = 0; i < right_len; i++){
        right_buf[i] = buffer[median+i];
    }
    i = 0; j = 0; k = left;
    while(i < left_len && j < right_len) {
        if(left_buf[i] < right_buf[j]){
            buffer[k] = left_buf[i];
            i++;
        } else {
            buffer[k] = right_buf[j];
            j++;
        }
        k++;
    }
    while(i < left_len) {
        buffer[k] = left_buf[i];
        i++;
        k++;
    }
    while(j < right_len) {
        buffer[k] = right_buf[j];
        j++;
        k++;
    }
}

void merge_sort(int* buffer, int left, int right) {
    int median;
    if(left < right-1) {
        median = left + (right - left)/2;
        merge_sort(buffer, left, median);
        merge_sort(buffer, median, right);
        merge(buffer, left, median, right);
    }
}

int write_buffer(FILE* output, int* buffer, int num_items){
    int i = 0;
    if(output == NULL) return 0;
    for(i = 0; i < num_items; i++){
        fprintf(output, "%d\n", *(buffer+i));
    }
    return num_items;
}

int create_runs(char* input_file, int page_size){
    FILE* input;
    FILE* output;
    char output_name[64];

    int *in_buffer;
    int has_data = 1;
    int count = 0;
    int num_pages = 0;
    input = open_file(input_file, "r");
    if(input == NULL) return -1;

    in_buffer = (int*) malloc(page_size*sizeof(int));
    if(in_buffer == NULL) {
        fclose(input);
        return -1;
    }

    has_data = 1;
    num_pages = 0;
    while(has_data) {
        for(count = 0; count < page_size; count++) {
            if(1 != fscanf(input, "%d\n", &in_buffer[count])) {
                has_data = 0;
                break;
            }
        }
        if(count > 0) {
            snprintf(output_name, sizeof(output_name), "r%d.txt", num_pages);
            output = open_file(output_name, "w");
            if(output_name == NULL) {
                fclose(input);
                free(in_buffer);
                return -1;
            }
            merge_sort(in_buffer, 0, count);
            write_buffer(output, in_buffer, count);
            fclose(output);
        }
        num_pages++;
    }
    return num_pages;
}
