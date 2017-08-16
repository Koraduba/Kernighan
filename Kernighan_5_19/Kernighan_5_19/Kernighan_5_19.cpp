#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum{ NAME, BRACKETS, PARENS };
char out[1000];
char token[MAXTOKEN];
int tokentype;

int main()
{
	
	int gettoken(void);
	char temp[MAXTOKEN];
	int type;

	while (gettoken() != EOF)
	{
		strcpy(out, token);
		while (gettoken() != '\n')
		{
			if (tokentype == BRACKETS || tokentype == PARENS)
				strcat(out, token);
			else if (tokentype == '*')
			{
				sprintf(temp, "(*%s)", out);
				strcpy(out, temp);
			}
			else if (tokentype == NAME)
			{
				sprintf(temp, "%s %s",token, out);
				strcpy(out, temp);
			}
			else printf("Error: invalid %s", token);
		}
		printf("%s\n", out);
	}
	return 0;


	return 0;
}

int gettoken(void)
{
	int c;
	char*p = token;

	
	int getch(void);
	void ungetch(int);

	while ((c = getch()) == ' ' || c == '\t');

	if (c == '(')
	{
		if ((c = getch()) == ')')
		{
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else
		{
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if (c == '[')
	{
		*p++ = c;
		while ((*p++ = getch()) != ']');
		*p = '\0';
		return tokentype = BRACKETS;
	}
	else if (isalpha(c))
	{
		*p++ = c;
		while (isalnum(c = getch())) *p++=c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else return tokentype = c;

}

#define MAXBUFFER 100
char buffer[MAXBUFFER];
int bufp = 0;


int getch(void)
{
	return (bufp > 0) ? buffer[--bufp] : getchar();
}

void ungetch(int c)
{
	if(bufp < MAXBUFFER) buffer[bufp++] = c;
	else printf("Error: buffer is full\n");
}