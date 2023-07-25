#include <stdio.h>
#include <string.h>

struct Entry
{
    char traditional[100];
    char simplified[100];
    char pinyin[500];
    char definitions[25][1000];
    int numDefinitions;
};

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    FILE *dictionary = fopen("cedict_ts.u8.bak", "r");
    if(dictionary == NULL)
    {
        printf("error opening dictionary\n");
        return 1;
    }

    struct Entry entries[argc - 1];
    for(int i = 0; i < argc - 1; i++)
    {
        entries[i].numDefinitions = 0;
    }
    
    char traditional[100];
    char simplified[100];
    char pinyin[500];
    char definitions[1000];

    while(fscanf(dictionary, "%s %s [%[^]]] %[^\n]", traditional, simplified, pinyin, definitions) == 4)
    {
        for(int i = 0; i < argc - 1; i++)
        {
            if(strcmp(traditional, argv[i + 1]) == 0 || strcmp(simplified, argv[i + 1]) == 0)
            {
                strcpy(entries[i].traditional, traditional);
                strcpy(entries[i].simplified, simplified);
                strcpy(entries[i].pinyin, pinyin);

                char *token = strtok(definitions, "/");
                while(token != NULL)
                {
                    strcpy(entries[i].definitions[entries[i].numDefinitions], token);
                    entries[i].numDefinitions++;
                    token = strtok(NULL, "/");
                }
            }
        }
    }

    fclose(dictionary);

    for(int i = 0; i < argc - 1; i++)
    {
        printf("[%s] [%s] [%s]\n", entries[i].traditional, entries[i].simplified, entries[i].pinyin);
        for(int j = 0; j < entries[i].numDefinitions - 1; j++)
        {
            printf("* %s\n", entries[i].definitions[j]);
        }
        printf("\n");
    }

    return 0;
}
