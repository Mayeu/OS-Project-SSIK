/**
 * @file test_string.c
 * @brief Test a lot of stuff
 */

#include "../kernel/kinout.h"

#include <string.h>

void
test_string()
{
  int             res;
  char           *pstr1 = "phrase de test";
  char           *pstr2 = "phrase de test";
  char           *pstr3 = "lol les zamis";
  char           *pstr4;

  char            str1[255];
  char            str2[255];
  char            output[255];

  strcpy(", genial!", str1);
  strcpy(",   djeuf sfsqq g   dsq sd   qsd qds!   ds :  ", str2);

  kprintln("-------------TEST MODULE STRING BEGIN-------------");

  // strcmp
  kprint("strcmp\t\t\t\t\t\t");
  res = strcmp(pstr1, pstr2);
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strcpy
  kprint("strcpy\t\t\t\t\t\t");
  strcpy(pstr1, output);
  res = strcmp(pstr1, output);
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strcpyn
  kprint("strcpyn\t\t\t\t\t\t");
  strcpyn(pstr1, output, 8);
  res = strcmp("phrase d", output);
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strcmpn
  kprint("strcmpn\t\t\t\t\t\t");
  res = strcmpn(pstr1, pstr3, 4);
  if (res == 1)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strlen
  kprint("strlen\t\t\t\t\t\t");
  res = strlen(pstr1);
  if (res == 14)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strchr
  kprint("strchr\t\t\t\t\t\t");
  pstr4 = strchr(pstr1, 'a');
  res = strcmp(pstr4, "ase de test");
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // strcat
  kprint("strcat\t\t\t\t\t\t");
  strcpy("salut les amis", output);
  strcat(output, str1);
  res = strcmp(output, "salut les amis, genial!");
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // itos
  kprint("itos\t\t\t\t\t\t");
  itos(15, output);
  res = strcmp(output, "15");
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // stoi
  kprint("stoi\t\t\t\t\t\t");
  res = stoi(output);
  if (res == 15)
    kprint("OK\n");
  else
    kprint("NOK\n");

  // trim
  kprint("trim\t\t\t\t\t\t");
  trim(str2);
  res = strcmp(", djeuf sfsqq g dsq sd qsd qds! ds :", str2);
  if (res == 0)
    kprint("OK\n");
  else
    kprint("NOK\n");


  kprintln("--------------TEST MODULE STRING END--------------");
  kprintln("");

}
