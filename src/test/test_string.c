/**
 * @file test_string.c
 * @brief Test a lot of stuff
 */

#include "../kernel/kinout.h"

#include <string.h>

void
test_string()
{
	int res;
	char *pstr1 = "phrase de test";
	char *pstr2 = "phrase de test";
	char *pstr3 = "lol les zamis";
	char *pstr4;

	char str1[255];
	char str2[255];
	char output[255];

	strcpy(", genial!", str1);
	strcpy(",   djeuf sfsqq g   dsq sd   qsd qds!   ds :  ", str2);

	kprint("Tests des fonctions de string.c\n");

	// strcmp
	kprint("strcmp: ");
	res = strcmp(pstr1, pstr2);
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strcpy
	kprint("strcpy: ");
	strcpy(pstr1, output);
	res = strcmp(pstr1, output);
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strcpyn
	kprint("strcpyn: ");
	strcpyn(pstr1, output, 8);
	res = strcmp("phrase d", output);
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strcmpn
	kprint("strcmpn: ");
	res = strcmpn(pstr1, pstr3, 4);
	if (res == 1)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strlen
	kprint("strlen: ");
	res = strlen(pstr1);
	if (res == 14)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strchr
	kprint("strchr: ");
	pstr4 = strchr(pstr1, 'a');
	res = strcmp(pstr4, "ase de test");
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// strcat
	kprint("strcat: ");
	strcpy("salut les amis", output);
	strcat(output, str1);
	res = strcmp(output, "salut les amis, genial!");
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// itos
	kprint("itos: ");
	itos(15, output);
	res = strcmp(output, "15");
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// stoi
	kprint("stoi: ");
	res = stoi(output);
	if (res == 15)
		kprint("OK\n");
	else
		kprint("ERROR\n");

	// trim
	kprint("trim: ");
	trim(str2);
	res = strcmp(", djeuf sfsqq g dsq sd qsd qds! ds :", str2);
	if (res == 0)
		kprint("OK\n");
	else
		kprint("ERROR\n");
}
