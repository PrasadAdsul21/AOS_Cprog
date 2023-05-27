#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
if (access(argv[1],F_OK)==0)
printf("File %s exists", argv[1]);
else
printf("File %s doesn't exist.", argv[1]);
return 0;
}
