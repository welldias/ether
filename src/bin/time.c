#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  struct timeval start, end;

  int milliseconds = 7;

  long mtime, seconds, useconds;    

  gettimeofday(&start, NULL);
  mtime = 0;
  while(milliseconds > mtime) {
    usleep(1000);
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  }

  printf("Elapsed time: %ld milliseconds\n", mtime);

  return 0;
}
