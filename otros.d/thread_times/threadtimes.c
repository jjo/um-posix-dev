/*
                                 [1][LINK]
                                      
From: greg@sherrill.kiva.net (Gregory Travis)
Subject: Re: Thread creation/switch times on Linux and NT (was Re: Linux users
working at Microsoft!)
Date: 2 Mar 1998 15:44:44 -0500

Well, I went out and figured out how to do threads under Linux.  Here
is program six, the linux thread program:
*/

/* Linux threads: Create 10,000 threads */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NPROCS 10000

void * MyThread(void *arg)
{
  return NULL;
}

main(int argc, char *argv[])
{
  int i;
  time_t st, et;
  pthread_t thread;
  int retcode;
  void *retval;

  time(&st);

  for(i = 0; i < NPROCS; i++) {
    retcode = pthread_create(&thread, NULL, MyThread, NULL);
    if(retcode != 0) {
      perror("pthread_create");
      exit(0);
    }
    retcode = pthread_join(thread, &retval);
    if(retcode != 0) {
      perror("phread_join");
    }
  }

  time(&et);

  printf("%d secs\n",et-st);
}

/*
 -----

This program took 3 seconds to run.  Here is my updated executive summary
comparing process/thread creation overhead between Linux and NT.  Again,
these tests were run on identical hardware and on otherwise unloaded systems.

Each test tested 10,000 instances of whatever it was tested (process
spawning (starting a new program), process forking (starting a
new process), or thread creation (starting a new thread)).  And, again,
I could not find anything identical to the fork() call for NT so there
is no "Process Create" entry for NT.



NT Spawner (spawnl):            120 Seconds (12.0 millisecond/spawn)
Linux Spawner (fork+exec):       57 Seconds ( 6.0 millisecond/spawn)

Linux Process Create (fork):     10 Seconds ( 1.0 millisecond/proc)

NT Thread Create                  9 Seconds ( 0.9 millisecond/thread)
Linux Thread Create               3 Seconds ( 0.3 millisecond/thread)

greg

References

   1. http://www.eklektix.com/lwn/
*/
