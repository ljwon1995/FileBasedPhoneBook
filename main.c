#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct __PhoneInfo{
    char name[30];
    char telNum[30];
}PhoneInfo;

typedef struct __PhoneBook{
    PhoneInfo* info;
    int content;
    int curCapacity;
}PhoneBook;

typedef struct __Searched{
    int searchedNum;
    int * idx;
}Searched;

int FindToken(char * buf, int size){
    int loop;
    for(loop = 0; loop < size; loop++){
        if(buf[loop] == '/'){
            return loop;
        }
    }
    
    return -1;
}

int LoadFromFile(PhoneBook * phoneBook){
    FILE * fp = fopen("PhoneBook.txt", "rt");
    char buf[70];
    char * str;
    int tokIdx;
    int loop;
    if(fp == NULL){
        return -1;
    }
    
    fscanf(fp, "%d %d", &phoneBook->curCapacity, &phoneBook->content);
    fgetc(fp);
    
    phoneBook->info = (PhoneInfo*)realloc((void*)phoneBook->info, sizeof(PhoneInfo)*phoneBook->curCapacity);
    for(loop = 0; loop < phoneBook->content; loop++){
        fgets(buf, sizeof(buf), fp);
        tokIdx = FindToken(buf, sizeof(buf));
        strncpy(phoneBook->info[loop].name, buf, tokIdx);
        phoneBook->info[loop].name[tokIdx] = '\0';
        str = &buf[tokIdx+1];
        strcpy(phoneBook->info[loop].telNum, str);
        phoneBook->info[loop].telNum[strlen(phoneBook->info[loop].telNum) - 1] = '\0';
    }
        
    
    fclose(fp);
    return 0;
}

int SaveToFile(PhoneBook * phoneBook){
    FILE * fp = fopen("PhoneBook.txt", "wt");
    int loop;
    
    if(fp == NULL){
        return -1;
    }
    
    fprintf(fp, "%d\n%d\n",phoneBook->curCapacity,phoneBook->content);
    for(loop = 0; loop < phoneBook->content; loop++){
        fputs(phoneBook->info[loop].name, fp);
        fputc('/', fp);
        fputs(phoneBook->info[loop].telNum, fp);
        fputc('\n', fp);
    }
    fclose(fp);
    return 0;
}

void InitializePhoneBook(PhoneBook * phoneBook){
    phoneBook->content = 0;
    phoneBook->curCapacity = 10;
    phoneBook->info = (PhoneInfo*)calloc(10, sizeof(PhoneInfo));
    
}

void IncCapacityOfPhoneBook(PhoneBook * phoneBook){
    phoneBook->curCapacity +=5;
    phoneBook->info = (PhoneInfo*)realloc(phoneBook->info, sizeof(PhoneInfo) * phoneBook->curCapacity);
    SaveToFile(phoneBook);
}

void PrintPhoneInfo(PhoneInfo pi){
    fputs("Name : ", stdout);
    fputs(pi.name, stdout);
    fputs("\nTel  : ", stdout);
    fputs(pi.telNum, stdout);
    putchar('\n');
}

void ClearReadBuf(void){
    while(getchar() != '\n');
}

void ShowMenu(void){
    puts("-------MENU-------");
    puts("1. Insert");
    puts("2. Delete");
    puts("3. Search");
    puts("4. Show All");
    puts("5. Quit");
    puts("------------------");
}

char GetChoice(void){
    
    char choice;

    while(1){
        fputs("Choose number : ", stdout);
        scanf("%c", &choice);
        ClearReadBuf();
        if(choice < '1' || choice > '5'){
            puts("ERROR! WRONG INPUT\n");
            continue;
        }
        else
            break;
    }
    puts("------------------");
    
    return choice;

}

void ShowBye(void){
    puts("Thank you for use Bye~");
}

void IsQuit(void){
    ShowBye();
}

void IsInsert(PhoneBook * phoneBook){
    
    puts("------Insert------");
    fputs("Name? : ", stdout);
    fgets(phoneBook->info[phoneBook->content].name, sizeof(phoneBook->info[phoneBook->content].name), stdin);
    phoneBook->info[phoneBook->content].name[strlen(phoneBook->info[phoneBook->content].name) - 1] = 0;
    
    
    fputs("Tel? : ", stdout);
    fgets(phoneBook->info[phoneBook->content].telNum, sizeof(phoneBook->info[phoneBook->content].telNum), stdin);
    phoneBook->info[phoneBook->content].telNum[strlen(phoneBook->info[phoneBook->content].telNum) - 1] = 0;
    
    
    phoneBook->content++;
    
    if(phoneBook->content == phoneBook->curCapacity){
        IncCapacityOfPhoneBook(phoneBook);
    }
    
    SaveToFile(phoneBook);
}

Searched IsSearch(PhoneBook * phoneBook){
    char searchingName[30];
    int idx;
    Searched s;
    
    puts("------Search------");
    
    
    s.searchedNum = 0;
    s.idx = (int*)calloc(1, sizeof(int));
    
    
    printf("Name of searching? : ");
    
    
    
    fgets(searchingName, sizeof(searchingName), stdin);
    searchingName[strlen(searchingName)-1] = 0;
    puts("------------------");
    puts("\n\n");
    for(idx = 0; idx < phoneBook->content; idx++){
        if(strcmp(searchingName, phoneBook->info[idx].name) == 0){
            s.searchedNum++;
            s.idx = (int*)realloc(s.idx, sizeof(int) * s.searchedNum);
            s.idx[s.searchedNum - 1] = idx;
        }
    }
    
    if(s.searchedNum == 0){
        puts("------------------");
        printf("No matching name!\n");
    }
    
    for(idx = 0; idx < s.searchedNum; idx++){
        printf("------Matched%-2d------\n", idx + 1);
        PrintPhoneInfo(phoneBook->info[s.idx[idx]]);
    }
    puts("------------------");
    
    return s;
    
}

void IsDelete(PhoneBook * phoneBook){
    Searched s;
    int deleteIdx;
    char choice;
    int loop;
    
    puts("------Delete------");
    s = IsSearch(phoneBook);
    printf("\n\n");
    
    if(s.searchedNum == 1){
        printf("Are you sure to delete? (Y/N)");
        choice = getchar();
        ClearReadBuf();
        switch (choice) {
            case 'Y':
            case 'y':
                for(loop = s.idx[0]; loop < (phoneBook->content) - 1; loop++){
                    phoneBook->info[loop] = phoneBook->info[loop + 1];
                }
                printf("Deleted successfully!\n");
                phoneBook->content--;
                break;
            case 'N':
            case 'n':
                
                break;
            
            default:
                puts("Wrong Input!");
                break;
        }
        
    }
    else if(s.searchedNum > 1){
        printf("Which one do you want to delete : ");
        scanf("%d", &deleteIdx);
        ClearReadBuf();
        deleteIdx--;
        
        printf("Are you sure to delete? (Y/N)");
        choice = getchar();
        ClearReadBuf();
        switch (choice) {
            case 'Y':
            case 'y':
                for(loop = s.idx[deleteIdx]; loop < (phoneBook->content) - 1; loop++){
                    phoneBook->info[loop] = phoneBook->info[loop + 1];
                }
                printf("Deleted successfully!\n");
                phoneBook->content--;
                break;
            case 'N':
            case 'n':
                
                break;
                
            default:
                puts("Wrong Input!");
                break;
        }
        
    }
    else{
        SaveToFile(phoneBook);
        return;
    }
    SaveToFile(phoneBook);
    
    
}

void IsShowAll(PhoneBook * phoneBook){
    int phoneIdx;
    for(phoneIdx = 0; phoneIdx < phoneBook->content; phoneIdx++){
        printf("------Info%-2d------\n", phoneIdx + 1);
        PrintPhoneInfo(phoneBook->info[phoneIdx]);

        
    }
    
    
}



int main(){
    char choice;
    PhoneBook phoneBook;
    

    
    InitializePhoneBook(&phoneBook);
    if(LoadFromFile(&phoneBook) == -1){
        return -1;
    }
    
    while(1){
        ShowMenu();
        choice = GetChoice();
        switch (choice) {
            case '1':
                puts("\n\n");
                IsInsert(&phoneBook);
                puts("------------------");
                puts("\n\n");
                break;
            case '2':
                puts("\n\n");
                IsDelete(&phoneBook);
                puts("------------------");
                puts("\n\n");
                break;
            case '3':
                puts("\n\n");
                IsSearch(&phoneBook);
                
                puts("\n\n");
                break;
            case '4':
                puts("\n\n");
                IsShowAll(&phoneBook);
                puts("------------------");
                puts("\n\n");
                break;
            case '5':
                puts("\n\n");
                IsQuit();
                puts("\n\n");
                SaveToFile(&phoneBook);
                return 0;
                break;
            default:
                break;
        }
        
    }
    
    
    
    
    
    
}
