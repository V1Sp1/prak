#include <stdio.h>

void pack(FILE *ifile, FILE *ofile)
{
    char n = 0, c, d;
    c = fgetc(ifile);
    d = c;
    while(c != EOF){
        if(c == d){
            ++n;
        } else {
            n = -n;
            fwrite(&n, sizeof(char), 1, ofile);
            fwrite(&d, sizeof(char), 1, ofile);
            n = 1;
        }
        d = c;
        c = fgetc(ifile);
    }
}

void unpack(FILE *ifile, FILE *ofile)
{
    char i, n, c;
    while(fread(&n, sizeof(char), 1, ifile) == 1){
        fread(&c, sizeof(char), 1, ifile);
        for(i = 0; i < -n; ++i){
            fputc(c, ofile);
        }
    }
}

int main(void)
{
    FILE *in, *pa, *un;
    in = fopen("input.txt", "r");
    if(!in){
        perror("input.txt");
    }
    pa = fopen("packed.txt", "w");
    if(!pa){
        perror("packed.txt");
    }
    pack(in, pa);
    fclose(in);
    fclose(pa);
    pa = fopen("packed.txt", "r");
    if(!pa){
        perror("packed.txt");
    }
    un = fopen("unpacked.txt", "w");
    if(!un){
        perror("packed.txt");
    }
    unpack(pa, un);
    fclose(pa);
    fclose(un);
    return 0;
}
