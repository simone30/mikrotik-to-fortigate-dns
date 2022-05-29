/*
  from mk to fg dns
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_STRINGS 100
#define MAX_LENGTH 50

char *truncString(char *str, int pos)
{
    size_t len = strlen(str);

    if (len > abs(pos))
    {
        if (pos > 0)
            str = str + pos;
        else
            str[len + pos] = 0;
    }

    return str;
}

int main()
{
    FILE *fd1, *fd2;

    char buf[200];
    char *res;
    char *domName = "";
    char *hostName = "";
    char *ip = "";
    char *arrDomain[NUM_STRINGS] = {};

    char fileName[20] = "mk-dns.conf";
    char menu[5];
    char confirm[2] = "Y";
    int lung = 0, a = 0, b = 0, c = 0, i = 0, x = 0, z = 0, dotNum = 0;
    int numDomain = 0, diverso = 0, uguale = 0;

    printf("Source file: %s\n", fileName);
    printf("Source file ready (Y/n): ");
    scanf("%s", menu);

    if (strcmp(menu, confirm) != 0)
    {
        return 0;
    }

    fd1 = fopen(fileName, "r");

    if (fd1 == NULL)
    {
        perror("Error");
        exit(1);
    }

    fd2 = fopen("fg-dns.conf", "w");
    fprintf(fd2, "config system dns-database\n");

    /*CERCO DOMINI DIVERSI*/
    while (1)
    {
        i = 0;
        x = 0;
        a = 0;
        dotNum = 0;
        res = fgets(buf, 200, fd1);
        // printf("%s", buf);
        if (res == NULL)
        {
            break;
        }

        while (buf[i] != '.' || dotNum != 3)
        {
            if (buf[i] == '.')
            {
                dotNum++;
            }

            i++;
        }

        i++;
        while (buf[lung] != '\0')
        {
            lung++;
        }
        lung--;

        domName = buf;
        domName = truncString(strdupa(domName), i);
        domName = truncString(strdupa(domName), -1);

        char *buf2 = truncString(strdupa(buf), -1);

        if (strcmp(domName, buf2) == 0)
        {
            // printf("SENZA DOMINIO\n");
            uguale = 1;
        }

        if (c == 0)
        {
            arrDomain[b] = domName;
            c = 1;
            b = 1;
        }
        else if (c == 1 && uguale == 0)
        {
            for (z = 0; z < b; z++)
            {
                // printf("confronto: %s - %s\n", domName, arrDomain[z]);
                if (strcmp(domName, arrDomain[z]) == 0)
                {
                    // printf("UGUALI\n");
                    uguale = 1;
                }
                else if (uguale != 1)
                {
                    // printf("DIVERSO\n");
                    diverso = 1;
                }
                // printf("Uguale: %d - Diverso:%d\n", uguale, diverso);
            }
        }

        if (diverso == 1 && uguale == 0)
        {
            // printf("SCRITTO in b = %d\n", b);
            arrDomain[b] = domName;
            b++;
        }

        diverso = 0;
        uguale = 0;
        // printf("domName: %s : %d\n", domName, b);
    }

    /*
    printf("\n\nLeggo: \n");
    for (z = 0; z < b; z++)
    {
        printf("%s\n", arrDomain[z]);
    }
    printf("\n\n");
    */

    /*STAMPO SU FILE GLI EDIT*/
    int numEdit = 1, d = 0, f = 0, g = 0, h = 0, equalNum = 0, lung2 = 0;

    fclose(fd1);
    fd1 = fopen(fileName, "r");

    for (int d = 0; d < b; d++)
    {

        fprintf(fd2, "\tedit \"%s\"\n", arrDomain[d]);
        fprintf(fd2, "\t\tset domain \"%s\"\n\t\tset ttl 5\n\t\tconfig dns-entry\n", arrDomain[d]);

        numEdit = 1;

        while (1)
        {
            i = 0;
            dotNum = 0;
            lung = 0;

            res = fgets(buf, 200, fd1);
            // printf("%s", buf);
            if (res == NULL)
            {
                break;
            }

            while (buf[i] != '.' || dotNum != 3)
            {
                if (buf[i] == '.')
                {
                    dotNum++;
                }
                i++;
            }
            i++;
            domName = buf;
            domName = truncString(strdupa(domName), i);
            domName = truncString(strdupa(domName), -1);

            while (domName[lung] != '\0')
            {
                lung++;
            }
            lung = lung + 2;

            if (strcmp(domName, arrDomain[d]) == 0)
            {

                f = 0;
                equalNum = 0;
                h = 0;
                g = 0;

                fprintf(fd2, "\t\t\tedit %d\n", numEdit);
                numEdit++;

                while (buf[f] != '=' || equalNum != 1)
                {
                    if (buf[f] == '=')
                    {
                        equalNum++;
                    }
                    f++;
                }
                f++;
                // printf("f = %d\n", f);
                hostName = buf;
                hostName = truncString(strdupa(hostName), f);
                hostName = truncString(strdupa(hostName), -lung);

                // printf("hostname = %s\n", hostName);

                fprintf(fd2, "\t\t\t\tset hostname \"%s\"\n", hostName);

                f = 0;
                while (buf[f] != '=')
                {
                    f++;
                }
                f++;
                ip = buf;
                ip = truncString(strdupa(ip), f);
                f = 0;
                while (ip[f] != '\0')
                {
                    if (ip[f] == 'n' || h == 1)
                    {
                        g++;
                        h = 1;
                    }
                    f++;
                }
                g++;
                ip = truncString(strdupa(ip), -g);

                // printf("ip = %s\n", ip);

                fprintf(fd2, "\t\t\t\tset ip %s\n", ip);
                fprintf(fd2, "\t\t\tnext\n");
            }

            for (int e = 0; e < 200; e++)
            {
                buf[e] = '\0';
            }
        }

        fprintf(fd2, "\tnext\n");

        fclose(fd1);
        fd1 = fopen(fileName, "r");
    }

    fprintf(fd2, "end");

    fclose(fd1);
    fclose(fd2);

    printf("\n\nEnd task in *fg-dns.conf* file\n\n");
    return 0;
}