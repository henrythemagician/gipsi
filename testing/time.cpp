/* difftime example */
#include <stdio.h>
#include <time.h>

int main ()
{
  time_t start,end;
  char szInput [256];
  double dif;

  time (&start);
  printf ("Please, enter your name: ");
  gets (szInput);
  time (&end);
  dif = difftime (start,end);
  printf ("Hi %s.\n", szInput);
  printf ("It took you %f seconds to type your name.\n", dif );
 
  return 0;
}
