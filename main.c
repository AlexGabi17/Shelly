#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_BUF 120

#include <stdio.h>

//------------------------HELP/MAN-------------------------
int xhelp(char *comanda) {

  printf("------------------Shelly Helper------------------\n\n");
  if (strcmp(comanda, "xclear") == 0)
    printf("xclear: delete shown terminal history \n");
  else if (strcmp(comanda, "xcd") == 0)
    printf("xcd: change directory\n");
  else if (strcmp(comanda, "xcp") == 0)
    printf("xcp: copy file in another file\n");
  else if (strcmp(comanda, "pwd") == 0)
    printf("xpwd: current path\n");
  else if (strcmp(comanda, "mkdir") == 0)
    printf("xmkdir: create new directory\n");
  else if (strcmp(comanda, "ls") == 0)
    printf("ls: show all files in current directory\n");
  else if (strcmp(comanda, "rmdir") == 0)
    printf("xrmdir: delete directory\n");
  else if (strcmp(comanda, "rm") == 0)
    printf("xrm: delete file\n");
  else if (strcmp(comanda, "&&") == 0)
    printf("&&: \n");
  else if (strcmp(comanda, "||") == 0)
    printf("||: \n");
  else if (strcmp(comanda, "|") == 0)
    printf("|: \n");
  else if (strcmp(comanda, "xtouch") == 0)
    printf("xtouch:      create new file\n");

  else if (strcmp(comanda, "xexit") == 0)
    printf("xexit: exit terminal \n");
  else if (strcmp(comanda, "") == 0) {
    printf("xclear:     deletes \n");
    printf("xcd:        change directory\n");
    printf("xcp:        copy file content in another file\n");
    printf("xpwd:       current path\n");
    printf("xls:        show all files in current directory\n");
    printf("xtouch:     create new file\n");
    printf("xmkdir:     create new directory\n");
    printf("xrmdir:     delete directory\n");
    printf("xexit:      exit terminal \n");
    printf("xrm:        delete file\n");
    printf("&&: \n");
    printf("||: \n");
    printf("|: \n");
    printf("\n");
  } else { // trebuie returnat de fapt eroare aici
    printf("Comanda nu este valida :(");
    return -1;
  }
  return 0;
}

//------------------------CLEAR-------------------------
int xclear() {
  write(1, "\33[H\33[2J", 7);
  return 0;
}
//------------------------MKDIR-------------------------
void xmkdir(char *name) {
  errno = 0;
  int res = mkdir(name, S_IRWXU);
  if (res == -1) {
    switch (errno) {
    case EACCES:
      printf("this directory does not allow to write");
      exit(EXIT_FAILURE);
    case EEXIST:
      printf("Directory already exists.");
      exit(EXIT_FAILURE);
    case ENAMETOOLONG:
      printf("Name too long");
      exit(EXIT_FAILURE);
    default:
      perror("mkdir");
      exit(EXIT_FAILURE);
    }
  }
}
//------------------------LS-------------------------
void xls(char *path) {
  struct dirent *entry = NULL;
  DIR *dp = NULL;

  dp = opendir(path);
  if (dp != NULL) {
    while ((entry = readdir(dp)))
      printf("%s\n", entry->d_name);
  }
  closedir(dp);
}
//----------------------RMDIR-------------------------------

void xrmdir(const char *path) {
  errno = 0;
  int res = rmdir(path);
  if (res == -1)
    switch (errno) {
    case EACCES:
      printf("This directory does not allow to write.\n");
      exit(EXIT_FAILURE);
    case EBUSY:
      printf("The directory is already in use.\n");
    case EEXIST:
      printf("Directory already exists.\n");
      exit(EXIT_FAILURE);
    case ENAMETOOLONG:
      printf("Name too long");
      exit(EXIT_FAILURE);
    default:
      perror("mkdir");
      exit(EXIT_FAILURE);
    }
}

//----------------------TOUCH-------------------------------

int xtouch(char *file) {
  if (strcmp(file, "") == 0) {
    perror("Fisierul creat are nevoie de un nume\n");
    return errno;
  }

  int fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("eroare la crearea fisierului\n");
    return errno;
  }

  close(fd);
  return 0;
}
//----------------------COPY----in meniu se va apela
// xcp(argv[1],argv[2])------------------------

int xcp(char *file1, char *file2)

{
  if (strcmp(file1, "") == 0) {
    perror("Trebuie pasat numele fisierului sursa\n");
    return errno;
  }

  int fd1 = open(file1, O_RDONLY);
  int fd2 = open(file2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd2 < 0) {
    perror("Eroare la deschiderea fisierului de scriere :(\n");
    return errno;
  } else {
    struct stat sb;
    if (stat(file1, &sb)) // struct stat va furniza detaliile despre fisier
    {
      perror("Eroare la deschiderea fisierului de intrare.\n");
      return errno;
    }
    int size = sb.st_size;
    char buffer[size];
    read(fd1, buffer, size);
    write(fd2, buffer, size);
  }
  close(fd1);
  close(fd2);
  return 0;
}
//------------------------cd-------------------------
void xcd(char *name) {
  if (chdir(name) != 0)
    perror("This path doesn't exist");
}
//------------------------TOLWR--AUXILIAR-------------------------
char *tolowercase(char *s) {

  int l = strlen(s);

  for (int i = 0; i < l; i++) {
    if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) {
      if (s[i] >= 'A' && s[i] <= 'Z')
        s[i] += 32;
    } else
      return NULL;
  }
  return s;
}

void comenzi(bool titlu) {

  if (titlu)
    printf("\t\t\t\tBine ati venit!\n\n");
  printf("\t\t\tComenzile acestui program sunt\n\n");
  printf("\t\t- \"xls\" pentru a lista toate fisierele/folderele din "
         "directorul curent\n");
  printf("\t\t- \"xcd\" pentru a schimba folderul curent\n");
  printf("\t\t- \"xhelp\" pentru a afisa lista tuturor comenzilor acestui "
         "program\n");
  printf("\t\t- \"xpwd\" pentru a afisa path-ul curent\n");
  printf("\t\t- \"xclear\" pentru a sterge consola\n");
  printf("\t\t- \"xmkdir\" pentru a crea un nou director\n");
  printf("\t\t- \"xexit\" pentru a opri consola\n");
  // printf("\t\t- \"\"\n");
  // printf("\t\t- \"\"\n");
  // printf("\t\t- \"\"\n");

  printf("\n\n\n");
}

int main(int argc, char *argv[]) {

  comenzi(true);
  // help("");
  char primaComanda[256] = "";
  char comanda2[256] = "";
  char linieComanda[2000];
  int i = 0, j = 0;
  char path[MAX_BUF];
  getcwd(path, MAX_BUF);

  // while(strcmp(primaComanda, "xexit") != 0){
  while (true) {
    printf("Shelly$ ");

    fgets(linieComanda, 2000, stdin);

    char *argx = strtok(linieComanda, " \n");

    if (strcmp(argx, "xls") == 0) {
      xls(path);
    } else if (strcmp(argx, "xcd") == 0) {
      argx = strtok(NULL, " \n");
      xcd(argx);
    } else if (strcmp(argx, "xhelp") == 0) {
      xhelp("");
    } else if (strcmp(argx, "xcp") == 0) {
      argx = strtok(NULL, " \n");
      xcp(argx, strtok(NULL, " \n"));
    } else if (strcmp(argx, "xpwd") == 0) {

      getcwd(path, MAX_BUF);
      printf("Current working directory: %s\n", path);

    } else if (strcmp(argx, "xclear") == 0) {
      xclear();

    } else if (strcmp(argx, "xtouch") == 0) {
      argx = strtok(NULL, " \n");
      xtouch(argx); // nvm
    } else if (strcmp(argx, "xmkdir") == 0) {

      argx = strtok(NULL, " \n"); // luam numele fisierului

      xmkdir(argx);

    } else if (strcmp(argx, "xrmdir") == 0) {
      argx = strtok(NULL, " \n");
      xrmdir(argx);
    } else if (strcmp(argx, "xexit") == 0) {
      break;
    } else {
      printf("\nEROARE: comanda nu este valida.\n");
      printf("Folositi comanda \"comenzi\" pentru a lista toate comenzile "
             "programului\n\n");
      strcpy(linieComanda, "");
    }
  }

  return 0;
}
