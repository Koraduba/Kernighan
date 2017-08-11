#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINES 5000
#define MAXFIELDS 5 
#define MAXLEN 100

char* wrdptr[MAXLEN];
char* nwrdptr[MAXLEN];
char* keys[MAXFIELDS];

int readlines(char*lineptr[], int nlines);
void writelines(char*lineptr[], int nlines);
void qsort(void*lineptr[], int left, int right, int(*comp) (void*, void*,int,int,int), int key0,int key1,int key2,int key3);
int numcmp(char*, char*,int,int,int);
int strcmp3(char*, char*,int,int,int);

void fsort(char*s, int len, char*k);

int fq = 0;// quantity of fields to sort.

int main(int argc, char* argv[])
{
	char* lineptr[MAXLINES];

	int i = 0;

	int key0 = 0;//field number to use for sorting
	int key1 = 0;//f
	int key2 = 0;//d
	int key3 = 0;//r
	int c;
	int nlines;

	int(*func)(char*, char*, int,int,int);
	func = strcmp3;

	if (--argc > 0 && (*++argv)[0] == '-')
		while (c = *++argv[0])
		{
			if (isdigit(c)) key0 = c-'0';
			switch (c)
			{
			case 'n':
				func = numcmp;
				break;
			case 'r':
				key3 = 1;
				break;
			case 'f':
				key1 = 1;
				func = strcmp3;
				break;
			case 'd':
				key2 = 1;
				func = strcmp3;
				break;
			}
		}

	while (--argc > 0 && (*++argv)[0] == '-')	keys[++fq] = *argv;

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
	{
		qsort((void**)lineptr, 0, nlines - 1, (int(*)(void*, void*, int,int,int))(func), key0,key1,key2,key3);

	}

	writelines(lineptr, nlines);

	return 0;
}


char* alloc(int n);
int getline(char s[], int n);


int readlines(char*lineptr[], int max)
{
	char s[MAXLEN];
	int len, nlines = 0;
	char*p;

	while ((len = getline(s, MAXLEN)) > 0)
	{
		if ((p = alloc(len + 1)) == 0 || nlines >= max)
		{
			printf("Stack is overfulled!!");
			return -1;
		}
		else
		{
			for (int i = 1; i <= fq;i++)fsort(s, len, keys[i]);
			strcpy(p, s);
			printf("%s!!!\n",p);
			lineptr[nlines++] = p;
		}

	}

	return nlines;
}

void writelines(char*lineptr[], int nlines)
{
	while (nlines-- > 0) printf("%s", *lineptr++);
}

int getline(char s[], int n)
{
	int c = 0;
	int i = 0;
	while (--n > 0 && (c = getchar()) != EOF && c != '\n') s[i++] = c;
	if (c == '\n') s[i++] = '\n';
	s[i] = '\0';
	return i;
}

#define MAXBUF 10000

char buf[MAXBUF];
char* bufp = buf;

char* alloc(int n)
{
	if (bufp + n - MAXBUF <= buf)
	{
		bufp += n;
		return bufp - n;
	}
	else return 0;

}

void swap(void *lineptr[], int, int);

void qsort(void* lineptr[], int left, int right, int(*comp)(void*, void*,int,int,int), int key0, int key1, int key2, int key3)
{
	int last;

	if (left >= right)return;
	if ((*comp)(lineptr[left], lineptr[(left + right) / 2],key0,key1,key2))
		swap(lineptr, left, (left + right) / 2);
	last = left;
	for (int i = left + 1; i <= right; i++)
	{

		if (((*comp)(lineptr[i], lineptr[left],key0,key1,key2) < 0) != key3) swap(lineptr, i, ++last);
	}
	if ((*comp)(lineptr[left], lineptr[last],key0,key1,key2))
		swap(lineptr, left, last);
	qsort(lineptr, left, last - 1, comp, key0,key1,key2,key3);
	qsort(lineptr, last + 1, right, comp, key0,key1,key2,key3);

	return;
}

void swap(void *lineptr[], int a, int b)
{
//	printf("+");
	void* temp = lineptr[a];
	lineptr[a] = lineptr[b];
	lineptr[b] = temp;

}

double atof(char*);
char* field(char*, int);


int numcmp(char*s1, char*s2, int key0, int key1, int key2)
{
	for (int i = key0; i>1;s1++)		
		if (*s1 == '\t')i--;

	for (int i = key0; i>1; s1++)
		if (*s2 == '\t')i--;

	double p1 = atof(s1);
	double p2 = atof(s2);

	if (p1 < p2) return -1;
	else if (p1 > p2) return 1;
	else return 0;
}


int strcmp3(char*s1, char* s2,int key0,int key1,int key2)
{	
	for (int i = key0; i>1; s1++)
		if (*s1 == '\t')i--;

	for (int i = key0; i>1; s1++)
		if (*s2 == '\t')i--;

	int p1, p2;
	do
	{
		while (!isdigit(*s1) && !isalpha(*s1) && *s1 != ' '&&*s1 != '\0'&&*s1!='\t'&&key2)s1++;
		while (!isdigit(*s2) && !isalpha(*s2) && *s2 != ' '&&*s2 != '\0'&&*s2!='\t'&&key2)s2++;

		if (((p1 = *s1) >= 'A'&&p1 <= 'Z')*key1)p1 += ('a' - 'A');
		if (((p2 = *s2) >= 'A'&&p2 <= 'Z')*key1)p2 += ('a' - 'A');

		if (p1 != p2) return(p1 > p2) ? 1 : -1;

		if (key0 > 0 && p1 == '\t') return 0;
		if (p1 == '\0')return 0;
		s1++;
		s2++;
	} while (p1 == p2);
}



double atof(char* s)
{
	double n = 0.0;
	int num, np = 1;

	while (isspace(*s)) s++;
	num = (*s == '-') ? -1 : 1;
	if (*s == '-' || *s == '+') s++;

	while (isdigit(*s))
	{
		n = n * 10 + (*s++) - '0';
	}
	if (*s == '.')s++;

	while (isdigit(*s))
	{
		n = n * 10 + (*s++) - '0';
		np *= 10;
	}

//	printf("%f\n", n*num / np);
	return n*num / np;
}


void fsort(char*s, int len, char*k)
{

	int c;
	int key1 = 0;
	int key2 = 0;
	int key3 = 0;
	int fn; //number of field to be sorted;
	int(*func)(char*, char*,int,int,int); // function for sorting
	func = strcmp3;

	if (k[0] != '-')return;
	while (c=*++k)
	{
		if (isdigit(c)) fn = c - '0';
		switch (c)
		{
		case 'n':
			func = numcmp;
			break;
		case 'r':
			key3 = 1;
			break;
		case 'f':
			key1 = 1;
			func = strcmp3;
			break;
		case 'd':
			key2 = 1;
			func = strcmp3;
			break;
		}
	}

//	printf("fn%d", fn);
	

		
	char*ss = s;
	int w = 0, lw = 0, rw = 0;;
	int nw = 0;
	int start = 0;
	int metka = 0;
	if (isspace(*s) || *s == ',')start=metka = 1;
	else metka = 0;
	char s_rev[MAXLEN];
	char*p = s_rev;
//	printf("%s", p);



	while (*s != '\0')
	{
	
		if (*s == '\t')
		{
			fn--;
			if (fn == 1)lw = w;
			if (fn == 0)rw = w-1;
		}



		if (!isspace(*s) && *s != ',' && metka == 0)
		{
			wrdptr[w++] = s;
			metka = 1;
		}		

		if ((isspace(*s) || *s == ',') && metka == 1)
		{
			nwrdptr[nw++] = s;
			metka = 0;
		}
			
		++s;
	}
	if (fn == 1) rw = w-1;

//	for (int i = 0; i < w; i++)printf("%s", wrdptr[i]);

	qsort((void**)wrdptr, lw, rw, (int(*)(void*, void*,int,int,int))(func), 1,key1,key2,key3);

//	for (int i = 0; i < w; i++)printf("%s", wrdptr[i]);

	int i = 0, j = 0, ii=0;

	for (i=0; i <w; i++,j++)
	{
		while ((*wrdptr[i] != ',')&&(!isspace(*wrdptr[i])))
		{
			*(p+ii) = *wrdptr[i];
			ii++; 
			wrdptr[i]++;
		}
		while (*nwrdptr[j] ==',' || isspace(*nwrdptr[j]))
		{
			*(p+ii) = *nwrdptr[j];
			ii++;
			nwrdptr[j]++;
		}
	}
	*(p + ii) = '\0';

	strcpy(ss, p);
//	printf("%s", p);
}